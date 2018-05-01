// Samuel: One of your two companions during the campaign
// Techy guy always opts for the path of least resistance

#appendto Dialogue

// Pre attack

func Dlg_Samuel_1(object clonk)
{
	MessageBox("$MsgSamuel1$", clonk);
	SetDialogueProgress(1);
	return StopDialogue();
}

func Dlg_Samuel_2(object clonk)
{
	MessageBox("$MsgSamuel2$", clonk, samuel, nil, false, [["$MsgSamuelINeedIgniter$", "Dlg_Samuel_INeedIgniter"]]);
	SetDialogueProgress(2);
	return true;
}

func Dlg_Samuel_3(object clonk)
{
	MessageBox("$MsgSamuel3$", clonk);
	SetDialogueProgress(3);
	return StopDialogue();
}

func Dlg_Samuel_INeedIgniter(object clonk)
{
	MessageBox("$MsgSamuelGoGetIt$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelGoGetIt_Short$");
	g_get_igniter = true;
	SetDialogueProgress(3);
	return StopDialogue();
}

public func Dlg_Samuel_LeaveChests(object clonk)
{
	MessageBox("$MsgSamuelChests$", nil, nil, clonk->GetOwner());
}

// Post attack

func Dlg_Samuel_SupplyRun1(object clonk)
{
	MessageBox("$MsgSamuelSR1$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelSR1_Short$");
	return true;
}

func Dlg_Samuel_SupplyRun2(object clonk)
{
	MessageBox("$MsgSamuelSR2$", clonk);
	return true;
}

func Dlg_Samuel_SupplyRun3(object clonk)
{
	MessageBoxBroadcast("$MsgSamuelSR3$", clonk);
	return true;
}

func Dlg_Samuel_SupplyRun4(object clonk)
{
	MessageBox("$MsgSamuelSR4$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelSR4_Short$");
	SetPlrView(clonk->GetOwner(), g_door, true);
	g_door->OpenDoor();

	return true;
}

func Dlg_Samuel_SupplyRun5(object clonk)
{
	MessageBox("$MsgSamuelSR5$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelSR5_Short$");
	AddSamuelGoal();

	if (!g_forward_open) {
		g_forward_open = true;
		AddImportantQuestionMark(paulas_dlg, "Question1Asked");
	}

	return StopDialogue();
}

func Dlg_Samuel_SupplyRun6(object clonk)
{
	var options = [["$MsgSamuelSRWhatAre$", "Dlg_Samuel_SupplyRunWhatAre"]];
	var foodstuff = ObjectCount(Find_Or(Find_ID(Seeds), Find_ID(Wheat)));
	if (foodstuff == 0)
		options[GetLength(options)] = ["{{Seeds}} $MsgSamuelSRNoSeeds$", "Dlg_Samuel_SupplyRunNewSeeds"];
	if (SamuelGoalDone())
		options[GetLength(options)] = ["{{Dialogue}} $MsgSamuelSRDone$", "Dlg_Samuel_SupplyRunDone"];
	options[GetLength(options)] = ["$MsgSamuelSRIGo$", "StopDialogue()"];
	MessageBox("$MsgSamuelSR6$", clonk, samuel, nil, false, options);
	SetDialogueProgress(6, "SupplyRun");
	return true;
}

func Dlg_Samuel_SupplyRunWhatAre(object clonk)
{
	MessageBox("$MsgSamuelSRBuildSomething$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelSRBS_Short$");

	SetDialogueProgress(6, "SupplyRun");
	return StopDialogue();
}

func Dlg_Samuel_SupplyRunNewSeeds(object clonk)
{
	MessageBox("$MsgSamuelSRNoSeeds2$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelSRNoS_Short$");

	CreateObjectAbove(Seeds, -5, 5, 11);
	CreateObjectAbove(Seeds, 5, 5, 11);

	SetDialogueProgress(6, "SupplyRun");
	return StopDialogue();
}

func Dlg_Samuel_SupplyRunDone(object clonk)
{
	if (SamuelGoalFinished()) {
		SetDialogueProgress(7, "SupplyRun");
		return StopDialogue();
	}

	var chests = FindObjects(Find_InRect(AbsX(700), AbsY(400), 160, 60), Find_ID(Chest));
	var breads = 0;
	for (var chest in chests)
		breads += chest->ContentsCount(Bread);

	if (breads < 25) {
		this->Message("$MsgSamuelSRDoneSure$");
		SetDialogueProgress(6, "SupplyRun");
		return StopDialogue();
	}

	FinishSamuelGoal();
	MessageBox("$MsgSamuelSRDone2$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelSRD2_Short$");

	SetDialogueProgress(7, "SupplyRun");
	return StopDialogue();
}

func Dlg_Samuel_SupplyRun7(object clonk)
{
	if (PaulaGoalFinished()) {
		var options = [["{{Icon_QuestionMark}} $MsgSamuelSRProceedY$", "Dlg_Samuel_SupplyRunMainQuest"], ["$MsgSamuelSRProceedN$", "Dlg_Samuel_SupplyRunNotYet"]];
		MessageBox("$MsgSamuelSRDone3$", clonk, samuel, nil, false, options);
	} else {
		var options = [["$MsgSamuelSRWhatNext$", "Dlg_Samuel_SupplyRunWhatNext"], ["$MsgSamuelSRIGo$", "StopDialogue()"]];
		MessageBox("$MsgSamuelSRDone3$", clonk, samuel, nil, false, options);
		SetDialogueProgress(7, "SupplyRun");
	}
	return true;
}

