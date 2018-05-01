// Gunnery Sergeant Hartman, in charge of the artillery platoon

#appendto Dialogue

func Dlg_Guinot_1(object clonk)
{
	clonk->DoSalute();
	var options = [["$MsgGuinotA1$", "Dlg_Guinot_2"], ["$MsgGuinotA2$", "Dlg_Guinot_6"], ["$MsgGuinotLeave$", "StopDialogue()"]];
	MessageBox(Format("$MsgRankGreeting$", GetNewbiesRank()), clonk, g_g_major, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// May I ask about the status of the fighting?
func Dlg_Guinot_2(object clonk)
{
	MessageBox("$MsgGuinot2$", clonk, g_g_major, nil, false, nil, nil, "$MsgGuinot2_Short$");
	SetDialogueProgress(3);

	return true;
}

func Dlg_Guinot_3(object clonk)
{
	MessageBox("$MsgGuinot3$", clonk, g_g_major, nil, false, nil, nil, "$MsgGuinot3_Short$");

	return true;
}

func Dlg_Guinot_4(object clonk)
{
	MessageBox("$MsgGuinot4$", clonk, clonk);

	return true;
}

func Dlg_Guinot_5(object clonk)
{
	MessageBox("$MsgGuinot5$", clonk, g_g_major);
	SetDialogueProgress(1);

	return true;
}

// Do you know how long the general's meeting will last?
func Dlg_Guinot_6(object clonk)
{
	MessageBox("$MsgGuinot6$", clonk, g_g_major);
	SetDialogueProgress(7);

	return true;
}

func Dlg_Guinot_7(object clonk)
{
	MessageBox("$MsgGuinot7$", clonk, clonk);

	return true;
}

func Dlg_Guinot_8(object clonk)
{
	MessageBox("$MsgGuinot8$", clonk, g_g_major);
	SetDialogueProgress(1);

	return true;
}