<<<<<<< HEAD
--[[-------------------------------------------------------------------------
Yes
---------------------------------------------------------------------------]]

#include "ballistics.lua"

Bullets_FlyBy = {}
Bullets_FlyBy_Sub = {}

Bullets_PlayerDamage = {}

local v
function init()
	local gunSounds = {}
	for i=0, 7 do
		gunSounds[#gunSounds + 1] = LoadSound("tools/gun"..i..".ogg")
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

	v = FindVehicles("", true)

	local lastShoot = 0
	TDMP_RegisterCallback("TestCallback", function(...) -- ... is for testing arguments
		-- pcall is IMPORTANT here, otherwise it would crash (access violation in lua's newkey function)
		pcall(function()
			local testString = "It is wednesday my boyyyyyyyyyyyyyyyyyyyyyyyyyyyys"
			local testNumber = 237685
			local tbl = {
				["index"] = "value",
				tbl = {
					a = 1,
					b = 4,
				}
			}

			-- Works fine
			for i, v in ipairs(v) do
				DriveVehicle(v, 1, 1, false)
			end

			--[[-------------------------------------------------------------------------
			21.12.2021: if you'll "do return end" here, heap corruption won't happen
			---------------------------------------------------------------------------]]

			-- making here an absolutely terrible code for testing it on high pressure. Though, it seem to be working, what actually surprises me
			local function GetAimDirection()
				local cam = GetCameraTransform()
				local forward = TransformToParentPoint(cam, Vec(0, 0, -1))
				local dir = VecSub(forward, cam.pos)

				return VecNormalize(dir), VecLength(dir)
			end

			local tr = GetPlayerTransform()
			local dir = GetAimDirection()--VecNormalize(tr.rot)

			if GetTime() - lastShoot >= .01 then
				lastShoot = GetTime()

				Ballistics:Shoot{
					Type = Ballistics.Type.Bullet,

					--Owner = ply,
					Pos = VecAdd(tr.pos, Vec(0,2,0)),
					Dir = dir,
					Vel = VecScale(dir, 250),
					Soft = .9,
					Medium = .6,
					Hard = .3,
					Damage = .20,
					--NoHole = isMe,

					HitPlayerAndContinue = true,
					Life = 0
				}
			end
		end)
	end)

	TDMP_CallCallback("TestCallback")
end

local skipped
function tick()
	if not skipped then skipped = true return end

	TDMP_Tick()
	TDMP_CallCallback("TestCallback")

	Ballistics:Tick()
=======
--[[-------------------------------------------------------------------------
Yes
---------------------------------------------------------------------------]]

#include "ballistics.lua"

Bullets_FlyBy = {}
Bullets_FlyBy_Sub = {}

Bullets_PlayerDamage = {}

local v
function init()
	local gunSounds = {}
	for i=0, 7 do
		gunSounds[#gunSounds + 1] = LoadSound("tools/gun"..i..".ogg")
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

	v = FindVehicles("", true)

	local lastShoot = 0
	TDMP_RegisterCallback("TestCallback", function(...) -- ... is for testing arguments
		-- pcall is IMPORTANT here, otherwise it would crash (access violation in lua's newkey function)
		pcall(function()
			local testString = "It is wednesday my boyyyyyyyyyyyyyyyyyyyyyyyyyyyys"
			local testNumber = 237685
			local tbl = {
				["index"] = "value",
				tbl = {
					a = 1,
					b = 4,
				}
			}

			-- Works fine
			for i, v in ipairs(v) do
				DriveVehicle(v, 1, 1, false)
			end

			--[[-------------------------------------------------------------------------
			21.12.2021: if you'll "do return end" here, heap corruption won't happen
			---------------------------------------------------------------------------]]

			-- making here an absolutely terrible code for testing it on high pressure. Though, it seem to be working, what actually surprises me
			local function GetAimDirection()
				local cam = GetCameraTransform()
				local forward = TransformToParentPoint(cam, Vec(0, 0, -1))
				local dir = VecSub(forward, cam.pos)

				return VecNormalize(dir), VecLength(dir)
			end

			local tr = GetPlayerTransform()
			local dir = GetAimDirection()--VecNormalize(tr.rot)

			if GetTime() - lastShoot >= .01 then
				lastShoot = GetTime()

				Ballistics:Shoot{
					Type = Ballistics.Type.Bullet,

					--Owner = ply,
					Pos = VecAdd(tr.pos, Vec(0,2,0)),
					Dir = dir,
					Vel = VecScale(dir, 250),
					Soft = .9,
					Medium = .6,
					Hard = .3,
					Damage = .20,
					--NoHole = isMe,

					HitPlayerAndContinue = true,
					Life = 0
				}
			end
		end)
	end)

	TDMP_CallCallback("TestCallback")
end

local skipped
function tick()
	if not skipped then skipped = true return end

	TDMP_Tick()
	TDMP_CallCallback("TestCallback")

	Ballistics:Tick()
>>>>>>> b2531f3704486ca58f86e389fed2ae93717be397
end