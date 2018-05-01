// Gunnery Sergeant Hartman, in charge of the artillery platoon

#appendto Dialogue

func Dlg_Gunny_1(object clonk)
{
	clonk->DoSalute();
	var options = [["$MsgGunnyA1$", "Dlg_Gunny_2"], ["$MsgGunnyA2$", "Dlg_Gunny_4"], ["$MsgGunnyLeave$", "StopDialogue()"]];
	MessageBox(Format("$MsgGunny1$", GetNewbiesRank()), clonk, g_gunny, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// Reporting for duty, Sir!
func Dlg_Gunny_2(object clonk)
{
	MessageBox(Format("$MsgGunny2$", GetNewbiesRank()), clonk, g_gunny);
	SetDialogueProgress(3);

	return true;
}

func Dlg_Gunny_3(object clonk)
{
	MessageBox(Format("$MsgGunny3$", GetNewbiesRank()), clonk, g_gunny);
	SetDialogueProgress(1);

	return true;
}

// What are you doing here?
func Dlg_Gunny_4(object clonk)
{
	MessageBox("$MsgGunny4$", clonk, g_gunny);
	SetDialogueProgress(5);

	return true;
}

func Dlg_Gunny_5(object clonk)
{
	MessageBox("$MsgGunny5$", clonk, g_gunny);
	SetDialogueProgress(1);

	return true;
}