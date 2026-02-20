---@meta

Input = {}

---Returns true if a key code is currently pressed
---@param key string Key name e.g. "SPACE", "LEFT_SHIFT"
---@return boolean
function Input.GetKey(key) end

---Returns true if a key code is pressed down this frame
---@param key string Key name e.g. "SPACE", "LEFT_SHIFT"
---@return boolean
function Input.GetKeyDown(key) end

---Returns true if a key code is released up this frame
---@param key string Key name e.g. "SPACE", "LEFT_SHIFT"
---@return boolean
function Input.GetKeyUp(key) end
