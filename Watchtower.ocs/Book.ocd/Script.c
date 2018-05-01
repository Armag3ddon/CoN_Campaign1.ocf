/* Old Book */

local burned;
local reading_progress = 1;

/*-- Engine Callbacks --*/

func Hit()
{
	Sound("Hits::GeneralHit?");
}

func Incineration()
{
	if (!burned)
	{
		SetGraphics("Burned");
		this.Description = "$DescriptionBurned$";
		burned = true;
	}
}

func Departure()
{
	reading_progress = 1;
}

/*-- Callbacks --*/

public func GetColor()
{
	return RGB(200, 190, 135);
}

/*-- Usage --*/

func ControlUse(object clonk, int x, int y)
{
	if (burned)
		return false;
	if (!clonk->~IsWalking())
		return false;
	return NextLine(clonk);
}

public func NextLine(object clonk)
{
	if (reading_progress == 6)
	{
		reading_progress = 1;
		return false;
	}

	clonk->SetComDir(COMD_Stop);
	var msg = "";
	if (reading_progress == 1)
		msg = "$Page1$";
	if (reading_progress == 2)
		msg = "$Page2$";
	if (reading_progress == 3)
		msg = "$Page3$";
	if (reading_progress == 4)
		msg = "$Page4$";
	if (reading_progress == 5)
		msg = "$Page5$";

	Dialogue->MessageBox(msg, clonk, this, clonk->GetOwner(), false, [["$Next$", "NextLine"]], this);
	reading_progress++;

	return true;
}

/*-- Status --*/

// Why not?
public func IsFuel() { return true; }
public func GetFuelAmount(int requested_amount) 
{ 
	return 100;
}

/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;
local Components = { Tree_Coniferous = 1 };
local BlastIncinerate = 5;
local ContactIncinerate = 4;
local Plane = 491;