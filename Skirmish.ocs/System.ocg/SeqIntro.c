/**
	Intro sequence

	Reporting in to the chief in command.
*/

#appendto Sequence

func Intro_Init()
{
	g_rail.MeshTransformation = Trans_Mul(Trans_Rotate(45,0,1), Trans_Scale(120));
	this.excluded_list = [];

	return true;
}

func Intro_Start(object hero)
{
	SetPlayerZoomByViewRange(NO_OWNER, 200,100, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(NO_OWNER, 200,100, PLRZOOM_LimitMax|PLRZOOM_LimitMin);

	this.hero = hero;

	return ScheduleNext(70);
}

func Intro_JoinPlayer(int plr)
{
	// Late joiner just stand around, they will be excluded from movement commands
	if (this.intro_closed)
	{
		if (this.excluded_list == nil)
			this.excluded_list = [];
		this.excluded_list[GetLength(this.excluded_list)] = plr;
		return false;
	}
	return true;
}

func Intro_1()
{
	MessageBoxAll("$MsgIntro1$", paula, true);
	SetViewTarget(paula);

	return ScheduleNext(110);
}

func Intro_2()
{
	samuel->SetCommand("MoveTo", nil, 75, 790);

	return ScheduleNext(20);
}

func Intro_3()
{
	MessageBoxAll("$MsgIntro2$", g_entrance_guard, true);
	return ScheduleNext(150);
}

func Intro_4()
{
	MessageBoxAll("$MsgIntro3$", samuel, true);

	samuel->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, samuel->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(1000,1000,0, 15, ANIM_Remove));
	samuel->PlayAnimation("AimArmsGeneric.R", CLONK_ANIM_SLOT_Movement + 1, Anim_Const(1100), Anim_Linear(0,0,1000, 15, ANIM_Hold));

	return ScheduleNext(110);
}

func Intro_5()
{
	samuel->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, samuel->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Remove), Anim_Linear(0,0,1000, 15, ANIM_Remove));

	return ScheduleNext(30);
}

func Intro_6()
{
	MessageBoxAll("$MsgIntro4$", g_entrance_guard, true);
	samuel->SetAction("Walk");

	g_entrance_guard->DoSalute(nil, g_entrance_guard, 130);

	return ScheduleNext(150);
}

func Intro_7()
{
	MessageBoxAll("$MsgIntro5$", samuel, true);

	return ScheduleNext(180);
}

func Intro_8()
{
	MessageBoxAll("$MsgIntro6$", g_entrance_guard, true);
	g_rail->CreateEffect(GateOpen, 1, 1);
	g_rail->Sound("Structures::DoorUnlock");

	return ScheduleNext(110);
}

local GateOpen = new Effect {
	Construction = func() {
		this.rot = 45;
	},
	Timer = func() {
		this.rot--;
		g_rail.MeshTransformation = Trans_Mul(Trans_Rotate(this.rot,0,1), Trans_Scale(120));
		if (this.rot <= 0)
			return -1;
	}
};

func Intro_9()
{
	samuel->SetDir(DIR_Left);
	MessageBoxAll("$MsgIntro7$", samuel, true);

	return ScheduleNext(110);
}

func Intro_10()
{
	samuel->CreateEffect(MoveToGeneral, 1, 5);
	paula->CreateEffect(MoveToGeneral, 1, 5);

	// Needs to be done for them to catch the ladders
	samuel->SetObjectLayer(nil);
	paula->SetObjectLayer(nil);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
			GetCrew(plr, j)->CreateEffect(MoveToGeneral, 1, 5);
	}

	this.intro_closed = true;

	return ScheduleNext(5);
}

