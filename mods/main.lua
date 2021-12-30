#include "tdmp/networking.lua"
#include "tdmp/hooks.lua"

#include "ballistics.lua"

Bullets_FlyBy = {}
Bullets_FlyBy_Sub = {}
Bullets_PlayerDamage = {}

-- It's HIGHLY recommended to cache local steamid
LocalSteamID = TDMP_LocalSteamId()

function init()
	local gunSounds = {}
	for i=0, 7 do
		gunSounds[#gunSounds + 1] = LoadSound("tools/gun"..i..".ogg")
	end

	local shotgunSounds = {}
	for i=0, 6 do
		shotgunSounds[#shotgunSounds + 1] = LoadSound("tools/shotgun"..i..".ogg")
	end

	for i=1, 23 do
		Bullets_FlyBy[i] = LoadSound("MOD/snd/bullets/flyby/" .. i .. ".ogg")
	end

	for i=1, 11 do
		Bullets_FlyBy_Sub[i] = LoadSound("MOD/snd/bullets/flyby/sub_" .. i .. ".ogg")
	end

	for i=1, 6 do
		Bullets_PlayerDamage[i] = LoadSound("MOD/snd/bullets/damage/" .. i .. ".ogg")
	end

	local launcher = {}
	for i=0, 4 do
		launcher[#launcher + 1] = LoadSound("tools/launcher"..i..".ogg")
	end

	TDMP_RegisterEvent("GunShot", function(jsonData)
		local data = json.decode(jsonData)

		local mediumDamage = ((math.min(math.max((data[2] - 1) * 0.5, 0), 1) * 0.20000002) + 0.34999999)
		local softDamage = mediumDamage + .1

		local isMe = data[1] == LocalSteamID
		Ballistics:Shoot{
			Type = Ballistics.Type.Bullet,

			Owner = data[1],
			Pos = data[4],
			Dir = data[3],
			Vel = VecScale(data[3], 250),
			Soft = softDamage,
			Medium = mediumDamage,
			Hard = 0,
			Damage = .20,
			NoHole = isMe,

			HitPlayerAndContinue = false,
			Life = 0
		}

		if not isMe then
			PlaySound(gunSounds[math.random(1, #gunSounds)], data[4], 1)
		end

		if not TDMP_IsServer() then return end

		TDMP_ServerStartEvent("GunShot", {
			Receiver = TDMP.Enums.Receiver.ClientsOnly, -- We've received that event already so we need to broadcast it only to clients, not again to ourself
			Reliable = true,

			DontPack = true,
			Data = jsonData
		})
	end)

	TDMP_RegisterEvent("ShotgunShot", function(jsonData)
		local data = json.decode(jsonData)

		local mediumDamage = math.min(math.max((data[2] - 3) * .5, 0), 1)
		mediumDamage = ((mediumDamage + mediumDamage) + 3) * .2

		local isMe = data[1] == LocalSteamID
		Ballistics:Shoot{
			Type = Ballistics.Type.Bullet,

			Owner = data[1],
			Pos = data[4],
			Dir = data[3],
			Vel = VecScale(data[3], 250),
			Soft = mediumDamage + .3,
			Medium = mediumDamage,
			Hard = 0,
			Damage = .35,
			NoHole = isMe,

			HitPlayerAndContinue = false,
			Life = 0
		}

		if not isMe then
			PlaySound(shotgunSounds[math.random(1, #shotgunSounds)], data[4], 1)
		end

		if not TDMP_IsServer() then return end

		TDMP_ServerStartEvent("ShotgunShot", {
			Receiver = TDMP.Enums.Receiver.ClientsOnly, -- We've received that event already so we need to broadcast it only to clients, not again to ourself
			Reliable = true,

			DontPack = true,
			Data = jsonData
		})
	end)

	TDMP_RegisterEvent("RocketShot", function(jsonData)
		local data = json.decode(jsonData)

		ParticleReset()
		ParticleType("fire")
		ParticleColor(.7, .6, .5)
		ParticleEmissive(.1)
		SpawnParticle(data[4], Vec(0, 1 + math.random(1,10) * .1, 0), .5)

		ParticleReset()
		ParticleType("darksmoke")
		ParticleColor(.7, .6, .5)
		ParticleEmissive(.2)
		SpawnParticle(data[4], Vec(0, 1 + math.random(1,5) * .1, 0), .4)

		local isMe = data[1] == LocalSteamID

		Ballistics:Shoot{
			Type = Ballistics.Type.Rocket,

			Owner = data[1],
			Pos = data[4],
			Dir = data[3],
			Vel = VecScale(data[3], 20.1),
			Damage = .30,
			NoHole = isMe,
			Explosion = 1.5,
			Gravity = -2,

			HitPlayerAndContinue = true,
			Life = 0
		}
		
		if not isMe then
			PlaySound(launcher[math.random(1, #launcher)], data[4], 1)
		end

		if not TDMP_IsServer() then return end

		TDMP_ServerStartEvent("RocketShot", {
			Receiver = TDMP.Enums.Receiver.ClientsOnly, -- We've received that event already so we need to broadcast it only to clients, not again to ourself
			Reliable = true,

			DontPack = true,
			Data = jsonData
		})
	end)
end

function GetAimDirection()
	local cam = GetPlayerCameraTransform()
	local forward = TransformToParentPoint(cam, Vec(0, 0, -1))
	local dir = VecSub(forward, cam.pos)

	return VecNormalize(dir), VecLength(dir)
end

local skipped
local nextGunShoot
local nextShotGunShoot
local nextRocketShoot
function tick()
	TDMP_Hook_Queue() -- Run this every tick in your script, if you're using Hook_AddListener in your script. This *seem* to be preventing from crashes, when calling hook from different thread(mod)
	Ballistics:Tick()

	local t = GetTime()
	local curTool = GetString("game.player.tool")
	if InputPressed("lmb") then
		if curTool == "gun" then
			if nextGunShoot and nextGunShoot > t then return end
			nextGunShoot = t + .25

			TDMP_ClientStartEvent("GunShot", {
				Reliable = true,

				Data = {LocalSteamID, GetInt("game.tool.gun.damage"), GetAimDirection(), GetPlayerCameraTransform().pos}
			})
		elseif curTool == "shotgun" then
			if nextShotGunShoot and nextShotGunShoot > t then return end
			nextShotGunShoot = t + .6

			TDMP_ClientStartEvent("ShotgunShot", {
				Reliable = true,

				Data = {LocalSteamID, GetInt("game.tool.shotgun.damage"), GetAimDirection(), GetPlayerCameraTransform().pos}
			})
		elseif curTool == "rocket" then
			if nextRocketShoot and nextRocketShoot > t then return end
			nextRocketShoot = t + .75

			TDMP_ClientStartEvent("RocketShot", {
				Reliable = true,

				Data = {LocalSteamID, 1, GetAimDirection(), GetPlayerCameraTransform().pos}
			})
		end
	end

	if not skipped then skipped = true return end
	TDMP_Tick()
end

local PacketUpdateRate = .0166666666 * 3
local nextCall = 0
function update()
	local t = TDMP_UnixTime()

	if t >= nextCall then
		nextCall = t + PacketUpdateRate
	else
		return
	end

	TDMP_Update()
end

function Remap(value, inMin, inMax, outMin, outMax)
	return outMin + (((value - inMin) / (inMax - inMin)) * (outMax - outMin))
end

function Clamp01(value)
	return math.min(math.max(value, 0), 1)
end

local modelDisabledbyVehicle = {}
function draw()
	UiAlign("top left")
	UiTranslate(1, 1)
	UiFont("bold.ttf", 24)
	UiColor(1, 1, 1, .1)
	UiText("TDMP ALPHA 0.1.0")

	UiAlign("center middle")

	if not GetBool("savegame.mod.tdmpShowNicks") then return end

	for i, ply in ipairs(TDMP_GetPlayers()) do
		if not TDMP_IsMe(ply.id) then
			if GetBool("savegame.mod.tdmpHideVoxModelsInVehicle") then
				if ply.veh and ply.veh ~= 0 then
					if not modelDisabledbyVehicle[ply.id] then
						TDMP_SetPlayerBodyEnabled(ply.id, false)

						modelDisabledbyVehicle[ply.id] = true
					end
				elseif modelDisabledbyVehicle[ply.id] then
					TDMP_SetPlayerBodyEnabled(ply.id, true)

					modelDisabledbyVehicle[ply.id] = nil
				end
			else
				if modelDisabledbyVehicle[ply.id] then
					TDMP_SetPlayerBodyEnabled(ply.id, true)

					modelDisabledbyVehicle[ply.id] = nil
				end
			end

			local pos = VecAdd(TDMP_GetPlayerTransform(ply.id).pos, Vec(0,2.2,0))
			local x, y, dist = UiWorldToPixel(pos)
			if dist > 0 then -- if not behind us
				local a = Clamp01(Remap(dist, 10, 35, 1, 0))
				if dist <= 36 then
					UiPush()
						UiTranslate(x, y)

						UiColor(1, 1, 1, a)
						UiText(ply.nick .. (GetBool("savegame.mod.tdmpShowDistanceFromPlayer") and (" [" .. math.floor(dist) .. "m]") or ""))
					UiPop()
				end
			end
		end
	end
end

Hook_AddListener("Shoot", "TDMP_DefaultShoot", function(shootData)
	Ballistics:Shoot(json.decode(shootData))
end)

Hook_AddListener("PlayerBodyCreated", "TDMP_DefaultDisablePlayermodels", function(owner)
	if GetBool("savegame.mod.tdmpUseVoxModels") then return end

	local owner = json.decode(ownerData)

	TDMP_SetPlayerBodyEnabled(owner[2], false)
end)

if GetString("savegame.mod.tdmpSettingsValid") == "" then -- default parameter in "GetBool/GetString/GetInt" functions for weak ppl. Though, I don't fucking know what "GetBoolParam" means, wtf is "Parameter"?
	SetBool("savegame.mod.tdmpShowNicks", true)
	SetBool("savegame.mod.tdmpShowDistanceFromPlayer", true)
	SetBool("savegame.mod.tdmpUseVoxModels", true)
	SetBool("savegame.mod.tdmpHideVoxModelsInVehicle", true)

	SetString("savegame.mod.tdmpSettingsValid", "1")
end