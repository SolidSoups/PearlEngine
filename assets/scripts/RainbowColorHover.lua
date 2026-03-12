local rainbowMode = false

function OnHoverEnter()
	rainbowMode = true
end
function OnHoverLeave()
	rainbowMode = false
end

function OnCreate()
	TextComp = Scene.GetText(entity)
	Hue = 0.0
end

function OnUpdate()
	if rainbowMode then
		DoRainbow(1)
	else
		DoRainbow(0.25)
	end
end

---@param speed number
function DoRainbow(speed)
	local dt = Time.deltaTime
	Hue = (Hue + dt * speed) % 1.0

	-- HSV to RGB
	local h = Hue * 6.0
	local i = math.floor(h)
	local f = h - i
	local q = 1.0 - f

	local r, g, b
	if i == 0 then
		r, g, b = 1, f, 0
	elseif i == 1 then
		r, g, b = q, 1, 0
	elseif i == 2 then
		r, g, b = 0, 1, f
	elseif i == 3 then
		r, g, b = 0, q, 1
	elseif i == 4 then
		r, g, b = f, 0, 1
	else
		r, g, b = 1, 0, q
	end

	TextComp.color = Vec3.new(r, g, b)
end
