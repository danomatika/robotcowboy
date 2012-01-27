/* udpreceive~ started 20100110 by Martin Peach based on netreceive~:           */
/* ------------------------ netreceive~ --------------------------------------- */
/*                                                                              */
/* Tilde object to receive uncompressed audio data from netsend~.               */
/* Written by Olaf Matthes <olaf.matthes@gmx.de>.                               */
/* Based on streamin~ by Guenter Geiger.                                        */
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
#if defined(UNIX) || defined(unix)
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#define SOCKET_ERROR -1
#endif
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> /* for socklen_t */
#endif

#ifndef SOL_IP
#define SOL_IP IPPROTO_IP
#endif

#define DEFAULT_AUDIO_BUFFER_FRAMES 16  /* a small circ. buffer for 16 frames */
#define DEFAULT_AVERAGE_NUMBER 10       /* number of values we store for average history */
#define DEFAULT_NETWORK_POLLTIME 1      /* interval in ms for polling for input data (Max/MSP only) */
#define DEFAULT_QUEUE_LENGTH 3          /* min. number of buffers that can be used reliably on your hardware */

#if defined(UNIX) || defined(unix)
#define CLOSESOCKET(fd) close(fd)
#endif
#ifdef _WIN32
#define CLOSESOCKET(fd) closesocket(fd)
#endif

/* ------------------------ udpreceive~ ----------------------------- */

typedef struct _udpreceive_tilde
{
    t_object        x_obj;
    t_outlet        *x_outlet1;
    t_outlet        *x_outlet2;
    t_outlet        *x_addrout;
    t_clock         *x_clock;
    t_atom          x_addrbytes[5];
    int             x_socket;
    int             x_connectsocket;
    int             x_nconnections;
    long            x_addr;
    unsigned short  x_port;
    t_symbol        *x_hostname;
    int             x_error;
    int             x_buffering;
    char            x_msg[256];

    /* buffering */
    int             x_framein;// index of next empty frame in x_frames[]
    int             x_frameout;// index of next frame to play back from x_frames[]
    t_frame         x_frames[DEFAULT_AUDIO_BUFFER_FRAMES];
    int             x_maxframes;
    long            x_tag_errors;
    int             x_sync;// zero if we didn't receive a tag when we expected one
    int             x_blocksize;
    int             x_blocksperrecv;
    int             x_blockssincerecv;

    int             x_nbytes;
    int             x_counter;// count of received frames
    int             x_average[DEFAULT_AVERAGE_NUMBER];
    int             x_averagecur;
    int             x_underflow;
    int             x_overflow;
    int             x_valid;
    long            x_samplerate;
    int             x_noutlets;
    int             x_vecsize;
    t_int           **x_myvec;  /* vector we pass on to the DSP routine */
} t_udpreceive_tilde;

/* function prototypes */
static void udpreceive_tilde_closesocket(t_udpreceive_tilde* x);
static void udpreceive_tilde_reset(t_udpreceive_tilde* x, t_floatarg buffer);
static void udpreceive_tilde_datapoll(t_udpreceive_tilde *x);
static void udpreceive_tilde_connectpoll(t_udpreceive_tilde *x);
static int udpreceive_tilde_createsocket(t_udpreceive_tilde* x, int portno);
static t_int *udpreceive_tilde_perform(t_int *w);
static void udpreceive_tilde_dsp(t_udpreceive_tilde *x, t_signal **sp);
static void udpreceive_tilde_info(t_udpreceive_tilde *x);
static void udpreceive_tilde_tick(t_udpreceive_tilde *x);
static void *udpreceive_tilde_new(t_floatarg fportno, t_floatarg outlets, t_floatarg blocksize);
static void udpreceive_tilde_free(t_udpreceive_tilde *x);
void udpreceive_tilde_setup(void);
static int udpreceive_tilde_sockerror(char *s);
static int udpreceive_tilde_setsocketoptions(int sockfd);
/* these would require to include some headers that are different
   between pd 0.36 and later, so it's easier to do it like this! */
