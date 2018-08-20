/**
	Pioneer Bridge
*/

local carriers;
local carried_by = 0;
local deployed = false;

func Initialize()
{
	carriers = [];
}

func Destruction()
{
	if (deployed)
		return;

	Campaign_Debris->CreateDebris("BridgeLeft",  GetX() - 15, GetY(), -1, 0, -10, 0, 70)->SetCon(130);
	Campaign_Debris->CreateDebris("BridgeRight", GetX() + 15, GetY(), +1, 0, +10, 0, 70)->SetCon(130);
	Sound("Environment::Tree::Crack");

	GameCall("PioneerBridgeDestroyed");
}

public func GetCarrierCount()
{
	var ret = 0;
	if (carriers[0])
		ret++;
	if (carriers[1])
		ret++;
	return ret;
}

public func GetCarrier()
{
	if (carriers[0])
		return carriers[0];
	if (carriers[1])
		return carriers[1];
}

// A pioneer wants to carry the bridge
public func LiftUp(object clonk)
{
	if (GetType(clonk) != C4V_C4Object) // ?? Weird bugs!
		return;
	if (!carriers[0])
	{
		carriers[0] = clonk;
		clonk->CreateEffect(Lifting, 1, 5, this, false);
		return;
	}
	if (!carriers[1] && clonk != carriers[0])
	{
		carriers[1] = clonk;
		clonk->CreateEffect(Lifting, 1, 5, this, true);
		return;
	}
}

public func Lifted(object clonk)
{
	carried_by++;
	if (carried_by >= 2)
	{
		SetCategory(C4D_StaticBack);
		AdjustPosition();
	}
}

public func AdjustPosition()
{
	if (carried_by >= 2)
	{
		var x = carriers[0]->GetX() - carriers[1]->GetX();
		x /= 2;
		x += carriers[1]->GetX();

		var y = carriers[0]->GetY() - carriers[1]->GetY();
		y /= 2;
		y+= Min(carriers[0]->GetY(), carriers[1]->GetY());

		SetPosition(x, y);

		if (GetY() >= 1185)
			RemoveObject();

		if (!g_pioneer_last_bridge)
			return;

		if (GetX() >= g_pioneer_last_bridge->GetX() - 10) // Can be deployed
		{
			Halt();
			deployed = true;
			if (!GameCall("CreateWoodenBridge"))
				GameCall("PioneerBridgePlaced");
			return RemoveObject();
		}
	} else if (GetLength(carriers) == 2) {
		if (carriers[0] && !GetEffect("Lifting", carriers[0]))
			carriers[0]->CreateEffect(Lifting, 1, 5, this, false);
		if (carriers[1] && !GetEffect("Lifting", carriers[1]))
			carriers[1]->CreateEffect(Lifting, 1, 5, this, false);
	}
}

public func LostCarrier(object carrier)
{
	if (!carrier || GetType(carrier) != C4V_C4Object)
		return;
	if (carrier != carriers[0] && carrier != carriers[1])
		return;
	if (carried_by == 0)
		return;

	carried_by--;
	if (carriers[0] == carrier)
		carriers[0] = nil;
	if (carriers[1] == carrier)
		carriers[1] = nil;

	if (carried_by == 1)
		SetPosition(GetX(), GetY()+5);

	Halt();
}

local Lifting = new Effect {
	Construction = func(object bridge, bool second) {
		if (GetType(this.Target) != C4V_C4Object)
			return;
		this.bridge = bridge;
		this.second = second;
		this.lifted = false;
		this.tumbling = false;

		var x = this.bridge->GetX() + 30;
		if (second)
			x = this.bridge->GetX() - 30;
		this.Target->PioneerMoveTo(x, this.bridge->GetY());
	},
	Timer = func() {
		if (GetType(this.Target) != C4V_C4Object)
			return -1;
		if (!this.lifted)
		{
			if (this.Target->PioneerIsMoving())
				return FX_OK;
			this.lifted = true;
			this.bridge->Lifted(this.Target);
			this.Target->SetDir(DIR_Right);
			this.Target->SetToWalkingSpeed();
			return FX_OK;
		} else {
			if (!this.bridge)
				return -1;

			this.bridge->AdjustPosition();

			if (!this.bridge)
				return -1;

			if (this.bridge.carried_by < 1)
				this.bridge->Lifted(this.Target);

			if (this.bridge->GetX() < 1035)
				if (this.Target->GetComDir() == COMD_Left)
					this.bridge->Halt();

			if (this.Target->GetAction() == "Tumble")
			{
				// Tumbling is bad. First off, stop the other carrier
				this.bridge->Halt();
				this.tumbling = true;
				// Check if my position is too far off
				var x = this.bridge->GetX() + 30;
				if (this.second)
					x = this.bridge->GetX() - 30;
				if (Distance(x, GetY(), GetX(), GetY()) > 30)
					return -1;
			}
		}
	},
	Destruction = func() {
		if (this.Target->GetAlive())
			this.Target->SetToRunningSpeed();
		if (this.bridge)
			this.bridge->LostCarrier(this.Target);
	}
};

// Receiving orders

public func Forward()
{
	if (carried_by < 2)
		return;

	carriers[0]->SetComDir(COMD_Right);
	carriers[1]->SetComDir(COMD_Right);
}

public func Backward()
{
	if (carried_by < 2)
		return;

	if (GetX() <= 1035)
		return;

	carriers[0]->SetComDir(COMD_Left);
	carriers[1]->SetComDir(COMD_Left);
}

public func Halt()
{
	if (carriers[0]) carriers[0]->SetComDir(COMD_Stop);
	if (carriers[1]) carriers[1]->SetComDir(COMD_Stop);
}

/*-- Properties --*/

local MeshTransformation = [0, -916, 0, 34624, 1000, 0, 0, -6000, 0, 0, 1000, 0];
local PictureTransformation = [-111, -1014, 629, 12834, 1260, 0, 222, -7560, -192, 586, 1091, -5880];

local Name = "$Name$";
local Description = "$Description$";