local MoveToGeneral = new Effect {
	Construction = func () {
		this.Target->SetCommand("MoveTo", nil, 287, 805);
		this.stage = 1;
	},
	Timer = func() {
		if (this.stage == 1)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->SetCommand("Wait", nil, nil, nil, nil, 20);
			this.Target->SetDir(DIR_Left);
			this.stage = 2;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.Target->SetDir(DIR_Left);
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->SetComDir(COMD_Left);
			this.Target->Jump();
			this.stage = 3;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			if (this.Target->GetAction() != "Climb")
				return FX_OK;
			this.Target->SetComDir(COMD_Up);
			this.stage = 4;
			return FX_OK;
		}
		if (this.stage == 4)
		{
			if (this.Target->GetAction() != "Walk")
				return FX_OK;
			if (this.Target == samuel)
			{
				this.Target->SetCommand("Wait", nil, nil, nil, nil, 10);
				this.stage = 10;
				return FX_OK;
			} else {
				this.Target->SetCommand("Wait", nil, nil, nil, nil, 45);
				this.stage = 5;
				return FX_OK;
			}
		}
		if (this.stage == 5)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			if (this.Target == paula)
			{
				this.Target->SetCommand("MoveTo", nil, 157, 743);
				return -1;
			} else {
				this.Target->SetCommand("MoveTo", nil, 162 + RandomX(-2,2), 743);
				return -1;
			}
		}
		if (this.stage == 10) // Samuel only
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->SetCommand("MoveTo", nil, 179, 743);
			this.stage = 11;
		}
		if (this.stage == 11)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->DoSalute();
			g_g_guard->DoSalute();
			this.Target->SetCommand("Wait", nil, nil, nil, nil, 35);
			this.stage = 12;
		}
		if (this.stage == 12)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->SetCommand("MoveTo", nil, 140, 743);
			var cover = FindObject(Find_ID(InventorCover));
			if (cover)
				cover.Visibility[0] = VIS_None;
			g_general->DoKneelComfort();
			this.stage = 13;
			return FX_OK;
		}
		if (this.stage == 13)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			return -1;
		}
	}
};

func Intro_11()
{
	if (GetEffect("MoveToGeneral", samuel))
		return ScheduleSame(5);

	samuel->DoSalute();
	MessageBoxAll("$MsgIntro8$", samuel, true);

	paula->DoSalute();
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
			GetCrew(plr, j)->DoSalute();
	}

	return ScheduleNext(120);
}

func Intro_12()
{
	paula->Message("@$MsgIntro8_Paula$");

	return ScheduleNext(20);
}

func Intro_13()
{
	g_general->SetDir(DIR_Right);

	return ScheduleNext(70);
}

func Intro_14()
{
	MessageBoxAll("$MsgIntro9$", g_general, true);
	paula->Message("");

	return ScheduleNext(140);
}

func Intro_15()
{
	MessageBoxAll("$MsgIntro10$", samuel, true);
	samuel->DoSalute();

	return ScheduleNext(20);
}

func Intro_16()
{
	samuel->CreateOfficersHat();

	return ScheduleNext(90);
}

func Intro_17()
{
	MessageBoxAll("$MsgIntro11$", g_general, true);
	g_general->DoSalute();

	return ScheduleNext(110);
}

func Intro_18()
{
	MessageBoxAll("$MsgIntro12$", g_general, true);

	return ScheduleNext(280);
}

func Intro_19()
{
	MessageBoxAll("$MsgIntro13$", samuel, true);

	return ScheduleNext(80);
}

func Intro_20()
{
	samuel->SetDir(DIR_Right);
	MessageBoxAll("$MsgIntro14$", samuel, true);

	return ScheduleNext(210);
}

func Intro_21()
{
	MessageBoxAll("$MsgIntro15$", paula, true);
	paula->DoSalute();

	return ScheduleNext(110);
}

func Intro_22()
{
	if (GetPlayerCount(C4PT_User) > 1)
		MessageBoxAll("$MsgIntro16_Multi$", paula, true);
	else
		MessageBoxAll("$MsgIntro16_Single$", paula, true);

	paula->CreateEffect(MoveToKitchen, 1, 5);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		if (IsValueInArray(this.excluded_list, plr))
			continue;
		for (var j = 0; j < GetCrewCount(plr); j++)
			GetCrew(plr, j)->CreateEffect(MoveToKitchen, 1, 5);
	}

	var cover = FindObject(Find_ID(InventorCover));
	if (cover)
		cover.Visibility[0] = VIS_Select;

	return ScheduleNext(5);
}

