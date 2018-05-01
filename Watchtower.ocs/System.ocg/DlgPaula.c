// Paula: One of your two companions during the campaign
// Smart thinker, will remind you of your conscience

#appendto Dialogue

// Pre attack

func Dlg_Paula_1(object clonk)
{
	MessageBox("$MsgPaula1$", clonk, paula);
	return true;
}

func Dlg_Paula_2(object clonk)
{
	MessageBox("$MsgPaula2$", clonk, paula, nil, nil, nil, nil, "$MsgPaula2_Short$");

	if (!this.advanced_igniter)
	{
		samuels_dlg->SetDialogueProgress(2);
		samuels_dlg->AddAttention();
		AdvanceGoal();
		this.advanced_igniter = true;
	}

	return StopDialogue();
}

func Dlg_Paula_3(object clonk)
{
	var options = [["$MsgPaulaWhereIsSam$", "Dlg_Paula_WhereIsSam"], ["$MsgPaulaBye$", "StopDialogue()"]];
	if (clonk->FindContents(Igniter))
		options = [["$MsgPaulaGotIt$", "Dlg_Paula_Igniter"], ["$MsgPaulaWhereIsSam$", "Dlg_Paula_WhereIsSam"], ["$MsgPaulaBye$", "StopDialogue()"]];
	MessageBox("$MsgPaula3$", clonk, paula, nil, false, options);
	SetDialogueProgress(3);
	return true;
}

func Dlg_Paula_WhereIsSam(object clonk)
{
	MessageBox("$MsgPaulaSam$", clonk, paula);
	return StopDialogue();
}

func Dlg_Paula_Igniter(object clonk)
{
	var igniter = clonk->FindContents(Igniter);
	if (igniter)
		StartSequence("Attack", 0, clonk, igniter);
}

// Post attack

func Dlg_Paula_SupplyRun1(object clonk)
{
	MessageBox("$MsgPaulaSR1$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaSR1_Short$");
	return true;
}

func Dlg_Paula_SupplyRun2(object clonk)
{
	MessageBox("$MsgPaulaSR2$", clonk);
	return true;
}

func Dlg_Paula_SupplyRun3(object clonk)
{
	MessageBoxBroadcast("$MsgPaulaSR3$", clonk, paula);
	return true;
}

func Dlg_Paula_SupplyRun4(object clonk)
{
	MessageBox("$MsgPaulaSR4$", clonk);
	return true;
}

func Dlg_Paula_SupplyRun5(object clonk)
{
	MessageBox("$MsgPaulaSR5$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaSR5_Short$");
	AddPaulaGoal();

	return StopDialogue();
}

func Dlg_Paula_SupplyRun6(object clonk)
{
	var options = [["$MsgPaulaSRHowIsIt$", "Dlg_Paula_SupplyRunHowIsIt"]];
	if (g_forward_open && !g_question1)
		options[GetLength(options)] = ["{{Icon_QuestionMark}} $MsgPaulaSRForwardGates$", "Dlg_Paula_SupplyRunGates"];
	if (PaulaGoalDone())
		options[GetLength(options)] = ["{{Dialogue}} $MsgPaulaSRDone$", "Dlg_Paula_SupplyRunDone"];
	options[GetLength(options)] = ["$MsgPaulaSRIGo$", "StopDialogue()"];

	MessageBox("$MsgPaulaSR6$", clonk, paula, nil, false, options);
	SetDialogueProgress(6, "SupplyRun");
	return true;
}

func Dlg_Paula_SupplyRunGates(object clonk)
{
	SetDialogueProgress(6, "SupplyRun");
	StartSequence("Question1", 0, clonk);
	return StopDialogue();
}

func Dlg_Paula_SupplyRunHowIsIt(object clonk)
{
	MessageBox("$MsgPaulaSRChatting$", clonk);
	SetDialogueProgress(6, "SupplyRun");

	return StopDialogue();
}

func Dlg_Paula_SupplyRunDone(object clonk)
{
	MessageBox("$MsgPaulaSRDone2$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaSRD2_Short$");
	SetDialogueProgress(7, "SupplyRun");

	return true;
}

