
MovementSpeed = 10
MovementForce = 10
JumpForce = 6

function OnCreate()
  Transform = Scene.GetTransform(entity)
  Rigidbody = Scene.GetRigidbody(entity)
  Transform.position = Vec3.new(0, 4, 0)
  Rigidbody:ClearForces()
  Rigidbody.velocity = Vec3.new(0,0,0)
end

function OnUpdate()
  local dirN = GetMovementDirection()

  if dirN:Length() > 0.1 then
    -- Transform:Translate(dirN * MovementSpeed * Time.deltaTime)
    local vel = Rigidbody.velocity 
    vel.x = dirN.x * MovementForce
    vel.z = dirN.z * MovementForce
    Rigidbody.velocity = vel
  else
    Rigidbody.velocity.x = 0
    Rigidbody.velocity.z = 0
  end

  if Input.GetKeyDown("SPACE") then
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
