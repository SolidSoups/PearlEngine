function OnCreate()
	transform = Scene.GetTransform(entity)
end

function OnUpdate()
	transform.rotation.y = transform.rotation.y + Time.deltaTime * 90.0
end