func Dlg_Paula_SupplyRun7(object clonk)
{
	if (this.work_done) { // two or more people talking at once could really mess things up!
		// This might lead to showing the following dialogue more than once
		SetDialogueProgress(8, "SupplyRun");
		return StopDialogue();
	}

	var lorries = paula->FindObjects(Find_InRect(-50, -50, 150, 65), Find_ID(Lorry), Find_Layer());
	var most_filled = nil;
	for (var lorry in lorries) {
		if (!most_filled || most_filled->ContentsCount(Rock) < lorry->ContentsCount(Rock))
			most_filled = lorry;
	}
	if (most_filled) {
		if (most_filled->ContentsCount(Rock) >= 40) {
			for (var i = 0; i < 40; i++)
				if (most_filled->FindContents(Rock))
					most_filled->FindContents(Rock)->RemoveObject();
			this.work_done = true;
		}
	}

	if (!this.work_done) {
		this->Message("$MsgPaulaSRDoneSure$");
		SetDialogueProgress(6, "SupplyRun");
		return StopDialogue();
	}

	var effect = GetEffect("Paula_WorkingEffect", paula);
	if (effect) // should always be there, otherwise ???
	{
		effect->SpeedUp();
		SetInteraction(false);
	}

	FinishPaulaGoal();
	return StopDialogue();
}

func Dlg_Paula_SupplyRun8(object clonk)
{
	MessageBox("$MsgPaulaSRDone3$", clonk);

	return true;
}

func Dlg_Paula_SupplyRun9(object clonk)
{
	var options = [["$MsgPaulaSRWhatNext$", "Dlg_Paula_SupplyRunWhatNext"]];
	if (g_forward_open && !g_question1)
		options[GetLength(options)] = ["{{Icon_QuestionMark}} $MsgPaulaSRForwardGates$", "Dlg_Paula_SupplyRunGates"];
	options[GetLength(options)] = ["$MsgPaulaSRIGo$", "StopDialogue()"];

	MessageBox("$MsgPaulaSRDone4$", clonk, paula, nil, false, options);
	SetDialogueProgress(9, "SupplyRun");
	return true;
}

func Dlg_Paula_SupplyRunWhatNext(object clonk)
{
	var msg = "";
	if (!SamuelGoalAdded())
		msg = "$MsgPaulaSRStartSamuel$";
	if (!SamuelGoalFinished())
		msg = "$MsgPaulaSRDoSamuel$";
	if (SamuelGoalFinished())
		msg = "$MsgPaulaSRDoneAll$";

	MessageBox(msg, clonk, paula, nil, nil, nil, nil, "$MsgPaulaSRDAll_Short$");
	SetDialogueProgress(9, "SupplyRun");

	return StopDialogue();
}

// Main quest has started

func Dlg_Paula_MainQuest1(object clonk)
{
	MessageBox("$MsgPaulaMQ1$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaMQ1_Short$");
	return true;
}

func Dlg_Paula_MainQuest2(object clonk)
{
	if (GetPaulaAttitude() > 50) // Friendly response
	{
		MessageBox("$MsgPaulaMQ2_Friendly$", clonk);
	} else { // Unfriendly response
		MessageBox("$MsgPaulaMQ2_Unfriendly$", clonk);
	}
	return true;
}

func Dlg_Paula_MainQuest3(object clonk)
{
	MessageBox("$MsgPaulaMQ3$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaMQ3_Short$");

	return StopDialogue();
}

func Dlg_Paula_MainQuest4(object clonk)
{
	var options = [
		["$MsgPaulaMQConcern1$", "Dlg_Paula_MainQuestConcern1"],
		["$MsgPaulaMQLight$", "Dlg_Paula_MainQuestLight"],
		["$MsgPaulaMQYes$", "StopDialogue()"]];

	MessageBox("$MsgPaulaMQ4$", clonk, paula, nil, false, options);
	SetDialogueProgress(4, "MainQuest");
	return true;
}

func Dlg_Paula_MainQuestConcern1(object clonk)
{
	MessageBox("$MsgPaulaMQConcern2$", clonk, paula, nil, false, [["$MsgPaulaMQConcern3$", "Dlg_Paula_MainQuestConcern2"]]);

	SetDialogueProgress(4, "MainQuest");
	return true;
}

func Dlg_Paula_MainQuestConcern2(object clonk)
{
	MessageBox("$MsgPaulaMQConcern4$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaMQC_Short$");

	SetDialogueProgress(4, "MainQuest");
	return StopDialogue();
}

func Dlg_Paula_MainQuestLight(object clonk)
{
	// Friendly response
	if (GetPaulaAttitude() > 50)
	{
		if (this.support_torches == nil) // No torches given
		{
			MessageBox("$MsgPaulaMQLight_Friendly$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaMQLF_Short$");
			var box = CreateObjectAbove(Crate, 0, 10, 11);
			box->CreateContents(Torch, 5);
			this.support_torches = true;
		} else { // Torches already given
			MessageBox("$MsgPaulaMQLight_Friendly2$", clonk);
		}
	} else { // Unfriendly response
		MessageBox("$MsgPaulaMQLight_Unfriendly$", clonk, paula, nil, nil, nil, nil, "$MsgPaulaMQLU_Short$");
	}
	SetDialogueProgress(4, "MainQuest");
	return StopDialogue();
}