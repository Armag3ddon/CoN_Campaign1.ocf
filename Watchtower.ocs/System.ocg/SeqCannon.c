/**
	Cannon construction sequence

	Main Quest is finished, it is time to REALLY fight back!
*/

#appendto Sequence

func Cannon_Init(object hero)
{
	this.mult = GetScenMapZoom(); // Laziness

	return true;
}

func Cannon_Start(object hero)
{
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_LimitMax|PLRZOOM_LimitMin);
	SetViewTarget(g_armory);

	this.hero = hero;
	Sound("UI::Ding", true);

	return ScheduleNext(35);
}

func Cannon_JoinPlayer(int plr)
{

	return true;
}

func Cannon_1()
{
	g_armory->Smoking();
	Sound("Structures::SteamEngine", true);

	return ScheduleNext(15);
}

func Cannon_2()
{
	g_armory->Smoking();

	return ScheduleNext(15);
}

func Cannon_3()
{
	g_armory->Smoking();

	return ScheduleNext(15);
}

func Cannon_4()
{
	g_armory->Smoking();

	return ScheduleNext(15);
}

func Cannon_5()
{
	g_armory->Smoking();
	Sound("Structures::SawmillRipcut", true);

	return ScheduleNext(15);
}

func Cannon_6()
{
	g_armory->Smoking();

	return ScheduleNext(15);
}

func Cannon_7()
{
	g_armory->Smoking();

	return ScheduleNext(15);
}

func Cannon_8()
{
	g_armory->Smoking();

	return ScheduleNext(15);
}

func Cannon_9()
{
	g_armory->Smoking();
	Sound("Structures::Repair", true);

	return ScheduleNext(45);
}

func Cannon_10()
{
	// Clear the way of shenanigans the players might have done
	DigFreeRect(81*this.mult,80*this.mult, 57*this.mult,8*this.mult, true);
	DigFreeRect(127*this.mult,62*this.mult, 5*this.mult,27*this.mult, true);

	g_elevator.case->SetPosition(1036, 557);
	samuel->SetPosition(1036, 557);
	samuel->ObjectCommand("Grab", g_elevator.case);
	g_elevator.case->MoveTo(86*this.mult);
	this.wait_time = 0;

	return ScheduleNext(5);
}

func Cannon_11()
{
	// Wait for elevator to arrive
	if (g_elevator.case->GetAction() != "DriveIdle")
	{
		this.wait_time++;
		if (this.wait_time >= 45)
		{
			g_elevator.case->SetPosition(1036, 86*this.mult);
			samuel->SetPosition(1036, 86*this.mult);
		} else
			return ScheduleSame(5);
	}

	samuel->SetCommand("UnGrab");
	samuel->AppendCommand("MoveTo", nil, 930, 685);

	this.cannon = CreateObject(Cannon, 714, 688, 11);
	this.cannon->SetMeshMaterial("KingdomCannon");

	return ScheduleNext(45);
}

func Cannon_12()
{
	if (GetPlayerCount(C4PT_User) > 1)
		MessageBoxAll("$MsgCan1_M$", samuel, true);
	else
		MessageBoxAll("$MsgCan1_S$", samuel, true);

	return ScheduleNext(110);
}

func Cannon_13()
{
	MessageBoxAll("$MsgCan2$", samuel, true);

	return ScheduleNext(200);
}

func Cannon_14()
{
	MessageBoxAll("$MsgCan3$", samuel, true);

	return ScheduleNext(200);
}

func Cannon_15()
{
	MessageBoxAll("$MsgCan4$", samuel, true);

	return ScheduleNext(200);
}

func Cannon_16()
{
	MessageBoxAll("$MsgCan5$", samuel, true);

	return ScheduleNext(250);
}

func Cannon_17()
{
	DimPlayers();
	MessageBoxAll("", nil, true);

	return ScheduleNext(5);
}

func Cannon_18()
{
	this.cannon->SetPosition(593, 440);
	samuel->SetPosition(601, 438);
	samuel->SetDir(DIR_Left);

	samuel->ObjectCommand("Grab", this.cannon);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			crew->SetPosition(619+RandomX(-10,10), 436);
			crew->SetDir(DIR_Left);
		}
	}

	SetViewTarget(this.cannon);

	return ScheduleNext(40);
}

func Cannon_19()
{
	UndimPlayers();
	this.cannon->TurnCannon(0);

	return ScheduleNext(35);
}

func Cannon_20()
{
	MessageBoxAll("$MsgCan6$", samuel, true);

	return ScheduleNext(180);
}

func Cannon_21()
{
	this.cannon->SetAnimationPosition(0, Anim_Linear(0, 0, 2500, 120, ANIM_Hold));
	Sound("CannonTurn", true);

	return ScheduleNext(140);
}

