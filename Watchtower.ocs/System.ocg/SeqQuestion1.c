/**
	Question 1
	
	Just to explain this question business.
*/

#appendto Sequence

func Question1_Start(object hero)
{
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_LimitMax|PLRZOOM_LimitMin);

	this.hero = hero;

	SetViewTarget(paula);

	MessageBoxAll("$MsgQuestion1_1$", this.hero, true);

	return ScheduleNext(140);
}

func Question1_1()
{
	MessageBoxAll("$MsgQuestion1_2$", paula, true);
	return ScheduleNext(210);
}

func Question1_2()
{
	if (!this.hero)
		this.hero = GetHero(paula);
	MessageBoxAll("$MsgQuestion1_3$", this.hero, true);
	return ScheduleNext(70);
}

func Question1_3()
{
	MessageBoxAll("$MsgQuestion1_4$", paula, true);
	return ScheduleNext(280);
}

func Question1_4()
{
	MessageBoxAll("$MsgQuestion1_5$", paula, true);
	return ScheduleNext(280);
}

func Question1_5()
{
	MessageBoxAll("$MsgQuestion1_6$", paula, true);
	return ScheduleNext(210);
}

func Question1_6()
{
	AskImportantQuestion(paula, "$MsgQuestion1_7$", "$MsgQuestion1_A$", "$MsgQuestion1_B$", "Question1_PaulaWins", "Question1_SamWins");

	this.await_answers = true;
	return ScheduleNext(5);
}

func Question1_7()
{
	if (this.await_answers)
		return ScheduleSame(5);

	return ScheduleNext(210);
}

// The first question is more of a tutorial questions and optional
// It doesn't affect the gameplay that much

func Question1_PaulaWins()
{
	if (!this.hero)
		this.hero = GetHero(paula);

	MessageBoxAll("$MsgQuestion1_A$", this.hero, true);
	this.await_answers = false;

	this.paula_wins = true;
	this.sam_wins = false;

	CharacterPaulaChange(2);
}

func Question1_SamWins()
{
	if (!this.hero)
		this.hero = GetHero(paula);

	MessageBoxAll("$MsgQuestion1_B$", this.hero, true);
	this.await_answers = false;

	this.paula_wins = false;
	this.sam_wins = true;

	CharacterPaulaChange(-2);
}

func Question1_8()
{
	if (this.paula_wins)
		MessageBoxAll(Format("$MsgQuestion1_A_1$|<c %s>$MsgQuestion1_Memory$</c>", GetImportantQuestionColor()), paula, true);
	if (this.sam_wins)
		MessageBoxAll(Format("$MsgQuestion1_B_1$|<c %s>$MsgQuestion1_Memory$</c>", GetImportantQuestionColor()), paula, true);

	return ScheduleNext(210);
}

func Question1_9()
{
	MessageBoxAll("$MsgQuestion1_8$", paula, true);
	return ScheduleNext(70);
}

func Question1_10()
{
	return Stop();
}

func Question1_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 300,200, PLRZOOM_LimitMin);

	g_question1 = true;

	return true;
}