function OnCollisionEnter(other, normal, pen)
	local otherName = Scene.GetNameComp(other).name
	if otherName == "Player" then
		Debug.Log("YOU WIN!!!")
	end
end
