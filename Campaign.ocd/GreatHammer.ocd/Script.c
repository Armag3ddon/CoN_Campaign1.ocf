/*
	Wooden Hammer

	Graphics by Ringwaul (CC-BY 3.0)
*/

local swingtime = 0;
local using;

static const WoodenHammer_SwingTime = 40;

/*-- Engine Callbacks --*/

func Construction()
{
	SetProperty("MeshTransformation", Trans_Mul(Trans_Rotate(90, 0, 0, 1), Trans_Translate(0, 1000)));
}

func Hit()
{
	Sound("Hits::Materials::Wood::WoodHit?");
}

/*-- Usage --*/

public func HoldingEnabled() { return true; }

public func RejectUse(object clonk)
{
	var proc = clonk->GetProcedure();
	return proc != "WALK" && proc != "SCALE";
}

public func ControlUseStart(object clonk, int ix, int iy)
{
	using = 1;
	// Create an offset, so that the hit matches with the animation
	swingtime = WoodenHammer_SwingTime*1/38;
	clonk->SetTurnType(1);
	clonk->SetHandAction(1);
	clonk->UpdateAttach();
	clonk->PlayAnimation("StrikePickaxe", CLONK_ANIM_SLOT_Arms, Anim_Linear(0, 0, clonk->GetAnimationLength("StrikePickaxe"), Pickaxe_SwingTime, ANIM_Loop), Anim_Const(1000));
	var fx = AddEffect("IntPickaxe", clonk, 1, 1, this);
	if (!fx) return false;
	fx.x = ix;
	fx.y = iy;
	return true;
}

public func ControlUseHolding(object clonk, int new_x, int new_y)
{
	// Can clonk use pickaxe?
	if (clonk->GetProcedure() != "WALK")
	{
		clonk->PauseUse(this);
		return true;
	}
	var fx = GetEffect("IntPickaxe", clonk);
	if (!fx) return clonk->CancelUse();
	fx.x = new_x; fx.y = new_y;
	return true;
}

func ControlUseCancel(object clonk, int ix, int iy)
{
	Reset(clonk);
	return true;
}

public func ControlUseStop(object clonk, int ix, int iy)
{
	Reset(clonk);
	return true;
}

public func Reset(clonk)
{
	using = 0;
	clonk->SetTurnType(0);
	clonk->SetHandAction(false);
	clonk->UpdateAttach();
	clonk->StopAnimation(clonk->GetRootAnimation(10));
	swingtime=0;
	RemoveEffect("IntPickaxe", clonk);
}

func DoSwing(object clonk, int ix, int iy)
{
	var x = clonk->GetCalcDir() * 12;
	CreateSmokeTrail(10, 75, x, 8, nil, true);
	CreateSmokeTrail(10, -75, x, 8, nil, true);
	Sound("Hits::Materials::Wood::DullWoodHit?");
}

func FxIntPickaxeTimer(object clonk, proplist effect, int time)
{
	++swingtime;
	if(swingtime >= WoodenHammer_SwingTime)
	{
		DoSwing(clonk,effect.x,effect.y);
		swingtime = 0;
	}

	var angle = Angle(0,0,effect.x,effect.y);
	var speed = 50;

	var iPosition = swingtime*180/WoodenHammer_SwingTime;
	speed = speed*(Cos(iPosition-45, 50)**2)/2500;
	// limit angle
	angle = BoundBy(angle,65,300);
	clonk->SetXDir(Sin(angle,+speed),100);
	clonk->SetYDir(Cos(angle,-speed),100);
}

/*-- Display --*/

public func GetCarryMode(object clonk, bool idle)
{
	if (!idle)
		return CARRY_HandBack;
	else
		return CARRY_Back;
}

public func GetCarrySpecial(clonk) { if(using == 1) return "pos_hand2"; }

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90, 0, 0, 1), Trans_Rotate(90, 0, 1, 0), Trans_Translate(0,2500));
}

func Definition(def) {
	SetProperty("PictureTransformation",Trans_Mul(Trans_Rotate(40, 0, 0, 1),Trans_Rotate(150, 0, 1, 0), Trans_Scale(900), Trans_Translate(600, 400, 1000)),def);
}

/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;
//MaxReach is the length of the pick from the clonk's hand
local MaxReach = 12;
local MaxPickDensity = 70; // can't pick granite
local ForceFreeHands = true;
local Components = {Wood = 1, Metal = 1};