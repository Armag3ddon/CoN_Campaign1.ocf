/**
	Briefing sequence

	Samuel has his orders ready
*/

#appendto Sequence

func Briefing_Init()
{
	if (GetType(this) != C4V_Def)
		this.stage = 0;

	Dialogue->FindByTarget(g_prisoner)->RemoveAttention();
	Dialogue->FindByTarget(g_cook)->RemoveAttention();
	Dialogue->FindByTarget(g_medic)->RemoveAttention();
	Dialogue->FindByTarget(g_soldiers[3])->RemoveAttention();
	paulas_dlg->RemoveAttention();

	g_question1 = true;

	return true;
}

func Briefing_Start(object hero)
{
	if (GetPlayerCount(C4PT_User) > 1)
		for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
		{
			var plr = GetPlayerByIndex(i, C4PT_User);
			for (var j = 0; j < GetCrewCount(plr); j++) {
				if (GetCrew(plr, j) == hero)
					continue;
				GetCrew(plr, j)->SetPosition(425 + RandomX(-10,10), 705);
			}
		}

	SetPlayerZoomByViewRange(NO_OWNER, 300,200, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 300,200, PLRZOOM_LimitMax|PLRZOOM_LimitMin);

	this.hero = hero;
	hero->SetPosition(235, 743);
	hero->SetDir(DIR_Left);

	SetViewTarget(samuel);
	samuel->SetDir(DIR_Right);

	return ScheduleNext(70);
}

func Briefing_JoinPlayer(int plr)
{
	// Position joining clonks whereever the action is currently at!
	if (this.stage == 0)
	{
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetPosition(425 + RandomX(-10,10), 705);
		}
	}

	if (this.stage == 1)
	{
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetPosition(81 + RandomX(-10, 10), 583);
		}
	}

	if (this.stage == 2)
	{
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetPosition(841 + RandomX(-20,20), 1085);
		}
	}

	return true;
}

func Briefing_1()
{
	MessageBoxAll("$MsgBriefing1$", samuel, true);
	FindObject(Find_ID(ElevatorLiftPlate))->MoveTo(nil, nil, samuel);

	return ScheduleNext(35);
}

func Briefing_2()
{
	if (GetPlayerCount(C4PT_User) > 1)
		for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
		{
			var plr = GetPlayerByIndex(i, C4PT_User);
			for (var j = 0; j < GetCrewCount(plr); j++) {
				if (GetCrew(plr, j) == this.hero)
					continue;
				GetCrew(plr, j)->CreateEffect(MoveToSamuel, 1, 5);
			}
		}
	paula->CreateEffect(MoveToSamuel, 1, 5);

	this.stage = 1;

	return ScheduleNext(70);
}

local MoveToSamuel = new Effect {
	Construction = func () {
		this.Target->SetCommand("MoveTo", nil, 320, 727);
		this.stage = 1;
	},
	Timer = func() {
		if (this.stage == 1)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->SetComDir(COMD_Left);
			this.Target->Jump();
			this.stage = 2;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.Target->SetComDir(COMD_Stop);
			this.stage = 3;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			return -1;
		}
	}
};

func Briefing_3()
{
	MessageBoxAll("$MsgBriefing2$", samuel, true);

	return ScheduleNext(65);
}

func Briefing_4()
{
	samuel->ObjectCommand("Grab", FindObject(Find_ID(ElevatorLiftPlate)));

	return ScheduleNext(35);
}

func Briefing_5()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetCommand("MoveTo", nil, 215 + RandomX(-4,4), 742);
		}
	}
	paula->SetCommand("MoveTo", nil, 215 + RandomX(-4,4), 742);

	return ScheduleNext(35);
}

func Briefing_6()
{
	MessageBoxAll("$MsgBriefing3$", paula, true);

	return ScheduleNext(25);
}

func Briefing_7()
{
	FindObject(Find_ID(ElevatorLiftPlate))->SetMoveDirection(COMD_Up);

	return ScheduleNext(10);
}

func Briefing_8()
{
	if (FindObject(Find_ID(ElevatorLiftPlate))->GetAction() == "Drive")
		return ScheduleSame(10);

	MessageBoxAll("$MsgBriefing4$", g_gunny, true);
	g_gunny->DoSalute();

	return ScheduleNext(70);
}

func Briefing_9()
{
	samuel->SetCommand("MoveTo", nil, 125, 577);
	MessageBoxAll("$MsgBriefing5$", samuel, true);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetCommand("MoveTo", nil, 81 + RandomX(-10, 10), 583);
		}
	}
	paula->SetCommand("MoveTo", nil, 81 + RandomX(-10, 10), 583);

	return ScheduleNext(35);
}

func Briefing_10()
{
	g_gunny->SetCommand("MoveTo", nil, 262, 546);

	return ScheduleNext(70);
}

