function OnCreate()
	transform = Scene.GetTransform(entity)
end

local speed = 5

function OnUpdate()
	-- transform.rotation.y = transform.rotation.y + Time.deltaTime * 90.0
	-- transform.position.y = Time.sin_time * 2
	move()
end

function move()
	if Input.IsKeyDown("LEFT_SHIFT") then
		speed = 10
	else
		speed = 5
	end

	local x = 0
	local y = 0

	-- Get input
	if Input.IsKeyDown("W") then
		x = x + speed
	end
	if Input.IsKeyDown("S") then
		x = x - speed
	end
	if Input.IsKeyDown("D") then
		y = y + speed
	end
	if Input.IsKeyDown("A") then
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
