/**
	Main quest sequence
	
	Supply run is finished, it is time to fight back!
*/

#appendto Sequence

func MainQuest_Init(object hero)
{
	// Samuel stops doing stuff
	RemoveEffect("Samuel_WorkingEffect", samuel);

	// Set elevator into position
	g_elevator.case->SetPosition(g_elevator.case->GetX(), 560);
	g_elevator.case->StopAutomaticMovement();
	g_elevator.case->Halt();

	// Get players into position
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			// Check if the clonk is around samuel
			if (!Inside(crew->GetX(), 660, 1090) || !Inside(crew->GetY(), 460, 570))
				crew->SetPosition(1037, 550);
		}
	}

	// Question 1 is passé
	g_question1 = true;
	// Question 2 will be passé (don't show the distracting question mark)
	g_question2 = true;

	paulas_dlg->RemoveAttention();
	samuels_dlg->RemoveAttention();

	this.mult = GetScenMapZoom(); // Laziness

	return true;
}

func MainQuest_Start(object hero)
{
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_LimitMax|PLRZOOM_LimitMin);
	SetViewTarget(samuel);

	this.hero = hero;

	samuel->SetDir(DIR_Left);

	return ScheduleNext(35);
}

func MainQuest_JoinPlayer(int plr)
{
	if (!this.elevator_used) {
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			// Check if the clonk is around samuel
			if (!Inside(crew->GetX(), 660, 1090) || !Inside(crew->GetY(), 460, 570))
				crew->SetPosition(1037, 550);
		}
	}

	return true;
}

func MainQuest_1()
{
	MessageBoxAll("$MsgMQ1$", samuel, true);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			crew->SetCommand("MoveTo", nil, 760+RandomX(-20,20), 560);
			// Doesn't work :(
			//crew->AppendCommand("Call", crew, DIR_Right, nil,nil, 15, "CommandedTurn");
			AddEffect("TurnAfterWalking", crew, 1, 10, nil, nil, DIR_Right);
		}
	}

	paula->SetCommand("MoveTo", nil, 795, 560);

	return ScheduleNext(120);
}

func MainQuest_2()
{
	MessageBoxAll("$MsgMQ2$", samuel, true);
	return ScheduleNext(320);
}

func MainQuest_3()
{
	MessageBoxAll("$MsgMQ3$", samuel, true);
	return ScheduleNext(110);
}

func MainQuest_4()
{
	MessageBoxAll("$MsgMQ4$", paula, true);
	return ScheduleNext(70);
}

func MainQuest_5()
{
	MessageBoxAll("$MsgMQ5$", samuel, true);
	return ScheduleNext(70);
}

func MainQuest_6()
{
	MessageBoxAll("$MsgMQ6$", samuel, true);
	return ScheduleNext(320);
}

func MainQuest_7()
{
	MessageBoxAll("$MsgMQ7$", samuel, true);
	return ScheduleNext(150);
}

func MainQuest_8()
{
	MessageBoxAll("$MsgMQ8$", paula, true);
	return ScheduleNext(90);
}

func MainQuest_9()
{
	MessageBoxAll("$MsgMQ9$", samuel, true);
	return ScheduleNext(210);
}

func MainQuest_10()
{
	if (!this.hero)
		this.hero = GetHero();

	MessageBoxAll("$MsgMQ10$", this.hero, true);
	return ScheduleNext(110);
}

// If the first important question has been answered, this dialogue changes
func MainQuest_11()
{
	if (GetPaulaAttitude() < 50) { // Players sided with samuel
		MessageBoxAll("$MsgMQ11_S$", samuel, true);
	} else {
		MessageBoxAll("$MsgMQ11_P$", paula, true);
	}

	return ScheduleNext(280);
}

func MainQuest_12()
{
	MessageBoxAll("$MsgMQ12$", samuel, true);
	return ScheduleNext(280);
}

func MainQuest_13()
{
	// Clear the way of shenanigans the players might have done
	DigFreeRect(81*this.mult,64*this.mult, 56*this.mult,8*this.mult, true);
	DigFreeRect(126*this.mult,37*this.mult, 6*this.mult,37*this.mult, true);

	// Let's move!
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			crew->SetCommand("MoveTo", g_elevator.case);
		}
	}
	paula->SetCommand("MoveTo", g_elevator.case);
	samuel->SetCommand("Grab", g_elevator.case);

	this.wait_time = 0;
	return ScheduleNext(5);
}

