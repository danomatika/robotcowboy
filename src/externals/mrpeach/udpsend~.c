/* udpsend~ started by Martin Peach on 20100110, based on netsend~              */
/* udpsend~ sends audio via udp only.*/
/* It is a PD external, all Max stuff has been removed from the source */
/* ------------------------ netsend~ ------------------------------------------ */
/*                                                                              */
/* Tilde object to send uncompressed audio data to netreceive~.                 */
/* Written by Olaf Matthes <olaf.matthes@gmx.de>.                               */
/* Based on streamout~ by Guenter Geiger.                                       */
/* Get source at http://www.akustische-kunst.org/                               */
/*                                                                              */
/* This program is free software; you can redistribute it and/or                */
/* modify it under the terms of the GNU General Public License                  */
/* as published by the Free Software Foundation; either version 2               */
/* of the License, or (at your option) any later version.                       */
/*                                                                              */
/* See file LICENSE for further informations on licensing terms.                */
/*                                                                              */
/* This program is distributed in the hope that it will be useful,              */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/* GNU General Public License for more details.                                 */
/*                                                                              */
/* You should have received a copy of the GNU General Public License            */
/* along with this program; if not, write to the Free Software                  */
/* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.  */
/*                                                                              */
/* Based on PureData by Miller Puckette and others.                             */
/*                                                                              */
/* This project was commissioned by the Society for Arts and Technology [SAT],  */
/* Montreal, Quebec, Canada, http://www.sat.qc.ca/.                             */
/*                                                                              */
/* ---------------------------------------------------------------------------- */

#include "m_pd.h"

#include "udpsend~.h"

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#if defined(UNIX) || defined(unix)
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#define SOCKET_ERROR -1
#endif
#ifdef _WIN32
#include <winsock2.h>
#include "pthread.h"
#endif

#ifdef MSG_NOSIGNAL
#define SEND_FLAGS /*MSG_DONTWAIT|*/MSG_NOSIGNAL
#else
#define SEND_FLAGS 0
#endif

#ifndef SOL_IP
#define SOL_IP IPPROTO_IP
#endif


/* ------------------------ udpsend~ ----------------------------- */

static t_class *udpsend_tilde_class;

static t_symbol *ps_nothing, *ps_localhost, *ps_vecsize;
static t_symbol *ps_format, *ps_channels, *ps_framesize;
static t_symbol *ps_sf_float, *ps_sf_16bit, *ps_sf_8bit;
static t_symbol *ps_sf_unknown, *ps_bitrate, *ps_hostname;

typedef struct _udpsend_tilde
{
    t_object        x_obj;
    t_outlet        *x_outlet;
    t_outlet        *x_outlet2;
    t_clock         *x_clock;
    int             x_fd;
    t_tag           x_tag;
    t_symbol*       x_hostname;
    int             x_portno;
    int             x_connectstate;
    char            *x_cbuf;
    int             x_cbufsize;
    int             x_blocksize; /* set to DEFAULT_AUDIO_BUFFER_SIZE or user-supplied argument 3 in udpsend_tilde_new() */
    int             x_blockspersend; /* set to x->x_blocksize / x->x_vecsize in udpsend_tilde_perform() */
    int             x_blockssincesend;

    long            x_samplerate; /* samplerate we're running at */
    int             x_vecsize; /* current DSP signal vector size */
    int             x_ninlets; /* number of inlets */
    int             x_channels; /* number of channels we want to stream */
    int             x_format; /* format of streamed audio data */
    int             x_count; /* total number of audio frames */
    t_int           **x_myvec; /* vector we pass on in the DSP routine */

    pthread_mutex_t x_mutex;
    pthread_t       x_childthread; /* a thread to initiate a connection to the remote port */
    pthread_attr_t  x_childthread_attr; /* child thread should have detached attribute so it can be cleaned up after it exits. */
    int             x_childthread_result; /* result from pthread_create. Zero if x_childthread represents a valid thread. */
} t_udpsend_tilde;

#define NO_CHILDTHREAD 1 /* not zero */

