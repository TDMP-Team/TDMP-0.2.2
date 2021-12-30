if GetString("savegame.mod.tdmpSettingsValid") == "" then -- default parameter in "GetBool/GetString/GetInt" functions for weak ppl. Though, I don't fucking know what "GetBoolParam" means, wtf is "Parameter"?
	SetBool("savegame.mod.tdmpShowNicks", true)
	SetBool("savegame.mod.tdmpShowDistanceFromPlayer", true)
	SetBool("savegame.mod.tdmpUseVoxModels", true)
	SetBool("savegame.mod.tdmpHideVoxModelsInVehicle", true)

	SetString("savegame.mod.tdmpSettingsValid", "1")
end

local buttonHeight = 0
local function boolOption(text, paramater, translate)
	if translate then
		UiTranslate(0, buttonHeight + 0)
	end

	local enabled = GetBool(paramater)

	UiPush()
		if enabled then
			UiColor(.5, 1, .5, .2)
			UiImageBox("ui/common/box-solid-6.png", 300, 40, 6, 6)

			UiColor(.5, 1, .5, .75)
		else
			UiColor(.5, .5, .5, .75)
		end

		UiTranslate(200, 0)
		UiAlign("left middle")
		UiText(enabled and "Enabled" or "Disabled")
	UiPop()

	if UiTextButton(text, 300, 40) then
		SetBool(paramater, not enabled)
	end
end

function draw()
	UiTranslate(UiCenter(), 250)
	UiAlign("center middle")

	--Title
	UiFont("bold.ttf", 48)
	UiText("TDMP Settings")

	local _, h = UiGetTextSize("ABC")

	UiTranslate(0, h)

	UiFont("bold.ttf", 24)
	UiText("Change whatever you want!")

	UiTranslate(0, h)

	UiFont("regular.ttf", 26)
	UiButtonImageBox("ui/common/box-outline-6.png", 6, 6)

	local _, h = UiGetTextSize("ABC")
	buttonHeight = h + 30

	UiTranslate(0, h)

	boolOption("Display player nickname", "savegame.mod.tdmpShowNicks")
	boolOption("Show distance from player", "savegame.mod.tdmpShowDistanceFromPlayer", true)
	boolOption("Use 3D model for players", "savegame.mod.tdmpUseVoxModels", true)
	boolOption("Hide player body in vehicle", "savegame.mod.tdmpHideVoxModelsInVehicle", true)

	
	UiTranslate(0, 100)
	if UiTextButton("Close", 200, 40) then
		Menu()
	end
end