func MainQuest_14()
{
	// Wait for samuel to reach the elevator
	if (!samuel->GetActionTarget())
	{
		// But do not wait forever. Maybe a cheeky player blocked the path
		this.wait_time++;
		if (this.wait_time > 70) // roughly 10 seconds, that is too long
		{
			// Warp
			var x = g_elevator.case->GetX();
			var y = g_elevator.case->GetY();
			samuel->EnsureClonkPosition(x,y);
			paula->EnsureClonkPosition(x,y);
			EnsureAllCrewPosition(x,y);
			return ScheduleNext(20);
		}
		return ScheduleSame(5);
	}
	// Give other clonks a few seconds to catch up
	MessageBoxAll("$MsgMQ13$", samuel, true);
	return ScheduleNext(180);
}

func MainQuest_15()
{
	// At this point, it is guaranteed that sam made it but maybe someone is missing (especially bad for paula!)
	paula->EnsureClonkPosition(samuel->GetX(), samuel->GetY(), 10);
	EnsureAllCrewPosition(samuel->GetX(), samuel->GetY(), 10);

	g_elevator.case->SetMoveDirection(COMD_Up);
	this.wait_time = 0;

	return ScheduleNext(5);
}

func MainQuest_16()
{
	// Wait for the elevator to reach a certain height
	if (g_elevator.case->GetY() > 47 * this.mult)
	{
		// But do not wait forever. Maybe a cheeky player blocked the path
		this.wait_time++;
		if (this.wait_time > 105) // roughly 15 seconds, that is too long
		{
			// Warp
			g_elevator.case->SetPosition(g_elevator.case->GetX(), g_elevator->GetY() + 33);
			g_elevator.case->SetMoveDirection(COMD_Up);

			var x = g_elevator.case->GetX();
			var y = g_elevator.case->GetY();
			samuel->EnsureClonkPosition(x,y);
			paula->EnsureClonkPosition(x,y);
			EnsureAllCrewPosition(x,y);
			return ScheduleNext(20);
		}
		return ScheduleSame(5);
	}

	locking_basement->SetPosition(locking_basement->GetX(), 10);
	samuel->Sound("UI::Click");
	this.wait_time = 0;
	return ScheduleNext(5);
}

func MainQuest_17()
{
	// Wait for the elevator to reach the top
	if (g_elevator.case->GetY() > 39 * this.mult)
	{
		// But do not wait forever. Maybe a cheeky player blocked the path
		this.wait_time++;
		if (this.wait_time > 35) // roughly 5 seconds, that is too long
		{
			// Warp
			g_elevator.case->SetPosition(g_elevator.case->GetX(), g_elevator->GetY() + 33);

			var x = g_elevator.case->GetX();
			var y = g_elevator.case->GetY();
			samuel->EnsureClonkPosition(x,y);
			paula->EnsureClonkPosition(x,y);
			EnsureAllCrewPosition(x,y);
			return ScheduleNext(20);
		}
		return ScheduleSame(5);
	}

	locking_basement->SetPosition(locking_basement->GetX(), 332);
	samuel->Sound("UI::Click");
	return ScheduleNext(20);
}

func MainQuest_18()
{
	samuel->SetCommand("UnGrab");
	samuel->AppendCommand("MoveTo", nil, 830, 310);

	paula->SetCommand("MoveTo", nil, 870, 310);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			crew->SetCommand("MoveTo", nil, 760 + RandomX(-10, 5), 260);
		}
	}

	if (GetPlayerCount(C4PT_User) > 1) {
		MessageBoxAll("$MsgMQ14_Multi$", samuel, true);
	} else {
		MessageBoxAll("$MsgMQ14_Single$", samuel, true);
	}

	this.wait_time = 0;
	return ScheduleNext(35);
}

