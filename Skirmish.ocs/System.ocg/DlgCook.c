// The camp's cook, the only one who will talk to you outside of military codex

#appendto Dialogue

func Dlg_Cook_1(object clonk)
{
	var options = [["$MsgCookA1$", "Dlg_Cook_2"], ["$MsgCookA2$", "Dlg_Cook_3"], ["$MsgCookA3$", "Dlg_Cook_6"], ["$MsgCookLeave$", "StopDialogue()"]];
	MessageBox("$MsgCook1$", clonk, g_cook, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// What are you cooking?
func Dlg_Cook_2(object clonk)
{
	MessageBox("$MsgCook2$", clonk, g_cook, nil, false, nil, nil, "$MsgCook2_Short$");
	SetDialogueProgress(1);

	return true;
}

// How much food do we have?
func Dlg_Cook_3(object clonk)
{
	MessageBox(Format("$MsgCook3$", GetNewbiesRank()), clonk, g_cook, nil, false, nil, nil, "$MsgCook3_Short$");
	SetDialogueProgress(4);

	return true;
}

func Dlg_Cook_4(object clonk)
{
	MessageBox("$MsgCook4$", clonk, g_cook, nil, false, nil, nil, "$MsgCook4_Short$");

	return true;
}

func Dlg_Cook_5(object clonk)
{
	MessageBox("$MsgCook5$", clonk, g_cook);
	SetDialogueProgress(1);

	return true;
}

// How is the battle faring?
func Dlg_Cook_6(object clonk)
{
	MessageBox("$MsgCook6$", clonk, g_cook, nil, false, nil, nil, "$MsgCook6_Short$");
	SetDialogueProgress(7);

	return true;
}

func Dlg_Cook_7(object clonk)
{
	MessageBox("$MsgCook7$", clonk, g_cook, nil, false, nil, nil, "$MsgCook7_Short$");

	return true;
}

func Dlg_Cook_8(object clonk)
{
	MessageBox("$MsgCook8$", clonk, g_cook, nil, false, nil, nil, "$MsgCook8_Short$");

	return true;
}

func Dlg_Cook_9(object clonk)
{
	MessageBox("$MsgCook9$", clonk, g_cook);

	return true;
}

func Dlg_Cook_10(object clonk)
{
	MessageBox("$MsgCook10$", clonk, g_cook, nil, false, nil, nil, "$MsgCook10_Short$");

	return true;
}

func Dlg_Cook_11(object clonk)
{
	MessageBox("$MsgCook11$", clonk, g_cook);
	SetDialogueProgress(1);

	return true;
}