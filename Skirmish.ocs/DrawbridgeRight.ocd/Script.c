/**
	The gate of the camp
*/

#include Library_SwitchTarget

local opened = true;
local gaffs_y = 0, gaffs;
local rope;

func Construction()
{
	SetCategory(C4D_StaticBack);
	return _inherited(...);
}

/*-- Opening / Closing --*/

// Reaction to operation by a switch
public func OnSetInputSignal(object operator, object switch, bool close_door)
{
	if (!close_door)
	{
		if(Open(false))
			if (operator != g_gate_guard)
				if (GetEffect("NPC_Gate_Guard", g_gate_guard))
					GetEffect("NPC_Gate_Guard", g_gate_guard)->OnGateOpened(this, operator);
	}
	else
	{
		Close(false);
	}

	_inherited(operator, switch, close_door, ...);
}

public func Open(bool instant)
{
	if (opened)
		return false;
	if (GetEffect("TurningEffect", this))
		return false;

	if (instant)
	{
		SetR(0);
		opened = true;
		AdjustGaffs();
	} else {
		this->CreateEffect(TurningEffect, 1, 2, +1);
	}
	return true;
}

public func Close(bool instant)
{
	if (!opened)
		return false;
	if (GetEffect("TurningEffect", this))
		return false;

	if (instant)
	{
		SetR(-80);
		opened = false;
		AdjustGaffs();
	} else {
		this->CreateEffect(TurningEffect, 1, 2, -1);
	}
	return true;
}

local TurningEffect = new Effect {
	Construction = func (int dir) {
		this.turn_direction = dir;
	},
	Timer = func () {
		if (this.turn_direction > 0)
		{
			var new_r = this.Target->GetR() + this.turn_direction;
			this.Target->SetR(new_r);
			this.Target->AdjustGaffs();
			if (this.Target->GetR() >= 0)
				return -1;
		} else {
			var new_r = this.Target->GetR() + this.turn_direction;
			this.Target->SetR(new_r);
			this.Target->AdjustGaffs();
			if (this.Target->GetR() <= -80)
				return -1;
		}
	},
	Destruction = func () {
		if (this.Target->GetR() >= 0)
			this.Target.opened = true;
		else
			this.Target.opened = false;
	}
};

/*-- Gaffs (lifting arms) --*/

public func AddGaffs(int y_diff)
{
	gaffs_y = y_diff;

	gaffs = CreateObject(Flag, GetX(), GetY() - y_diff, GetOwner());
	gaffs->SetBeam();
	gaffs->SetPosition(GetX(), GetY() - y_diff);
	gaffs->SetTurnPosition((GetR() + 90) * -1);

	rope = CreateObject(GateLine, 0, - y_diff / 2, GetOwner());
	AdjustGaffs();
}

func AdjustGaffs()
{
	if (!this.gaffs)
		return;
	this.gaffs->SetTurnPosition((GetR() + 90) * -1);
	this.LineAttach = GetLineAttach();
	this.gaffs.LineAttach = GetGaffsLineAttach();
	this.rope->ConnectPoints(GetX() + this.LineAttach[0],
	                         GetY() + this.LineAttach[1],
	                         this.gaffs->GetX() + this.gaffs.LineAttach[0],
	                         this.gaffs->GetY() + this.gaffs.LineAttach[1]);
}

func GetLineAttach()
{
	var r = GetR();
	var x = Cos(r, 70)-10;
	var y = Sin(r, 70)+5;
	return [x,y];
}

func GetGaffsLineAttach()
{
	var r = GetR();
	var x = Cos(r, 30);
	var y = Cos(r, 40) - 30 + Sin(r*2, 10);
	return [x,y];
}

/*-- Destruction --*/

public func Destruction()
{
	// Create global particles, so they don't get removed when the bridge is removed.
	Global->CreateParticle("WoodChip", GetX(), GetY() + 5, PV_Random(-15, 15), PV_Random(-13, -6), PV_Random(36 * 3, 36 * 10), Particles_WoodChip(), 20);
	var particles =
	{
		Prototype = Particles_Dust(),
		R = 50, G = 50, B = 50,
		Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(2, 10), 1000, 0),
	};
	for (var cnt = 0; cnt < 24; ++cnt)
	{
		var x = RandomX(-36, 36);
		var y = RandomX(-6, 6);
		Global->CreateParticle("Dust", GetX() + x, GetY() + y, PV_Random(-3, 3), PV_Random(-3, -3), PV_Random(18, 36), particles, 2);
		CastPXS("Ashes", 5, 30, x, y);
	}
}

public func Damage(int change, int cause, int cause_plr)
{
	// Damaged and burnt bridges appear darker.
	var darkness = 255 - 180 * GetDamage() / this.HitPoints;
	SetClrModulation(RGB(darkness, darkness, darkness));
	// Let the structure library handle the rest.
	return _inherited(change, cause, cause_plr, ...);
}

/*-- Properties --*/

// this.MeshTransformation = Trans_Mul(Trans_Rotate(90, 0, 0, 1), Trans_Scale(1000, 916, 1000), Trans_Translate(-6000, -37800, 0))
local MeshTransformation = [0, -916, 0, 34624, 1000, 0, 0, -6000, 0, 0, 1000, 0];
// def.PictureTransformation = Trans_Mul(Trans_Rotate(90, 0, 0, 1), Trans_Rotate(30, 1, 0, 0), Trans_Rotate(10, 0, 1, 0), Trans_Scale(1280), Trans_Scale(1000, 916, 1000), Trans_Translate(-6000, -12000, 0))
local PictureTransformation = [-111, -1014, 629, 12834, 1260, 0, 222, -7560, -192, 586, 1091, -5880];
local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 2;
local ContactIncinerate = 8;
local NoBurnDecay = true;
local HitPoints = 80;
local Components = {Wood = 3, Metal = 1};
