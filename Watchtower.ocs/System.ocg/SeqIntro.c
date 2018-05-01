/**
	Intro sequence
	
	Briefly exlpain the situation.
*/

#appendto Sequence

static const Intro_CarryEffect = {
	Timer = func() {
		if (!this.Target->FindContents(Rock))
		{
			if (this.pickup && this.Target->GetX() < 800)
			{
				var rock = this.Target->FindObject(Find_ID(Rock), Find_Distance(20));
				if (rock)
				{
					rock->Enter(this.Target);
					this.pickup = false;
				}
			} else if (!this.pickup)
			{
				this.Target->SetCommand("MoveTo", nil, 790 + RandomX(-4, 4), 810);
				this.pickup = true;
			}
		} else {
			if (!this.drop)
			{
				this.Target->SetCommand("MoveTo", nil, 945 + RandomX(-4, 4), 810);
				this.drop = true;
			} else if (this.Target->GetX() > 935)
			{
				this.Target->ControlThrow(this.Target->FindContents(Rock), 915, 810);
				this.drop = false;
			}
		}
	}
};

func Intro_Init()
{
	for (var i = 0; i < 20; i++)
	{
		var x = 790 + RandomX(-5, 5);
		var y = 810;
		while (!GBackSolid(x, y))
			y++;
		y--;
		CreateObjectAbove(Rock, x, y);
	}

	return true;
}

func Intro_Start(object hero)
{
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 300,150, PLRZOOM_LimitMax|PLRZOOM_LimitMin);

	this.hero = hero;

	return ScheduleNext(140);
}

func Intro_JoinPlayer(int plr)
{
	// Late joiner just stand around
	if (this.intro_closed) return false;
	var crew;
	for(var index = 0; crew = GetCrew(plr, index); ++index)
	{
		crew->CreateEffect(Intro_CarryEffect, 1, 35);
	}
	return true;
}

func Intro_1()
{
	MessageBoxAll("$MsgIntro1$", this.hero, true);
	return ScheduleNext(280);
}

func Intro_2()
{
	MessageBoxAll("$MsgIntro2$", this.hero, true);
	return ScheduleNext(210);
}

func Intro_3()
{
	this.intro_closed = true;

	if (GetPlayerCount(C4PT_User) == 1)
		MessageBoxAll("$MsgIntroPaula_Single$", paula, true);
	else
		MessageBoxAll("$MsgIntroPaula_Multi$", paula, true);
	return ScheduleNext(140);
}

func Intro_4()
{
	return Stop();
}

func Intro_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 300,200, PLRZOOM_LimitMin);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
			GetCrew(plr, j)->RemoveEffect("Intro_CarryEffect", GetCrew(plr, j));
	}

	paulas_dlg->SetInteraction(true);
	paulas_dlg->AddAttention();
	samuels_dlg->SetInteraction(true);
	AdvanceGoal();

	return true;
}