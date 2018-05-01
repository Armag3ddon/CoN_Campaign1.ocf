// Crate

func Construction()
{
	SetProperty("MeshTransformation", Trans_Mul(Trans_Rotate(RandomX(-180,180), 0, 1, 0), Trans_Scale(15)));
}

public func GreatEffect()
{
	AddTimer("Woosh", 10);
	return this;
}

func Woosh()
{
	Sound("Hits::Materials::Wood::WoodHit?");
}

func Hit()
{
	if (!this.firsthit)
	{
		RemoveTimer("Woosh");
		this.firsthit = true;
		CreateSmokeTrail(100, 75, 5, 5, nil, true);
		CreateSmokeTrail(100, -75, -5, 5, nil, true);
		Sound("Crash");
		return;
	}
	Sound("Hits::Materials::Wood::WoodHit?");
}

local Name = "$Name$";
local Description = "$Description$";
local Touchable = 1;
local Plane = 380;