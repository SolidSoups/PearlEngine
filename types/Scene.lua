---@meta

---@class Scene
---@field NULL_ENTITY number
Scene = {}

---@param name string
---@return integer Entity ID, or 0 (NULL_ENTITY) if not found
function Scene.FindEntityByName(name) end

---@param entity integer
---@return NameComponent|nil
function Scene.GetNameComp(entity) end

---@param entity integer
---@return TextComponent|nil
function Scene.GetText(entity) end

---@param entity integer
---@return Transform|nil
function Scene.GetTransform(entity) end

---@param entity integer
---@return Rigidbody|nil
function Scene.GetRigidbody(entity) end

---@param entity integer
---@return Camera|nil
function Scene.GetCamera(entity) end

---@param entity integer
---@return bool succesful
function Scene.SetMainCamera(entity) end

---@param path string path to the scene JSON file
function Scene.LoadScene(path) end

--- Reload the current scene
function Scene.ReloadCurrentScene() end

--- Destroys an entity
---@param entity integer Entity ID
function Scene.DestroyEntity(entity) end

--- Get the best (persisted) time for a level
---@param level number 1, 2 or 3
function Scene.GetBestTime(level) end

--- Get the current in-progress time for a level
---@param level number 1, 2 or 3
function Scene.GetCurrentTime(level) end

--- Save the level times
function Scene.SaveLevelTimes() end
--- Load the level times
function Scene.LoadLevelTimes() end

--- Start the timer for a level
---@param level number 1, 2 or 3
function Scene.StartLevelTimer(level) end

--- Load all the level times
function Scene.StopLevelTimers() end

--- Check if the game is paused
---@return boolean isPaused
function Scene.IsGamePaused() end

--- Set the paused state of the game
---@param isPaused boolean
function Scene.SetGamePaused(isPaused) end
