/*--
	Gatling Gun
	Author: Clonkonaut

	Rapid firing bullet shooter
--*/

#include Library_HasExtraSlot
#include Library_ElevatorControl
//#include Library_Destructible

local animAim;
local turnDir;
local animTurn;

local Fire_Velocity = 175;

public func IsArmoryProduct() { return true; }
public func IsVehicle() { return true; }

func Initialize()
{
	turnDir = 0;
	animTurn = 0;
	animAim = PlayAnimation("Aim", 1, Anim_Const(0));
	this.MeshTransformation = Trans_Rotate(animTurn, 0, 1);
}

//some left-overs from Lorry script. Not sure of it's purpose...
func ContactLeft()
{
	if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

func ContactRight()
{
	if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

func RejectCollect(id def, object obj)
{
	// Only accept bullets
	if (def != LeadBullet)
		return true;
	// Max bullets
	if (ContentsCount() >= 5)
		return true;
	return false;
}

/*-- Control --*/

public func ControlUseStart(object clonk, int x, int y)
{
	var result = CheckForBullets(clonk);
	if (!result)
	{
		clonk->CancelUse();
		return true;
	}

	if(clonk->GetOwner() != GetOwner()) SetOwner(clonk->GetOwner());

	//Animation
	var r = ConvertAngle(Angle(0,0,x,y));
	SetAnimationPosition(animAim, Anim_Const(AnimAngle(r)*3954444/100000)); //conversion. Apparently 90 blender frames is 3559 ogre frames.
	return true;
}

private func CheckForBullets(object clonk)
{
	// Check for bullets, is the only content.
	if (!Contents(0))
	{
		// Look for bullets inside the shooter.
		var bullets = FindObject(Find_Container(clonk), Find_ID(LeadBullet));
		if (bullets)
		{
			bullets->Exit();
			bullets->Enter(this);
			Sound("Hits::Materials::Wood::WoodHit?");
		}
		else
		{
			PlayerMessage(clonk->GetOwner(),"$TxtNeedsAmmo$");
			return false;
		}
	}
	return true;
}

public func HoldingEnabled() { return true; }

local angPrec = 1000;

public func ControlUseHolding(object clonk, int x, int y)
{
	if (!clonk) return true;
	
	var r = ConvertAngle(Angle(0,0,x,y,angPrec));

	SetCannonAngle(r);

	if (CheckForBullets(clonk))
		if (!GetEffect("IntCooldown", this))
		{
			var bullets = Contents(0);
			if (!bullets)
				bullets = FindObject(Find_Container(clonk), Find_ID(LeadBullet));
			if (bullets)
				DoFire(bullets->TakeObject(), clonk, r);
		}

	return true;
}

public func SetCannonAngle(int r)
{
	return SetAnimationPosition(animAim, Anim_Const(AnimAngle(r/angPrec)*3954444/100000));
}

private func AnimAngle(int angle)
{
	//Conversion for animation
	var r = Normalize(angle,-180);
	r = r - GetR();
	r = Abs(r);
	r = 180-r-90;
	r = BoundBy(r,0,90);
	return r;
}

private func ConvertAngle(int angle)
{
	var nR = BoundBy(Normalize(angle,-180 * angPrec,angPrec), (-90 * angPrec) + (GetR() * angPrec), (90 * angPrec) + (GetR() * angPrec));

	//Turn the cannon into the pointed direction
	if(nR - (GetR() * angPrec) < 0 && turnDir == 1) TurnCannon(0);
	if(nR - (GetR() * angPrec) > 0 && turnDir == 0) TurnCannon(1);

	//renormalize the angle to 0/360 from -180/180
	return Normalize(nR,0,angPrec);
}

func ControlLeft(object clonk)
{
	if(turnDir == 1){
		TurnCannon(0);
	}
}

func ControlRight(object clonk)
{
	if(turnDir == 0){
		TurnCannon(1);
	}
}

public func TurnCannon(int dir, bool instant)
{
	turnDir = dir;
	//Remove any old effect
	if(GetEffect("IntTurnCannon", this)) RemoveEffect("IntTurnCannon", this);
	// Instant turn?
	if (instant)
	{
		// Simply set anim position to desired side
		animTurn = 0;
		if (dir == DIR_Left) animTurn = 180;
		this.MeshTransformation = Trans_Rotate(animTurn, 0, 1);
	}
	else
	{
		// Non-instant turn: Add timer to turn
		return AddEffect("IntTurnCannon", this, 1, 1, this);
	}
}

func FxIntTurnCannonTimer(object cannon, proplist effect, int timer)
{
	if (turnDir)
		animTurn += 10;
	else
		animTurn -= 10;
	animTurn = BoundBy(animTurn, 0, 180);
	//advance turn animation
	this.MeshTransformation = Trans_Rotate(animTurn, 0, 1);
	if (animTurn == 0 || animTurn == 180)
		return -1;
}

protected func DoFire(object iammo, object clonk, int angle)
{
	var musk_front = 21;
	var musk_up = 25;
	var musk_y_offset = -8;
	var musk_x_offset = Cos(angle, 8, angPrec) * -(turnDir * 2 - 1); // To addjust for the graphics, offsetted barrel
	// Calculate offset for shot and effects.
	var off_x = Sin(angle, musk_front, angPrec) + musk_x_offset;
	var off_y = -Cos(angle, musk_up, angPrec) + musk_y_offset;

	iammo->Launch(clonk, angle + RandomX(-this.BulletSpread, this.BulletSpread), RandomX(-1, 1), RandomX(this.BulletSpeed[0], this.BulletSpeed[1]), off_x, off_y, angPrec);

	// Muzzle Flash & gun smoke.
	var x = Sin(angle, 20, angPrec);
	var y = -Cos(angle, 20, angPrec);
	CreateParticle("Smoke", off_x, off_y, PV_Random(x - 20, x + 20), PV_Random(y - 20, y + 20), PV_Random(40, 60), Particles_Smoke(), 20);
	CreateMuzzleFlash(off_x, off_y, angle / angPrec, 20);
	CreateParticle("Flash", 0, 0, 0, 0, 8, Particles_Flash());
	// Gun blast sound.
	Sound("Objects::Weapons::Blunderbuss::GunShoot?");

	AddEffect("IntCooldown", this, 1, 5);
}

local Name = "$Name$";
local Description = "$Description$";
local Touchable = 1;
local BorderBound = C4D_Border_Sides;
local ContactCalls = true;
local Components = {Metal = 4, Wood = 2};
local HitPoints = 150;
local BulletSpread = 300;
local BulletSpeed = [196, 204];