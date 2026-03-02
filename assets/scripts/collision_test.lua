function OnCollisionEnter()
	Debug.Log("Lua collision")
end

function OnUpdate()
	if Input.GetKeyDown("J") then
		Scene.LoadScene("assets/test_load.json")
	end
end
