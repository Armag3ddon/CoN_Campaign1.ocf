/*-- Elevator case --*/

#include Library_Structure

local elevator;

/* Interaction */

public func GetCallDescription()
{
	return "$InteractCall$";
}

// Return true when a clonk can call this case (interaction icon is shown)
public func Ready(object clonk)
{
	if (!elevator) return false;
	if (GetCasePusher()) return false;
	if(GetEffect("MoveTo", this)) return false;
	// Clonk is out of reach
	if (clonk->GetY() < elevator->GetY()) return false;
	// No need to call?
	if (Inside(clonk->GetY(), GetY()-10, GetY()+10)) return false;
	// Enemy
	if (Hostile(elevator->GetOwner(), clonk->GetOwner())) return false;
	// PathFree
	if (clonk->GetY() < GetY())
	{
		if (!PathFree(GetX(), GetY(), GetX(), clonk->GetY())) return false;
	} else if (!PathFree(GetX(), GetY()+14, GetX(), clonk->GetY()))
		return false;

	return true;
}

public func CallCase(object clonk)
{
	MoveTo(nil, nil, clonk, true);
}

// Called when an interaction is highlighted.
public func DrawCustomInteractionSelector(object dummy, object clonk, int interaction_index, extra_data)
{
	if (extra_data && extra_data.Fn == "CallCase")
	{
		var max_y = clonk->GetY() - GetY();
		// Relocate the dummy to the Clonk.
		dummy->SetVertex(0, VTX_Y, -max_y);
		
		// And draw some particles upwards.
		var particles =
		{
			Prototype = Particles_Trajectory(),
			Size = PV_Sin(PV_Step(5, PV_Random(90)), 2, 3),
		};
		particles = Particles_Colored(particles, GetPlayerColor(clonk->GetOwner()));
		
		var dir = 1;
		if (max_y < 0) dir = -1;
		max_y = Abs(max_y);
		for (var y = 0; y < max_y; y += 5)
			dummy->CreateParticle("SphereSpark", 0, -y * dir + 10, 0, 0, 0, particles, 1);
		return true;
	}
	return false;
}

/*-- Callbacks --*/

// Case is not a structure, but uses the library.
public func IsStructure() { return false; }

private func Initialize()
{
	AddEffect("ElevatorUpperLimitCheck", this, 1, 1, this);
	AddEffect("FetchVehicles", this, 1, 4, this);

	SetProperty("MeshTransformation", Trans_Mul(Trans_Scale(550), Trans_Rotate(5,0,1)));

	return _inherited(...);
}

// Called by the elevator
public func Connect(object connect_to)
{
	elevator = connect_to;
	SetComDir(COMD_None);
	SetAction("DriveIdle");
}

private func Destruction()
{
	if(elevator)
		elevator->LostCase();
	return _inherited(...);
}

public func LostElevator()
{
	RemoveObject();
}

public func ForwardEngineStart(int direction)
{
	if (elevator)
		elevator->StartEngine(direction, true);
}

public func ForwardEngineStop()
{
	if (elevator)
		elevator->StopEngine(true);
}

/* Security checks */

// Repositions the case if for some reason it's too far up
private func FxElevatorUpperLimitCheckTimer(target, effect, time)
{
	if (!elevator)
		return FX_Execute_Kill;

	var d = GetY() - (elevator->GetY() + 20);

	// HOW COULD THIS HAPPEN :C
	if (d <= 0)
	{
		if (GetYDir() < 0)
		{
			SetPosition(GetX(), GetY() - d);
			ContactTop();
		}
		else if (GetYDir() == 0)
			SetPosition(GetX(), GetY() - d);

		effect.Interval = 1;
		return FX_OK;
	}

	// everything okay, adjust timer accordingly
	// check less often if far away from elevator
	// note: d > 0
	var t = BoundBy(d / 3, 1, 20);
	effect.Interval = t;
	return FX_OK;
}

// Returns the first clonk found pushing this case
private func GetCasePusher()
{
	var in_rect = Find_InRect(-13, -13, 26, 26);
	for (var pusher in FindObjects(in_rect, Find_Action("Push")))
	{
		var act_target = pusher->GetActionTarget();
		if (act_target == this)
			return pusher;
		if (GetEffect("ElevatorControl", act_target) && GetEffect("ElevatorControl", act_target).case == this)
			return pusher;
	}
	return nil;
}