func Dlg_Samuel_SupplyRunWhatNext(object clonk)
{
	var msg = "";
	if (!PaulaGoalAdded())
		msg = "$MsgSamuelSRStartPaula$";
	if (!PaulaGoalFinished())
		msg = "$MsgSamuelSRDoPaula$";

	MessageBox(msg, clonk, samuel, nil, nil, nil, nil, "$MsgSamuelSRP_Short$");
	SetDialogueProgress(7, "SupplyRun");

	return StopDialogue();
}

func Dlg_Samuel_SupplyRunMainQuest(object clonk)
{
	StartSequence("MainQuest");
	return StopDialogue();
}

func Dlg_Samuel_SupplyRunNotYet(object clonk)
{
	SetDialogueProgress(7, "SupplyRun");

	return StopDialogue();
}

// Main quest has started

func Dlg_Samuel_MainQuest1(object clonk)
{
	MessageBox("$MsgSamuelMQ1$", clonk);
	return true;
}

func Dlg_Samuel_MainQuest2(object clonk)
{
	if (GetSamuelAttitude() > 50) // Friendly response
	{
		MessageBox("$MsgSamuelMQ2_Friendly$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQ2_Short$");
	} else { // Unfriendly response
		MessageBox("$MsgSamuelMQ2_Unfriendly$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQ2_Short$");
	}
	return true;
}

func Dlg_Samuel_MainQuest3(object clonk)
{
	MessageBoxBroadcast("$MsgSamuelMQ3$", clonk);
	AddMushroomGoal();

	return true;
}

func Dlg_Samuel_MainQuest4(object clonk)
{
	MessageBox("$MsgSamuelMQ4$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQ4_Short$");

	return StopDialogue();
}

func Dlg_Samuel_MainQuest5(object clonk)
{
	var options = [["$MsgSamuelMQPaula$", "Dlg_Samuel_MainQuestPaula"]];

	if (!this.support_lamp) {
		if (!this.support_lamp_count) {
			options[GetLength(options)] = ["$MsgSamuelMQLight$", "Dlg_Samuel_MainQuestLight"];
		} else {
			options[GetLength(options)] = ["$MsgSamuelMQLightLost$", "Dlg_Samuel_MainQuestLight"];
		}
	} else {
		options[GetLength(options)] = ["$MsgSamuelMQLight$", "Dlg_Samuel_MainQuestLight"];
	}

	if (g_bunker_spotted && g_door3) {
		options[GetLength(options)] = ["$MsgSamuelMQDoor$", "Dlg_Samuel_MainQuestDoor"];
	}

	if (MushroomGoalAdded() && clonk->FindContents(Campaign_LittleCaveMushroom)) {
		options[GetLength(options)] = ["$MsgSamuelMQMushroom$", "Dlg_Samuel_MainQuestMushroom"];
	}

	var message = "$MsgSamuelMQ5$";
	if (!g_door3) // bunker was opened
		message = "$MsgSamuelMQ6$";
	options[GetLength(options)] = ["$MsgSamuelMQNo$", "StopDialogue()"];
	MessageBox(message, clonk, samuel, nil, false, options);
	SetDialogueProgress(5, "MainQuest");
	return true;
}

func Dlg_Samuel_MainQuestPaula(object clonk)
{
	MessageBox("$MsgSamuelMQPaulaOkay$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQPaula_Short$");

	SetDialogueProgress(5, "MainQuest");
	return StopDialogue();
}

func Dlg_Samuel_MainQuestLight(object clonk)
{
	// Friendly response
	if (GetSamuelAttitude() > 50)
	{
		if (this.support_lamp == nil) // No lamp exists
		{
			if (this.support_lamp_count) { // Second lamp (final one)
				if (this.support_lamp_count > 1) { // No more lamps
					MessageBox("$MsgSamuelMQLight_FLL2$", clonk);
				} else { // Final lamp
					MessageBox("$MsgSamuelMQLight_FLL$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQLF_Short$");
					var box = CreateObjectAbove(Crate, 0, 10, 11);
					this.support_lamp = box->CreateContents(Lantern);
					this.support_lamp_count = 2;
				}
			} else { // First lamp
				MessageBox("$MsgSamuelMQLight_Friendly$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQLF_Short$");
				var box = CreateObjectAbove(Crate, 0, 10, 11);
				this.support_lamp = box->CreateContents(Lantern);
				this.support_lamp_count = 1;
			}
		} else { // Lamp still exists
			MessageBox("$MsgSamuelMQLight_FriendlyLamp$", clonk);
		}
	} else { // Unfriendly response
		MessageBox("$MsgSamuelMQLight_Unfriendly$", clonk);
	}
	SetDialogueProgress(5, "MainQuest");
	return StopDialogue();
}

func Dlg_Samuel_MainQuestDoor(object clonk)
{
	MessageBox("$MsgSamuelMQDoorBoom$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQDB_Short$");

	SetDialogueProgress(5, "MainQuest");
	return StopDialogue();
}

func Dlg_Samuel_MainQuestMushroom(object clonk)
{
	var mushroom = clonk->FindContents(Campaign_LittleCaveMushroom);
	if (mushroom) {
		if (!mushroom.burned) {
			MessageBox("$MsgSamuelMQMushroomExcellent$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQME_Short$");
			mushroom->RemoveObject();
			CharacterSamuelChange(1); // You get a little respect
			g_mushrooms++;
		} else {
			MessageBox("$MsgSamuelMQMushroomBurned$", clonk, samuel, nil, nil, nil, nil, "$MsgSamuelMQMB_Short$");
		}
	} else {
		MessageBox("$MsgSamuelMQMushroomWhere$", clonk);
	}

	SetDialogueProgress(5, "MainQuest");
	return StopDialogue();
}