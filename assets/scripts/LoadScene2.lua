function OnCollisionEnter(other, normal, pen)
	local otherName = Scene.GetNameComp(other).name
	if otherName == "Player" then
		Scene.LoadScene("assets/Level2.json")
	end
end