EXTERN void sys_rmpollfn(int fd);
EXTERN void sys_addpollfn(int fd, void* fn, void *ptr);

static t_class *udpreceive_tilde_class;
static t_symbol *ps_format, *ps_channels, *ps_framesize, *ps_overflow, *ps_underflow, *ps_packets,
                *ps_queuesize, *ps_average, *ps_sf_float, *ps_sf_16bit, *ps_sf_8bit, 
                *ps_sf_mp3, *ps_sf_aac, *ps_sf_unknown, *ps_bitrate, *ps_hostname, *ps_nothing,
                *ps_tag_errors;

/* remove all pollfunctions and close socket */
static void udpreceive_tilde_closesocket(t_udpreceive_tilde* x)
{
    sys_rmpollfn(x->x_socket);
    outlet_float(x->x_outlet1, 0);
    CLOSESOCKET(x->x_socket);
    x->x_socket = -1;
}

static void udpreceive_tilde_reset(t_udpreceive_tilde* x, t_floatarg buffer)
{
    int     i;

    x->x_counter = 0;
    x->x_nbytes = 0;
    x->x_framein = 0;
    x->x_frameout = 0;
    x->x_blockssincerecv = 0;
    x->x_blocksperrecv = x->x_blocksize / x->x_vecsize;
    x->x_tag_errors = 0;

    for (i = 0; i < DEFAULT_AVERAGE_NUMBER; i++)
        x->x_average[i] = x->x_maxframes;
    x->x_averagecur = 0;

    i = (int)buffer;    
    if ((i > 0)&&(i < DEFAULT_AUDIO_BUFFER_FRAMES))
    {
        x->x_maxframes = i;
        post("udpreceive~: set buffer to %d frames)", x->x_maxframes);
    }
    else if (i != 0) /* special case of 0 leaves buffer size unchanged */
    {
        post("udpreceive~: buffer must be between 1 and %d frames)", DEFAULT_AUDIO_BUFFER_FRAMES-1);
    }
    x->x_underflow = 0;
    x->x_overflow = 0;
    x->x_buffering = 1;
}

