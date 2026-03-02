---@meta

---@class Scene
---@field NULL_ENTITY number
Scene = {}

---@param name string
---@return integer Entity ID, or 0 (NULL_ENTITY) if not found
function Scene.FindEntityByName(name) end

---@param entity integer
---@return Transform|nil
function Scene.GetTransform(entity) end

---@param entity integer
---@return Camera|nil
function Scene.GetCamera(entity) end

---@param entity integer
---@return bool succesful
function Scene.SetMainCamera(entity) end

---@param path string path to the scene JSON file
function Scene.LoadScene(path) end
