// Corporal Smith, on guard duty at the armory.

#appendto Dialogue

func Dlg_Smith_1(object clonk)
{
	if (!GetEffect("NPC_Guard_Smith", g_corporal))
	{
		MessageBox("$MsgSmith16$", clonk, g_corporal, nil, true);
		SetDialogueProgress(1);
		return StopDialogue();
	}

	var options = [["$MsgSmithA1$", "Dlg_Smith_2"], ["$MsgSmithA2$", "Dlg_Smith_7"], ["$MsgSmithLeave$", "StopDialogue()"]];
	if (g_question1)
		PushFront(options, ["$MsgSmithA3$", "Dlg_Smith_15"]);

	MessageBox(Format("$MsgSmith1$", GetNewbiesRank()), clonk, g_corporal, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// What are you guarding?
func Dlg_Smith_2(object clonk)
{
	MessageBox("$MsgSmith2$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith2_Short$");
	SetDialogueProgress(3);

	return true;
}

func Dlg_Smith_3(object clonk)
{
	MessageBox("$MsgSmith3$", clonk, clonk, nil, false, nil, nil, "$MsgSmith3_Short$");

	return true;
}

func Dlg_Smith_4(object clonk)
{
	MessageBox("$MsgSmith4$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith4_Short$");

	return true;
}

func Dlg_Smith_5(object clonk)
{
	MessageBox("$MsgSmith5$", clonk, clonk, nil, false, nil, nil, "$MsgSmith5_Short$");

	return true;
}

func Dlg_Smith_6(object clonk)
{
	MessageBox("$MsgSmith6$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith6_Short$");
	SetDialogueProgress(1);

	return true;
}

// Is the fighting bad?
func Dlg_Smith_7(object clonk)
{
	MessageBox("$MsgSmith7$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith7_Short$");
	SetDialogueProgress(8);

	return true;
}

func Dlg_Smith_8(object clonk)
{
	MessageBox("$MsgSmith8$", clonk, clonk, nil, false, nil, nil, "$MsgSmith8_Short$");

	return true;
}

func Dlg_Smith_9(object clonk)
{
	MessageBox("$MsgSmith9$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith9_Short$");

	return true;
}

func Dlg_Smith_10(object clonk)
{
	MessageBox("$MsgSmith10$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith10_Short$");

	return true;
}

func Dlg_Smith_11(object clonk)
{
	MessageBox("$MsgSmith11$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith11_Short$");

	return true;
}

func Dlg_Smith_12(object clonk)
{
	MessageBox("$MsgSmith12$", clonk, g_corporal);

	return true;
}

func Dlg_Smith_13(object clonk)
{
	MessageBox("$MsgSmith13$", clonk, g_corporal, nil, false, nil, nil, "$MsgSmith13_Short$");

	return true;
}

func Dlg_Smith_14(object clonk)
{
	MessageBox("$MsgSmith14$", clonk, g_corporal);
	SetDialogueProgress(1);

	return true;
}

// Corporal. Here are my orders.
func Dlg_Smith_15(object clonk)
{
	MessageBoxBroadcast("$MsgSmith15$", clonk, g_corporal);
	RemoveEffect("NPC_Guard_Smith", g_corporal);
	SetDialogueProgress(1);

	return StopDialogue();
}