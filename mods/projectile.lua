Projectile = Projectile or {}
Projectile.__index = Projectile

local DamageMult = {
	Head = 1.2,
	Body = 1,
	Legs = 0.5
}

function Projectile:GetPos()
	return self.Pos
end

function Projectile:OnHit(hitData)
	local isBullet = self.Type == Ballistics.Type.Bullet

	if not hitData.Player or not self.HitPlayerAndContinue then
		self.Life = self.Life - 1 -- penetrating

		if self.Life <= 0 then
			self.hit = true
		end

		if not hitData.Player then
			if isBullet then
				PlaySound(Bullets_FlyBy[math.random(1,#Bullets_FlyBy)], self.Pos, .5)
			end
		end
	end

	ParticleReset()
	ParticleType("smoke")

	if hitData.Player then
		if hitData.Player.steamId ~= self.lastDamaged then
			-- blood
			ParticleColor(1, 0, 0)
			ParticleRadius(1)
			ParticleTile(14)
			SpawnParticle(hitData.HitPos, Vec(0,-1 - math.random(1,10) * .1,0), .5) -- blood goes down
			SpawnParticle(hitData.HitPos, Vec(0,-1 - math.random(1,10) * .1,0), .5)
			SpawnParticle(hitData.HitPos, Vec(0,-1 - math.random(1,10) * .1,0), .5)

			self.lastDamaged = hitData.Player.steamId

			local dmg = (self.Damage * (DamageMult[hitData.HitPart] or 1))
			if hitData.Player.steamId == LocalSteamID then
				SetPlayerHealth(GetPlayerHealth() - dmg)
			end
			Hook_Run("TDMP_PlayerDamaged", {
				Pos = self.Pos, Damage = dmg, Hit = hitData.HitPart, ID = hitData.Player.steamId
			})

			if isBullet then
				PlaySound(Bullets_PlayerDamage[math.random(1, #Bullets_PlayerDamage)], TDMP_GetPlayerTransform(hitData.Player.id).pos) -- OldPos is made for better understanding "from where I was shot"
			end
		end
	else
		-- dust (ig i need to remove it one day)
		ParticleColor(.7, .6, .5)
		ParticleTile(0)
		SpawnParticle(hitData.HitPos, Vec(0, 1 + math.random(1,10) * .1, 0), 15.1) -- smoke goes up
	end

	if not self.NoHole then
		if (hitData.MakeHole or not hitData.Player or not self.HitPlayerAndContinue) then
			if isBullet then
				MakeHole(hitData.MakeHole or hitData.HitPos, self.Soft, self.Medium, self.Hard)
			else
				Explosion(hitData.MakeHole or hitData.HitPos, self.Explosion)
			end
		end
	end
	
	if self.PostHit then
		self:PostHit()
	else
		self.Damage = self.Damage * .35
		self.Soft = self.Soft * .7
		self.Medium = self.Medium * .7
		self.Hard = self.Hard * .7
	end
end

function Projectile:Tick()
	if self.hit then return end

	if GetTime() >= self.DieTime then self.hit = true return end

	for i=1,3 do
		if math.abs(self.Pos[i]) >= 1000 then self.hit = true return end -- shot in air?
	end

	local ts = GetTimeStep()
	if self.Gravity ~= 0 then
		self.Vel = VecAdd(self.Vel, Vec(0, self.Gravity*ts, 0))
	end
	local point2 = VecAdd(self.Pos, VecScale(self.Vel, 1*ts))

	if not self.NoHole then -- "NoHole" actually supposed to mean that local player has used weapon
		if self.Tracer then
			DrawLine(self.Pos, point2)
		end

		if self.Type == Ballistics.Type.Rocket then
			ParticleReset()
			ParticleType("smoke")
			ParticleColor(1, 1, 1,  .5, .5, .5)
			ParticleEmissive(.1, 0)
			ParticleRadius(.1, .25)
			SpawnParticle(self.Pos, VecScale(self.Dir, -1), .65)
		end
	end

	self.OldPos = self.Pos

	local dir = VecSub(point2, self.Pos)
	local hitData = Ballistics:Hit(self.Pos, point2, VecNormalize(dir), self.Owner, VecLength(dir), self)
	if hitData then
		self:OnHit(hitData)

		self.Pos = hitData.Player and point2 or hitData.MakeHole or hitData.HitPos or point2
	else
		self.Pos = point2
	end
end

return setmetatable(Projectile,
	{
		__call = function(self, data)
			if data.Tracer == nil then
				data.Tracer = true
			end

			data.DieTime = GetTime() + 10
			data.lastDamaged = ""

			return setmetatable(data, Projectile)
		end
	}
)