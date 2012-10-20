--[[
 robotcowboy app settings file
 See https://github.com/danomatika/robotcowboy for documentation
--]]

-- DON'T EDIT ANY OF THIS!
-- create tables
rc = {}
rc.settings = {}
rc.settings.osc = {}
rc.settings.visual = {}
rc.settings.audio = {}
rc.settings.midi = {}
rc.settings.midi.inputs = {}
rc.settings.midi.outputs = {}
-- OK, YOU CAN EDIT THE FOLLOWING BELOW ...

-- robotcowboy app settings

--
-- MAIN SETTINGS
--

-- the base document directory
-- if this is a relative path, it is assumed to be relative to your home directory
-- 
-- if this directory does not exist, it will be created on startup
--
-- note: changing this has no effect on iOS
--
-- default is "Documents/robotcowboy" on desktop, "Documents" on iOS
rc.settings.docsPath = "Documents/robotcowboy"

-- an array of alternative settings files to load
-- if these are relative paths, they are relative to the documents dir
--
-- note: you can override existing settings in a new file, but keep in mind the order
-- you use
--
-- note: you cannot override the docsPath, it's only loaded from the main settings file
-- in ~/.robotcowboy/settings.lua or ~/Documents/settings.lua on iOS
--
-- empty by default
rc.settings.files = {"settings.lua"}

--
-- PATHS
--

-- the base scene directory
-- if this is a relative path, it is relative to the documents dir 
--
-- note: changing this has no effect on iOS
--
-- default is "scenes"
rc.settings.scenePath = "scenes"

--
-- PLAYLIST
--

-- load all scene folders found in the scene dir?
-- this adds everything found to the current playlist
--
-- default is false
rc.settings.loadAllScenesInSceneDir = false

-- specify a playlist manually
-- if these are relative paths, they are relative to the scene dir
--
-- this is ignored if loadAllScenesInSceneDir is true
--
-- empty by default
rc.settings.playlist = {}

--
-- OSC
--

-- the port to receive osc messages on
--
-- default is 9009
rc.settings.osc.receivePort = 9009

-- the host ip and port to send osc messages to,
-- note this is only used if either the visual or the audio subsystems are configured to
-- send osc outside of the application
--
-- default is "127.0.0.1" & 8880
rc.settings.osc.sendAddress = "127.0.0.1"
rc.settings.osc.sendPort = 8880

-- send osc messages outside of the application?
--
-- normally, osc messages are passed between the visual and audio subsystems internally,
-- setting these to false causes the messages to be sent to the send address & port
--
-- this is designed for application streaming, separation, and debugging
--
-- default is false
rc.settings.visual.sendsOut = false
rc.settings.audio.sendsOut = false

--
-- MIDI
--

-- connection whitelist for midi ports
--
-- these are string arrays of the allowed midi inport and output ports,
-- if any of the given ports are found on startup, they will be auto connected
--
-- default is "Session 1" for input & output for the iOS networking session
rc.settings.midi.inputs = {"IAC Driver Pure Data Out", "Session 1"}
rc.settings.midi.outputs = {"IAC Driver Pure Data In", "Session 1"}

--
-- LOG
--

-- set the log level to enable/disable more detailed debug messaging to the console
-- default is 2
--[[
	verbose 	0
	notice		1
	warning 	2
	error		3
	fatal error 4
	silent		5
]]--
rc.settings.logLevel = 0
