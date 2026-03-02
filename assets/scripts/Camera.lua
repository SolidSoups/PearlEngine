local cameraAngle = 30
local pivotLength = 10

function OnCreate()
	Transform = Scene.GetTransform(entity)

	local playerEntity = Scene.FindEntityByName("Player")
	if playerEntity == Scene.NULL_ENTITY then
		Debug.Error("Camera.lua: Couldn't find player entity")
	end
	PlayerTransform = Scene.GetTransform(playerEntity)
end

function OnLateUpdate()
	local relativePosition =
		Vec3.new(0, math.sin(math.rad(cameraAngle)) * pivotLength, -math.cos(math.rad(cameraAngle)) * pivotLength)
	Transform.position = PlayerTransform.position + relativePosition
	Transform:LookAt(PlayerTransform.position)
end