static void udpreceive_tilde_datapoll(t_udpreceive_tilde *x)
{
    int                 ret;
    int                 n;
    long                addr;
    unsigned short      port;
    struct sockaddr_in  from;
    socklen_t           fromlen = sizeof(from);
    t_tag               *tag_ptr;

    n = x->x_nbytes;

    if (x->x_nbytes == 0)   /* we ate all the samples and need a new header tag */
    {
        tag_ptr = &x->x_frames[x->x_framein].tag;
        /* receive header tag */
        ret = recvfrom(x->x_socket, (char*)tag_ptr, sizeof(t_tag), 0,
            (struct sockaddr *)&from, &fromlen);
        /* get the sender's ip */
        addr = ntohl(from.sin_addr.s_addr);
        port = ntohs(from.sin_port);
        /* output addr/port only if changed */
        if (!((addr == x->x_addr)&&(port == x->x_port)))
        {
            x->x_addr = addr;
            x->x_port = port;
            x->x_addrbytes[0].a_w.w_float = (x->x_addr & 0xFF000000)>>24;
            x->x_addrbytes[1].a_w.w_float = (x->x_addr & 0x0FF0000)>>16;
            x->x_addrbytes[2].a_w.w_float = (x->x_addr & 0x0FF00)>>8;
            x->x_addrbytes[3].a_w.w_float = (x->x_addr & 0x0FF);
            x->x_addrbytes[4].a_w.w_float = x->x_port;
            outlet_list(x->x_addrout, &s_list, 5L, x->x_addrbytes);
        }
        if (ret <= 0)   /* error */
        {
            if (0 == udpreceive_tilde_sockerror("recv tag")) return;
            udpreceive_tilde_reset(x, 0);
            return;
        }
        else if (ret != sizeof(t_tag))
        {
            /* incomplete header tag: return and try again later */
            /* in the hope that more data will be available */
            error("udpreceive~: got incomplete header tag");
            return;
        }
        /* make sure this is really a tag */
        if (!((tag_ptr->tag[0] == 'T')&&(tag_ptr->tag[1] == 'A')&&(tag_ptr->tag[2] == 'G')&&(tag_ptr->tag[3] == '!')))
        {
            ++x->x_tag_errors;
            if (x->x_sync) error("udpreceive~: bad header tag (%d)", x->x_tag_errors);
            x->x_sync = 0;
            /* tag length is 16 bytes, a multiple of the data frame size, so eventually we should resync on a tag */
            return;
        }
        /* adjust byte order if necessary */
        tag_ptr->count = ntohl(tag_ptr->count);
        tag_ptr->framesize = ntohl(tag_ptr->framesize);

        /* get info from header tag */
        if (tag_ptr->channels > (x->x_noutlets-1))
        {
            error("udpreceive~: incoming stream has too many channels (%d)", tag_ptr->channels);
            x->x_counter = 0;
            return;
        }
        x->x_nbytes = n = tag_ptr->framesize;
        x->x_sync = 1;
    }
    else    /* we already have header tag or some data and need more */
    {
        ret = recvfrom(x->x_socket, (char*)x->x_frames[x->x_framein].data + x->x_frames[x->x_framein].tag.framesize - n,
            n, 0, (struct sockaddr *)&from, &fromlen);
        if (ret > 0)
        {
            n -= ret;
        }
        else if (ret < 0)   /* error */
        {
            if ( 0 == (ret = udpreceive_tilde_sockerror("recv data"))) return;
#ifdef _WIN32
            if ( ret == WSAEFAULT)            
#else
            if ( ret == EFAULT)
#endif
            {
                post ("udpreceive~: EFAULT: %p %lu %d", x->x_frames[x->x_framein].data, x->x_frames[x->x_framein].tag.framesize, n);
                return; 
            }            
            udpreceive_tilde_reset(x, 0);
            return;
        }

        x->x_nbytes = n;
        if (n == 0) /* a complete packet is received */
        {
            if (x->x_frames[x->x_framein].tag.format == SF_AAC)
            {
                error("udpreceive~: don't know how to decode AAC format");
                return;
            }
            x->x_counter++;
            x->x_framein++;
            x->x_framein %= DEFAULT_AUDIO_BUFFER_FRAMES;

            /* check for buffer overflow */
            if (x->x_framein == x->x_frameout)
            {
                x->x_overflow++;
            }
        }
    }
}

static void udpreceive_tilde_connectpoll(t_udpreceive_tilde *x)
{
    socklen_t           sockaddrlen = sizeof(struct sockaddr);
    struct sockaddr_in  incomer_address;
    int                 fd = accept(x->x_connectsocket, (struct sockaddr*)&incomer_address, &sockaddrlen);

    if (fd < 0) 
    {
        post("udpreceive~: accept failed");
        return;
    }
    if (x->x_socket != -1)
    {
        post("udpreceive~: new connection");
        udpreceive_tilde_closesocket(x);
    }

    udpreceive_tilde_reset(x, 0);
    x->x_socket = fd;
    x->x_nbytes = 0;
    x->x_hostname = gensym(inet_ntoa(incomer_address.sin_addr));
    sys_addpollfn(fd, udpreceive_tilde_datapoll, x);
    outlet_float(x->x_outlet1, 1);
}

static int udpreceive_tilde_createsocket(t_udpreceive_tilde* x, int portno)
{
    struct sockaddr_in  server;
    int                 sockfd;

    /* create a socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        udpreceive_tilde_sockerror("socket");
        return 0;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    /* assign server port number */

    server.sin_port = htons((u_short)portno);
    post("udpreceive~: listening to port number %d", portno);

    udpreceive_tilde_setsocketoptions(sockfd);

    /* name the socket */
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
         udpreceive_tilde_sockerror("bind");
         CLOSESOCKET(sockfd);
         return 0;
    }

    x->x_socket = sockfd;
    x->x_nbytes = 0;
    sys_addpollfn(sockfd, udpreceive_tilde_datapoll, x);
    return 1;
}