/* function prototypes */
static int udpsend_tilde_sockerror(char *s);
static void udpsend_tilde_closesocket(int fd);
static void udpsend_tilde_notify(t_udpsend_tilde *x);
static void udpsend_tilde_disconnect(t_udpsend_tilde *x);
static void *udpsend_tilde_doconnect(void *zz);
static void udpsend_tilde_connect(t_udpsend_tilde *x, t_symbol *host, t_floatarg fportno);
static t_int *udpsend_tilde_perform(t_int *w);
static void udpsend_tilde_dsp(t_udpsend_tilde *x, t_signal **sp);
static void udpsend_tilde_channels(t_udpsend_tilde *x, t_floatarg channels);
static void udpsend_tilde_format(t_udpsend_tilde *x, t_symbol* form, t_floatarg bitrate);
static void udpsend_tilde_float(t_udpsend_tilde* x, t_floatarg arg);
static void udpsend_tilde_info(t_udpsend_tilde *x);
static void *udpsend_tilde_new(t_floatarg inlets, t_floatarg blocksize);
static void udpsend_tilde_free(t_udpsend_tilde* x);
void udpsend_tilde_setup(void);

/* functions */
static void udpsend_tilde_notify(t_udpsend_tilde *x)
{
    pthread_mutex_lock(&x->x_mutex);
    x->x_childthread_result = NO_CHILDTHREAD; /* connection thread has ended */
    outlet_float(x->x_outlet, x->x_connectstate); /* we should be connected */
    pthread_mutex_unlock(&x->x_mutex);
}

static void udpsend_tilde_disconnect(t_udpsend_tilde *x)
{
    pthread_mutex_lock(&x->x_mutex);
    if (x->x_fd != -1)
    {
        udpsend_tilde_closesocket(x->x_fd);
        x->x_fd = -1;
        x->x_connectstate = 0;
        outlet_float(x->x_outlet, 0);
    }
    pthread_mutex_unlock(&x->x_mutex);
}

/* udpsend_tilde_doconnect runs in the child thread, which terminates as soon as a connection is  */
static void *udpsend_tilde_doconnect(void *zz)
{
    t_udpsend_tilde     *x = (t_udpsend_tilde *)zz;
    struct sockaddr_in  server;
    struct hostent      *hp;
    int                 intarg = 1;
    int                 sockfd;
    int                 portno;
    int                 broadcast = 1;/* nonzero is true */
    t_symbol            *hostname;

    pthread_mutex_lock(&x->x_mutex);
    hostname = x->x_hostname;
    portno = x->x_portno;
    pthread_mutex_unlock(&x->x_mutex);

    /* create a socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
         post("udpsend~: connection to %s on port %d failed", hostname->s_name,portno); 
         udpsend_tilde_sockerror("socket");
         x->x_childthread_result = NO_CHILDTHREAD;
         return (0);
    }

#ifdef SO_BROADCAST
    if( 0 != setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (const void *)&broadcast, sizeof(broadcast)))
    {
        udpsend_tilde_sockerror("setting SO_BROADCAST");
    }
#endif /* SO_BROADCAST */

    /* connect socket using hostname provided in command line */
    server.sin_family = AF_INET;
    hp = gethostbyname(x->x_hostname->s_name);
    if (hp == 0)
    {
        post("udpsend~: bad host?");
        x->x_childthread_result = NO_CHILDTHREAD;
        return (0);
    }

#ifdef SO_PRIORITY
    /* set high priority, LINUX only */
    intarg = 6;	/* select a priority between 0 and 7 */
    if (setsockopt(sockfd, SOL_SOCKET, SO_PRIORITY, (const char*)&intarg, sizeof(int)) < 0)
    {
        udpsend_tilde_sockerror("setting SO_PRIORITY");
    }
#endif

    memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);

    /* assign client port number */
    server.sin_port = htons((unsigned short)portno);

    /* try to connect */
    if (connect(sockfd, (struct sockaddr *) &server, sizeof (server)) < 0)
    {
        udpsend_tilde_sockerror("connecting stream socket");
        udpsend_tilde_closesocket(sockfd);
        x->x_childthread_result = NO_CHILDTHREAD;
        return (0);
    }

    post("udpsend~: connected host %s on port %d", hostname->s_name, portno);

    pthread_mutex_lock(&x->x_mutex);
    x->x_fd = sockfd;
    x->x_connectstate = 1;
    clock_delay(x->x_clock, 0);/* udpsend_tilde_notify is called in next clock tick */
    pthread_mutex_unlock(&x->x_mutex);
    return (0);
}

