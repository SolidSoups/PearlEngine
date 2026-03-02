---@meta

---@class Rigidbody
---@field mass number 
---@field gravity number
---@field velocity Vec3 runtime velocity
---@field force Vec3 runtime accumulative frame force
Rigidbody = {}


---Add a force to the rigidbody
---@param f Vec3 force to add
function Rigidbody:AddForce(f) end

---Add an impulse force to the rigidbody
---@param i Vec3 impulse to add
function Rigidbody:AddImpulse(i) end

---Clears all forces acting on the rigidbody
function Rigidbody:ClearForces() end

