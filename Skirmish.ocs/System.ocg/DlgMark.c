// Corporal Mark, guarding the forward bunker

#appendto Dialogue

func Dlg_Mark_1(object clonk)
{
	var options = [["$MsgMarkA1$", "Dlg_Mark_2"], ["$MsgMarkA2$", "Dlg_Mark_5"], ["$MsgCorporalLeave$", "StopDialogue()"]];
	MessageBox(Format("$MsgRankGreeting$", GetNewbiesRank()), clonk, g_soldiers[1], nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// How is it?
func Dlg_Mark_2(object clonk)
{
	MessageBox("$MsgMark2$", clonk, g_soldiers[1]);
	SetDialogueProgress(3);

	return true;
}

func Dlg_Mark_3(object clonk)
{
	MessageBox("$MsgMark3$", clonk, clonk);

	return true;
}

func Dlg_Mark_4(object clonk)
{
	MessageBox("$MsgMark4$", clonk, g_soldiers[1]);
	SetDialogueProgress(1);

	return true;
}

// Why are you not sniping?
func Dlg_Mark_5(object clonk)
{
	MessageBox("$MsgMark5$", clonk, g_soldiers[1]);
	SetDialogueProgress(1);

	return true;
}

// Clonk is trying to steal stuff
func Dlg_Mark_Pickup(object clonk)
{
	MessageBox(Format("$CommandMarkPickup$", GetNewbiesRank()), clonk, g_soldiers[1], clonk->GetOwner(), true);
}