/* Queue is 1 to 16 frames long */
#define QUEUESIZE (int)((x->x_framein + DEFAULT_AUDIO_BUFFER_FRAMES - x->x_frameout) % DEFAULT_AUDIO_BUFFER_FRAMES)
/* Block is a set of sample vectors inside a frame, one vector per channel */
#define BLOCKOFFSET (x->x_blockssincerecv * x->x_vecsize * x->x_frames[x->x_frameout].tag.channels)

static t_int *udpreceive_tilde_perform(t_int *w)
{
    t_udpreceive_tilde      *x = (t_udpreceive_tilde*) (w[1]);
    int                     n = (int)(w[2]);
    t_float                 *out[DEFAULT_AUDIO_CHANNELS];
    const int               offset = 3;
    const int               channels = x->x_frames[x->x_frameout].tag.channels;
    int                     i = 0;

    x->x_valid = 0;
    for (i = 0; i < x->x_noutlets; i++)
    {
        out[i] = (t_float *)(w[offset + i]);
    }

    /* set our vector size to the local vector size */
    if (n != x->x_vecsize)
    {
        x->x_vecsize = n;
        x->x_blocksperrecv = x->x_blocksize / x->x_vecsize;
        x->x_blockssincerecv = 0;
    }

    /* check whether there is enough data in buffer */
    if (x->x_buffering && (x->x_counter < x->x_maxframes))
    {
        goto bail;
    }
    x->x_buffering = 0;

    /* check for buffer underflow */
    if (x->x_framein == x->x_frameout)
    {
        x->x_underflow++;
        goto bail;
    }

    x->x_valid = 1;
    /* queue balancing */
    x->x_average[x->x_averagecur] = QUEUESIZE;
    if (++x->x_averagecur >= DEFAULT_AVERAGE_NUMBER)
        x->x_averagecur = 0;

    switch (x->x_frames[x->x_frameout].tag.format)
    {
        case SF_FLOAT:
        {
            int32_t* buf = (int32_t *)x->x_frames[x->x_frameout].data + BLOCKOFFSET;
            flint   fl;

            /* swap bytes if necessary */
            while (n--)
            {
                for (i = 0; i < channels; i++)
                {
                    fl.i32 = ntohl(*buf++);
                    *out[i]++ = fl.f32;
                }
                for (i = channels; i < (x->x_noutlets-1); i++)
                {
                    *out[i]++ = 0.;
                }
                *out[i]++ = x->x_valid;
            }
            x->x_error = 0;
            break;
        }
        case SF_16BIT:
        {
            short* buf = (short *)x->x_frames[x->x_frameout].data + BLOCKOFFSET;
            /* swap bytes if necessary */
            while (n--)
            {
                for (i = 0; i < channels; i++)
                {
                    *out[i]++ = (t_float)((short)(ntohs(*buf++)) * 3.051850e-05);
                }
                for (i = channels; i < (x->x_noutlets-1); i++)
                {
                    *out[i]++ = 0.;
                }
                *out[i]++ = x->x_valid;
            }
            x->x_error = 0;
            break;
        }
        case SF_8BIT:     
        {
            unsigned char* buf = (unsigned char *)x->x_frames[x->x_frameout].data + BLOCKOFFSET;

            while (n--)
            {
                for (i = 0; i < channels; i++)
                {
                    *out[i]++ = (t_float)((0.0078125 * (*buf++)) - 1.0);
                }
                for (i = channels; i < (x->x_noutlets-1); i++)
                {
                    *out[i]++ = 0.;
                }
                *out[i]++ = x->x_valid;
            }
            x->x_error = 0;
            break;
        }
        case SF_MP3:
        {
            if (x->x_error != 1)
            {
                x->x_error = 1;
                sprintf(x->x_msg, "udpreceive~: mp3 format not supported");
                clock_delay(x->x_clock, 0);
            }
            break;
        }
        case SF_AAC:
        {
            if (x->x_error != 2)
            {
                x->x_error = 2;
                sprintf(x->x_msg, "udpreceive~: aac format not supported");
                clock_delay(x->x_clock, 0);
            }
            break;
        }
        default:
            if (x->x_error != 3)
            {
                x->x_error = 3;
                sprintf(x->x_msg, "udpreceive~: unknown format (%d)",x->x_frames[x->x_frameout].tag.format);
                clock_delay(x->x_clock, 0);
            }
            break;
    }

    if (!(x->x_blockssincerecv < x->x_blocksperrecv - 1))
    {
        x->x_blockssincerecv = 0;
        x->x_frameout++;
        x->x_frameout %= DEFAULT_AUDIO_BUFFER_FRAMES;
    }
    else x->x_blockssincerecv++;

    return (w + offset + x->x_noutlets);

bail:
    /* set output to zero */
    while (n--) for (i = 0; i < x->x_noutlets; i++) *(out[i]++) = 0.;
    return (w + offset + x->x_noutlets);
}