func MainQuest_19()
{
	// Wait for Samuel to reach the stairs
	if (samuel->GetX() > 840)
	{
		// But do not wait forever
		this.wait_time++;
		if (this.wait_time > 35)
		{
			samuel->EnsureClonkPosition(830, 310);
			paula->EnsureClonkPosition(870, 310);
			EnsureAllCrewPosition(730, 310);
			return ScheduleNext(35);
		}
		return ScheduleSame(5);
	}
	return ScheduleNext(35);
}

func MainQuest_20()
{
	g_stairs->FirstPart();
	g_stairs.Plane = samuel.Plane + 1;
	samuel->SetCommand("MoveTo", nil, 854, 298);

	MessageBoxAll("$MsgMQ15$", paula, true);
	this.wait_time = 0;
	return ScheduleNext(35);
}

func MainQuest_21()
{
	// Wait for Samuel to climb the stairs
	if (samuel->GetY() > 296)
	{
		// But do not wait forever
		this.wait_time++;
		if (this.wait_time > 15)
		{
			samuel->EnsureClonkPosition(850, 295, 2);
			return ScheduleNext(1);
		}
		return ScheduleSame(5);
	}

	return ScheduleNext(5);
}

func MainQuest_22()
{
	g_stairs->SecondPart();
	samuel->SetCommand("MoveTo", nil, 822, 285);

	this.wait_time = 0;
	return ScheduleNext(5);
}

func MainQuest_23()
{
	// Wait for Samuel to climb the stairs
	if (samuel->GetX() > 820)
	{
		// But do not wait forever
		this.wait_time++;
		if (this.wait_time > 15)
		{
			samuel->EnsureClonkPosition(823, 283, 2);
			return ScheduleNext(1);
		}
		return ScheduleSame(5);
	}

	return ScheduleNext(5);
}

func MainQuest_24()
{
	samuel->SetDir(DIR_Right);

	MessageBoxAll("$MsgMQ16$", samuel, true);

	return ScheduleNext(70);
}

func MainQuest_25()
{
	MessageBoxAll("$MsgMQ17$", samuel, true);

	return ScheduleNext(55);
}

func MainQuest_26()
{
	var enemy = CreateObject(Clonk, 1880, 90, 11);
	this.plane = CreateObject(Airplane, 1880, 90, 11);
	enemy->Enter(this.plane);
	this.plane->FaceLeft();
	this.plane->PlaneMount(enemy);
	this.plane->StartInstantFlight(-95, 15);

	return ScheduleNext(55);
}

func MainQuest_27()
{
	MessageBoxAll("$MsgMQ18$", samuel, true);
	SetViewTarget(this.plane);

	return ScheduleNext(35);
}

func MainQuest_28()
{
	this.rocket = this.plane->CreateContents(Boompack);
	this.plane->FireRocket(this.rocket, 0, 0);
	var effect = GetEffect("IntControlRocket", this.rocket);
	effect.x = paula->GetX();
	effect.y = paula->GetY();
	this.rocket->SetFuel(999999);
	SetViewTarget(this.rocket);

	return ScheduleNext(5);
}

func MainQuest_29()
{
	if (this.rocket)
		return ScheduleSame(5);

	SetViewTarget(samuel);
	paula->Fling(2, -4);
	MessageBoxAll("$MsgMQ19$", samuel, true);

	g_scout->CreateObjectAbove(Ruin_InventorsLab, 0, g_scout->GetDefBottom() - g_scout->GetY(), 10);
	g_scout->RemoveObject();

	return ScheduleNext(35);
}

func MainQuest_30()
{
	DimPlayers();
	MessageBoxAll("", nil, true);

	return ScheduleNext(15);
}

func MainQuest_31()
{
	if (this.plane)
		this.plane->RemoveObject(false);

	samuel->SetPosition(940, 558);
	paula->SetPosition(920, 558);

	paula->PlayAnimation("Dead", CLONK_ANIM_SLOT_Movement+1, Anim_Const(paula->GetAnimationLength("Dead")), Anim_Const(1000));
	paula.noidleactions = true;

	paula->SetDir(DIR_Left);
	samuel->SetDir(DIR_Left);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			crew->SetPosition(880+RandomX(-20,20), 558);
			crew->SetDir(DIR_Right);
		}
	}

	g_elevator.case->SetPosition(g_elevator.case->GetX(), 560);
	g_elevator.case->StopAutomaticMovement();
	g_elevator.case->Halt();

	// Debugging
		if (GetType(this) == C4V_C4Object)
			return ScheduleNext(50);
}

