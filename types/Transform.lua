---@meta

---@class Transform
---@field position Vec3
---@field rotation Vec3
---@field scale Vec3
Transform = {}

---@param v Vec3
function Transform:Translate(v) end

---@param v Vec3
function Transform:SetPosition(v) end

---@param v Vec3
function Transform:SetRotation(v) end

---@param v Vec3
function Transform:SetScale(v) end
