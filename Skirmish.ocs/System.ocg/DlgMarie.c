// Private Marie, sniping from the forward bunker

#appendto Dialogue

func Dlg_Marie_1(object clonk)
{
	var options = [["$MsgMarieA1$", "Dlg_Marie_2"], ["$MsgMarieA2$", "Dlg_Marie_3"], ["$MsgMarieLeave$", "StopDialogue()"]];
	MessageBox("$MsgMarie1$", clonk, g_soldiers[0], nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// What are you doing?
func Dlg_Marie_2(object clonk)
{
	MessageBox("$MsgMarie2$", clonk, g_soldiers[0]);
	SetDialogueProgress(1);

	return true;
}

// Are you successful?
func Dlg_Marie_3(object clonk)
{
	MessageBox("$MsgMarie3$", clonk, g_soldiers[0]);
	SetDialogueProgress(1);

	return true;
}