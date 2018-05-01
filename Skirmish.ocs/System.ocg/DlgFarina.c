// Private Farina, guarding the gate

#appendto Dialogue

func Dlg_Farina_1(object clonk)
{
	var options = [["$MsgFarinaA1$", "Dlg_Farina_2"], ["$MsgPrivateLeave$", "StopDialogue()"]];
	MessageBox(Format("$MsgRankGreeting$", GetNewbiesRank()), clonk, g_gate_guard, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// Can I go outside?
func Dlg_Farina_2(object clonk)
{
	MessageBox("$MsgFarina2$", clonk, g_gate_guard);
	SetDialogueProgress(3);

	return true;
}

func Dlg_Farina_3(object clonk)
{
	MessageBox("$MsgFarina3$", clonk, g_gate_guard);

	return true;
}

func Dlg_Farina_4(object clonk)
{
	MessageBox("$MsgFarina4$", clonk, g_gate_guard);
	SetDialogueProgress(1);

	return true;
}