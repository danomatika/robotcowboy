--[[
 Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 
 BSD Simplified License.
 For information on usage and redistribution, and for a DISCLAIMER OF ALL
 WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 
 See https://github.com/danomatika/robotcowboy for documentation
--]]

-- create tables
rc = {}
rc.settings = {}
rc.settings.osc = {}
rc.settings.visual = {}
rc.settings.audio = {}
rc.settings.midi = {}
rc.settings.midi.inputs = {}
rc.settings.midi.outputs = {}

-- settings
rc.settings.scenePath = "scenes"

rc.settings.osc.sendAddress = "127.0.0.1"
rc.settings.osc.sendPort = 8880
rc.settings.osc.receivePort = 9009

rc.settings.visual.sendsOut = false
rc.settings.audio.sendsOut = false

rc.settings.midi.inputs = {"IAC Driver Pure Data Out"}
rc.settings.midi.outputs = {"IAC Driver Pure Data In"}