func Cannon_22()
{
	MessageBoxAll("$MsgCan7$", samuel, true);

	return ScheduleNext(50);
}

func Cannon_23()
{
	SetViewTarget(g_barricade);
	SetPlayerZoomByViewRange(NO_OWNER, 450,350, PLRZOOM_LimitMax|PLRZOOM_LimitMin);

	this.helper = CreateObject(Dummy, 36, 837, 11);
	this.helper->SetLightRange(300, 200);

	g_playershots = CreateArray(GetPlayerCount(C4PT_User));

	return ScheduleNext(35);
}

func Cannon_24()
{
	MessageBoxAll("$MsgCan8$", samuel, true);
	g_fireatwill = true;
	RemoveEffect("BarrageEffect", Scenario);
	g_barricade->ClearInvincible();

	return ScheduleNext(5);
}

func Cannon_25()
{
	if (!g_destruction)
		return ScheduleSame(5);
	return ScheduleNext(5);
}

// Definition call by PlayerControl.c
func CannonShot(int plr, int x, int y)
{
	if (!g_playershots[plr])
	{
		var shot = CreateObject(Boompack, 360, 320, 11);
		shot->SetFuel(300);
		shot->Launch(Angle(360,320, x,y), nil, samuel);
		shot->SetMeshMaterial("BunkerBusterBoompack");
		AddEffect("FlightRotation", shot, 151, 1, nil, Sequence);
		var effect = AddEffect("IntControlRocket", shot, 100, 1, nil, Airplane);
		effect.x = x;
		effect.y = y;
		g_playershots[plr] = shot;
		Sound("CannonShot", true, nil, plr);
		return true;
	}
	return false;
}

func FxFlightRotationTimer(object boompack, effect, int time)
{
	if (effect.rotation == nil)
		effect.rotation = 0;
	effect.rotation += 2;
	if (effect.rotation > 360) effect.rotation = 0;

	boompack.MeshTransformation = Trans_Rotate(effect.rotation, 0,1);
	return FX_OK;
}

func Cannon_26()
{
	g_fireatwill = false;
	g_barricade->Destroy();

	return ScheduleNext(35);
}

func Cannon_27()
{
	MessageBoxAll("$MsgCan9$", samuel, true);

	return ScheduleNext(35);
}

func Cannon_28()
{
	SetViewTarget(samuel);
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_LimitMax|PLRZOOM_LimitMin);

	return ScheduleNext(5);
}

func Cannon_29()
{
	samuel->SetCommand("Ungrab");
	samuel->SetDir(DIR_Right);

	MessageBoxAll("$MsgCan10$", samuel, true);

	paula->SetPosition(822, 282);
	paula->PlayAnimation("Stand", CLONK_ANIM_SLOT_Movement+2, Anim_Linear(0, 0, paula->GetAnimationLength("Stand"), 35, ANIM_Loop), Anim_Const(1000));
	paula->PlayAnimation("CloseEyes", CLONK_ANIM_SLOT_Movement+3, Anim_Const(0), Anim_Const(1000));
	paula->SetDir(DIR_Right);

	return ScheduleNext(180);
}

func Cannon_30()
{
	MessageBoxAll("$MsgCan11$", paula, true);
	g_barricade->RemoveObject();

	return ScheduleNext(5);
}

func Cannon_31()
{
	SetViewTarget(paula);

	return ScheduleNext(70);
}

func Cannon_32()
{
	MessageBoxAll("$MsgCan12$", samuel, true);

	return ScheduleNext(110);
}

func Cannon_33()
{
	MessageBoxAll("$MsgCan13$", paula, true);

	return ScheduleNext(120);
}

func Cannon_34()
{
	this.plane1 = CreateMiniAirplane(115 * this.mult, 88 * this.mult);
	this.plane2 = CreateMiniAirplane(116 * this.mult, 87 * this.mult);
	this.plane3 = CreateMiniAirplane(117 * this.mult, 86 * this.mult);

	this.plane2->Sound("Objects::Plane::PropellerLoop",true,40,nil,1);

	return ScheduleNext(60);
}

func CreateMiniAirplane(int x, int y)
{
	var plane = CreateObject(Airplane, x, y, 10);
	var clonk = CreateObject(Clonk, x, y, 10);
	clonk->Enter(plane);
	plane->FaceLeft();
	plane->PlaneMount(clonk);
	plane.Plane = -1;
	plane.BorderBound = nil;
	plane.MeshTransformation = Trans_Mul(Trans_Rotate(90,0,0,1), Trans_Translate(-1000,-3375,0), Trans_Rotate(25,0,1,0), Trans_Scale(250));

	plane->StartInstantFlight(-95, 5);
	plane->RemoveEffect("IntPlane", plane);
	plane->AddEffect("CustomPlane", plane, 1, 1, nil, this);
	plane->SetPropellerSound(0);

	while (plane->GetVertexNum())
		plane->RemoveVertex();

	return plane;
}

