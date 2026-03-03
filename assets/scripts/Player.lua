MovementSpeed = 10
MovementForce = 10
JumpForce = 7
JumpTimes = 0

function OnCreate()
	Transform = Scene.GetTransform(entity)
	Rigidbody = Scene.GetRigidbody(entity)
	local spawnEntity = Scene.FindEntityByName("SpawnPoint")
	SpawnTransform = Scene.GetTransform(spawnEntity)

	Transform.position = SpawnTransform.position
end

function OnCollisionEnter(other, normal, pen)
	local name = Scene.GetNameComp(other).name
	if name == "Floor" then
		if JumpTimes >= 2 then
			JumpTimes = 1
		else
			JumpTimes = 0
		end
	elseif name == "Lava" then
		Debug.Log("You died")
		Rigidbody:ClearForces()
		Rigidbody.velocity = Vec3.new(0, 0, 0)
		Transform.position = SpawnTransform.position
	end
end

function OnUpdate()
	local dirN = GetMovementDirection()

	local targetX = 0
	local targetZ = 0
	local t = math.min(1, 10 * Time.deltaTime)
	if dirN:Length() > 0.1 then
		-- Transform:Translate(dirN * MovementSpeed * Time.deltaTime)
		targetX = dirN.x * MovementForce
		targetZ = dirN.z * MovementForce
	end
	local vel = Rigidbody.velocity
	vel.x = vel.x + (targetX - vel.x) * t
	vel.z = vel.z + (targetZ - vel.z) * t
	Rigidbody.velocity = vel

	if Input.GetKeyDown("SPACE") and JumpTimes < 2 then
		JumpTimes = JumpTimes + 1
		local vel = Rigidbody.velocity
		vel.y = 0
		Rigidbody.velocity = vel
		Rigidbody:AddImpulse(Vec3.new(0, 1, 0) * JumpForce)
	end
end

--- Will return the movement direction using WASD
function GetMovementDirection()
	local x = 0.0
	local y = 0.0

	-- horizontal
	if Input.GetKey("A") or Input.GetKey("LEFT") then
		x = x + 1.0
	end
	if Input.GetKey("D") or Input.GetKey("RIGHT") then
		x = x - 1.0
	end

	-- vertical
	if Input.GetKey("W") or Input.GetKey("UP") then
		y = y + 1.0
	end
	if Input.GetKey("S") or Input.GetKey("DOWN") then
		y = y - 1.0
	end

	-- Combine and normalize
	local dir = Vec3.new(x, 0, y)
	dir:Normalize()

	return dir
end
