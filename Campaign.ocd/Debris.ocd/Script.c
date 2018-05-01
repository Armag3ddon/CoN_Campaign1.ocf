/**
	Debris

	Broken parts, lying around.
*/

// Definition call
public func CreateDebris(string name, int x, int y, int xdir, int ydir, int rdir, int r, int fadeout)
{
	var debris = CreateObject(Campaign_Debris, x, y, NO_OWNER);

	if (name == "Brick")
		name = Format("Brick%d", Random(this->GetBrickCount()) + 1);

	debris->SetGraphics(name);

	debris->SetSpeed(xdir, ydir);

	if (r == nil)
		r = Random(360);

	debris->SetR(r);
	debris->SetRDir(rdir);

	if (fadeout)
		debris->FadeOutIn(fadeout);

	return debris;
}

func GetBrickCount() { return 2; }

public func FadeOutIn(int time)
{
	ScheduleCall(this, "FadeOut", time, nil, 70, true);
}

/*-- Properties --*/

local Name = "$Name$";
local Plane = 800;