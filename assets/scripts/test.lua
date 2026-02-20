function OnCreate()
	transform = Scene.GetTransform(entity)
end

local dt = 0
local last_time = 0

function OnUpdate()
	transform.rotation.y = transform.rotation.y + Time.deltaTime * 90.0
	transform.position.y = Time.sin_time * 2
end

function OnDestroy()
	Debug.Log("Im getting destroyed! whooooo")
end
