-- wengwengweng

local fs = require("fs")
local app = require("app")
local plat = require("plat")

print(plat)
print(fs.read_text("Makefile"))

app.run({
	init = function()
		print("ok")
	end,
	frame = function()
		if (app.key_pressed("esc")) then
			app.quit()
		end
		print(app.time())
	end,
})

