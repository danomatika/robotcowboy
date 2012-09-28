
-- variables
pos = math.point() -- point class
vel = 20.0 -- float

----------------------------------------------------
function setup()

	graphics.background(127)
	graphics.setRectMode(graphics.rectangle.CENTER)
	
	-- set ball pos
	pos:set(app.width()/2, app.height()/2)
end

----------------------------------------------------
function update()

	-- animate ball
	pos.x = pos.x + vel
	if pos.x < 100 or pos.x > app.width()-100 then
		vel = vel * -1
	end
	
	-- send normalized square pos
	osc.sendFloat("/audio/square", math.map(pos.x, 100, app.width()-100, 0, 1))
end

----------------------------------------------------
function draw()
	
	-- draw ball
	graphics.fill()
	graphics.setColor(255)
	graphics.rect(pos, 50, 50)
	
	-- draw cursor
	graphics.noFill()
	if app.mousePressed() and app.mouseButton() == app.mouse.LEFT then
		graphics.setHexColor(0xFF0000) -- red
		graphics.circle(app.mouseX(), app.mouseY(), 20, 20)
	end
end

----------------------------------------------------
function mousePressed(x, y, button)
	print("script mousePressed "..x.." "..y.." "..button)
	
	-- send the current mouse position using a message
	msg = osc.message()
	msg:addInt(x)
	msg:addInt(y)
	msg:addInt(button)
	msg:setAddress("/audio/mouse")
	osc.sendMessage(msg)
end

----------------------------------------------------
function keyPressed(key)
	print("script keyPressed \""..string.char(key).."\"")
	
	-- send the current key value as a simple int
	osc.sendInt("/audio/key", key);
end

----------------------------------------------------
-- process incoming osc messages
function oscReceived(msg)
	if msg:getAddress() == "/visual/test" then
		local f1 = msg:getFloat(0)
		local s1 = msg:getString(1)
		print("received OSC: "..msg:getAddress().." "..f1.." "..s1)
	else
		print("received OSC: "..msg:getAddress())
	end
end
