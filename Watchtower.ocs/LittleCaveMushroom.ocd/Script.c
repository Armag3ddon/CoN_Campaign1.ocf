/*-- Little Cave Mushroom --*/

func Construction()
{
	// every mushroom is unique!
	var len = GetAnimationLength("Rotate1"); // length of all animations is the same
	PlayAnimation("Pitch1", 1, Anim_Const(RandomX(0, len)), Anim_Const(500));
	PlayAnimation("Pitch2", 2, Anim_Const(RandomX(0, len)), Anim_Const(500));
	PlayAnimation("Rotate1", 2, Anim_Const(RandomX(0, len)), Anim_Const(500));

	PlayAnimation("Head1", 2, Anim_Const(RandomX(0, len)), Anim_Const(500));
	PlayAnimation("Head2", 2, Anim_Const(RandomX(0, len)), Anim_Const(500));
	PlayAnimation("Head3", 2, Anim_Const(RandomX(0, len)), Anim_Const(500));
	PlayAnimation("Head4", 2, Anim_Const(RandomX(0, len)), Anim_Const(500));
}

func Initialize()
{
	SetMeshMaterial("Campaign_LittleCaveMushroom");

	SetCon(20);

	AddEffect("Sparkle", this, 1, 30 + RandomX(-3, 3), this);
}

local burned;

func Damage(int change, int cause)
{
	_inherited(change, cause);

	if (this && !burned && GetDamage() > MaxDamage()/3 && OnFire())
	{
		SetClrModulation(RGB(100, 100, 100));
		burned = true;
	}
}

func MaxDamage()
{
	return 50;
}

func Entrance()
{
	if (GetCategory() & C4D_StaticBack)
		SetCategory(C4D_None);
}

/* Carrying */

public func GetCarryMode()
{
	return CARRY_Hand;
}

public func GetCarryTransform()
{
	return Trans_Scale(200);
}

public func GetCarryBone()
{
	return "Bone.002";
}

/* Sparkling */

func FxSparkleStart(target, effect, temp)
{
	if (temp) return;
	var color = RGB(255, 20, 255);
	effect.particles =
	{
		Prototype = Particles_MagicRing(),
		R = (color >> 16) & 0xff,
		G = (color >>  8) & 0xff,
		B = (color >>  0) & 0xff,
	};
}

func FxSparkleTimer(target, effect, effect_time)
{
	if(this->Contained() || !Random(2)) return FX_OK;
	CreateParticle("MagicRing", 0, 0, 0, 0, effect.Interval, effect.particles, 1);
	return FX_OK;
}

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 2;
local ContactIncinerate = 6;
local NoBurnDecay = 1;
local Collectible = true;
local Plane = 480;