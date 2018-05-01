/**
	Elevator sequence
	
	Just a little introduction to samuel.
*/

#appendto Sequence

func Elevator_Start(object hero)
{
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_LimitMax|PLRZOOM_LimitMin);

	this.hero = hero;
	SetViewTarget(hero);

	Sound("Environment::Tree::Crack", true);
	MessageBoxAll("$MsgElevator1$", this.hero, true);

	return ScheduleNext(60);
}

func Elevator_1()
{
	MessageBoxAll("$MsgElevator2$", samuel, true);
	return ScheduleNext(20);
}

func Elevator_2()
{
	this.clonks = FindObjects(Find_InRect(g_elevator.case->GetX()-15, g_elevator.case->GetY()-15, 30, 30), Find_OCF(OCF_CrewMember));

	for (var clonk in this.clonks)
	{
		var comdir = COMD_Right;
		if (clonk->GetDir() == DIR_Left)
			comdir = COMD_Left;
		clonk->DoRoll();
		clonk->SetComDir(comdir);
		Schedule(clonk, "SetComDir(COMD_Stop)", 35);
	}

	return ScheduleNext(15);
}

func Elevator_3()
{
	CreateObject(Campaign_Crate, g_elevator.case->GetX() + RandomX(-1,1), g_elevator.case->GetY()-150, 11)->GreatEffect()->SetR(RandomX(-45,45));
	CreateObject(Campaign_Crate, g_elevator.case->GetX() + RandomX(-1,1), g_elevator.case->GetY()-150, 11)->GreatEffect()->SetR(RandomX(-45,45));

	return ScheduleNext(35);
}

func Elevator_4()
{
	MessageBoxAll("$MsgElevator3$", samuel, true);

	return ScheduleNext(110);
}

func Elevator_5()
{
	if (GetPlayerCount(C4PT_User) == 1)
		MessageBoxAll("$MsgElevator_Single$", samuel, true);
	else
		MessageBoxAll("$MsgElevator_Multi$", samuel, true);

	return ScheduleNext(35);
}

func Elevator_6()
{
	return Stop();
}

func Elevator_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 300,200, PLRZOOM_LimitMin);

	g_elevator.case.firstmove = false;
	g_elevator.case.blocked = true;

	return true;
}