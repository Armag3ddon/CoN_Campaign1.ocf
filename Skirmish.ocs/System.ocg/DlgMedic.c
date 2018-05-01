// The camp's medic, very busy with all the wounded.

#appendto Dialogue

func Dlg_Medic_1(object clonk)
{
	var options = [["$MsgMedicA1$", "Dlg_Medic_2"], ["$MsgMedicA2$", "Dlg_Medic_3"], ["$MsgMedicLeave$", "StopDialogue()"]];
	MessageBox("$MsgMedic1$", clonk, g_medic, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// You have painkillers?
func Dlg_Medic_2(object clonk)
{
	MessageBox("$MsgMedic2$", clonk, g_medic, nil, false, nil, nil, "$MsgMedic2_Short$");
	SetDialogueProgress(1);

	return true;
}

// How are the wounded?
func Dlg_Medic_3(object clonk)
{
	MessageBox("$MsgMedic3$", clonk, g_medic, nil, false, nil, nil, "$MsgMedic3_Short$");
	SetDialogueProgress(4);

	return true;
}

func Dlg_Medic_4(object clonk)
{
	MessageBox("$MsgMedic4$", clonk, g_medic, nil, false, nil, nil, "$MsgMedic4_Short$");

	return true;
}

func Dlg_Medic_5(object clonk)
{
	MessageBox("$MsgMedic5$", clonk, g_medic, nil, false, nil, nil, "$MsgMedic5_Short$");
	SetDialogueProgress(1);

	return true;
}