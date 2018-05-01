/**
	Mysterious cracks that randomly appear in tunnels below the Dark Forest.
*/

local last_free;
local mat_color;
local state = 0;

func Construction()
{
	var tex = GetTexture(0, 0);
	if (tex)
	{
		mat_color = GetAverageTextureColor(tex);
	}
	else
	{
		mat_color = RGB(100, 50, 5);
	}
	
	last_free = false;

	SetR(Random(360));
	Hide();
	AddTimer("CheckFree", 100 + Random(10));
}

func Initialize()
{
	SetAction("Be");
}

public func DugOut(object clonk)
{
	return CheckFree(true);
}

private func CheckFree(bool by_dig_free)
{
	if (GBackSolid() == last_free)
	{
		last_free = !last_free;
		if (!last_free)
			Hide();
		else
		{
			// Look what's appearing behind the dust!
			DugOutDust();
			Show();
		}
	} else if (last_free && state < 2)
	{
		if (CanExpand() && !Random(30))
		{
			if (CheckClonkInVicity())
			{
				Expand();
			}
		}
	}
}

func DugOutDust()
{
	var particles =
	{
		Prototype = Particles_Colored(Particles_Dust(), mat_color),
		ForceY = PV_Gravity(100),
		DampingX = 750, DampingY = 750,
		Size = PV_KeyFrames(0, 0, 2, 100, 10, 1000, 4),
		Alpha = PV_KeyFrames(0, 0, 0, 100, 100, 1000, 0),
		OnCollision = PC_Die()
	};

	CreateParticle("Dust", 0, 0, PV_Random(-40, 40), PV_Random(-40, 40), PV_Random(10, 60), particles, 40 + Random(20));
}

func SetState(int new_state)
{
	state = new_state;
	SetPhase(state);
}

func CanExpand()
{
	return !(GBackSolid(-5,0) && GBackSolid(5, 0) && GBackSolid(0, -5) && GBackSolid(0, 5));
}

func CheckClonkInVicity()
{
	return FindObject(Find_Distance(150), Find_ID(Clonk), Find_OCF(OCF_Alive), Find_Not(Find_Func("GBackSky")));
}

func Expand()
{
	SetState(state + 1);
	DugOutDust();
	Sound("Crack");

	for (var i = 0; i < 10; i++)
		InsertMaterial(Material("Earth"), 0,0, RandomX(-1,1), 0);

	if (Random(30))
		CastObjects(Rock, 1+Random(2), 10);
	if (!Random(3))
	{
		for (var i = 0; i < 10; i++)
			InsertMaterial(Material("Lava"), 0,0, RandomX(-1,1), 0);
	}
}

/* Hidden state: Still show in editor */

func Hide()
{
	this.Visibility = VIS_Editor;
	SetClrModulation(0x80ffffff);
	SetState(0);
}

func Show()
{
	this.Visibility = VIS_All;
	SetClrModulation(0xa0ffffff);
}

local ActMap = {
	Be = {
		Prototype = Action,
		Name = "Be",
		Length = 3,
		Delay = 0,
		X = 0,
		Y = 0,
		Wdt = 20,
		Hgt = 20
	}
};

local Name = "$Name$";