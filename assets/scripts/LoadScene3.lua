function OnCollisionEnter(other, normal, pen)
	local otherName = Scene.GetNameComp(other).name
	if otherName == "Player" then
		Scene.LoadScene("assets/Level3.json")
		Debug.Log("Collided!")
	end
end
