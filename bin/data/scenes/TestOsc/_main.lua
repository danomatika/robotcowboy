
----------------------------------------------------
function setup()
	rc.app.setWindowTitle("osc test")	
end

----------------------------------------------------
function update()
	rc.gfx.background(127)
end

----------------------------------------------------
function draw()
	if rc.app.mousePressed and rc.app.mouseButton == 1 then
		rc.gfx.circle(app.mouseX, app.mouseY, 20, 20)
	end
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
	rc.osc.sendInt("/audio/key", key);
end

----------------------------------------------------
function mousePressed(x, y, button)
	print("script mousePressed "..x.." "..y.." "..button)
	--bundle = rc.osc.bundle()
	msg = rc.osc.message()
	msg:addInt(x)
	msg:addInt(y)
	msg:addInt(button)
	msg:setAddress("/audio/mouse")
	rc.osc.sendMessage(msg)
	--bundle:addMessage(msg)
	--rc.osc.sendBundle(bundle)
end
