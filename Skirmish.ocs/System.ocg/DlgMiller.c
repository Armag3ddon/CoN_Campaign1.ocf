// A sergeant, guarding the general

#appendto Dialogue

func Dlg_Miller_1(object clonk)
{
	MessageBox(Format("$MsgMiller1$", GetNewbiesRank()), clonk, g_g_guard, nil, true);
	g_g_guard->SetDir(DIR_Right);
	SetDialogueProgress(1);
	return StopDialogue();
}