static void udpsend_tilde_connect(t_udpsend_tilde *x, t_symbol *host, t_floatarg fportno)
{
    pthread_mutex_lock(&x->x_mutex);
    if (x->x_childthread_result == 0)
    {
        pthread_mutex_unlock(&x->x_mutex);
        post("udpsend~: already trying to connect");
        return;
    }
    if (x->x_fd != -1)
    {
        pthread_mutex_unlock(&x->x_mutex);
        post("udpsend~: already connected");
        return;
    }

    if (host != ps_nothing)
        x->x_hostname = host;
    else
        x->x_hostname = ps_localhost; /* default host */

    if (!fportno)
        x->x_portno = DEFAULT_PORT;
    else
        x->x_portno = (int)fportno;
    x->x_count = 0;

    /* start child thread to connect */
    /* sender thread should start out detached so its resouces will be freed when it is done */
    if (0!= (x->x_childthread_result = pthread_attr_init(&x->x_childthread_attr)))
    {
        pthread_mutex_unlock(&x->x_mutex);
        post("udpsend~: pthread_attr_init failed: %d", x->x_childthread_result);
        return;
    }
    if(0!= (x->x_childthread_result = pthread_attr_setdetachstate(&x->x_childthread_attr, PTHREAD_CREATE_DETACHED)))
    {
        pthread_mutex_unlock(&x->x_mutex);
        post("udpsend~: pthread_attr_setdetachstate failed: %d", x->x_childthread_result);
        return;
    }
    if (0 != (x->x_childthread_result = pthread_create(&x->x_childthread, &x->x_childthread_attr, udpsend_tilde_doconnect, x)))
    {
        pthread_mutex_unlock(&x->x_mutex);
        post("udpsend~: couldn't create sender thread (%d)", x->x_childthread_result);
        return;
    }
    pthread_mutex_unlock(&x->x_mutex);
}

static t_int *udpsend_tilde_perform(t_int *w)
{
    t_udpsend_tilde* x = (t_udpsend_tilde*) (w[1]);
    int n = (int)(w[2]);
    t_float *in[DEFAULT_AUDIO_CHANNELS];
    const int offset = 3;
    char* bp = NULL;
    int i, length = x->x_blocksize * SF_SIZEOF(x->x_tag.format) * x->x_tag.channels;
    int sent = 0;

    pthread_mutex_lock(&x->x_mutex);

    for (i = 0; i < x->x_ninlets; i++)
        in[i] = (t_float *)(w[offset + i]);

    if (n != x->x_vecsize)	/* resize buffer */
    {
        x->x_vecsize = n;
        x->x_blockspersend = x->x_blocksize / x->x_vecsize;
        x->x_blockssincesend = 0;
        length = x->x_blocksize * SF_SIZEOF(x->x_tag.format) * x->x_tag.channels;
    }

    /* format the buffer */
    switch (x->x_tag.format)
    {
        case SF_FLOAT:
        {
            int32_t* fbuf = (int32_t *)x->x_cbuf + (x->x_blockssincesend * x->x_vecsize * x->x_tag.channels);
            flint fl;

            while (n--)
                for (i = 0; i < x->x_tag.channels; i++)
                {
                    fl.f32 = *(in[i]++);
                    *fbuf++ = htonl(fl.i32);
                }
            break;
        }
        case SF_16BIT:
        {
            short* cibuf = (short *)x->x_cbuf + (x->x_blockssincesend * x->x_vecsize * x->x_tag.channels);

            while (n--) 
                for (i = 0; i < x->x_tag.channels; i++)
                    *cibuf++ = htons((short)floor(32767.5 * *(in[i]++)));/* signed binary */
            break;
        }
        case SF_8BIT:
        {
            unsigned char* cbuf = (unsigned char*)x->x_cbuf + (x->x_blockssincesend * x->x_vecsize * x->x_tag.channels);

            while (n--) 
                for (i = 0; i < x->x_tag.channels; i++)
                    *cbuf++ = (unsigned char)floor(128. * (1.0 + *(in[i]++))); /* offset binary */
            break;
        }
        default:
            break;
    }

    if (!(x->x_blockssincesend < x->x_blockspersend - 1))	/* time to send the buffer */
    {
        x->x_blockssincesend = 0;
        x->x_count++;	/* count data packet we're going to send */

        if (x->x_fd != -1)
        {
            bp = (char *)x->x_cbuf;
            /* fill in the header tag */
            x->x_tag.tag[0] = 'T';
            x->x_tag.tag[1] = 'A';
            x->x_tag.tag[2] = 'G';
            x->x_tag.tag[3] = '!';
            x->x_tag.framesize = htonl(length);
            x->x_tag.count = htonl(x->x_count);
            /* send the format tag */
            if (send(x->x_fd, (char*)&x->x_tag, sizeof(t_tag), SEND_FLAGS) < 0)
            {
                udpsend_tilde_sockerror("send tag");
                pthread_mutex_unlock(&x->x_mutex);
                udpsend_tilde_disconnect(x);
                return (w + offset + x->x_ninlets);
            }
            if (length != 0)
/* UDP: max. packet size is 64k (incl. headers) so we have to split */
            {
#ifdef __APPLE__
                /* WARNING: due to a 'bug' (maybe Apple would call it a feature?) in OS X
                    send calls with data packets larger than 16k fail with error number 40!
                    Thus we have to split the data packets into several packets that are 
                    16k in size. The other side will reassemble them again. */
                int size = DEFAULT_UDP_PACKT_SIZE;
                if (length < size)  /* maybe data fits into one packet? */
                    size = length;
                /* send the buffer */
                for (sent = 0; sent < length;)
                {
                    int ret = 0;
                    ret = send(x->x_fd, bp, size, SEND_FLAGS);
                    if (ret <= 0)
                    {
                        udpsend_tilde_sockerror("send data");
                        pthread_mutex_unlock(&x->x_mutex);
                        udpsend_tilde_disconnect(x);
                        return (w + offset + x->x_ninlets);
                    }
                    else
                    {
                        bp += ret;
                        sent += ret;
                        if ((length - sent) < size)
                            size = length - sent;
                    }
                }
#else
                /* If there is any data, send the buffer, the OS might segment it into smaller packets */
                int ret = send(x->x_fd, bp, length, SEND_FLAGS);
                if (ret <= 0)
                {
                    post ("udpsend~: sending length %ld", length);
                    udpsend_tilde_sockerror("send data");
                    pthread_mutex_unlock(&x->x_mutex);
                    udpsend_tilde_disconnect(x);
                    return (w + offset + x->x_ninlets);
                }
#endif
            }
        }

/* check whether user has updated any parameters */
        if (x->x_tag.channels != x->x_channels)
        {
            x->x_tag.channels = x->x_channels;
        }
        if (x->x_tag.format != x->x_format)
        {
            x->x_tag.format = x->x_format;
        }
    }
    else
    {
        x->x_blockssincesend++;
    }
    pthread_mutex_unlock(&x->x_mutex);
    return (w + offset + x->x_ninlets);
}

