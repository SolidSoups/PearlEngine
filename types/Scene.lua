---@meta

Scene = {}

---@param name string
---@return integer Entity ID, or 0 (NULL_ENTITY) if not found
function Scene.FindEntityByName(name) end

---@param entity integer
---@return Transform|nil
function Scene.GetTransform(entity) end