local MoveToKitchen = new Effect {
	Construction = func () {
		this.Target->SetCommand("MoveTo", nil, 239, 743);
		this.stage = 1;
	},
	Timer = func () {
		if (this.stage == 1)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->SetComDir(COMD_Right);
			this.Target->Jump();
			this.stage = 2;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			if (this.Target->GetAction() != "Walk")
				return FX_OK;
			this.Target->SetCommand("MoveTo", nil, 358, 727);
			this.stage = 3;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.Target->SetComDir(COMD_Right);
			this.Target->Jump();
			this.stage = 4;
			return FX_OK;
		}
		if (this.stage == 4)
		{
			if (this.Target->GetAction() != "Walk")
				return FX_OK;
			this.Target->SetCommand("MoveTo", nil, 427+RandomX(-10,10), 705);
			return -1;
		}
	}
};

func Intro_23()
{
	if (GetEffect("MoveToKitchen", paula))
		return ScheduleSame(5);

	MessageBoxAll("$MsgIntro17$", paula, true);

	return ScheduleNext(20);
}

func Intro_24()
{
	g_cook->SetDir(DIR_Left);

	return ScheduleNext(140);
}

func Intro_25()
{
	MessageBoxAll("$MsgIntro18$", g_cook, true);

	return ScheduleNext(210);
}

func Intro_26()
{
	if (!this.hero)
		this.hero = GetHero(paula);

	if (Distance(this.hero->GetX(), this.hero->GetY(), 394, 705) > 300)
	{
		this.hero->SetPosition(394, 705);
		this.hero->SetCommand("None");
		this.hero->SetComDir(COMD_None);
		if (IsValueInArray(this.excluded_list, this.hero->GetOwner()))
			RemoveArrayValue(this.excluded_list, this.hero->GetHero(), true);
	} else {
		this.hero->SetCommand("MoveTo", nil, 392, 705);
	}

	paula->SetCommand("MoveTo", nil, 406, 705);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			if (GetCrew(plr, j) == this.hero)
				continue;
			GetCrew(plr, j)->SetCommand("MoveTo", nil, 422 + RandomX(-10,10), 705);
		}
	}

	MessageBoxAll("$MsgIntro19$", paula, true);

	return ScheduleNext(35);
}

func Intro_27()
{
	if (this.hero)
	{
		this.hero->SetDir(DIR_Right);
		this.hero->DoSitDown();
		this.hero->RemoveBackpack();
		this.sitting_hero = this.hero;
		this.backpack1 = CreateObject(BackpackGraphic, 386, 712, NO_OWNER);
		this.backpack1.MeshTransformation = Trans_Mul(Trans_Scale(650), Trans_Rotate(-45, 0,1));
	}
	paula->DoSitDown();
	paula->RemoveBackpack();
	this.backpack2 = CreateObject(BackpackGraphic, 411, 712, NO_OWNER);
	this.backpack2.MeshTransformation = Trans_Mul(Trans_Scale(650), Trans_Rotate(45, 0,1));

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			if (GetCrew(plr, j) == this.hero)
				continue;
			GetCrew(plr, j)->SetDir(DIR_Left);
		}
	}

	return ScheduleNext(100);
}

