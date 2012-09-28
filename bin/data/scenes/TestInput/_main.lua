
----------------------------------------------------
function setup()
	
	graphics.background(100)
	graphics.noAutoBackground() -- turn off for alpha fade
	graphics.smooth()
	
	-- print test
	print(string.format("%f", math.pi))
	write("testt")
	write(" 123")
	write("KAAA\nKAA\nddd")
	print("check it out")
	
	-- mouse button constants
	print("left m button: "..tostring(app.mouse.LEFT))
	print("middle m button: "..tostring(app.mouse.MIDDLE))
	print("right m button: "..tostring(app.mouse.RIGHT))
end

----------------------------------------------------
function update()
end

----------------------------------------------------
function draw()
	
	-- do an alpha fade
	graphics.fill()
	graphics.setColor(0, 0, 0, 5)
	graphics.rect(0, 0, app.width(), app.height())
	
	-- draw a line form the center of the screen to the mouse pos
	graphics.setLineWidth(3)
	graphics.setColor(200, 100, 100)
	graphics.line(app.width()/2, app.height()/2, app.mouseX(), app.mouseY()) 
	graphics.setLineWidth(1)
	
	-- draw a circle at the mouse pos
	if app.mousePressed() then
		graphics.setHexColor(0x00FFFF)
		if(app.mouseButton() == app.mouse.LEFT) then
			graphics.fill()
		else
			graphics.noFill()
		end
		graphics.circle(app.mouseX(), app.mouseY(), 20)
		graphics.fill()
		graphics.setHexColor(0xFF0000)
		graphics.circle(app.pmouseX(), app.pmouseY(), 5)
	end
	
	
	-- display input variables
	graphics.setColor(255)
	graphics.bitmapString(
		"mouse: "..app.mouseX().." "..app.mouseY().."\n"..
		"pmouse: "..app.pmouseX().." "..app.pmouseY().."\n"..
		"mousePressed: "..tostring(app.mousePressed()).."\n"..
		"mouseButton: "..app.mouseButton().."\n"..
		"keyPressed: "..tostring(app.keyPressed()).."\n"..
		"key: "..app.keyCode(), 10, 300)
end

----------------------------------------------------
function exit()
end

----------------------------------------------------
function mousePressed(x, y, button)
	
	buttonString = "none"
	
	if button == app.mouse.LEFT then buttonString = "left"
	elseif button == app.mouse.MIDDLE then buttonString = "middle"
	elseif button == app.mouse.RIGHT then buttonString = "right"
	end 
	
	print(buttonString.." mousePressed "..x.." "..y.." "..button)
end

----------------------------------------------------
function keyPressed(key)

	if key == app.key.LEFT then print("LEFT key pressed")
	elseif key == app.key.RIGHT then print("RIGHT key pressed")
	elseif key == app.key.UP then print("UP key pressed")
	elseif key == app.key.DOWN then print("DOWN key pressed")
	elseif key == string.byte(" ") then
		graphics.clear(100)
	else
		print("script keyPressed \""..tostring(key).."\"")
	end
	
end

----------------------------------------------------
function keyReleased(key)
end