func Briefing_11()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetDir(DIR_Right);
		}
	}
	paula->SetDir(DIR_Right);

	return ScheduleNext(110);
}

func Briefing_12()
{
	MessageBoxAll("$MsgBriefing6$", paula, true);

	return ScheduleNext(150);
}

func Briefing_13()
{
	MessageBoxAll("$MsgBriefing7$", samuel, true);

	return ScheduleNext(80);
}

func Briefing_14()
{
	MessageBoxAll("$MsgBriefing8$", paula, true);

	return ScheduleNext(120);
}

func Briefing_15()
{
	MessageBoxAll("$MsgBriefing9$", samuel, true);

	return ScheduleNext(180);
}

func Briefing_16()
{
	MessageBoxAll("$MsgBriefing10$", samuel, true);

	return ScheduleNext(270);
}

func Briefing_17()
{
	MessageBoxAll("$MsgBriefing11$", paula, true);

	return ScheduleNext(130);
}

func Briefing_18()
{
	MessageBoxAll("$MsgBriefing12$", samuel, true);

	return ScheduleNext(250);
}

func Briefing_19()
{
	// In this question, you can only choose to either support your favourite or stay neutral, doing no change
	if (!this.hero)
		this.hero = GetHero(samuel);

	if (GetPaulaAttitude() > 50) // Paula is the favourite
	{
		AskImportantQuestion(this.hero, "$MsgBriefing13$", "$MsgBriefing13_A$", "$MsgBriefing13_B$", "Briefing_PaulaWins", "Briefing_Neutral");
	} else // Samuel is the favourite
	{
		AskImportantQuestion(this.hero, "$MsgBriefing13$", "$MsgBriefing13_C$", "$MsgBriefing13_B$", "Briefing_SamuelWins", "Briefing_Neutral");
	}

	this.await_answers = true;
	return ScheduleNext(5);
}

func Briefing_20()
{
	if (this.await_answers)
		return ScheduleSame(5);

	return ScheduleNext(300);
}

func Briefing_SamuelWins()
{
	if (!this.hero)
		this.hero = GetHero(samuel);

	MessageBoxAll("$MsgBriefing14_C$", this.hero, true);
	this.await_answers = false;

	this.paula_wins = false;
	this.neutral = false;
	this.sam_wins = true;

	// It's actually only losing!
	CharacterPaulaChange(-10);
}

func Briefing_PaulaWins()
{
	if (!this.hero)
		this.hero = GetHero(samuel);

	MessageBoxAll("$MsgBriefing14_A$", this.hero, true);
	this.await_answers = false;

	this.paula_wins = true;
	this.neutral = false;
	this.sam_wins = false;

	// It's actually only losing!
	CharacterSamuelChange(-10);
}

func Briefing_Neutral()
{
	if (!this.hero)
		this.hero = GetHero(samuel);

	MessageBoxAll("$MsgBriefing14_B$", this.hero, true);
	this.await_answers = false;

	this.paula_wins = false;
	this.neutral = true;
	this.sam_wins = false;

	// No changes?!
}

func Briefing_21()
{
	// Split dialogues!
	if (this.paula_wins)
	{
		MessageBoxAll(Format("$MsgBriefing15_A$|<c %s>$MsgBriefing15_Memory1$</c>", GetImportantQuestionColor()), samuel, true);
		return ScheduleNext(300);
	}
	if (this.neutral)
	{
		MessageBoxAll(Format("$MsgBriefing15_B$|<c %s>$MsgBriefing15_Memory2$</c>", GetImportantQuestionColor()), samuel, true);
		return ScheduleNext(200);
	}
	if (this.sam_wins)
	{
		MessageBoxAll(Format("$MsgBriefing15_C$|<c %s>$MsgBriefing15_Memory3$</c>", GetImportantQuestionColor()), paula, true);
		paula->SetCommand("MoveTo", nil, 43, 591);
		return ScheduleNext(200);
	}
}

func Briefing_22()
{
	if (this.paula_wins)
	{
		if (GetPlayerCount(C4PT_User) > 1)
			MessageBoxAll("$MsgBriefing16_A_Multi$", paula, true);
		else
			MessageBoxAll("$MsgBriefing16_A_Single$", paula, true);
		return ScheduleNext(270);
	}
	if (this.neutral)
	{
		MessageBoxAll("$MsgBriefing16_B$", paula, true);
		return ScheduleNext(190);
	}
	if (this.sam_wins)
	{
		MessageBoxAll("$MsgBriefing16_C$", samuel, true);
		paula->SetDir(DIR_Right);
		return ScheduleNext(230);
	}
}

func Briefing_23()
{
	MessageBoxAll("$MsgBriefing17$", samuel, true);

	return ScheduleNext(270);
}

