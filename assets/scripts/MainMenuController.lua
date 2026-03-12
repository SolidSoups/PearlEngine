-- main, pick a level
local refs = {}

function SetButton(button, state)
	button.isVisible = state
	button.isButton = state
end

function SetLevelTexts(state)
	SetButton(refs.level1Text, state)
	SetButton(refs.level2Text, state)
	SetButton(refs.level3Text, state)
end

function SetPlay(state)
	SetButton(refs.playText, state)
end
function SetBack(state)
	SetButton(refs.backText, state)
end

function OnCreate()
	-- get entities
	local playEntity = Scene.FindEntityByName("PLAY TEXT")
	local backEntity = Scene.FindEntityByName("BACK TEXT")
	local level1Ent = Scene.FindEntityByName("LEVEL 1")
	local level2Ent = Scene.FindEntityByName("LEVEL 2")
	local level3Ent = Scene.FindEntityByName("LEVEL 3")

	-- get texts
	refs.playText = Scene.GetText(playEntity)
	refs.backText = Scene.GetText(backEntity)
	refs.level1Text = Scene.GetText(level1Ent)
	refs.level1Text.onClick = function()
		Scene.LoadScene("assets/Level1.json")
	end
	refs.level2Text = Scene.GetText(level2Ent)
	refs.level2Text.onClick = function()
		Scene.LoadScene("assets/Level2.json")
	end
	refs.level3Text = Scene.GetText(level3Ent)
	refs.level3Text.onClick = function()
		Scene.LoadScene("assets/Level3.json")
	end
	SetLevelTexts(false)
	SetBack(false)
	SetPlay(true)

	refs.playText.onClick = function()
		SetLevelTexts(true)
		SetBack(true)
		SetPlay(false)
	end

	refs.backText.onClick = function()
		SetLevelTexts(false)
		SetBack(false)
		SetPlay(true)
	end
end
