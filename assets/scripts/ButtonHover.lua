local hoverColor = Vec3.new(0.8, 0.6, 0.6)
local defaultColor = Vec3.new(1, 1, 1)

function OnCreate()
	TextComp = Scene.GetText(entity)
end

function OnHoverEnter()
	TextComp.color = hoverColor
end

function OnHoverLeave()
	TextComp.color = defaultColor
end