static void udpsend_tilde_dsp(t_udpsend_tilde *x, t_signal **sp)
{
    int i;

    pthread_mutex_lock(&x->x_mutex);

    x->x_myvec[0] = (t_int*)x;
    x->x_myvec[1] = (t_int*)sp[0]->s_n;

    x->x_samplerate = sp[0]->s_sr;

    for (i = 0; i < x->x_ninlets; i++)
    {
        x->x_myvec[2 + i] = (t_int*)sp[i]->s_vec;
    }

    pthread_mutex_unlock(&x->x_mutex);

    if (DEFAULT_AUDIO_BUFFER_SIZE % sp[0]->s_n)
    {
        error("udpsend~: signal vector size too large (needs to be even divisor of %d)", DEFAULT_AUDIO_BUFFER_SIZE);
    }
    else
    {
        dsp_addv(udpsend_tilde_perform, x->x_ninlets + 2, (t_int*)x->x_myvec);
    }
}

static void udpsend_tilde_channels(t_udpsend_tilde *x, t_floatarg channels)
{
    pthread_mutex_lock(&x->x_mutex);
    if (channels >= 0 && channels <= x->x_ninlets)
    {
        x->x_channels = (int)channels;
        post("udpsend~: channels set to %d", (int)channels);
    }
    else post ("udpsend~ number of channels must be between 0 and %d", x->x_ninlets);
    pthread_mutex_unlock(&x->x_mutex);
}

static void udpsend_tilde_format(t_udpsend_tilde *x, t_symbol* form, t_floatarg bitrate)
{
    pthread_mutex_lock(&x->x_mutex);
    if (!strncmp(form->s_name,"float", 5) && x->x_tag.format != SF_FLOAT)
    {
        x->x_format = (int)SF_FLOAT;
    }
    else if (!strncmp(form->s_name,"16bit", 5) && x->x_tag.format != SF_16BIT)
    {
        x->x_format = (int)SF_16BIT;
    }
    else if (!strncmp(form->s_name,"8bit", 4) && x->x_tag.format != SF_8BIT)
    {
        x->x_format = (int)SF_8BIT;
    }

    post("udpsend~: format set to %s", form->s_name);
    pthread_mutex_unlock(&x->x_mutex);
}

