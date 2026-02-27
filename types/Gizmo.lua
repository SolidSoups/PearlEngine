---@meta

---@class Gizmo
Gizmo = {}

---Draws a gizmo line
---@param a Vec3 starting point
---@param b Vec3 ending point
---@param color Vec3 color
function Gizmo.DrawLine(a, b, color) end

---Draws a gizmo wire box
---@param center Vec3 center point
---@param size Vec3 size (width, height, depth)
---@param color Vec3 color
function Gizmo.DrawWireBox(center, size, color) end

---Draws a gizmo wire sphere
---@param center Vec3 center point
---@param radius number radius of sphere
---@param color Vec3 color
function Gizmo.DrawWireSphere(center, radius, color) end
