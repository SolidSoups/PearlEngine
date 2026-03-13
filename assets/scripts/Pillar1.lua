local bottomHeight = -6.5
local topHeight = 5.0
local travelTime = 4.0 -- seconds
local travelDown = true

function OnCreate()
	Transform = Scene.GetTransform(entity)
end

function OnUpdate()
	-- let''s do this'
	if travelDown then
		local ySpeed = (bottomHeight - topHeight) / travelTime
		local vel = Vec3.new(0, ySpeed, 0)

		local pos = Transform.position
		pos = pos + vel * Time.deltaTime
		Transform.position = pos

		if pos.y <= bottomHeight then
			travelDown = false
		end
	else
		local ySpeed = (topHeight - bottomHeight) / travelTime
		local vel = Vec3.new(0, ySpeed, 0)

		local pos = Transform.position
		pos = pos + vel * Time.deltaTime
		Transform.position = pos

		if pos.y >= topHeight then
			travelDown = true
		end
	end
end