func FxCustomPlaneTimer(object target, effect, int timer)
{
	//Lift
	var lift = 21;

	// Engine smoke
	if (target->GBackSky())
	{
		var colour = 255 - (target->GetDamage() * 3);
		var min = PV_Random(1, 2);
		var max = PV_Random(4, 7);
		var particles = 
		{
			Prototype = Particles_Smoke(),
			R = colour, G = colour, B = colour,
			Size = PV_Linear(min, max)
		};
		target->CreateParticle("Smoke", -Sin(target->GetR(), 10), Cos(target->GetR(), 10), 0, 0, PV_Random(36, 2 * 36), particles, 1);
	}

	//Throttle-to-thrust lag
	if(timer % 10 == 0)
	{
		if(target.throttle > target.thrust) ++target.thrust;
		if(target.throttle < target.thrust) --target.thrust;
	}

	//propellor
	var change = target->GetAnimationPosition(target.propanim) + target.thrust * 3;
	if(change > target->GetAnimationLength("Propellor"))
		change = (target->GetAnimationPosition(target.propanim) + target.thrust * 3) - target->GetAnimationLength("Propellor");
	if(change < 0)
		change = (target->GetAnimationLength("Propellor") - target.thrust * 3);

	target->SetAnimationPosition(target.propanim, Anim_Const(change));

	//Thrust
	target->SetXDir(Sin(target->GetR(), target.thrust) + target->GetXDir(100), 100);
	target->SetYDir(-Cos(target->GetR(), target.thrust) + target->GetYDir(100) - lift, 100);

	//Drag
	var maxspeed = 20;
	var speed = Distance(0, 0, target->GetXDir(), target->GetYDir());
	if(speed > maxspeed)
	{
		target->SetXDir(target->GetXDir(100)*maxspeed/speed, 100);
		target->SetYDir(target->GetYDir(100)*maxspeed/speed, 100);
	}
}

func Cannon_35()
{
	SetViewTarget(this.plane2);

	return ScheduleNext(35);
}

func Cannon_36()
{
	MessageBoxAll("$MsgCan14$", samuel, true);

	return ScheduleNext(5);
}

func Cannon_37()
{
	if (this.plane2->GetX() > 10)
		return ScheduleSame(5);
	return ScheduleNext(5);
}

func Cannon_38()
{
	this.helper->SetPosition(288, 631);
	SetViewTarget(this.helper);
	this.beacon = this.helper->CreateObject(Dummy, -20,0, NO_OWNER);

	return ScheduleNext(20);
}

func Cannon_39()
{
	var effect = AddEffect("Beacon", this.beacon, 1, 5, nil, Sequence);
	effect.smoke =
	{
		Prototype = Particles_Smoke(),
		R = 150, G = 150, B = 150,
		Size = PV_Linear(PV_Random(2, 4), PV_Random(7, 10))
	};
	effect.fire =
	{
		R = PV_KeyFrames(0, 0, 255, 300, 255, 600, 255, 1000, 0),
		G = PV_KeyFrames(0, 0, 255, 300, 255, 600, 0, 1000, 0),
		B = PV_KeyFrames(0, 0, 255, 300, 0, 600, 0, 1000, 0),
		Alpha = 100,
		Size = PV_KeyFrames(0, 1, 500, 2, 1000, 1),
		BlitMode = GFX_BLIT_Additive
	};

	return ScheduleNext(45);
}

func FxBeaconTimer(object target)
{
	target->CreateParticle("Smoke", 0, -2, PV_Random(1,4), -2, PV_Random(36, 72), this.smoke, 1);
	target->CreateParticle("Flash", 0, 0, 0, 0, PV_Random(3, 8), this.fire, 1);
}

func Cannon_40()
{
	MessageBoxAll("$MsgCan15$", paula, true);

	return ScheduleNext(75);
}

func Cannon_41()
{
	MessageBoxAll("$MsgCan16$", samuel, true);

	return ScheduleNext(35);
}

func Cannon_42()
{
	Sound("DrumsOfWar", true);

	return ScheduleNext(140);
}

func Cannon_43()
{
	MessageBoxAll("$MsgCan17$", samuel, true);
	EliminatePlayer(10);

	return ScheduleNext(320);
}

func Cannon_44()
{
	return Stop();
}

func Cannon_Stop()
{
	GameCall("EndScenario");

	return true;
}