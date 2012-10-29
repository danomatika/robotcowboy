
-- variables
world = physics.World()

-- arrays
circles = {}
polygons = {}

----------------------------------------------------
function setup()
	graphics.backgroundHex(0xFDEFC2)
	
	-- setup world
	world:setup()
	world:setGravity(0, 20)
	world:createGround()
	world:createBounds(-100, -100, app.width()+100, app.height()+100)
	world:frameRate(30)
end

----------------------------------------------------
function update()

	-- add circles now and then
	if app.frame()%100 == 0 then
		local c = physics.Circle()
		c:setPhysics(1, 0.5, 0.1)
		c:setup(world, app.width()/2+math.random(-50, 50), -20, math.random(3, 20))
		table.insert(circles, c)
	end

	world:update()
end

----------------------------------------------------
function draw()
	
	-- circles
	graphics.fill()
	graphics.setHexColor(0xC0dd3B)
	for i=1,#circles do
		circles[i]:draw()
	end
	
	-- polygons
	graphics.setHexColor(0x444342)
	for i=1,#polygons do
		polygons[i]:draw()
	end
	
	local info = "Draw a shape with the mouse\n"
		.."Press n to add circles\n"
		.."Press c to clear everything\n";
	graphics.setHexColor(0x444342)
	graphics.bitmapString(info, 200, 30)
end

----------------------------------------------------
function exit()
end

----------------------------------------------------
function mousePressed(x, y, button)
	local p = physics.Polygon()
	p:addVertex(x, y)
	table.insert(polygons, p)
end

----------------------------------------------------
function mouseDragged(x, y, button)
	polygons[#polygons]:addVertex(x, y)
end

----------------------------------------------------
function mouseReleased(x, y, button)
	if #polygons > 0 then
		local p = polygons[#polygons]
		p:addVertex(x, y)
		p:simplify()
		p:setup(world)
	end
end

----------------------------------------------------
function touchDown(touch)
	local p = physics.Polygon()
	p:addVertex(touch.x, touch.y)
	table.insert(polygons, p)
end

----------------------------------------------------
function touchMoved(touch)
	polygons[#polygons]:addVertex(touch.x, touch.y)
end

----------------------------------------------------
function touchUp(touch)
	if #polygons > 0 then
		local p = polygons[#polygons]
		p:addVertex(touch.x, touch.y)
		p:simplify()
		p:setup(world)
	end
end

----------------------------------------------------
function touchDoubleTap(touch)
	-- clear by setting an empty table
	circles = {}
	polygons = {}
end

----------------------------------------------------
function keyPressed(key)
	
	if key == string.byte("n") then	
		-- add a new circle
		local c = physics.Circle()
		c:setPhysics(1.0, 0.5, 0.5)
		c:setup(world, app.mouseX(), app.mouseY(), 10, 10)
		table.insert(circles, c)
	elseif key == string.byte("c") then
		-- clear by setting an empty table
		circles = {}
		for i=1,#polygons do
			polygons[i]:clear()
		end
		polygons = {}
	end
end

----------------------------------------------------
function keyReleased(key)
end
