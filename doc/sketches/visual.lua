----------------------------------------------------
img1 = gfx.image()
gfx.stroke(255)
gfx.fill(100)
gfx.noStroke()
gfx.noFill()

-- 3d vector (like PVector)
vec1 = math.vector()
vec1.x
vec1.y
vec1.z
vec1.set(1, 2)
vec1.set(1, 2, 3)
vec3 = vec1 + vec2
vec3 = vec1 - vec2
vec3 = vec1 * vec2
vec3 = vec1 / vec2
vec3 = vec1.dot(vec2)
vec3 = vec1.cross(vec2)
len = vec1.magnitude()
vec1.normalize()

app.getFramerate()
app.background(255)

----------------------------------------------------
-- osc sending

-- one offs
osc:send("/audio/empty")
osc:sendFloat("/audio/float1", 1)
osc:sendString("/audio/string1", "kaadd")

-- osc message
local msg = osc.msg()
msg:addFloat(app:getFramerate())
osc:send("/audio/framerate", msg)
msg:clear()

-- osc bundle
local bundle = osc:bundle()
bundle:addMsg(msg)
osc:send("/audio/bundle", bundle)
	
----------------------------------------------------
-- osc message callback function
function oscReceived(msg)
	
	if msg:addressEq("/visual/test") and msg:typesEq("fs") then
		local f1 = msg:getFloat(0)
		local s1 = msg:getString(1)
		print("received "+msg:getAddress()+" "+msg:getTypes)
	end
	
end
