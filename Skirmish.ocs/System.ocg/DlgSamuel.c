// Samuel: One of your two companions during the campaign
// Techy guy always opts for the path of least resistance

#appendto Dialogue

// Prelude

func Dlg_Samuel_1(object clonk)
{
	var options = [];
	if (GetEffect("MultiplayerWaitArea", this))
		options = [["$MsgSamuelLeave$", "StopDialogue()"]];
	else
		options = [["{{Icon_QuestionMark}} $MsgSamuelA1$", "Dlg_Samuel_2"], ["$MsgSamuelLeave$", "StopDialogue()"]];
	MessageBox("$MsgSamuel1$", clonk, samuel, nil, false, options);
	SetDialogueProgress(1);
	return true;
}

// Yes.
func Dlg_Samuel_2(object clonk)
{
	if (GetPlayerCount(C4PT_User) >= 1)
	{
		if (CreateMultiplayerWaitingRadius(this, 100, "Dlg_Samuel_3", clonk))
		{
			MessageBox("$MsgSamuel2_Negative$", clonk, samuel);
			SetDialogueProgress(1);
		} else {
			MessageBox("$MsgSamuel2_Positive$", clonk, samuel);
			SetDialogueProgress(3);
		}
	} else {
		MessageBox("$MsgSamuel2_Positive$", clonk, samuel);
		SetDialogueProgress(3);
	}

	return true;
}

func Dlg_Samuel_3(object clonk)
{
	StartSequence("Briefing", nil, clonk);
	return StopDialogue();
}