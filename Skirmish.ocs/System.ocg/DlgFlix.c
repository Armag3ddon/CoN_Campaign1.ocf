// Private Flix, observing from the back bunker

#appendto Dialogue

func Dlg_Flix_1(object clonk)
{
	var options = [["$MsgFlixA1$", "Dlg_Flix_2"], ["$MsgFlixA2$", "Dlg_Flix_5"], ["$MsgFlixLeave$", "StopDialogue()"]];
	MessageBox("$MsgFlix1$", clonk, g_soldiers[3], nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// Isn't this dangerous?
func Dlg_Flix_2(object clonk)
{
	MessageBox("$MsgFlix2$", clonk, g_soldiers[3], nil, false, nil, nil, "$MsgFlix2_Short$");
	SetDialogueProgress(3);

	return true;
}

func Dlg_Flix_3(object clonk)
{
	MessageBox("$MsgFlix3$", clonk, g_soldiers[3]);

	return true;
}

func Dlg_Flix_4(object clonk)
{
	MessageBox("$MsgFlix4$", clonk, g_soldiers[3], nil, false, nil, nil, "$MsgFlix4_Short$");
	SetDialogueProgress(1);

	return true;
}

// What do you see?
func Dlg_Flix_5(object clonk)
{
	MessageBox("$MsgFlix5$", clonk, g_soldiers[3], nil, false, nil, nil, "$MsgFlix5_Short$");
	SetDialogueProgress(6);

	return true;
}

func Dlg_Flix_6(object clonk)
{
	MessageBox("$MsgFlix6$", clonk, g_soldiers[3], nil, false, nil, nil, "$MsgFlix6_Short$");

	return true;
}

func Dlg_Flix_7(object clonk)
{
	MessageBox("$MsgFlix7$", clonk, clonk);

	return true;
}

func Dlg_Flix_8(object clonk)
{
	MessageBox("$MsgFlix8$", clonk, g_soldiers[3], nil, false, nil, nil, "$MsgFlix8_Short$");

	return true;
}

func Dlg_Flix_9(object clonk)
{
	MessageBox("$MsgFlix9$", clonk, clonk);

	return true;
}

func Dlg_Flix_10(object clonk)
{
	MessageBox("$MsgFlix10$", clonk, g_soldiers[3]);
	SetDialogueProgress(1);

	return true;
}

// Clonk is trying to steal stuff
func Dlg_Flix_Pickup(object clonk)
{
	MessageBox(Format("$CommandFlixPickup$", GetNewbiesRank()), clonk, g_soldiers[3], clonk->GetOwner(), true);
}