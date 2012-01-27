
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

----------------------------------------------------
function oscReceived(msg)
	if msg:getAddress() == "/visual/test" then
		local f1 = msg:getFloat(0)
		local s1 = msg:getString(1)
		print("lua received "..msg:getAddress().." "..f1.." "..s1)
	else
		print("lua received "..msg:getAddress())
	end
end

----------------------------------------------------
function keyPressed(key)
	print("script keyPressed \""..string.char(key).."\"")
	osc.sendInt("/audio/key", key);
end

----------------------------------------------------
function mousePressed(x, y, button)
	print("script mousePressed "..x.." "..y.." "..button)
	msg = osc.message()
	msg:addInt(x)
	msg:addInt(y)
	msg:addInt(button)
	msg:setAddress("/audio/mouse")
	osc.sendMessage(msg)
end
