/*
	Pocketwatch

	Author: Ringwaul

	Tells the time
*/

local watch_anim;

/*-- Engine Callbacks --*/

func Construction()
{
	watch_anim = PlayAnimation("time", 5, Anim_Const(1));
	AddTimer("WatchUpdate", 35);
}

func Hit()
{
	Sound("Hits::GeneralHit?");
}

/*-- Production --*/

public func IsTool() { return true; }
public func IsToolProduct() { return true; }

/*-- Display --*/

public func WatchUpdate()
{
	var time = Time->GetTime();

	if(time != nil)
	{
		var watch_time = ((13305 * (time * 2) / 1000) - 1);
		if(watch_time > GetAnimationLength("time")) watch_time = (watch_time - GetAnimationLength("time"));
		SetAnimationPosition(watch_anim, Anim_Const(watch_time));
	}
}

public func GetCarryMode(clonk) { return CARRY_HandBack; }

public func GetCarryTransform(clonk, sec, back)
{
	if(!sec) return Trans_Mul(Trans_Rotate(90, 0, 1), Trans_Rotate(180, 1), Trans_Translate(-3200, 0, -500));
	return Trans_Mul(Trans_Rotate(90, 0, 1), Trans_Translate(-3200, 0));
}

func Definition(def)
{
	SetProperty("PictureTransformation", Trans_Mul(Trans_Rotate(15,1,0,0), Trans_Rotate(5,0,1,0), Trans_Rotate(-5,0,0,1), Trans_Translate(500,-400,0), Trans_Scale(1350)),def);
}

/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;
local Components = {Metal = 2};