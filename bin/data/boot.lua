--[[
 Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 
 BSD Simplified License.
 For information on usage and redistribution, and for a DISCLAIMER OF ALL
 WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 
 See https://github.com/danomatika/robotcowboy for documentation

 This file is called whenever a scene is loaded and is used to setup
 any extra lua variables, etc needed by the state.

 Adapted/inspired from LOVE: https://love2d.org/
--]]

-- make sure rc table exists
if not rc then rc = {} end

-- defines
math.PI = 3.14159265358979323846
math.TWO_PI = 6.28318530717958647693
math.FOUR_PI = 12.56637061435917295385
math.HALF_PI = 1.57079632679489661923

-- override default prints
print = rc.print
write = rc.write