local EatAndSit = new Effect {
	Construction = func () {
		this.initialized = false;
		this.stage = 0;
		this.wait_time = 35;
	},
	Timer = func () {
		if (!this.initialized)
		{
			this.wait_time--;
			if (this.wait_time <= 0)
			{
				this.shoulder_anim = this.Target->TransformBone("skeleton_arm_upper.R", Trans_Rotate(0, 1), CLONK_ANIM_SLOT_Movement+1, Anim_Const(500));
				this.hand_anim = this.Target->TransformBone("skeleton_arm_hand.R", Trans_Rotate(0, 1), CLONK_ANIM_SLOT_Movement+1, Anim_Const(500));
				this.hand_close = this.Target->PlayAnimation("Close2Hand", CLONK_ANIM_SLOT_Movement+1, Anim_Const(0), Anim_Const(500));
				this.initialized = true;
				this.wait_time = 0;
			}
			return FX_OK;
		}
		if (this.stage == 0)
		{
			var percentage = this.wait_time * 100 / 20;
			this.wait_time++;
			this.Target->SetAnimationBoneTransform(this.hand_anim, Trans_Rotate(percentage * 45 / 100, 1));
			this.Target->SetAnimationBoneTransform(this.shoulder_anim, Trans_Mul(Trans_Rotate(percentage * -85 / 100, 1), Trans_Rotate(percentage * -150 / 100, 0,0,1)));
			if (this.wait_time == 13)
				this.Target->SetAnimationPosition(this.hand_close, Anim_Linear(0,0, this.Target->GetAnimationLength("Close2Hand"), 7, ANIM_Hold));
			if (this.wait_time == 20)
			{
				this.stage = 1;
				this.wait_time = 0;
			}
			return FX_OK;
		}
		if (this.stage == 1)
		{
			var percentage = this.wait_time * 100 / 20;
			this.wait_time++;
			this.Target->SetAnimationBoneTransform(this.hand_anim, Trans_Mul(Trans_Rotate(45 + (percentage * -90 / 100), 1), Trans_Rotate(percentage * -75 / 100, 0,0,1)));
			this.Target->SetAnimationBoneTransform(this.shoulder_anim, Trans_Mul(Trans_Rotate(-85 + (percentage * -80 / 100), 1), Trans_Rotate(-150 + (percentage * 40 / 100), 0,0,1)));
			if (this.wait_time == 13)
				this.Target->SetAnimationPosition(this.hand_close, Anim_Linear(this.Target->GetAnimationLength("Close2Hand"), this.Target->GetAnimationLength("Close2Hand"), 0, 7, ANIM_Hold));
			if (this.wait_time == 20)
			{
				this.stage = 2;
				this.wait_time = 0;
			}
			return FX_OK;
		}
		if (this.stage == 2)
		{
			var percentage = this.wait_time * 100 / 20;
			this.wait_time++;
			this.Target->SetAnimationBoneTransform(this.hand_anim, Trans_Mul(Trans_Rotate(-45 + (percentage * 45 / 100), 1), Trans_Rotate(-75 + (percentage * 75 / 100), 0,0,1)));
			this.Target->SetAnimationBoneTransform(this.shoulder_anim, Trans_Mul(Trans_Rotate(-165 + (percentage * 165 / 100), 1), Trans_Rotate(-110 + (percentage * 110 / 100), 0,0,1)));
			if (this.wait_time == 20)
			{
				this.stage = 3;
				this.wait_time = 0;
			}
			return FX_OK;
		}
		if (this.stage == 3)
		{
			this.wait_time++;
			if (this.wait_time > Random(100))
			{
				this.stage = 0;
				this.wait_time = 0;
			}
		}
	},
	Destruction = func () {
		if (this.Target)
		{
			this.Target->StopAnimation(this.shoulder_anim);
			this.Target->StopAnimation(this.hand_anim_anim);
			this.Target->StopAnimation(this.hand_close);
		}
	}
};

func Intro_28()
{
	g_cook->SetCommand("MoveTo", nil, 402, 705);

	MessageBoxAll("$MsgIntro20$", g_cook, true);

	return ScheduleNext(80);
}

func Intro_29()
{
	this.bread = CreateObject(Bread, 399, 703, REPUBLIC);
	this.bread->SetCategory(C4D_StaticBack);
	this.bread.Plane = g_cook.Plane - 1;

	if (this.sitting_hero)
		this.sitting_hero->CreateEffect(EatAndSit, 1, 1);
	paula->CreateEffect(EatAndSit, 1, 1);

	return ScheduleNext(115);
}

