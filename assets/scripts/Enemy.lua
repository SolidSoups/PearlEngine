local playerRange = 20
local followSpeed = 8
local jumpinterval = 0.3
local groundHeight = 7.5

function OnCollisionEnter(other, normal, pen)
	local name = Scene.GetNameComp(other).name
	if name == "Lava" then
		-- Die (Destroy entity will rearrange the rigidbody cache!!!)
		--Scene.DestroyEntity(entity)
	end
	-- Fake die
	if name == "Lava" then
		Transform.position = Vec3.new(100, 100, 100)
		Rigidbody.gravity = 0
	end
end

function OnCreate()
	Transform = Scene.GetTransform(entity)
	Rigidbody = Scene.GetRigidbody(entity)
	StartY = Transform.position.y

	local playerEntity = Scene.FindEntityByName("Player")
	PlayerTransform = Scene.GetTransform(playerEntity)

	TimeAcc = 0.0
end

function OnUpdate()
	local enemyHeight = Transform.position.y - 1
	local distToGround = math.abs(groundHeight - enemyHeight)
	local isGrounded = distToGround < 0.02

	if isGrounded then
		TimeAcc = TimeAcc + Time.deltaTime
	else
		TimeAcc = 0
	end

	local meToPlayer = PlayerTransform.position - Transform.position
	local playerDist = meToPlayer:Length()

	if playerDist < playerRange then
		FollowPlayer()
	else
		-- jump every 2 seconds
		while TimeAcc >= jumpinterval do
			Rigidbody:AddImpulse(Vec3.new(0, 5, 0))
			TimeAcc = TimeAcc - jumpinterval
		end
	end
end

function FollowPlayer()
	local pos = Transform.position
	local playerPos = PlayerTransform.position
	local dir = (playerPos - pos)
	dir:Normalize()

	local vel = Rigidbody.velocity
	vel.x = dir.x * followSpeed
	vel.z = dir.z * followSpeed
	Rigidbody.velocity = vel
end
