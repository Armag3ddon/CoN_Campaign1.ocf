// Paula: One of your two companions during the campaign
// Smart thinker, will remind you of your conscience

#appendto Dialogue

// Prelude

func Dlg_Paula_1(object clonk)
{
	var options = [["$MsgPaulaA1$", "Dlg_Paula_2"], ["$MsgPaulaA2$", "Dlg_Paula_3"], ["$MsgPaulaLeave$", "StopDialogue()"]];
	MessageBox("$MsgPaula1$", clonk, paula, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// No, he's still with the general.
func Dlg_Paula_2(object clonk)
{
	MessageBox("$MsgPaula2$", clonk, paula);
	SetDialogueProgress(1);

	return true;
}

// Are you alright?
func Dlg_Paula_3(object clonk)
{
	if (GetPaulaAttitude() > 50) // Friendly response
	{
		MessageBox("$MsgPaula3_Friendly$", clonk, paula);
		SetDialogueProgress(1);
	} else {
		MessageBox("$MsgPaula3_Unfriendly1$", clonk, paula);
		SetDialogueProgress(4);
	}

	return true;
}

func Dlg_Paula_4(object clonk)
{
	MessageBox("$MsgPaula3_Unfriendly2$", clonk, clonk);

	return true;
}

func Dlg_Paula_5(object clonk)
{
	MessageBox("$MsgPaula3_Unfriendly3$", clonk, paula);
	SetDialogueProgress(1);

	return true;
}

/* Preparation */

// Before the first battle takes place

func Dlg_Paula_Preparation1(object clonk)
{
	var options = [["$MsgPaulaPrepA1$", "Dlg_Paula_Preparation2"], ["$MsgPaulaPrepA2$", "Dlg_Paula_Preparation5"], ["$MsgPaulaPrepA3$", "Dlg_Paula_Preparation8"], ["$MsgPaulaPrepLeave$", "StopDialogue()"]];
	MessageBox("$MsgPaulaPrep1$", clonk, paula, nil, false, options);
	SetDialogueProgress(1, "Preparation");
	return true;
}

// Ready!
func Dlg_Paula_Preparation2(object clonk)
{
	var options = [["$MsgPaulaPrepA1_Safety2$", "StopDialogue()"], ["$MsgPaulaPrepA1_Safety3$", "Dlg_Paula_Preparation3"]];
	MessageBox("$MsgPaulaPrepA1_Safety$", clonk, paula, nil, false, options);
	SetDialogueProgress(1, "Preparation");

	return true;
}

func Dlg_Paula_Preparation3(object clonk)
{
	if (GetPlayerCount(C4PT_User) >= 1)
	{
		if (CreateMultiplayerWaitingRadius(this, 50, "Dlg_Paula_Preparation4", clonk))
		{
			MessageBox("$MsgPaulaPrep2_Negative$", clonk, paula);
			SetDialogueProgress(1, "Preparation");
		} else {
			MessageBox("$MsgPaulaPrep2_Positive$", clonk, paula);
			SetDialogueProgress(4, "Preparation");
		}
	} else {
		MessageBox("$MsgPaulaPrep2_Positive$", clonk, paula);
		SetDialogueProgress(4, "Preparation");
	}

	return true;
}

func Dlg_Paula_Preparation4(object clonk)
{
	StartSequence("FirstAssault", nil, clonk);
	return StopDialogue();
}

// I don't know what weapons to get.
func Dlg_Paula_Preparation5(object clonk)
{
	MessageBox("$MsgPaulaPrep3$", clonk, paula, nil, false, nil, nil, "$MsgPaulaPrep3_Short$");
	SetDialogueProgress(6, "Preparation");

	return true;
}

func Dlg_Paula_Preparation6(object clonk)
{
	MessageBox("$MsgPaulaPrep4$", clonk, paula, nil, false, nil, nil, "$MsgPaulaPrep4_Short$");

	return true;
}

func Dlg_Paula_Preparation7(object clonk)
{
	MessageBoxBroadcast("$MsgPaulaPrep5$", clonk, paula);

	SetDialogueProgress(1, "Preparation");

	return true;
}

// I'm worried.
func Dlg_Paula_Preparation8(object clonk)
{
	// All negative answer: You have sided with Sam
	if (GetPaulaAttitude() <= 32)
		MessageBox("$MsgPaulaPrep6_AllNeg$", clonk, paula);
	// Negative answer: you staid neutral, but in Watchtower opted for Sam
	if (Inside(GetPaulaAttitude(), 38, 42))
		MessageBox("$MsgPaulaPrep6_Neg$", clonk, paula);
	// Positive answer: in Watchtower, you opted for Paula
	if (Inside(GetPaulaAttitude(), 58, 60))
		MessageBox("$MsgPaulaPrep6_Pos$", clonk, paula);
	// All positive answer: in Watchtower, you opted for Paula regarding BOTH questions!
	if (GetPaulaAttitude() >= 62)
		MessageBox("$MsgPaulaPrep6_AllPos$", clonk, paula);

	SetDialogueProgress(1, "Preparation");

	return true;
}

/* First Assault done */

// Called by the First Assault effect (see FirstAssault.c)
func CheckForBattleFinished(object clonk)
{
	if (GetPlayerCount(C4PT_User) >= 1)
	{
		if (CreateMultiplayerWaitingRadius(this, 100, "BattleFinished", clonk))
		{
			return;
		} else {
			StartSequence("FirstAssaultDone", nil, clonk);
		}
	} else {
		StartSequence("FirstAssaultDone", nil, clonk);
	}
}