func Intro_30()
{
	MessageBoxAll("$MsgIntro21$", paula, true);
	g_cook->SetCommand("MoveTo", nil, 446, 705);

	return ScheduleNext(140);
}

func Intro_31()
{
	if (!this.hero)
		this.hero = GetHero(paula);

	MessageBoxAll("$MsgIntro22$", this.hero, true);

	return ScheduleNext(240);
}

func Intro_32()
{
	MessageBoxAll("$MsgIntro23$", paula, true);

	return ScheduleNext(240);
}

func Intro_33()
{
	MessageBoxAll("$MsgIntro24$", paula, true);

	return ScheduleNext(350);
}

func Intro_34()
{
	if (!this.hero)
		this.hero = GetHero(paula);

	MessageBoxAll("$MsgIntro25$", this.hero, true);

	return ScheduleNext(160);
}

func Intro_35()
{
	MessageBoxAll("$MsgIntro26$", paula, true);

	return ScheduleNext(350);
}

func Intro_36()
{
	MessageBoxAll("$MsgIntro27$", paula, true);

	return ScheduleNext(330);
}

func Intro_37()
{
	if (!this.hero)
		this.hero = GetHero(paula);

	MessageBoxAll("$MsgIntro28$", this.hero, true);

	return ScheduleNext(180);
}

func Intro_38()
{
	if (GetPaulaAttitude() > 50) // Paula let's you in on some detail of her past
	{
		MessageBoxAll("$MsgIntro29_Friendly$", paula, true);

		return ScheduleNext(350);
	} else {
		MessageBoxAll("$MsgIntro29_Unfriendly$", paula, true);

		return ScheduleNext(150);
	}
}

func Intro_39()
{
	if (this.bread)
		this.bread->RemoveObject();
	if (this.sitting_hero)
		RemoveEffect("EatAndSit", this.sitting_hero);
	RemoveEffect("EatAndSit", paula);

	MessageBoxAll("$MsgIntro30$", paula, true);

	return ScheduleNext(20);
}

func Intro_40()
{
	if (this.sitting_hero)
	{
		this.sitting_hero->DoSitUp();
		this.sitting_hero->AttachBackpack();
	}
	paula->DoSitUp();
	paula->AttachBackpack();
	if (this.backpack1)
		this.backpack1->RemoveObject();
	if (this.backpack2)
		this.backpack2->RemoveObject();

	return ScheduleNext(50);
}

func Intro_41()
{
	MessageBoxAll("$MsgIntro31$", paula, true);

	return ScheduleNext(200);
}

func Intro_42()
{
	return Stop();
}

func Intro_Stop()
{
	SetPlayerZoomByViewRange(NO_OWNER, 400,300, PLRZOOM_Set);
	SetPlayerZoomByViewRange(NO_OWNER, 500,400, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(NO_OWNER, 200,100, PLRZOOM_LimitMin);

	if (Distance(paula->GetX(), paula->GetY(), 406, 705) > 50)
	{
		paula->SetPosition(406, 705);
		paula->SetDir(DIR_Left);
	}
	if (Distance(samuel->GetX(), samuel->GetY(), 140, 743) > 50)
	{
		samuel->SetPosition(140, 743);
		samuel->SetDir(DIR_Left);
	}

	paulas_dlg->SetInteraction(true);
	paulas_dlg->AddAttention();

	Dialogue->FindByTarget(g_prisoner)->AddAttention();
	Dialogue->FindByTarget(g_cook)->AddAttention();
	Dialogue->FindByTarget(g_medic)->AddAttention();
	Dialogue->FindByTarget(g_soldiers[3])->AddAttention();

	g_headquarters->CloseBarrier();

	AdvanceGoal();

	GameCall("AddNPCIdleEffects");
	GameCall("CreateEnemyGuns");

	return true;
}