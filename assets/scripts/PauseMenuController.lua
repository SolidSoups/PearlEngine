local refs = {}

function OnCreate()
	local pauseHeaderEnt = Scene.FindEntityByName("PausedHeader")
	local resumeButtonEnt = Scene.FindEntityByName("ResumeButton")
	local menuButtonEnt = Scene.FindEntityByName("MenuButton")

	-- Get text components
	refs.pauseHeader = Scene.GetText(pauseHeaderEnt)
	refs.resumeButton = Scene.GetText(resumeButtonEnt)
	refs.menuButton = Scene.GetText(menuButtonEnt)

	-- Set callbacks
	refs.resumeButton.onClick = function()
		Scene.SetGamePaused(false)
		SetPaused(false)
	end
	refs.menuButton.onClick = function()
		Scene.SetGamePaused(false)
		Scene.StopLevelTimers()
		Scene.LoadScene("assets/MainMenu.json")
	end

	SetPaused(false)
end

function OnUpdate()
	if Input.GetKeyDown("ESCAPE") then
		if Scene.IsGamePaused() then
			Scene.SetGamePaused(false)
			SetPaused(false)
		else
			Scene.SetGamePaused(true)
			SetPaused(true)
		end
	end
end

---@param state boolean set the texts and button state
function SetPaused(state)
	refs.pauseHeader.isVisible = state
	refs.resumeButton.isVisible = state
	refs.resumeButton.isButton = state
	refs.menuButton.isVisible = state
	refs.menuButton.isButton = state
end
