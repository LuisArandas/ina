this.autowatch = 1;
this.inlets = 3
this.outlets = 3

local gl = require("opengl")
local GL = gl

function apply()
	outlet(0, gl.GetString(GL.VERSION))
	outlet(1, gl.GetString(GL.VENDOR))
	outlet(2, gl.GetString(GL.RENDERER))
end