static void udpsend_tilde_float(t_udpsend_tilde* x, t_floatarg arg)
{
    if (arg == 0.0)
        udpsend_tilde_disconnect(x);
    else
        udpsend_tilde_connect(x,x->x_hostname,(float) x->x_portno);
}

/* send stream info */
static void udpsend_tilde_info(t_udpsend_tilde *x)
{
    t_atom list[2];
    t_symbol *sf_format;
    t_float bitrate;

    bitrate = (t_float)((SF_SIZEOF(x->x_tag.format) * x->x_samplerate * 8 * x->x_tag.channels) / 1000.);

    switch (x->x_tag.format)
    {
        case SF_FLOAT:
        {
            sf_format = ps_sf_float;
            break;
        }
        case SF_16BIT:
        {
            sf_format = ps_sf_16bit;
            break;
        }
        case SF_8BIT:
        {
            sf_format = ps_sf_8bit;
            break;
        }
        default:
        {
            sf_format = ps_sf_unknown;
            break;
        }
    }

    /* --- stream information (t_tag) --- */

    /* audio format */
    SETSYMBOL(list, (t_symbol *)sf_format);
    outlet_anything(x->x_outlet2, ps_format, 1, list);

    /* channels */
    SETFLOAT(list, (t_float)x->x_tag.channels);
    outlet_anything(x->x_outlet2, ps_channels, 1, list);

    /* current signal vector size */
    SETFLOAT(list, (t_float)x->x_vecsize);
    outlet_anything(x->x_outlet2, ps_vecsize, 1, list);

    /* framesize */
    SETFLOAT(list, (t_float)(ntohl(x->x_tag.framesize)));
    outlet_anything(x->x_outlet2, ps_framesize, 1, list);

    /* bitrate */
    SETFLOAT(list, (t_float)bitrate);
    outlet_anything(x->x_outlet2, ps_bitrate, 1, list);

    /* IP address */
    SETSYMBOL(list, (t_symbol *)x->x_hostname);
    outlet_anything(x->x_outlet2, ps_hostname, 1, list);
}

static void *udpsend_tilde_new(t_floatarg inlets, t_floatarg blocksize)
{
    int i;

    t_udpsend_tilde *x = (t_udpsend_tilde *)pd_new(udpsend_tilde_class);
    if (x)
    {
        for (i = sizeof(t_object); i < (int)sizeof(t_udpsend_tilde); i++)
            ((char *)x)[i] = 0; 

        if ((int)inlets < 1 || (int)inlets > DEFAULT_AUDIO_CHANNELS)
        {
            error("udpsend~: Number of channels must be between 1 and %d", DEFAULT_AUDIO_CHANNELS);
            return NULL;
        }
        x->x_ninlets = (int)inlets;
        for (i = 1; i < x->x_ninlets; i++)
            inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
 
        x->x_outlet = outlet_new(&x->x_obj, &s_float);
        x->x_outlet2 = outlet_new(&x->x_obj, &s_list);
        x->x_clock = clock_new(x, (t_method)udpsend_tilde_notify);

        x->x_myvec = (t_int **)t_getbytes(sizeof(t_int *) * (x->x_ninlets + 3));
        if (!x->x_myvec)
        {
            error("udpsend~: out of memory");
            return NULL;
        }

        pthread_mutex_init(&x->x_mutex, 0);

        x->x_hostname = ps_localhost;
        x->x_portno = DEFAULT_PORT;
        x->x_connectstate = 0;
        x->x_childthread_result = NO_CHILDTHREAD;
        x->x_fd = -1;

        x->x_tag.format = x->x_format = SF_FLOAT;
        x->x_tag.channels = x->x_channels = x->x_ninlets;
        x->x_vecsize = 64; /* this is updated in the perform routine udpsend_tilde_perform */
        x->x_cbuf = NULL;
        if (blocksize == 0) x->x_blocksize = DEFAULT_AUDIO_BUFFER_SIZE; 
        else if (DEFAULT_AUDIO_BUFFER_SIZE%(int)blocksize)
        {
            error("udpsend~: blocksize must fit snugly in %d", DEFAULT_AUDIO_BUFFER_SIZE);
            return NULL;
        } 
        else x->x_blocksize = (int)blocksize; //DEFAULT_AUDIO_BUFFER_SIZE; /* <-- the only place blocksize is set */
        x->x_blockspersend = x->x_blocksize / x->x_vecsize; /* 1024/64 = 16 blocks */
        x->x_blockssincesend = 0;
        x->x_cbufsize = x->x_blocksize * sizeof(t_float) * x->x_ninlets;
        x->x_cbuf = (char *)t_getbytes(x->x_cbufsize);

#if defined(UNIX) || defined(unix)
        /* we don't want to get signaled in case send() fails */
        signal(SIGPIPE, SIG_IGN);
#endif
    }

    return (x);
}

