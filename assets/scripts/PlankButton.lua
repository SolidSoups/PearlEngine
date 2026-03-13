local enablePlank = false
local speed = 10.0 -- two units per second
local vel = Vec3.new(0, speed, 0)

function OnCreate()
	local plankEnt = Scene.FindEntityByName("Plank")

	PlankTransform = Scene.GetTransform(plankEnt)
end

function OnCollisionEnter(other, normal, pen)
	local otherName = Scene.GetNameComp(other).name
	if otherName == "Player" then
		enablePlank = true
	end
end

function OnUpdate()
	if not enablePlank then
		return
	end

	local pos = PlankTransform.position
	if pos.y >= 7.5 then
		pos.y = pos.y - speed * Time.deltaTime
		PlankTransform.position = pos
	end
end
