/**
	Attack sequence
	
	The problem arrives.
*/

#appendto Sequence

func Attack_Init(object hero, object igni)
{
	this.igniter = igni;

	this.igniter->Exit();
	this.igniter->Enter(paula);

	Fuse->Create(g_dynamite, this.igniter);

	var pos = paula->GetItemPos(this.igniter);
	paula->SetHandItemPos(0, pos);

	this.enemy = CreateObjectAbove(Clonk, 296, 920, 10);
	this.enemy->MakeInvincible();
	this.enemy->MakeNonFlammable();
	this.enemy->SetDir(DIR_Right);
	this.enemy->CreateContents(Blunderbuss)->CreateContents(LeadBullet);

	this.mult = GetScenMapZoom();

	return true;
}

func Attack_Start(object hero)
{
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_LimitMax|PLRZOOM_LimitMin);
	SetViewTarget(paula);

	this.hero = hero;

	return ScheduleNext(35);
}

func Attack_JoinPlayer(int plr)
{
	return true;
}

func Attack_1()
{
	MessageBoxAll("$MsgAttack1$", paula, true);
	return ScheduleNext(280);
}

func Attack_2()
{
	MessageBoxAll("$MsgAttack2$", paula, true);

	var danger_clonks = FindObjects(Find_InRect(77*this.mult, 112*this.mult, 28*this.mult, 23*this.mult), Find_OCF(OCF_CrewMember));
	// Better get out of the danger zone!
	for (var clonk in danger_clonks)
		clonk->SetCommand("MoveTo", nil, 900, 1036);

	this.kegs = FindObjects(Find_InRect(37*this.mult, 104*this.mult, 25*this.mult, 16*this.mult), Find_ID(PowderKeg));
	// Secure the powder kegs from fire
	for (var keg in this.kegs)
		keg->MakeNonFlammable();

	return ScheduleNext(210);
}

func Attack_3()
{
	MessageBoxAll("$MsgAttack3$", samuel, true);
	return ScheduleNext(140);
}

func Attack_4()
{
	MessageBoxAll("$MsgAttack4$", paula, true);
	return ScheduleNext(140);
}

func Attack_5(object hero, object igniter)
{
	this.igniter->ControlUseStart(paula);
	return ScheduleNext(45);
}

func Attack_6()
{
	SetViewTarget(g_dynamite);
	return ScheduleNext(10);
}

func Attack_7()
{
	if (FindObject(Find_ID(Dynamite), Find_InRect(52*this.mult, 108*this.mult, 37*this.mult, 18*this.mult)))
		return ScheduleSame(10);
	return ScheduleNext(10);
}

func Attack_8()
{
	SetViewTarget(paula);
	MessageBoxAll("$MsgAttack5$", paula, true);
	paula->SetCommand("MoveTo", nil, 85*this.mult, 125*this.mult);

	for (var keg in this.kegs)
		if (keg)
			keg->RemoveEffect("IntNonFlammable", keg);
	this.special_keg = this.kegs[0];
	this.enemy->FindContents(Blunderbuss).loaded = true;
	this.enemy->FindContents(Blunderbuss)->ControlUseStart(this.enemy, this.special_keg->GetX() - this.enemy->GetX(), (this.enemy->GetY() - this.special_keg->GetY())*-1);

	return ScheduleNext(250);
}

func Attack_9()
{
	SetViewTarget(this.enemy);
	this.light = this.enemy->CreateObject(Dummy, 0,0, NO_OWNER);
	this.light->SetLightRange(300, 80);
	MessageBoxAll("$MsgAttack6$", paula, true);
	Sound("Warning", true);
	return ScheduleNext(140);
}

func Attack_10()
{
	paula->SetCommand("None");
	this.helper = CreateObject(Dummy, this.special_keg->GetX(), this.special_keg->GetY(), NO_OWNER);
	this.helper->SetLightRange(300, 80);
	this.enemy->FindContents(Blunderbuss)->ControlUseStop(this.enemy, this.special_keg->GetX() - this.enemy->GetX(), this.special_keg->GetY() - this.enemy->GetY());
	return ScheduleNext(10);
}

func Attack_11()
{
	SetViewTarget(this.helper);
	if (this.special_keg)
		this.special_keg->Incinerate();
	return ScheduleNext(35);
}