func Briefing_24()
{
	MessageBoxAll("$MsgBriefing18$", samuel, true);

	return ScheduleNext(250);
}

func Briefing_25()
{
	MessageBoxAll("$MsgBriefing19$", samuel, true);

	return ScheduleNext(290);
}

func Briefing_26()
{
	MessageBoxAll("$MsgBriefing20$", paula, true);

	return ScheduleNext(140);
}

func Briefing_27()
{
	MessageBoxAll("$MsgBriefing21$", samuel, true);

	return ScheduleNext(300);
}

func Briefing_28()
{
	MessageBoxAll("$MsgBriefing22$", samuel, true);

	return ScheduleNext(290);
}

func Briefing_29()
{
	MessageBoxAll("$MsgBriefing23$", samuel, true);

	return ScheduleNext(270);
}

func Briefing_30()
{
	MessageBoxAll("$MsgBriefing24$", samuel, true);

	return ScheduleNext(290);
}

func Briefing_31()
{
	MessageBoxAll("$MsgBriefing25$", samuel, true);

	return ScheduleNext(270);
}

func Briefing_32()
{
	if (GetPlayerCount(C4PT_User) > 1)
		MessageBoxAll("$MsgBriefing26_Multi$", samuel, true);
	else
		MessageBoxAll("$MsgBriefing26_Single$", samuel, true);

	g_player_rank++; // Promotion!

	return ScheduleNext(270);
}

func Briefing_33()
{
	MessageBoxAll("$MsgBriefing27$", samuel, true);

	return ScheduleNext(220);
}

func Briefing_34()
{
	if (GetPlayerCount(C4PT_User) > 1)
		MessageBoxAll("$MsgBriefing28_Multi$", paula, true);
	else
		MessageBoxAll("$MsgBriefing28_Single$", paula, true);

	return ScheduleNext(200);
}

func Briefing_35()
{
	DimPlayers();

	return ScheduleNext(10);
}

func Briefing_36()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetPosition(841 + RandomX(-20,20), 1085);
			GetCrew(plr, j)->SetDir(DIR_Right);
		}
	}

	paula->SetPosition(893, 1085);
	paula->SetDir(DIR_Left);

	SetViewTarget(paula);

	GameCall("CreatePioneerSquad");

	MessageBoxAll("", nil, true);

	this.stage = 2;

	return ScheduleNext(35);
}

func Briefing_37()
{
	UndimPlayers();

	return ScheduleNext(35);
}

func Briefing_38()
{
	MessageBoxAll("$MsgBriefing29$", paula, true);

	for (var i = 0; i < GetLength(g_pioneers); i++)
	{
		g_pioneers[i]->SetCommand("MoveTo", nil, 800 + RandomX(-10, 10), 1087);
	}

	return ScheduleNext(350);
}

func Briefing_39()
{
	for (var i = 0; i < GetLength(g_pioneers); i++)
	{
		g_pioneers[i]->DoSalute();
	}

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++) {
			GetCrew(plr, j)->SetDir(DIR_Left);
		}
	}

	MessageBoxAll("$MsgBriefing30$", g_pioneers[0], true);

	return ScheduleNext(65);
}

func Briefing_40()
{
	if (GetPlayerCount(C4PT_User) > 1)
		MessageBoxAll("$MsgBriefing31_Multi$", paula, true);
	else
		MessageBoxAll("$MsgBriefing31_Single$", paula, true);

	paula->DoSalute();

	return ScheduleNext(200);
}

func Briefing_41()
{
	return Stop();
}

func Briefing_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 200,100, PLRZOOM_LimitMin);

	paulas_dlg->SetDialogueProgress(1, "Preparation", true);
	Dialogue->FindByTarget(g_corporal)->AddAttention();

	samuel->SetPosition(688, 753);
	samuel->SetDir(DIR_Right);

	GameCall("AddNPCPreparationEffects");
	GameCall("CreateAssaultTeam");

	AdvanceGoal();

	// New respawn flag!

	FindObject(Find_ID(Flagpole)).neutral = false;

	var banner = CreateObject(Flagpole, 916, 1070, REPUBLIC);
	banner->SetKingdomBanner();
	banner->MakeInvincible();
	banner.MeshTransformation = Trans_Mul(banner.MeshTransformation, Trans_Scale(500));

	// Close off the area
	g_hatch->MakeInvincible();
	g_hatch->SetObjectLayer(g_hatch);
	g_hatch.lib_structure.basement->MakeInvincible();
	g_hatch.lib_structure.basement->SetObjectLayer(g_hatch);

	// Don't let the player wreak havoc with the chests
	var chests = FindObjects(Find_ID(Chest));
	for (var chest in chests)
	{
		chest->MakeInvincible();
		chest->CreateEffect(ResupplyChest, 1, 70);
	}

	// Late joiners
	g_start_position = [841, 1085];

	return true;
}