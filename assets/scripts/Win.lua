local refs = {}
function OnCreate()
	local winTextEnt = Scene.FindEntityByName("YOU WIN")
	local goBackTextEnt = Scene.FindEntityByName("BACK TO MENU")

	refs.winText = Scene.GetText(winTextEnt)
	refs.goBackText = Scene.GetText(goBackTextEnt)

	refs.winText.isButton = false
	refs.winText.isVisible = false
	refs.goBackText.isButton = false
	refs.goBackText.isVisible = false
	refs.goBackText.onClick = function()
		Scene.LoadScene("assets/MainMenu.json")
	end

	local timeEnt = Scene.FindEntityByName("TimerView")
	TimerViewText = Scene.GetText(timeEnt)
	TimerViewText.text = "Time: 00:00"
	TimerViewText:Remesh()
	Scene.StartLevelTimer(3)
end

function OnUpdate()
	TimerViewText.text = "Time: " .. Scene.GetCurrentTime(3)
	TimerViewText:Remesh()
end

function OnCollisionEnter(other, normal, pen)
	local otherName = Scene.GetNameComp(other).name
	if otherName == "Player" then
		Scene.StopLevelTimers()
		Scene.SaveLevelTimes()
		refs.winText.isButton = true
		refs.winText.isVisible = true
		refs.goBackText.isButton = true
		refs.goBackText.isVisible = true
	end
end
