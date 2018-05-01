/**
	Acid spit

	This is what the bats spit to attack light sources
*/

public func ProjectileDamage() { return 5; }
public func TumbleStrength() { return 100; }
public func FlightTime() { return 30; }

func Particles_AcidHit()
{
	return
	{
		Size = PV_Linear(2, 0),
		ForceY = GetGravity(),
		DampingY = PV_Linear(1000,700),
		DampingX = PV_Linear(1000,700),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		OnCollision = PC_Die(),
		CollisionVertex = 500,
		R = PV_Linear(128,32),
		G = 255,
		B = PV_Linear(128,32),
		Alpha = PV_Random(255,0,3),
		BlitMode = GFX_BLIT_Additive,
	};
}

func Hit()
{
	if (GetEffect("HitCheck", this))
	{
		RemoveEffect("HitCheck", this);
		Sound("Liquids::Corrode");
		CreateParticle("StarSpark", 0, 0, PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(10, 20), Particles_AcidHit(), 10);
		RemoveObject();
	}
	return;
}

public func Launch(object shooter, int angle, int dist, int speed, int offset_x, int offset_y, int prec_angle)
{
	SetController(shooter->GetController());
	AddEffect("HitCheck", this, 1, 1, nil, nil, shooter, true);

	LaunchProjectile(angle, dist, speed, offset_x, offset_y, prec_angle);
	
	// Remove after some time.
	SetAction("Travel");
	SetComDir(COMD_None);

	// Sound.
	Sound("Objects::Weapons::Blunderbuss::BulletShot?");
}

public func HitObject(object obj)
{
	if (WeaponCanHit(obj))
	{
		if (obj->GetAlive())
			Sound("Hits::ProjectileHitLiving?");
		else
			Sound("Liquids::Corrode");
		
		obj->~OnProjectileHit(this);
		WeaponDamage(obj, this->ProjectileDamage(), FX_Call_EngObjHit, false);
		WeaponTumble(obj, this->TumbleStrength());
		if (!this)
			return;
	}
	RemoveObject();
	return;
}

protected func Traveling()
{
	if (GetActTime() > FlightTime())
		RemoveObject();
}

local ActMap = {
	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		StartCall = "Traveling"
	},
};
local Name = "$Name$";
local Description = "$Description$";