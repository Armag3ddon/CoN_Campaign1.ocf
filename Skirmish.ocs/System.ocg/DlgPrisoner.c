// A prisoner of war who is forced to shovel coal into the steam engine

#appendto Dialogue

func Dlg_Prisoner_1(object clonk)
{
	var options = [["$MsgPrisonerA1$", "Dlg_Prisoner_2"], ["$MsgPrisonerA2$", "Dlg_Prisoner_5"], ["$MsgPrisonerA3$", "Dlg_Prisoner_9"], ["$MsgPrisonerA4$", "Dlg_Prisoner_13"], ["$MsgPrisonerLeave$", "StopDialogue()"]];
	MessageBox("$MsgPrisoner1$", clonk, g_prisoner, nil, false, options, nil, "$MsgPrisoner1_Short$");
	SetDialogueProgress(1);
	return true;
}

// Why?
func Dlg_Prisoner_2(object clonk)
{
	MessageBox("$MsgPrisoner2$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner2_Short$");
	SetDialogueProgress(3);

	return true;
}

func Dlg_Prisoner_3(object clonk)
{
	MessageBox("$MsgPrisoner3$", clonk, clonk);

	return true;
}

func Dlg_Prisoner_4(object clonk)
{
	MessageBox("$MsgPrisoner4$", clonk, g_prisoner);
	SetDialogueProgress(1);

	return true;
}

// Who are you?
func Dlg_Prisoner_5(object clonk)
{
	MessageBox("$MsgPrisoner5$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner5_Short$");
	SetDialogueProgress(6);

	return true;
}

func Dlg_Prisoner_6(object clonk)
{
	MessageBox("$MsgPrisoner6$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner6_Short$");

	return true;
}

func Dlg_Prisoner_7(object clonk)
{
	MessageBox("$MsgPrisoner7$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner7_Short$");

	return true;
}

func Dlg_Prisoner_8(object clonk)
{
	MessageBox("$MsgPrisoner8$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner8_Short$");
	GameCall("TalkedToPrisoner");
	SetDialogueProgress(1);

	return true;
}

// Why have you attacked us?
func Dlg_Prisoner_9(object clonk)
{
	MessageBox("$MsgPrisoner9$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner9_Short$");
	SetDialogueProgress(10);

	return true;
}

func Dlg_Prisoner_10(object clonk)
{
	MessageBox("$MsgPrisoner10$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner10_Short$");

	return true;
}

func Dlg_Prisoner_11(object clonk)
{
	MessageBox("$MsgPrisoner11$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner11_Short$");

	return true;
}

func Dlg_Prisoner_12(object clonk)
{
	MessageBox("$MsgPrisoner12$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner12_Short$");
	SetDialogueProgress(1);

	return true;
}

// Will your people free you?
func Dlg_Prisoner_13(object clonk)
{
	MessageBox("$MsgPrisoner13$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner13_Short$");
	SetDialogueProgress(14);

	return true;
}

func Dlg_Prisoner_14(object clonk)
{
	MessageBox("$MsgPrisoner14$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner14_Short$");

	return true;
}

func Dlg_Prisoner_15(object clonk)
{
	MessageBox("$MsgPrisoner15$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner15_Short$");

	return true;
}

func Dlg_Prisoner_16(object clonk)
{
	MessageBox("$MsgPrisoner16$", clonk, g_prisoner, nil, false, nil, nil, "$MsgPrisoner16_Short$");

	return true;
}

func Dlg_Prisoner_17(object clonk)
{
	MessageBox("$MsgPrisoner17$", clonk, g_prisoner);
	SetDialogueProgress(1);

	return true;
}