static void udpreceive_tilde_dsp(t_udpreceive_tilde *x, t_signal **sp)
{
    int i;

    x->x_myvec[0] = (t_int*)x;
    x->x_myvec[1] = (t_int*)sp[0]->s_n;

    x->x_samplerate = (long)sp[0]->s_sr;

    if (x->x_blocksize % sp[0]->s_n)
    {
        error("udpreceive~: signal vector size too large (needs to be even divisor of %d)", x->x_blocksize);
    }
    else
    {
        for (i = 0; i < x->x_noutlets; i++)
        {
            x->x_myvec[2 + i] = (t_int*)sp[i + 1]->s_vec;
        }
        dsp_addv(udpreceive_tilde_perform, x->x_noutlets + 2, (t_int*)x->x_myvec);
    }
}

/* send stream info */
static void udpreceive_tilde_info(t_udpreceive_tilde *x)
{
    t_atom      list[2];
    t_symbol    *sf_format;
    t_float     bitrate;
    int         i, avg = 0;

    for (i = 0; i < DEFAULT_AVERAGE_NUMBER; i++)
        avg += x->x_average[i];

    bitrate = (t_float)((SF_SIZEOF(x->x_frames[x->x_frameout].tag.format) * x->x_samplerate * 8 * x->x_frames[x->x_frameout].tag.channels) / 1000.);

    switch (x->x_frames[x->x_frameout].tag.format)
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
        case SF_MP3:
        {
            sf_format = ps_sf_mp3;
            break;
        }
        case SF_AAC:
        {
            sf_format = ps_sf_aac;
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
    SETFLOAT(list, (t_float)x->x_frames[x->x_frameout].tag.channels);
    outlet_anything(x->x_outlet2, ps_channels, 1, list);

    /* framesize */
    SETFLOAT(list, (t_float)x->x_frames[x->x_frameout].tag.framesize);
    outlet_anything(x->x_outlet2, ps_framesize, 1, list);

    /* bitrate */
    SETFLOAT(list, (t_float)bitrate);
    outlet_anything(x->x_outlet2, ps_bitrate, 1, list);

    /* --- internal info (buffer and network) --- */
    /* overflow */
    SETFLOAT(list, (t_float)x->x_overflow);
    outlet_anything(x->x_outlet2, ps_overflow, 1, list);

    /* underflow */
    SETFLOAT(list, (t_float)x->x_underflow);
    outlet_anything(x->x_outlet2, ps_underflow, 1, list);

    /* queuesize */
    SETFLOAT(list, (t_float)QUEUESIZE);
    outlet_anything(x->x_outlet2, ps_queuesize, 1, list);

    /* average queuesize */
    SETFLOAT(list, (t_float)((t_float)avg / (t_float)DEFAULT_AVERAGE_NUMBER));
    outlet_anything(x->x_outlet2, ps_average, 1, list);

    /* total packets */
    SETFLOAT(list, (t_float)x->x_counter);
    outlet_anything(x->x_outlet2, ps_packets, 1, list);

    /* total tag errors */
    SETFLOAT(list, (t_float)x->x_tag_errors);
    outlet_anything(x->x_outlet2, ps_tag_errors, 1, list);

    outlet_list(x->x_addrout, &s_list, 5L, x->x_addrbytes);
}

static void udpreceive_tilde_tick(t_udpreceive_tilde *x)
{
/* post a message once, outside of perform routine */
    post("%s", x->x_msg);
}

static void *udpreceive_tilde_new(t_floatarg fportno, t_floatarg outlets, t_floatarg blocksize)
{
    t_udpreceive_tilde  *x;
    int                 i;

    if (fportno == 0) fportno = DEFAULT_PORT;

    x = (t_udpreceive_tilde *)pd_new(udpreceive_tilde_class);
    if (x)
    {
        for (i = sizeof(t_object); i < (int)sizeof(t_udpreceive_tilde); i++)  
            ((char *)x)[i] = 0; 

        if ((int)outlets < 1 || (int)outlets > DEFAULT_AUDIO_CHANNELS)
        {
            error("udpreceive~: Number of channels must be between 1 and %d", DEFAULT_AUDIO_CHANNELS);
            return NULL;
        }

        x->x_noutlets = (int)outlets + 1; // extra outlet for valid flag
        for (i = 0; i < x->x_noutlets; i++)
            outlet_new(&x->x_obj, &s_signal);
        x->x_outlet2 = outlet_new(&x->x_obj, &s_anything);
        x->x_addrout = outlet_new(&x->x_obj, &s_list);
        for (i = 0; i < 5; ++i)
        {
            x->x_addrbytes[i].a_type = A_FLOAT;
            x->x_addrbytes[i].a_w.w_float = 0;
        }
        x->x_addr = 0;
        x->x_port = 0;
        x->x_myvec = (t_int **)t_getbytes(sizeof(t_int *) * (x->x_noutlets + 3));
        if (!x->x_myvec)
        {
            error("udpreceive~: out of memory");
            return NULL;
        }

        x->x_connectsocket = x->x_socket = -1;
        x->x_nconnections = x->x_underflow = x->x_overflow = 0;
        x->x_hostname = ps_nothing;
/* allocate space for 16 frames of 1024 X numchannels floats*/
        for (i = 0; i < DEFAULT_AUDIO_BUFFER_FRAMES; i++)
        {
            x->x_frames[i].data = (char *)t_getbytes(DEFAULT_AUDIO_BUFFER_SIZE * (x->x_noutlets-1) * sizeof(t_float));
        }
        x->x_clock = clock_new(&x->x_obj.ob_pd, (t_method)udpreceive_tilde_tick);
        
        x->x_sync = 1;
        x->x_tag_errors = x->x_framein = x->x_frameout = x->x_valid = 0;
        x->x_maxframes = DEFAULT_QUEUE_LENGTH;
        x->x_vecsize = 64; /* we'll update this later */
        if (blocksize == 0) x->x_blocksize = DEFAULT_AUDIO_BUFFER_SIZE; 
        else if (DEFAULT_AUDIO_BUFFER_SIZE%(int)blocksize)
        {
            error("udpreceive~: blocksize must fit snugly in %d", DEFAULT_AUDIO_BUFFER_SIZE);
            return NULL;
        } 
        else x->x_blocksize = (int)blocksize; //DEFAULT_AUDIO_BUFFER_SIZE; /* <-- the only place blocksize is set */
        x->x_blockssincerecv = 0;
        x->x_blocksperrecv = x->x_blocksize / x->x_vecsize;
        x->x_buffering = 1;

        if (!udpreceive_tilde_createsocket(x, (int)fportno))
        {
            error("udpreceive~: failed to create listening socket");
            return (NULL);
        }
    }
    return (x);
}

static void udpreceive_tilde_free(t_udpreceive_tilde *x)
{
    int i;

    if (x->x_connectsocket != -1)
    {
        sys_rmpollfn(x->x_connectsocket);
        CLOSESOCKET(x->x_connectsocket);
    }
    if (x->x_socket != -1)
    {
        sys_rmpollfn(x->x_socket);
        CLOSESOCKET(x->x_socket);
    }

    /* free memory */
    t_freebytes(x->x_myvec, sizeof(t_int *) * (x->x_noutlets + 3));
    for (i = 0; i < DEFAULT_AUDIO_BUFFER_FRAMES; i++)
    {
         t_freebytes(x->x_frames[i].data, DEFAULT_AUDIO_BUFFER_SIZE * (x->x_noutlets-1) * sizeof(t_float));
    }
    clock_free(x->x_clock);
}

void udpreceive_tilde_setup(void)
{
    udpreceive_tilde_class = class_new(gensym("udpreceive~"), 
        (t_newmethod) udpreceive_tilde_new, (t_method) udpreceive_tilde_free,
        sizeof(t_udpreceive_tilde),  0, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);

    class_addmethod(udpreceive_tilde_class, nullfn, gensym("signal"), 0);
    class_addmethod(udpreceive_tilde_class, (t_method)udpreceive_tilde_info, gensym("info"), 0);
    class_addmethod(udpreceive_tilde_class, (t_method)udpreceive_tilde_dsp, gensym("dsp"), 0);
    class_addmethod(udpreceive_tilde_class, (t_method)udpreceive_tilde_reset, gensym("reset"), A_DEFFLOAT, 0);
    class_addmethod(udpreceive_tilde_class, (t_method)udpreceive_tilde_reset, gensym("buffer"), A_DEFFLOAT, 0);
    post("udpreceive~ v%s, (c) 2004 Olaf Matthes, 2010 Martin Peach", VERSION);

    ps_format = gensym("format");
    ps_tag_errors = gensym("tag_errors");
    ps_channels = gensym("channels");
    ps_framesize = gensym("framesize");
    ps_bitrate = gensym("bitrate");
    ps_overflow = gensym("overflow");
    ps_underflow = gensym("underflow");
    ps_queuesize = gensym("queuesize");
    ps_average = gensym("average");
    ps_packets = gensym("packets");
    ps_hostname = gensym("ipaddr");
    ps_sf_float = gensym("_float_");
    ps_sf_16bit = gensym("_16bit_");
    ps_sf_8bit = gensym("_8bit_");
    ps_sf_mp3 = gensym("_mp3_");
    ps_sf_aac = gensym("_aac_");
    ps_sf_unknown = gensym("_unknown_");
    ps_nothing = gensym("");
}

/* error handlers */
static int udpreceive_tilde_sockerror(char *s)
{
#ifdef _WIN32
    int err = WSAGetLastError();
    if (err == 10054) return 1;
    else if (err == 10040) post("udpreceive~: %s: message too long (%d)", s, err);
    else if (err == 10053) post("udpreceive~: %s: software caused connection abort (%d)", s, err);
    else if (err == 10055) post("udpreceive~: %s: no buffer space available (%d)", s, err);
    else if (err == 10060) post("udpreceive~: %s: connection timed out (%d)", s, err);
    else if (err == 10061) post("udpreceive~: %s: connection refused (%d)", s, err);
    else post("udpreceive~: %s: %s (%d)", s, strerror(err), err);
#else
    int err = errno;
    post("udpreceive~: %s: %s (%d)", s, strerror(err), err);
#endif
#ifdef _WIN32
    if (err == WSAEWOULDBLOCK)
#endif
#if defined(UNIX) || defined(unix)
    if (err == EAGAIN)
#endif
    {
        return 0;   /* recoverable error */
    }
    return err;   /* indicate non-recoverable error */
}

static int udpreceive_tilde_setsocketoptions(int sockfd)
{
    int sockopt = 1;
    if (setsockopt(sockfd, SOL_IP, TCP_NODELAY, (const char*)&sockopt, sizeof(int)) < 0)
        post("udpreceive~: setsockopt NODELAY failed");

    sockopt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&sockopt, sizeof(int)) < 0)
        post("udpreceive~: setsockopt REUSEADDR failed");
    return 0;
}

/* fin udpreceive~.c */
