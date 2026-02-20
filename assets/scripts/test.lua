function OnCreate()
	transform = Scene.GetTransform(entity)

	cam_entity = Scene.FindEntityByName("Main Camera")
	if cam_entity > -1 then
		cam_transform = Scene.GetTransform(cam_entity)
	end
end

local speed = 5

function OnUpdate()
	cam_transform.rotation.y = cam_transform.rotation.y + Time.deltaTime * 20.0
	cam_transform.position.y = Time.sin_time * 1 + 1.8
	move()

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
	transform.position.x = transform.position.x + x * Time.deltaTime
	transform.position.z = transform.position.z + y * Time.deltaTime
end

function OnDestroy()
	Debug.Log("Im getting destroyed! whooooo")
end