func Attack_12()
{
	for(var i = 0; i < GetLength(this.kegs); i++)
		if (this.kegs[i])
			this.kegs[i]->Explode(30);
	return ScheduleNext(10);
}

func Attack_13()
{
	var dummy = CreateObject(Dummy, 462, 107*this.mult);
	dummy->Explode(40);
	this.enemy->SetCommand("MoveTo", nil, 8, 114*this.mult);
	return ScheduleNext(10);
}

func Attack_14()
{
	var dummy = CreateObject(Dummy, 462, 104*this.mult);
	dummy->Explode(40);
	this.helper->SetPosition(462, 104*this.mult);
	return ScheduleNext(10);
}

func Attack_15()
{
	var dummy = CreateObject(Dummy, 462, 101*this.mult);
	dummy->Explode(20);
	dummy = CreateObject(Dummy, 452, 102*this.mult);
	dummy->Explode(20);
	dummy = CreateObject(Dummy, 472, 101*this.mult);
	dummy->Explode(20);

	tower_guard->Fling(-2, -4);
	Sound("Clonk::Skin::Alchemist::Hurt?", true);
	tower_guard->FindContents(Javelin)->Exit(0,0, 0, 3, -5, 15);
	tower_guard->FindContents(Helmet)->Exit(0,0, 0, -3, -4, -9);
	tower_guard->SetInvincibility(false);
	tower_guard.Hit = this.Attack_GuardHit;

	Campaign_Debris->CreateDebris("Carriage", g_cannon->GetX(), g_cannon->GetY(), -2, -3, -20);
	var debris = Campaign_Debris->CreateDebris("Barrel", g_cannon->GetX(), g_cannon->GetY(), 2, -20, 12);
	g_cannon->RemoveObject();

	SetViewTarget(debris);

	g_elevator.case->SetPosition(g_elevator.case->GetX(), 704);
	samuel->SetPosition(g_elevator.case->GetX(), 690);

	return ScheduleNext(210);
}

func Attack_16()
{
	samuel->ObjectCommand("Grab", g_elevator.case);
	g_elevator.case->SetMoveDirection(COMD_Down);
	MessageBoxAll("$MsgAttack7$", samuel, true);

	return ScheduleNext(20);
}

func Attack_17()
{
	SetViewTarget(samuel);

	return ScheduleNext(110);
}

func Attack_18()
{
	MessageBoxAll("$MsgAttack8$", paula, true);

	return ScheduleNext(140);
}

func Attack_19()
{
	MessageBoxAll("$MsgAttack9$", samuel, true);

	return ScheduleNext(140);
}

func Attack_20()
{
	DimPlayers();
	MessageBoxAll("", nil, true);

	return ScheduleNext(20);
}

func Attack_21()
{
	// Enemy is no longer needed
	while(this.enemy->Contents())
		this.enemy->Contents()->RemoveObject();
	this.enemy->RemoveObject();

	// Gather the crew

	samuel->SetPosition(880, 436);
	paula->SetPosition(860, 436);
	samuel->SetDir(DIR_Left);
	paula->SetDir(DIR_Right);

	paula->SetCommand("None");
	paula->SetComDir(COMD_None);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		var crew;
		for (var j = 0; crew = GetCrew(plr, j); j++)
		{
			crew->SetPosition(820 + RandomX(-10, 10), 436);
			crew->SetDir(DIR_Right);
		}
	}

	// Secure the elevator!

	g_elevator.case->SetPosition(g_elevator.case->GetX(), 448);

	SetViewTarget(paula);

	return ScheduleNext(40);
}

func Attack_22()
{
	UndimPlayers();
	MessageBoxAll("$MsgAttack10$", paula, true);

	return ScheduleNext(210);
}

func Attack_23()
{
	MessageBoxAll("$MsgAttack11$", samuel, true);

	return ScheduleNext(140);
}

func Attack_24()
{
	MessageBoxAll("$MsgAttack12$", paula, true);

	return ScheduleNext(140);
}

func Attack_25()
{
	MessageBoxAll("$MsgAttack13$", samuel, true);

	return ScheduleNext(70);
}

func Attack_26()
{
	MessageBoxAll("$MsgAttack13$", samuel, true);

	return ScheduleNext(70);
}

func Attack_27()
{
	MessageBoxAll("$MsgAttack14$", paula, true);

	return ScheduleNext(45);
}

