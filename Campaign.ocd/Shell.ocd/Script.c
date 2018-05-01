/*--
	Shell

	Goes boom.
--*/

local armed = false;
local boosted = false;

/*-- Engine Callbacks --*/

func Construction()
{
	SetGraphics("", Boompack);
	SetProperty("MeshTransformation", Trans_Scale(500));
}

func Hit()
{
	if (armed)
	{
		GameCall("ShellImpact", this);
		Explode(50);
	}
}

/*-- Callbacks --*/

public func IsProjectileTarget(object projectile)
{
	return true;
}

public func OnProjectileHit(object projectile)
{
	Hit();
}

public func IsExplosive() { return true; }

/*-- Usage --*/

public func OnCannonShot()
{
	armed = true;
	SetProperty("Collectible",0);
	SetCategory(C4D_Vehicle);
	this->CreateEffect(Flying, 1, 5);
	SetR(Angle(0,0, GetXDir(1000), GetYDir(1000), 1000)/1000);

	if (boosted)
	{
		SetXDir(GetXDir()*6/5);
		SetYDir(GetYDir()*6/5);
	}
}

local Flying = new Effect {
	Timer = func () {
		this.Target->SetR(Angle(0,0, this.Target->GetXDir(1000), this.Target->GetYDir(1000), 1000)/1000);
	}
};

/*-- Production --*/

func IsArmoryProduct() { return true; }

public func Definition(def)
{
	def.PictureTransformation = Trans_Mul(Trans_Translate(-3000, -1000, 0), Trans_Rotate(45,0,0,1),Trans_Rotate(-35,1,0,0),Trans_Scale(1200));
}

/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;
local BlastIncinerate = 1;
local ContactIncinerate = 1;
local Components = {Metal = 1, Firestone = 1};