/* Vehicle control */

public func OutOfRange(object vehicle)
{
	if(Abs(GetY() - vehicle->GetY()) > 10) return true;

	var dist = vehicle->GetObjWidth();
	var min_x = GetX() - dist;
	var max_x = GetX() + dist;

	if(vehicle->GetX() < min_x) return true;
	if(vehicle->GetX() > max_x) return true;
	return false;
}

private func FxFetchVehiclesTimer(object target, proplist effect, int time)
{
	if (!elevator)
		return FX_Execute_Kill;

	// look for vehicles
	var additional = 5;
	var x = GetX() - 12 - additional;
	var w = GetX() + 12 + additional;
	var y = GetY() - 12;
	var h = GetY() + 15;

	// Fetch vehicles
	for (var vehicle in FindObjects(Find_InRect(x - GetX(), y - GetY(), w - x, h - y), Find_Category(C4D_Vehicle), Find_NoContainer(), Find_Func("FitsInElevator")))
	{
		if (GetEffect("ElevatorControl", vehicle))
			continue;
		if (vehicle->FitsInDoubleElevator())
			continue;
		vehicle->SetSpeed();
		// If a clonk is pushing the vehicle, its receive another push after 1 frame
		Schedule(vehicle, "SetSpeed()", 2);
		vehicle->SetR();
		var x = GetX();
		vehicle->SetPosition(x, GetY() + GetBottom() - 3 - vehicle->GetBottom());
		AddEffect("ElevatorControl", vehicle, 30, 5, vehicle, nil, this);
		Sound("Objects::Connect");
	}

	return FX_OK;
}

/* Movement */

private func SetMoveDirection(int dir, bool user_requested, bool drill)
{
	// no change?
	if (dir == COMD_Up && (GetYDir() < 0)) return;
	if (dir == COMD_Down && (GetYDir() > 0)) return;

	// already reached top/bottom?
	if (GetContact(-1, CNAT_Bottom) && dir == COMD_Down && !drill)
		return;
	if (GetContact(-1, CNAT_Top) && dir == COMD_Up)
		return;
	if (dir == COMD_Stop)
		return Halt();

	var action = "Drive";
	if (drill)
		action = "Drill";

	SetAction(action);
	SetComDir(dir);
	elevator->StartEngine(dir);
}

func Halt(bool user_requested, bool power_out)
{
	// Stop the engine if it was still moving.
	if(elevator)
		elevator->StopEngine();

	// Clear speed.
	SetAction("DriveIdle");
	SetYDir();
	SetComDir(COMD_Stop);
}

func StopAutomaticMovement()
{
	if (GetEffect("MoveTo", this))
	{
		RemoveEffect("MoveTo", this);
		Halt();
	}
}

// Moves the case to the specific y-coordinate
// delay in frames, so the elevator does not freak out
// target will be checked again for COMD_Stop and distance after delay run out
public func MoveTo(int y, int delay, object target, bool user_requested)
{
	// Not idle?
	if (!CheckIdle() && !user_requested)
		return;
	Halt();
	var effect = AddEffect("MoveTo", this, 1, 2, this);
	effect.delay = delay;
	effect.move_to_y = y;
	effect.target = target;
	effect.user_requested = user_requested;
	return;
}

private func FxMoveToTimer(object target, proplist effect, int time)
{
	if (time < effect.delay) return FX_OK;
	// what would take more than 10 seconds?
	if ((time - effect.delay) / 36 > 10) return FX_Execute_Kill;

	var y = effect.move_to_y;
	if (effect.target)
	{
		y = effect.target->GetY();
		// Target is a swimming clonk?
		if (effect.target->~IsSwimming())
			// Clonk swims at the surface?
			if (!effect.target->GBackSemiSolid(0, -5))
				y -= 5;
				// Offset y a little bit so the clonk can stand in the case
	}

	// Target gone? Don't move and remove effect.
	if (y == nil)
	{
		Halt();
		return FX_Execute_Kill;
	}

	// Target moves away from elevator shaft, finish movement but stop following
	if (effect.target)
		if(Abs(GetX() - effect.target->GetX()) > 100)
		{
			effect.move_to_y = effect.target->GetY();
			effect.target = nil;
		}

	// Destination reached? Stop effect and movement.
	if (Abs(GetY() - y) < 5)
	{
		Halt();
		return -1;
	}

	var dir = COMD_Up;
	if (y > GetY())
		dir = COMD_Down;
	SetMoveDirection(dir, effect.user_requested, false);
	return 1;
}

