// Lieutenant Bort, explosives armorer

#appendto Dialogue

func Dlg_Bort_1(object clonk)
{
	var options = [["$MsgBortA1$", "Dlg_Bort_2"], ["$MsgBortA2$", "Dlg_Bort_6"], ["$MsgBortLeave$", "StopDialogue()"]];
	MessageBox(Format("$MsgRankGreeting$", GetNewbiesRank()), clonk, g_armorer, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// You have a torch on you!
func Dlg_Bort_2(object clonk)
{
	MessageBox("$MsgBort2$", clonk, g_armorer, nil, false, nil, nil, "$MsgBort2_Short$");
	SetDialogueProgress(3);

	return true;
}

func Dlg_Bort_3(object clonk)
{
	MessageBox("$MsgBort3$", clonk, clonk);

	return true;
}

func Dlg_Bort_4(object clonk)
{
	MessageBox("$MsgBort4$", clonk, g_armorer, nil, false, nil, nil, "$MsgBort4_Short$");

	return true;
}

func Dlg_Bort_5(object clonk)
{
	MessageBox("$MsgBort5$", clonk, g_armorer, nil, false, nil, nil, "$MsgBort5_Short$");
	SetDialogueProgress(1);

	return true;
}

// You are the armorer?
func Dlg_Bort_6(object clonk)
{
	MessageBox("$MsgBort6$", clonk, g_armorer);
	SetDialogueProgress(7);

	return true;
}

func Dlg_Bort_7(object clonk)
{
	MessageBox("$MsgBort7$", clonk, g_armorer, nil, false, nil, nil, "$MsgBort7_Short$");

	return true;
}

func Dlg_Bort_8(object clonk)
{
	MessageBox("$MsgBort8$", clonk, g_armorer, nil, false, nil, nil, "$MsgBort8_Short$");

	return true;
}

func Dlg_Bort_9(object clonk)
{
	MessageBox("$MsgBort9$", clonk, g_armorer);
	SetDialogueProgress(1);

	return true;
}

// Clonk is trying to steal stuff
func Dlg_Bort_Pickup(object clonk)
{
	MessageBox(Format("$CommandBortPickup$", GetNewbiesRank()), clonk, g_armorer, clonk->GetOwner(), true);
}