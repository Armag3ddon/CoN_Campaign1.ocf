// Private Derek, hammering

#appendto Dialogue

func Dlg_Derek_1(object clonk)
{
	var options = [["$MsgDerekA1$", "Dlg_Derek_2"], ["$MsgPrivateLeave$", "StopDialogue()"]];
	MessageBox(Format("$MsgRankGreeting$", GetNewbiesRank()), clonk, g_soldiers[2], nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// Why are you hammering?
func Dlg_Derek_2(object clonk)
{
	MessageBox("$MsgDerek2$", clonk, g_soldiers[2]);
	SetDialogueProgress(1);

	return true;
}