func CheckIdle()
{
	// If there is someone pushing the case it is not idle.
	if (GetCasePusher())
		return false;
	return true;
}

/* Contact */

func ContactTop()
{
	Halt();
}

func ContactBottom()
{
	// try to dig free
	if (GetAction() == "Drill")
	{
		Drilling();

		// wee!
		if (!GetContact(-1, CNAT_Bottom))
		{
			SetComDir(COMD_Down);
			return;
		}
	}
	Halt();
}

/*-- Controls --*/

// Send elevator to the clicked position.
public func ControlUseStart(object clonk, int x, int y)
{
	MoveTo(GetY() + y, 0, nil, true);
	Sound("UI::Click", { player = clonk->GetOwner() });
	// Do not trigger a UseStop-callback.
	return false;
}

public func ControlDown(object clonk)
{
	if (!elevator) return true;

	// Pressing down when already on ground results in drilling.
	var drill = !!GetContact(-1, CNAT_Bottom);

	StopAutomaticMovement();
	SetMoveDirection(COMD_Down, true, drill);
	return true;
}

public func ControlUp(object clonk)
{
	if (!elevator) return true;

	// what is that player even doing
	if (GetY() <= elevator->GetY() + 20)
	{
		Sound("UI::Click", {player = clonk->GetOwner()});
		return true;
	}
	
	StopAutomaticMovement();
	SetMoveDirection(COMD_Up, true, false);
	return true;
}

public func ControlStop(object clonk, int control)
{
	if (control == CON_Up && GetYDir() <= 0)
		Halt(true);
	else if (control == CON_Down && GetYDir() >= 0)
		Halt(true);

	return true;
}

/*-- Digging --*/

private func Drilling()
{
	var additional_y = 1;
	var rect = Rectangle(GetX() - 12, GetY() - 13 - additional_y, GetX() + 12, GetY() + 13 + additional_y);
	DigFreeRect(rect.x, rect.y, rect.wdt - rect.x, rect.hgt - rect.y);
}

public func DigOutObject(object obj)
{
	// Get the clonk controlling this elevator and forward the callback to this clonk.
	var clonk = GetCasePusher();
	if (clonk)
		return clonk->~DigOutObject(obj);
	// If not handled by a clonk , remove the material if it is supposed to be in the bucket.
	if (obj->~IsBucketMaterial())
		return obj->RemoveObject();
	return;
}

/*-- Scenario saving --*/

func SaveScenarioObject() { return false; }


/*-- Properties --*/

local ActMap = {
	Drive = {
		Prototype = Action,
		Name = "Drive",
		Procedure = DFA_FLOAT,
		FacetBase = 1,
		Speed = 200,
		Accel = 2,
		Decel = 5,
		NextAction = "Drive",
	},
	DriveIdle = {
		Prototype = Action,
		Name = "DriveIdle",
		Procedure = DFA_FLOAT,
		FacetBase = 1,
		NextAction = "DriveIdle",
	},
	Drill = {
		Prototype = Action,
		Name = "Drill",
		Procedure = DFA_FLOAT,
		FacetBase = 1,
		Speed = 100,
		Accel = 2,
		Decel = 10,
		Delay = 1,
		Length = 1,
		PhaseCall = "Drilling",
		NextAction = "Drill",
		Sound = "Structures::Elevator::Drilling",
		DigFree = 1
	}
};

local Name = "$Name$";
local Description = "$Description$";
local Touchable = 2;
local HitPoints = 50;
local Plane = 250;
local BorderBound = C4D_Border_Top | C4D_Border_Bottom;
local ContactCalls = true;
local Components = {Wood = 1};
