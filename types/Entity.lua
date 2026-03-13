---@meta

---The entity ID this script is attached to. Injected by engine.
---@type integer
entity = 0

--- Called when a collider enters a collision
---@param other Entity the entity that triggered this collision
---@param normal Vec3 the normal of the collision from, pointed torwards the other entity
---@param pen number the penetration of the collision
function OnCollisionEnter(other, normal, pen) end
