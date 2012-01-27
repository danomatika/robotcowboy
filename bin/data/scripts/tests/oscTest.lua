
----------------------------------------------------
function setup()
	print("script setup")

	app.setWindowTitle("osc test")	
end

----------------------------------------------------
function update()
	gfx.background(255)
end

----------------------------------------------------
function draw()
end

----------------------------------------------------
function exit()
	print("script finished")
end

-- input callbacks

----------------------------------------------------
function keyPressed(key)
	print("script keyPressed \""..string.char(key).."\"")
	osc.sendInt("/key", key);
end

----------------------------------------------------
function mousePressed(x, y, button)
	print("script mousePressed "..x.." "..y.." "..button)
	msg = osc.message()
	msg:addInt(x)
	msg:addInt(y)
	msg:addInt(button)
	msg:setAddress("/mouse")
	osc.sendMessage(msg)
end