func MainQuest_32()
{
	SetViewTarget(samuel);
	UndimPlayers();

	return ScheduleNext(35);
}

func MainQuest_33()
{
	MessageBoxAll("$MsgMQ20$", samuel, true);

	return ScheduleNext(90);
}

func MainQuest_34()
{
	if (GetPlayerCount(C4PT_User) > 1)
		MessageBoxAll("$MsgMQ21_Multi$", samuel, true);
	else
		MessageBoxAll("$MsgMQ21_Single$", samuel, true);

	return ScheduleNext(210);
}

func MainQuest_35()
{
	MessageBoxAll("$MsgMQ22$", samuel, true);

	return ScheduleNext(175);
}

func MainQuest_36()
{
	MessageBoxAll("$MsgMQ23$", paula, true);

	return ScheduleNext(90);
}

func MainQuest_37()
{
	MessageBoxAll("$MsgMQ24$", paula, true);

	return ScheduleNext(90);
}

func MainQuest_38()
{
	MessageBoxAll("$MsgMQ25$", samuel, true);

	return ScheduleNext(280);
}

func MainQuest_39()
{
	MessageBoxAll("$MsgMQ26$", paula, true);

	return ScheduleNext(140);
}

func MainQuest_40()
{
	if (!this.hero)
		this.hero = GetHero(samuel);

	MessageBoxAll("$MsgMQ27$", this.hero, true);

	return ScheduleNext(90);
}

func MainQuest_41()
{
	MessageBoxAll("$MsgMQ28$", samuel, true);

	return ScheduleNext(280);
}

func MainQuest_42()
{
	AskImportantQuestion(samuel, "$MsgMQ29$", "$MsgMQ_A$", "$MsgMQ_B$", "MainQuest_SamWins", "MainQuest_PaulaWins");

	this.await_answers = true;
	return ScheduleNext(5);
}

func MainQuest_43()
{
	if (this.await_answers)
		return ScheduleSame(5);

	return ScheduleNext(210);
}

func MainQuest_SamWins()
{
	if (!this.hero)
		this.hero = GetHero(samuel);

	MessageBoxAll("$MsgMQ_A$", this.hero, true);
	this.await_answers = false;

	this.paula_wins = false;
	this.sam_wins = true;

	CharacterSamuelChange(10);
	CharacterPaulaChange(-10);
}

func MainQuest_PaulaWins()
{
	if (!this.hero)
		this.hero = GetHero(samuel);

	MessageBoxAll("$MsgMQ_B$", this.hero, true);
	this.await_answers = false;

	this.paula_wins = true;
	this.sam_wins = false;

	CharacterSamuelChange(-10);
	CharacterPaulaChange(10);
}

func MainQuest_44()
{
	if (this.sam_wins) {
		MessageBoxAll(Format("$MsgMQ30_A$|<c %s>$MsgMQ30_Memory$</c>", GetImportantQuestionColor()), samuel, true);
		CreateObject(ImperialFlamethrower, 771, 515, 11);
	}
	if (this.paula_wins)
		MessageBoxAll(Format("$MsgMQ30_B$|<c %s>$MsgMQ30_Memory$</c>", GetImportantQuestionColor()), samuel, true);

	return ScheduleNext(280);
}

func MainQuest_45()
{
	MessageBoxAll("$MsgMQ31$", samuel, true);

	return ScheduleNext(280);
}

func MainQuest_46()
{
	MessageBoxAll("$MsgMQ32$", samuel, true);
	SetViewTarget(g_door2);
	g_door2->OpenDoor();

	return ScheduleNext(180);
}

func MainQuest_47()
{
	// New dialogue options

	samuels_dlg->SetDialogueProgress(1, "MainQuest", true);
	paulas_dlg->SetDialogueProgress(1, "MainQuest", true);

	return Stop();
}

func MainQuest_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 300,200, PLRZOOM_LimitMin);

	AdvanceGoal();
	AddMetalGoal();

	return true;
}