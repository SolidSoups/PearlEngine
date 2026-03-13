function OnCreate()
	local timeEnt = Scene.FindEntityByName("TimerView")
	TimerViewText = Scene.GetText(timeEnt)
	TimerViewText.text = "Time: 00:00"
	TimerViewText:Remesh()
	Scene.StartLevelTimer(1)
end

function OnUpdate()
	TimerViewText.text = "Time: " .. Scene.GetCurrentTime(1)
	TimerViewText:Remesh()
end

function OnCollisionEnter(other, normal, pen)
	local otherName = Scene.GetNameComp(other).name
	if otherName == "Player" then
		Scene.StopLevelTimers()
		Scene.LoadScene("assets/Level2.json")
	end
end
