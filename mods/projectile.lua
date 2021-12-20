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

-- Can be overwritten
function Projectile:OnHit(hitData)
	local isBullet = self.Type == Ballistics.Type.Bullet

	if not hitData.Player or not self.HitPlayerAndContinue then
		self.Life = self.Life - 1 -- penetrating

		if self.Life <= 0 then
			-- Here it means that our projectile hitten something and projectile would be removed
			self.hit = true
		end

		-- so lets play sounds
		if not hitData.Player then
			if isBullet then
				PlaySound(Bullets_FlyBy[math.random(1,#Bullets_FlyBy)], self.Pos, .5)
			end
		end
	end

	ParticleReset()
	ParticleType("smoke")

	if hitData.Player then
		if hitData.Player.LastDamaged ~= self then
			ParticleColor(1, 0, 0)
			ParticleTile(14)
			SpawnParticle(hitData.HitPos, Vec(0,-1 - math.random(1,10) * .1,0), .5) -- blood goes down

			hitData.Player.LastDamaged = self
			hitData.Player:Damage(self.Damage * (DamageMult[hitData.HitPart] or 1))

			if isBullet then
				PlaySound(Bullets_PlayerDamage[math.random(1, #Bullets_PlayerDamage)], hitData.Player:GetPos()) -- OldPos is made for better understanding "from where I was shot"
			end
		end
	else
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

	self.Damage = self.Damage * .35
	self.Soft = self.Soft * .7
	self.Medium = self.Medium * .7
	self.Hard = self.Hard * .7
end


function DebugCross(start, size, r, g, b)
	DrawLine(VecAdd(start, Vec(size,0,0)), VecAdd(start, Vec(-size,0,0)), r, g, b)
	DrawLine(VecAdd(start, Vec(0,size,0)), VecAdd(start, Vec(0,-size,0)), r, g, b)
	DrawLine(VecAdd(start, Vec(0,0,size)), VecAdd(start, Vec(0,0,-size)), r, g, b)
end

function Projectile:Tick()
	if self.hit then return end

	if GetTime() >= self.DieTime then self.hit = true return end

	for i=1,3 do
		if math.abs(self.Pos[i]) >= 1000 then self.hit = true return end -- shot in air?
	end

	if self.Gravity ~= 0 then
		self.Vel = VecAdd(self.Vel, Vec(0, self.Gravity*GetTimeStep(), 0))
	end
	local point2 = VecAdd(self.Pos, VecScale(self.Vel, 1*GetTimeStep()))

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
	local hitData = Ballistics:Hit(self.Pos, point2, VecNormalize(VecSub(point2, self.Pos)), self.Owner, VecLength(VecSub(point2, self.Pos)), self)
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
				data.Tracer = true--math.random(100) <= 15
			end

			data.DieTime = GetTime() + 10

			return setmetatable(data, Projectile)
		end
	}
)