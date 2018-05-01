/**
	Flag

	Sign of rule.
*/

local anim, slow;

public func SetSlow()
{
	StopAnimation(anim);
	anim = PlayAnimation("Slow", 1, Anim_Linear(0, 0, GetAnimationLength("Slow"), 156, ANIM_Loop), Anim_Const(1000));
	slow = true;
}

public func BreakDown(int rdir, int xdir, int ydir)
{
	SetCategory(C4D_None);
	SetRDir(rdir);
	SetSpeed(xdir, ydir);
}

public func SetBeam()
{
	StopAnimation(anim);
	ChangeDef(FlagAsABeam);
	SetGraphics("", Flag);
	SetMeshMaterial("JustABeam");
	anim = nil;
	slow = nil;
}

/*-- Engine Callbacks --*/

func Initialize()
{
	anim = PlayAnimation("Wave", 1, Anim_Linear(0, 0, GetAnimationLength("Wave"), 78, ANIM_Loop), Anim_Const(1000));
}

func SaveScenarioObject(props)
{
	if (!inherited(props, ...)) return false;
	if (slow) props->AddCall("Slow", this, "SetSlow");
	return true;
}

/*-- Display --*/

func Definition(def)
{
	SetProperty("MeshTransformation", Trans_Rotate(60,0,1,0),def);
}

/*-- Properties --*/

local Name = "$Name$";