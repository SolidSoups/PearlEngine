---@meta

---@class Camera
---@field fov number
---@field aspect_mod number
---@field near_plane number
---@field far_plane number
Camera = {}

---Returns true if this camera is the main target
---@return boolean
function Transform:IsMainCamera() end
