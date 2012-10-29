
-- variables
world = physics.World()
anchor = physics.Circle()
circles = {} -- array
joints = {} -- array

----------------------------------------------------
function setup()
	graphics.backgroundHex(0xFDEFC2)
	
	-- setup world
	world:setup()
	world:setGravity(0, 10)
	world:frameRate(30)
	world:grabbing()

	anchor:setup(world, app.width()/2, 20, 4)
	
	-- add some circles
	for i=1,3 do
		local c = physics.Circle()
		c:setPhysics(3.0, 0.53, 0.1)
		c:setup(world, app.width()/2, 100+(i*20), 8)
		circles[i] = c
	end
	
	-- now connect each circle with a joint
	for i=1,#circles do
		local j = physics.Joint()
		-- if this is the first point, connect to top anchor
		if i == 1 then
			j:setup(world, anchor.body, circles[i].body)
		else
			j:setup(world, circles[i-1].body, circles[i].body)
		end
		j.length = 25
		table.insert(joints, j)
	end
end

----------------------------------------------------
function update()
	-- do the physics calcs
	world:update()
end

----------------------------------------------------
function draw()
	
	graphics.setHexColor(0xF2AB01)
	anchor:draw()
	
	-- circles
	graphics.fill()
	graphics.setHexColor(0x01B1F2)
	for i=1,#circles do
		circles[i]:draw()
	end
	
	-- joints
	graphics.setHexColor(0x444342)
	for i=1,#joints do
		joints[i]:draw()
	end
	
	local info = "Press [n] to add a new joint\n"
		.."click and pull the chain around\n";
	graphics.setHexColor(0x444342)
	graphics.bitmapString(info, 30, 30)
end

----------------------------------------------------
function exit()
end

----------------------------------------------------
function mousePressed(x, y, button)
	
end

----------------------------------------------------
function keyPressed(key)
	
	if key == string.byte("n") then
		
		-- add a new circle
		local c = physics.Circle()
		c:setPhysics(3.0, 0.53, 0.1)
		local p = circles[#circles].position
		c:setup(world, p.x + math.random(-30, 30), p.y - 30, 8)
		table.insert(circles, c)
	
		-- get this and prev circles
		local a = #circles - 1
		local b = #circles
		
		-- now connect the circle with a joint
		local j = physics.Joint()
		j:setup(world, circles[a].body, circles[b].body)
		j.length = 25
		table.insert(joints, j)
	end
end

----------------------------------------------------
function keyReleased(key)
end
