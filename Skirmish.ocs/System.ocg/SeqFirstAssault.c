/**
	First Assault sequence

	Paula will explain a few last details
*/

#appendto Sequence

func FirstAssault_Init()
{
	Dialogue->FindByTarget(g_soldiers[2])->RemoveAttention();
	paulas_dlg->RemoveAttention();

	for (var pioneer in g_pioneers)
		RemoveEffect("NPC_Pioneer_Warmup", pioneer);

	return true;
}

func FirstAssault_Start(object hero)
{
	this.hero = hero;

	SetViewTarget(paula);
	paula->SetDir(DIR_Left);

	return ScheduleNext(20);
}

func FirstAssault_1()
{
	MessageBoxAll("$MsgFirstAssault1$", paula, true);

	return ScheduleNext(50);
}

func FirstAssault_2()
{
	MessageBoxAll("$MsgFirstAssault2$", paula, true);

	return ScheduleNext(180);
}

func FirstAssault_3()
{
	if (GetPlayerCount(C4PT_User) > 1)
		MessageBoxAll("$MsgFirstAssault3_Multi$", paula, true);
	else
		MessageBoxAll("$MsgFirstAssault3_Single$", paula, true);

	return ScheduleNext(170);
}

func FirstAssault_4()
{
	MessageBoxAll("$MsgFirstAssault4$", paula, true);

	return ScheduleNext(230);
}

func FirstAssault_5()
{
	MessageBoxAll("$MsgFirstAssault5$", paula, true);

	return ScheduleNext(150);
}

func FirstAssault_6()
{
	MessageBoxAll("$MsgFirstAssault6$", paula, true);

	return ScheduleNext(160);
}

func FirstAssault_7()
{
	MessageBoxAll("$MsgFirstAssault7$", paula, true);

	return ScheduleNext(230);
}

func FirstAssault_8()
{
	MessageBoxAll("$MsgFirstAssault8$", paula, true);

	return ScheduleNext(50);
}

func FirstAssault_9()
{
	MessageBoxAll("$MsgFirstAssault9$", g_pioneers[0], true);

	return ScheduleNext(75);
}

func FirstAssault_10()
{
	this.dummy = CreateObject(Campaign_Icon_Commands);

	MessageBoxAll("$MsgFirstAssault10$", this.dummy, true);

	return ScheduleNext(230);
}

func FirstAssault_11()
{
	MessageBoxAll("$MsgFirstAssault11$", this.dummy, true);

	return ScheduleNext(230);
}

func FirstAssault_12()
{
	SetViewTarget(samuel);

	return ScheduleNext(35);
}

func FirstAssault_13()
{
	MessageBoxAll("$MsgFirstAssault12$", samuel, true);

	this.dummy->RemoveObject();

	return ScheduleNext(80);
}

func FirstAssault_14()
{
	Sound("Siren", true);

	return ScheduleNext(20);
}

func FirstAssault_15()
{
	FindObject(Find_ID(Campaign_DrawbridgeRight))->Open();

	return ScheduleNext(250);
}

func FirstAssault_16()
{
	for (var clonk in g_assault)
		clonk->SetCommand("MoveTo", nil, 2000, 680);

	this.jump_trigger = CreateObject(Sequence, 939, 802);
	this.jump_trigger->SetTrigger({Radius = 25, Trigger = "object_enter_region_circle"});
	this.jump_trigger->SetTriggerID(Clonk);
	this.jump_trigger.trigger.Radius = 15;
	this.jump_trigger.action={Function = "Action_script"};
	this.jump_trigger.action.Context={Function = "triggering_object"};
	this.jump_trigger.action.Script={Function = "string_constant", Value = "Jump()"};

	this.jump_trigger2 = CreateObject(Sequence, 1374, 701);
	this.jump_trigger2->SetTrigger({Radius = 25, Trigger = "object_enter_region_circle"});
	this.jump_trigger2->SetTriggerID(Clonk);
	this.jump_trigger2.trigger.Radius = 15;
	this.jump_trigger2.action={Function = "Action_script"};
	this.jump_trigger2.action.Context={Function = "triggering_object"};
	this.jump_trigger2.action.Script={Function = "string_constant", Value = "Jump()"};

	return ScheduleNext(150);
}

func FirstAssault_17()
{
	MessageBoxAll("$MsgFirstAssault13$", paula, true);

	return ScheduleNext(35);
}

func FirstAssault_18()
{
	SetViewTarget(paula);

	return ScheduleNext(20);
}

func FirstAssault_19()
{
	FindObject(Find_ID(StoneDoor))->OpenDoor();

	return ScheduleNext(20);
}

func FirstAssault_20()
{
	return Stop();
}

func FirstAssault_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 200,100, PLRZOOM_LimitMin);

	paulas_dlg->SetInteraction(false);

	paula->SetObjectLayer(nil);
	paula->CreateContents(Blunderbuss)->CreateContents(LeadBullet);
	paula->UpdateAttach();

	if (GetType(this) == C4V_C4Object)
	{
		this.jump_trigger->RemoveObject();
		Schedule(this.jump_trigger2, "RemoveObject()", 500);
	}

	Global->CreateEffect(PreFirst_Battle, 1, 20);
	Global->CreateEffect(First_Battle, 1, 5);
	SetRallyPoint(1075, 1095);
	g_pioneer_last_bridge = FindObject(Find_ID(WoodenBridge), Find_AtPoint(1240, 1171));
	g_bridges = [g_pioneer_last_bridge];

	AdvanceGoal();

	for (var pioneer in g_pioneers)
	{
		pioneer->ClearInvincible();
		RemoveEffect("IntNonFlammable", pioneer);
	}

	var bridges = FindObjects(Find_ID(WoodenBridge));
	for (var bridge in bridges)
		bridge->MakeInvincible();

	GUI_Controller->ShowCommandBar();

	return true;
}