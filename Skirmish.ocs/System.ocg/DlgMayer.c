// Private Mayer, guarding the back

#appendto Dialogue

func Dlg_Mayer_1(object clonk)
{
	var options = [["$MsgMayerA1$", "Dlg_Mayer_2"], ["$MsgPrivateLeave$", "StopDialogue()"]];
	MessageBox(Format("$MsgRankGreeting$", GetNewbiesRank()), clonk, g_entrance_guard, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// I need to leave.
func Dlg_Mayer_2(object clonk)
{
	MessageBox("$MsgMayer2$", clonk, g_entrance_guard);
	SetDialogueProgress(3);

	return true;
}

func Dlg_Mayer_3(object clonk)
{
	MessageBox("$MsgMayer3$", clonk, clonk);

	return true;
}

func Dlg_Mayer_4(object clonk)
{
	MessageBox("$MsgMayer4$", clonk, g_entrance_guard);
	SetDialogueProgress(1);

	return true;
}