static void udpsend_tilde_free(t_udpsend_tilde* x)
{
    udpsend_tilde_disconnect(x);

    /* free the memory */
    if (x->x_cbuf)t_freebytes(x->x_cbuf, x->x_cbufsize);
    if (x->x_myvec)t_freebytes(x->x_myvec, sizeof(t_int) * (x->x_ninlets + 3));

    clock_free(x->x_clock);

    pthread_mutex_destroy(&x->x_mutex);
}

void udpsend_tilde_setup(void)
{
    udpsend_tilde_class = class_new(gensym("udpsend~"), (t_newmethod)udpsend_tilde_new, (t_method)udpsend_tilde_free,
        sizeof(t_udpsend_tilde), 0, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
    class_addmethod(udpsend_tilde_class, nullfn, gensym("signal"), 0);
    class_addmethod(udpsend_tilde_class, (t_method)udpsend_tilde_dsp, gensym("dsp"), 0);
    class_addfloat(udpsend_tilde_class, udpsend_tilde_float);
    class_addmethod(udpsend_tilde_class, (t_method)udpsend_tilde_info, gensym("info"), 0);
    class_addmethod(udpsend_tilde_class, (t_method)udpsend_tilde_connect, gensym("connect"), A_DEFSYM, A_DEFFLOAT, 0);
    class_addmethod(udpsend_tilde_class, (t_method)udpsend_tilde_disconnect, gensym("disconnect"), 0);
    class_addmethod(udpsend_tilde_class, (t_method)udpsend_tilde_channels, gensym("channels"), A_FLOAT, 0);
    class_addmethod(udpsend_tilde_class, (t_method)udpsend_tilde_format, gensym("format"), A_SYMBOL, A_DEFFLOAT, 0);
    class_sethelpsymbol(udpsend_tilde_class, gensym("udpsend~"));
    post("udpsend~ v%s, (c) 2004-2005 Olaf Matthes, 2010 Martin Peach", VERSION);
    post("udpsend~ Default blocksize is %d", DEFAULT_AUDIO_BUFFER_SIZE);

    ps_nothing = gensym("");
    ps_localhost = gensym("localhost");
    ps_hostname = gensym("ipaddr");
    ps_format = gensym("format");
    ps_channels = gensym("channels");
    ps_vecsize = gensym("vecsize");
    ps_framesize = gensym("framesize");
    ps_bitrate = gensym("bitrate");
    ps_sf_float = gensym("_float_");
    ps_sf_16bit = gensym("_16bit_");
    ps_sf_8bit = gensym("_8bit_");
    ps_sf_unknown = gensym("_unknown_");
}

/* Utility functions */

static int udpsend_tilde_sockerror(char *s)
{
#ifdef _WIN32
    int err = WSAGetLastError();
    if (err == 10054) return 1;
    else if (err == 10053) post("udpsend~: %s: software caused connection abort (%d)", s, err);
    else if (err == 10055) post("udpsend~: %s: no buffer space available (%d)", s, err);
    else if (err == 10060) post("udpsend~: %s: connection timed out (%d)", s, err);
    else if (err == 10061) post("udpsend~: %s: connection refused (%d)", s, err);
    else post("udpsend~: %s: %s (%d)", s, strerror(err), err);
#else
    int err = errno;
    post("udpsend~: %s: %s (%d)", s, strerror(err), err);
#endif
#ifdef _WIN32
    if (err == WSAEWOULDBLOCK)
#endif
#if defined(UNIX) || defined(unix)
    if (err == EAGAIN)
#endif
    {
        return 1;	/* recoverable error */
    }
    return 0;	/* indicate non-recoverable error */
}

static void udpsend_tilde_closesocket(int fd)
{
#if defined(UNIX) || defined(unix)
    close(fd);
#endif
#ifdef _WIN32
    closesocket(fd);
#endif
}

/* fin udpsend~.c */
