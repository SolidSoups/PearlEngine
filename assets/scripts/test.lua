function OnCreate()
	Transform = Scene.GetTransform(entity)

	CameraEntity = Scene.FindEntityByName("Main Camera")
	if CameraEntity > -1 then
		-- Setup camera properties to be correct
		CameraCamera = Scene.GetCamera(CameraEntity)
		CameraCamera.fov = 60
		CameraCamera.aspect_mod = 1

		-- adjust location of camera transform
		CameraTransform = Scene.GetTransform(CameraEntity)
		CameraTransform.position.x = Transform.position.x - 5
		CameraTransform.position.y = Transform.position.y + 5
		CameraTransform.position.z = Transform.position.z
		CameraTransform:LookAt(Transform.position)

		Scene.SetMainCamera(CameraEntity)
	end
end

local speed = 5

function OnUpdate()
	move()

	-- copy camera transform
	CameraTransform.position.x = Transform.position.x - 5
	CameraTransform.position.y = Transform.position.y + 5
	CameraTransform.position.z = Transform.position.z

	if Input.GetKeyDown("SPACE") then
		Debug.Log("I AM JUMPING!")
	end
end

function move()
	if Input.GetKey("LEFT_SHIFT") then
		speed = 10
	else
		speed = 5
	end

	local x = 0
	local y = 0

	-- Get input
	if Input.GetKey("W") then
		x = x + speed
	end
	if Input.GetKey("S") then
		x = x - speed
	end
	if Input.GetKey("D") then
		y = y + speed
	end
	if Input.GetKey("A") then
		y = y - speed
	end

	if x == 0 and y == 0 then
		return
	end

	-- normalize
	local len = math.sqrt(x * x + y * y)
	x = (x / len) * speed
	y = (y / len) * speed

	-- move character
	Transform.position.x = Transform.position.x + x * Time.deltaTime
	Transform.position.z = Transform.position.z + y * Time.deltaTime
end

function OnDestroy()
	Debug.Log("Im getting destroyed! whooooo")
end