func Attack_28()
{
	MessageBoxAll("$MsgAttack15$", samuel, true);

	return ScheduleNext(180);
}

func Attack_29()
{
	samuel->SetCommand("MoveTo", nil, 950, 436);
	samuel->SetToWalkingSpeed();

	return ScheduleNext(35);
}

func Attack_30()
{
	MessageBoxAll("$MsgAttack16$", samuel, true);

	return ScheduleNext(140);
}

func Attack_31()
{
	samuel->SetDir(DIR_Left);
	samuel->SetToRunningSpeed();

	MessageBoxAll("$MsgAttack17$", samuel, true);

	return ScheduleNext(110);
}

func Attack_32()
{
	MessageBoxAll("$MsgAttack18$", GetHero(), true);

	return ScheduleNext(110);
}

func Attack_33()
{
	MessageBoxAll("$MsgAttack19$", samuel, true);

	return ScheduleNext(280);
}

func Attack_34()
{
	this.boompack = CreateObject(Boompack, 5*this.mult, 83*this.mult, 10);
	this.boompack->SetFuel(999999);
	this.boompack->SetDirectionDeviation(0);
	this.boompack->Launch(91);
	this.boompack.Hit = this.Attack_BoompackHit;

	MessageBoxAll("$MsgAttack20$", samuel, true);

	return ScheduleNext(5);
}

func Attack_35()
{
	SetViewTarget(this.boompack);

	return ScheduleNext(5);
}

func Attack_36()
{
	if (this.boompack)
		return ScheduleSame(5);

	return ScheduleNext(110);
}

func Attack_37()
{
	MessageBoxAll("$MsgAttack21$", samuel, true);

	// Get NPCs into position

	samuel->SetPosition(810, 557);
	paula->SetPosition(676, 614);

	samuel->SetDir(DIR_Right);
	paula->SetDir(DIR_Left);

	samuels_dlg->SetDialogueProgress(1, "SupplyRun", true);
	paulas_dlg->SetDialogueProgress(1, "SupplyRun", true);

	var hammer = paula->CreateContents(Hammer);
	paula->SetHandItemPos(0, paula->GetItemPos(hammer));

	var basement = CreateObject(Basement, 652, 612, 11);
	basement->SetWidth(25);
	basement->MakeInvincible();

	CreateObject(SamuelsProject, 832, 563, 11);
	CreateObject(SamuelsProject, 853, 564, 11);

	paula->CreateEffect(Paula_WorkingEffect, 1, 5, basement); // See NPCs.c
	samuel->CreateEffect(Samuel_WorkingEffect, 1, 5);

	// You can get the tools

	g_chests_allowed = true;

	// The enemies dig in
	g_barricade = CreateObject(Campaign_Barricade, 15, 840, 10);
	CreateObject(Basement, 16, 715, 11);
	CreateObject(Basement, 46, 729, 11);
	CreateObject(Basement, 62, 742, 11);
	CreateObject(Basement, 76, 756, 11);
	// The enemies attack
	Scenario->CreateEffect(BarrageEffect, 1, 350);

	// Debugging
		if (GetType(this) == C4V_C4Object)
			return Stop();
}

func Attack_GuardHit()
{
	this->Kill();
	this.Hit = nil;
}

func Attack_BoompackHit()
{
	ClearFreeRect(329,718, 56,20);
	CreateObject(BrokenTower, AbsX(358), AbsY(733));

	for (var i = 0; i < 7; i++)
		Campaign_Debris->CreateDebris("Brick", 355 + RandomX(-10, 10), 728, RandomX(-20, 20), RandomX(-30, -10), RandomX(-15, 15), nil, 200);

	var flag = FindObject(Find_ID(Flag), Find_AtPoint(AbsX(357), AbsY(699)));
	if (flag)
		flag->BreakDown(RandomX(-15, 15), RandomX(-10, 10), -20);

	this.Hit = Boompack.Hit;
	this->Hit();
}

func Attack_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 300,200, PLRZOOM_LimitMin);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
			GetCrew(plr, j)->CreateObject(Shovel);
	}

	var dynamites = FindObjects(Find_ID(Dynamite));
	for (var dynamite in dynamites)
		dynamite.Collectible = true;

	g_door4->OpenDoor();

	AdvanceGoal();

	return true;
}