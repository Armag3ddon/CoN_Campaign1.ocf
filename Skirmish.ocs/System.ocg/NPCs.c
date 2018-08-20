// NPC are being busy and not boring like boring NPCs

static const NPC_Idle_General = new Effect {
	Construction = func () {
		this.stage = 0;
		this.wait_time = 0;
	},
	Timer = func () {
		if (this.stage == 0 && Random(15))
		{
			return FX_OK;
		} else if (this.stage == 0) {
			if (Random(2))
			{
				this.stage = 1;
			} else {
				this.stage = 2;
			}
			return FX_OK;
		}
		if (this.stage == 1)
		{
			if (this.wait_time >= 4)
			{
				this.wait_time = 0;
				this.stage = 0;
				return FX_OK;
			}
			if (this.wait_time == 0)
				this.Target->DoKneelComfort();
			this.wait_time++;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.Target->SetCommand("MoveTo", nil, 142, 741);
			this.stage = 3;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			if (Random(10))
				return FX_OK;
			this.Target->DoSitDown();
			this.stage = 4;
			return FX_OK;
		}
		if (this.stage == 4)
		{
			this.wait_time++;
			if (this.wait_time <= 10)
				return FX_OK;
			if (Random(10))
				return FX_OK;
			this.wait_time = 0;
			this.stage = 5;
			this.Target->DoSitUp();
			return FX_OK;
		}
		if (this.stage == 5)
		{
			this.wait_time++;
			if (this.wait_time <= 3)
				return FX_OK;
			this.wait_time = 0;
			this.Target->SetCommand("MoveTo", nil, 117, 741);
			this.stage = 6;
			return FX_OK;
		}
		if (this.stage == 6)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.stage = 0;
			return FX_OK;
		}
	}
};

static const NPC_Idle_Cook = new Effect {
	Construction = func () {
		this.activity = false;
		this.mushroom = false;
		this.anim = nil;
		this.attach = 0;
	},
	Timer = func() {
		if (!this.activity && Random(15))
			return FX_OK;

		if (!this.activity)
		{
			this.mushroom = true;

			this.stage = 0;
			this.wait_time = 0;

			this.activity = true;
		}

		if (this.mushroom)
		{
			if (this.stage == 0)
			{
				this.Target.noidleactions = true;
				this.attach = this.Target->AttachMesh(Mushroom, "pos_hand1", "main", Mushroom->GetCarryTransform());
				this.anim = this.Target->PlayAnimation("CarryCrossbow", CLONK_ANIM_SLOT_Movement + 1, Anim_Const(0), Anim_Linear(0,0, 1000, 16, ANIM_Hold));
				this.turns = 0;
				this.stage = 1;
				return FX_OK;
			}
			if (this.stage == 1)
			{
				this.wait_time++;
				if (this.wait_time < 2)
					return FX_OK;
				this.stage = 2;
				this.wait_time = 5;
			}
			if (this.stage == 2)
			{
				this.wait_time++;
				if (this.wait_time < 5)
					return FX_OK;
				this.turns++;
				if (this.turns > 10)
				{
					this.stage = 4;
					return FX_OK;
				}
				this.Target->SetAnimationPosition(this.anim, Anim_Linear(0,0, 600, 40, ANIM_Hold));
				this.stage = 3;
				this.wait_time = 0;
				return FX_OK;
			}
			if (this.stage == 3)
			{
				this.wait_time++;
				if (this.wait_time < 5)
					return FX_OK;
				this.Target->SetAnimationPosition(this.anim, Anim_Linear(600,600, 0, 40, ANIM_Hold));
				this.stage = 2;
				this.wait_time = 0;
				return FX_OK;
			}
			if (this.stage == 4)
			{
				this.Target->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, 200, 16, ANIM_Remove), Anim_Linear(0,0, 1000, 16, ANIM_Remove));
				this.Target->SetAnimationWeight(this.anim, Anim_Linear(1000,1000, 0, 16, ANIM_Remove));
				this.Target->PlayAnimation("Close2Hand", CLONK_ANIM_SLOT_Hands, Anim_Const(900), Anim_Const(1000));
				this.Target->Sound("Liquids::SmokeSizzle", false, 100, nil, nil, 100);
				this.Target->DetachMesh(this.attach);
				this.attach = nil;
				this.stage = 5;
				this.wait_time = 0;
				return FX_OK;
			}
			if (this.stage == 5)
			{
				this.wait_time++;
				if (this.wait_time < 2)
					return FX_OK;
				this.stage = 0;
				this.wait_time = 0;
				this.Target->StopAnimation(this.anim);
				this.Target.noidleactions = false;
				this.anim = nil;
				this.mushroom = false;
				this.activity = false;
			}
		}
	}
};

static const NPC_Idle_Medic = {
	Construction = func () {
		this.activity = false;
		this.walk = false;
		this.comfort = false;
		this.wait_time = 0;
	},
	Timer = func () {
		if (!this.activity && Random(10))
			return FX_OK;

		if (!this.activity)
		{
			if (!Random(3))
			{
				this.comfort = true;
				this.Target->DoKneelComfort(nil, 70);
			} else {
				this.walk = true;
				this.Target->SetCommand("MoveTo", nil, RandomX(245, 330), 639);
			}

			this.activity = true;
		}

		if (this.comfort)
		{
			this.wait_time++;
			if (this.wait_time > 10)
			{
				this.activity = false;
				this.comfort = false;
				this.wait_time = 0;
			}
			return FX_OK;
		}

		if (this.walk)
		{
			if (!this.Target->GetCommand())
			{
				this.activity = false;
				this.walk = false;
			}
			return FX_OK;
		}
	}
};

static const NPC_Idle_Prisoner = {
	Construction = func () {
		this.stage = 0;
		this.wait_time = 0;
		this.anim = nil;
	},
	Timer = func () {
		if (this.stage == 0)
		{
			this.Target->SetCommand("MoveTo", nil, 466, 821);
			this.stage = 1;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.anim = this.Target->PlayAnimation("Dig", CLONK_ANIM_SLOT_Movement, Anim_Const(0), Anim_Linear(0,0, 1000, 15, ANIM_Hold));
			this.stage = 2;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.wait_time++;
			if (this.wait_time < 2)
				return FX_OK;
			var particles =
			{
				Prototype = Particles_Dust(),
				R = 20,
				G = 20,
				B = 20,
				Size = PV_KeyFrames(0, 0, 0, 300, 20, 1000, 7),
			};
			this.Target->CreateParticle("Dust", 8, 8, PV_Random(-6, 6), PV_Random(-6, 6), PV_Random(18, 36), particles, 8);
			this.Target->CreateParticle("Dust", 8, 8, PV_Random(-6, 6), PV_Random(-6, 6), PV_Random(18, 36), particles, 8);
			this.Target->Sound("Clonk::Action::Dig::Dig?", nil, 100, nil, nil, 200);
			this.Target->SetAnimationWeight(this.anim, Anim_Linear(1000,1000, 0, 15, ANIM_Remove));
			this.anim = this.Target->PlayAnimation("MusketAimArms", CLONK_ANIM_SLOT_Movement + 1, Anim_Const(500), Anim_Linear(0,0, 1000, 25, ANIM_Hold));
			this.Target->SetAttachTransform(this.Target.hand_display.hand_mesh[0], Trans_Rotate(-75, 1));
			this.stage = 3;
			this.wait_time = 0;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			this.wait_time++;
			if (this.wait_time < 4)
				return FX_OK;
			this.Target->SetCommand("MoveTo", nil, 401, 822);
			this.stage = 4;
			this.wait_time = 0;
			return FX_OK;
		}
		if (this.stage == 4)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			var particles =
			{
				Prototype = Particles_Dust(),
				R = 20,
				G = 20,
				B = 20,
				Size = PV_KeyFrames(0, 0, 0, 300, 20, 1000, 7),
			};
			this.Target->CreateParticle("Dust", -8, -4, PV_Random(-6, 6), PV_Random(-6, 6), PV_Random(18, 36), particles, 8);
			this.Target->CreateParticle("Dust", -8, -4, PV_Random(-6, 6), PV_Random(-6, 6), PV_Random(18, 36), particles, 8);
			this.Target->Sound("Objects::Clonk", nil, 75, nil, nil, 200);
			this.Target->SetAnimationWeight(this.anim, Anim_Linear(1000,1000, 0, 15, ANIM_Remove));
			this.Target->PlayAnimation("Stand", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0,0, 200, 15, ANIM_Remove), Anim_Linear(0,0, 1000, 15, ANIM_Remove));
			this.Target->UpdateAttach();
			this.anim = 0;
			this.stage = 5;
			return FX_OK;
		}
		if (this.stage == 5)
		{
			this.wait_time++;
			if (this.wait_time < 2)
				return FX_OK;
			this.Target->SetCommand("Wait", nil, nil, nil, nil, Random(100)+100);
			this.stage = 6;
			this.wait_time = 0;
			return FX_OK;
		}
		if (this.stage == 6)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.stage = 0;
			return FX_OK;
		}
	}
};

static const NPC_Idle_Hartman = new Effect {
	Construction = func (proplist scout) {
		this.stage = 0;
		this.wait_time = 0;
		this.scout = scout;
	},
	Timer = func () {
		if (this.stage == 0)
		{
			this.Target->SetDir(DIR_Left);
			this.stage = 1;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			if (this.scout)
				this->RandomScoutMessage();
			this.stage = 2;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.wait_time++;
			if (this.wait_time < 2)
				return FX_OK;
			this.wait_time = 0;
			this->RandomGunnyMessage();
			this.Target->SetDir(DIR_Right);
			this.stage = 3;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			this.wait_time++;
			if (this.wait_time < 2)
				return FX_OK;
			this.wait_time = 0;
			this.Target->Message("$MsgGunnyFire$");
			this.stage = 4;
			return FX_OK;
		}
		if (this.stage == 4)
		{
			for (var gunner in g_gunners)
			{
				var cannon = gunner->GetActionTarget();
				if (!cannon)
					return;
				var shell = cannon->CreateObject(ArtilleryShell, 0,0, REPUBLIC);
				shell->SetObjectLayer(nil);
				shell.boosted = true;
				cannon->DoFire(shell, gunner, 22000);
			}
			this.stage = 5;
			return FX_OK;
		}
		if (this.stage == 5)
		{
			this.wait_time++;
			if (this.wait_time < 10)
				return FX_OK;
			if (Random(4))
				return FX_OK;
			this.wait_time = 0;
			this.stage = 0;
			return FX_OK;
		}
	},
	RandomScoutMessage = func () {
		var rnd = Random(5);
		if (rnd == 0)
			this.scout->Message("$RndMsgScout1$");
		if (rnd == 1)
			this.scout->Message("$RndMsgScout2$");
		if (rnd == 2)
			this.scout->Message("$RndMsgScout3$");
		if (rnd == 3)
			this.scout->Message("$RndMsgScout4$");
		if (rnd == 4)
			this.scout->Message("$RndMsgScout5$");
	},
	RandomGunnyMessage = func () {
		var rnd = Random(5);
		if (rnd == 0)
			this.Target->Message("$RndMsgGunny1$");
		if (rnd == 1)
			this.Target->Message("$RndMsgGunny2$");
		if (rnd == 2)
			this.Target->Message("$RndMsgGunny3$");
		if (rnd == 3)
			this.Target->Message("$RndMsgGunny4$");
		if (rnd == 4)
			this.Target->Message("$RndMsgGunny5$");
	}
};

static const NPC_Idle_Guinot = new Effect {
	Construction = func () {
		this.stage = 0;
		this.wait_time = 0;
	},
	Timer = func () {
		if (!Random(150))
		{
			var dialogue = Dialogue->FindByTarget(this.Target);
			if (!dialogue)
				this->RandomMessage();
			else if (!GetEffect("Dialogue_ShortMsg", dialogue))
				this->RandomMessage();
		}
		if (this.stage == 0)
		{
			this.Target->SetCommand("MoveTo", nil, 102, 716);
			this.stage = 1;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.wait_time = 20*Random(10);
			this.stage = 2;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.wait_time -= 20;
			if (this.wait_time >= 0)
				return FX_OK;
			this.stage = 3;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			this.Target->SetCommand("MoveTo", nil, 132, 716);
			this.stage = 4;
			return FX_OK;
		}
		if (this.stage == 4)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.wait_time = 20*Random(10);
			this.stage = 0;
			return FX_OK;
		}
	},
	RandomMessage = func () {
		var rand = Random(5);
		var msg = "";
		if (rand == 0)
			msg = "$RndMsgGuinot1$";
		if (rand == 1)
			msg = "$RndMsgGuinot2$";
		if (rand == 2)
			msg = "$RndMsgGuinot3$";
		if (rand == 3)
			msg = "$RndMsgGuinot4$";
		if (rand == 4)
			msg = "$RndMsgGuinot5$";

		CustomMessage(Format("<c %x>%s:</c>|%s", this.Target->GetColor(), this.Target->GetName(), msg), this.Target, NO_OWNER);
	}
};

static const NPC_Guard_Smith = new Effect {
	Construction = func () {
		this.trespassers = [];
	},
	Timer = func () {
		if (this.Target->GetDir() == DIR_Left)
			this.Target->SetDir(DIR_Right);

		if (GetLength(this.trespassers))
			this->ReleaseTrespassers();

		var clonks = this.Target->FindObjects(Find_InRect(-50, -30, 30, 50), Find_OCF(OCF_CrewMember), Find_AnyLayer());
		for (var clonk in clonks)
		{
			if (IsValueInArray(this.trespassers, clonk))
				continue;
			PushBack(this.trespassers, clonk);
			Dialogue->FindByTarget(this.Target)->MessageBox(Format("$CommandMsgSmith1$", GetNewbiesRank()), clonk, this.Target, clonk->GetOwner(), true);
			clonk->SetCrewEnabled(false);
			clonk->SetCommand("MoveTo", nil, 390, 1090);
		}
	},
	ReleaseTrespassers = func() {
		var to_release = [];
		for (var clonk in this.trespassers)
		{
			if (clonk->GetX() >= 340)
				PushBack(to_release, clonk);
		}
		for (var clonk in to_release)
		{
			RemoveArrayValue(this.trespassers, clonk, true);
			clonk->SetCrewEnabled(true);
			SetCursor(clonk->GetOwner(), clonk, true);
		}
	}
};

static const NPC_Guard_Fighting = new Effect {
	Construction = func () {
		this.stage = 0;
		this.wait_time = 0;
		this.aim_angle = 0;
	},
	Timer = func () {
		if (this.stage == 0)
		{
			this.Target->SetDir(DIR_Right);
			this.stage = 1;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			if (!this.Target->Contents())
				this.Target->CreateContents(Blunderbuss);
			if (!this.Target->Contents()->Contents())
				this.Target->Contents()->CreateContents(LeadBullet);

			this.Target->Contents(0)->ControlUseStart(this.Target, 50, -15);
			this.aim_angle = 15;
			this.stage = 2;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			if (!this.Target->IsAiming())
				return FX_OK;
			if (!Random(5))
			{
				this.stage = 4;
				return FX_OK;
			}
			this.stage = 3;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			this.aim_angle = BoundBy(this.aim_angle + RandomX(-2, 2), 15, 30);
			this.Target->Contents(0)->ControlUseHolding(this.Target, 50, this.aim_angle*-1);
			if (!Random(5))
			{
				this.stage = 4;
				return FX_OK;
			}
			return FX_OK;
		}
		if (this.stage == 4)
		{
			this.Target->Contents(0)->ControlUseStart(this.Target, 50, this.aim_angle*-1);
			this.Target->Contents(0)->ControlUseHolding(this.Target, 50, this.aim_angle*-1);
			this.Target->Contents(0)->ControlUseStop(this.Target, 50, this.aim_angle*-1);
			this.stage = 5;
			return FX_OK;
		}
		if (this.stage == 5)
		{
			while (GetEffect("IntAim", this.Target))
				RemoveEffect("IntAim", this.Target);
			this.Target->SetDir(DIR_Left);
			this.Target->DoKneelComfort(nil, 230);
			this.wait_time = 240;
			this.stage = 6;
			return FX_OK;
		}
		if (this.stage == 6)
		{
			if (this.wait_time <= 0)
			{
				this.stage = 0;
				return FX_OK;
			}
			this.wait_time -= 20;
			return FX_OK;
		}
	}
};

static const NPC_Enemy_Gatling = new Effect {
	Construction = func() {
		this.firing = false;
	},
	Timer = func() {
		var clonk = this.Target->FindObject(Find_InRect(-300, -85, 350, 160), Find_OCF(OCF_CrewMember), Find_Allied(REPUBLIC), Find_AnyLayer());
		if (clonk && this.Target->GetActionTarget()) {
			if (!this.Target->GetActionTarget()->FindContents(LeadBullet))
				this.Target->GetActionTarget()->CreateContents(LeadBullet);
			var x = clonk->GetX() - this.Target->GetX();
			var y = clonk->GetY() - this.Target->GetY();
			if (!this.firing) {
				this.Target->GetActionTarget()->~ControlUseStart(this.Target, x, y);
				this.firing = true;
			}
			this.Target->GetActionTarget()->~ControlUseHolding(this.Target, x, y);
		} else if (this.firing) {
			this.Target->GetActionTarget()->~ControlUseStop(this.Target, -300, 0);
			this.firing = false;
		}
	}
};

static const NPC_Gate_Guard = new Effect {
	Construction = func() {
		this.activity = false;
		this.stage = 0;
	},
	Timer = func() {
		if (this.activity)
		{
			if (this.stage == 0)
			{
				this.Target->ObjectCommand("UnGrab");
				this.stage++;
				return FX_OK;
			}
			if (this.stage == 1)
			{
				this.Target->SetCommand("MoveTo", nil, 658, 753);
				this.stage++;
				return FX_OK;
			}
			if (this.stage == 2)
			{
				if (this.Target->GetCommand())
					return FX_OK;
				CustomMessage(Format("<c %x>%s:</c>|$CommandMsgFarina2$", this.Target->GetColor(), this.Target->GetName()), this.Target, NO_OWNER);
				this.stage++;
				return FX_OK;
			}
			if (this.stage == 3)
			{
				if (!this.door.opened)
					return FX_OK;
				g_spinwheel2->ControlUp(this.Target);
				this.stage++;
				return FX_OK;
			}
			if (this.stage == 4)
			{
				this.Target->SetCommand("MoveTo", nil, 710, 753);
				this.stage++;
				return FX_OK;
			}
			if (this.stage == 5)
			{
				if (this.Target->GetCommand())
					return FX_OK;
				this.Target->ObjectCommand("Grab", this.cannon);
				this.activity = false;
				return FX_OK;
			}
		}
	},
	OnGateOpened = func(object gate, object clonk) {
		if (this.activity)
			return;
		this.activity = true;
		this.stage = 0;
		this.door = gate;
		this.cannon = this.Target->GetActionTarget();
		Dialogue->FindByTarget(this.Target)->MessageBox(Format("$CommandMsgFarina1$", GetNewbiesRank()), clonk, this.Target, clonk->GetOwner(), true);
	}
};

static const NPC_Pioneer_Warmup = new Effect {
	Construction = func(int pioneer_number) {
		this.activity = 0;
		this.wait_time = 0;
		this.number = pioneer_number;
		// First two pioneers will construct a wooden bridge, just for show
		if (pioneer_number == 0)
			this.bridge = this.Target->CreateObject(PioneerBridge, 0,0, REPUBLIC);
		this.Target.noidleactions = true;
	},
	Timer = func(int time) {
		if (this.bridge)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			if (Random(4))
			{
				this.Target->PlayAnimation("ThrowArms", CLONK_ANIM_SLOT_Arms, Anim_Linear(0, 0, this.Target->GetAnimationLength("ThrowArms"), 25, ANIM_Remove));
				Schedule(this.Target, "Sound(\"Structures::DoorUnlock\")", 18);
			} else {
				this.Target->SetCommand("MoveTo", nil, this.bridge->GetX() + RandomX(-35, 35), this.Target->GetY());
			}
			return FX_OK;
		}
		if (this.number == 1)
		{
			this.bridge = FindObject(Find_ID(PioneerBridge));
			return FX_OK;
		}
		if (this.activity == 0)
		{
			this.activity = Random(3) + 1;
		}
		if (this.activity == 1) // Wait
		{
			this.wait_time += time;
			if (this.wait_time > 140)
			{
				this.wait_time = 0;
				this.activity = 0;
			}
			return FX_OK;
		}
		if (this.activity == 2) // Jump
		{
			this.Target->Jump();
			this.Target->SetYDir(-25);
			this.activity = 4;
			return FX_OK;
		}
		if (this.activity == 3) // Move
		{
			this.Target->SetCommand("MoveTo", nil, RandomX(610, 830), this.Target->GetY());
			this.activity = 5;
			return FX_OK;
		}
		if (this.activity == 4) // Jump #2
		{
			if (this.Target->GetAction() == "Jump")
				return FX_OK;
			this.activity = 0;
			return FX_OK;
		}
		if (this.activity == 5) // Move #2
		{
			if (this.Target->GetCommand())
				return FX_OK;
			this.activity = 0;
			return FX_OK;
		}
	},
	Destruction = func() {
		if (this.bridge)
		{
			this.bridge->LiftUp(this.Target);
		} else {
			this.Target->SetCommand("MoveTo", nil, 830 + RandomX(-10, 10), this.Target->GetY());
		}
	}
};

static const NPC_Derek_StopWhenBurnt = new Effect {
	Destruction = func(int reason) {
		if (reason == FX_Call_RemoveClear)
			if (g_soldiers[2] && g_soldiers[2]->GetAlive())
			{
				CustomMessage(Format("<c %x>%s:</c>|$BurntMsgDerek1$", g_soldiers[2]->GetColor(), g_soldiers[2]->GetName()), g_soldiers[2], NO_OWNER);
				if (g_soldiers[2]->Contents())
					g_soldiers[2]->Contents()->ControlUseStop(g_soldiers[2]);
			}
	}
};

// Adds a save effect to move the pioneers
// Because with those NPCs, it is vital to not have them stuck anywhere
global func PioneerMoveTo(int x, int y)
{
	if (!this)
		return;

	if (PioneerIsMoving())
		RemoveEffect("NPC_Pioneer_MoveTo", this);

	return this->CreateEffect(NPC_Pioneer_MoveTo, 1, 30, x, y);
}

global func PioneerIsMoving()
{
	if (!this)
		return;

	return GetEffect("NPC_Pioneer_MoveTo", this);
}

global func PioneerStopMoving()
{
	if (!this)
		return;

	if (!this->PioneerIsMoving())
		return false;

	this->RemoveEffect("NPC_Pioneer_MoveTo", this);
	this->SetCommand("None");
	this->SetComDir(COMD_Stop);
	return true;
}

static const NPC_Pioneer_MoveTo = new Effect {
	Construction = func(int x, int y) {
		this.x = x;
		this.y = y;
		this.current_x = this.Target->GetX();
		this.current_y = this.Target->GetY();
		this.stuck_time = 0;
		this.jumped = false;
		this.magic = false;
		this.Target->SetCommand("MoveTo", nil, x, y);
	},
	Timer = func() {
		// Check if the clonk is stuck
		if (Distance(this.Target->GetX(), this.Target->GetY(), this.current_x, this.current_y) < 7)
		{
			this.stuck_time++;
			if (this.stuck_time > 4) { // Clonk must be stuck at some kind of ledge, try stuff
				if (this.Target->GetCommand() && !this.jumped) {
					var comdir = COMD_DownLeft;
					if (this.x > this.Target->GetX())
						comdir = COMD_DownRight;
					this.Target->SetCommand("None");
					this.Target->SetComDir(comdir);
					return FX_OK;
				} else {
					var comdir = COMD_Left;
					if (this.x > this.Target->GetX())
						comdir = COMD_Right;
					this.Target->SetComDir(comdir);
					this.Target->Jump();
					this.Target->SetCommand("MoveTo", nil, this.x, this.y);
				}
				// Still stuck? Even after attempting a jump?
				if (this.stuck_time > 10)
				{
					// Try magic
					this.Target->SetPosition(this.Target->GetX() + RandomX(-1,1), this.Target->GetY()-10);
					this.magic = true;
				}
				// Still stuck? Even after magic has happened?
				if (this.stuck_time > 15)
				{
					// Warp to target
					this.Target->SetPosition(this.x, this.y);
					return -1;
				}
			}
		} else if (this.stuck_time) {
			this.stuck_time = 0;
			this.jumped = false;
			this.magic = false;
		}
		this.current_x = this.Target->GetX();
		this.current_y = this.Target->GetY();
		if (Distance(this.Target->GetX(), this.Target->GetY(), this.x, this.Target->GetY()) < 7)
			return -1;
		if (!this.Target->GetCommand())
			this.Target->SetCommand("MoveTo", nil, this.x, this.y);
	}
};

static const NPC_Pioneer_MoveIntoAction = new Effect {
	Construction = func() {
		this.stage = 0;
		this.Target->SetCommand("Exit");
	},
	Timer = func() {
		if (this.stage == 0)
		{
			if (this.Target->Contained())
				return FX_OK;
			this.Target->SetComDir(COMD_Left);
			if (this.Target->GetAction() != "Walk")
				this.stage++;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			this.Target->SetComDir(COMD_Down);
			if (this.Target->GetAction() == "Walk")
				this.stage++;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.Target->PioneerMoveTo(g_pioneer_rally_point[0], g_pioneer_rally_point[1]);
			GameCall("AddNewPioneer", this.Target);
			return -1;
		}
	}
};

static const NPC_Pioneer_Protection = new Effect {
	Timer = func() {
		if (GetEffect("Lifting", this.Target))
			return FX_OK;
		if (GetEffect("NPC_Pioneer_DynamiteThrowing", this.Target))
			return FX_OK;

		var projectiles = this.Target->FindObjects(Find_InRect(-150, -50, 300, 80), Find_Or(Find_Category(C4D_Object), Find_Func("IsDangerous4AI"), Find_Func("IsArrow")), Find_OCF(OCF_HitSpeed2), Find_NoContainer(), Sort_Distance());
		for (var obj in projectiles)
		{
			var dx = obj->GetX() - this.Target->GetX(), dy = obj->GetY() - this.Target->GetY();
			var vx = obj->GetXDir(), vy = obj->GetYDir();
			if (Abs(dx) > 40 && vx)
				dy += (Abs(10 * dx / vx)**2) * GetGravity() / 200;
			var v2 = Max(vx * vx + vy * vy, 1);
			var d2 = dx * dx + dy * dy;
			var time_to_impact = 10 * Sqrt(d2) / Sqrt(v2);
			if (time_to_impact > 20)
			{
				// Won't hit within the next 20 frames.
				continue;
			}
			// Distance at which projectile will pass clonk should be larger than clonk size (erroneously assumes clonk is a sphere).
			var l = dx * vx + dy * vy;
			if (l < 0 && Sqrt(d2 - l * l / v2) <= this.Target->GetCon() / 8)
			{
				// Not if there's a wall between.
				if (!PathFree(this.Target->GetX(), this.Target->GetY(), obj->GetX(), obj->GetY()))
					continue;
				// Try to use the club to bat away objects if available.
				if (this->ExecuteClubProtection(obj, time_to_impact))
					return true;
			}
		}
		return FX_OK;
	},
	ExecuteClubProtection = func(object projectile, int time_to_impact) {
		// Don't use club on explosives.
		if (projectile->~HasExplosionOnImpact())
			return false;
		// Check for a club which can be used.
		var club = this.Target->FindObject(Find_Container(this.Target), Find_ID(Club));
		if (!club)
			return false;
		// Assume we are using it so just wait.
		if (club->RejectUse(this.Target) || time_to_impact > 8)
			return true;
		var dx = projectile->GetX() - this.Target->GetX();
		var dy = projectile->GetY() - this.Target->GetY();
		// Execute all control commands in a few frames.
		this.Target->SetHandItemPos(0, this.Target->GetItemPos(club));
		if (club->~ControlUseStart(this.Target, dx, dy))
		{
			ScheduleCall(club, "~ControlUseHolding", 1, 0, this.Target, dx, dy);
			ScheduleCall(club, "~ControlUseStop", 2, 0, this.Target, dx, dy);
			return true;
		}
		return false;
	}
};

static const NPC_Pioneer_DynamiteThrowing = new Effect {
	Start = func(int temp) {
		if (temp)
			return;

		this.stage = 0;
		this.wait = 0;
		this.dynamite = this.Target->CreateContents(Dynamite);
		this.club = this.Target->FindContents(Club);

		// Get a target to throw dynamite at
		var fx = GetEffect("First_Battle");
		if (!GetLength(fx.kutorian))
		{
			this.target = fx.current_door;
		} else {
			this.target = fx.kutorian[Random(GetLength(fx.kutorian))];
		}
		if (!this.target || !this.club)
		{
			this.dynamite->RemoveObject();
			return -1;
		}
		// Check right now if there is a possible way to get the dynamite to the target
		// otherwise don't bother even trying
		var speed = this->CalculateClubAcceleration(this.dynamite);
		var angle = AI_HelperFunctions->GetBallisticAngle(this.Target->GetX(), this.Target->GetY(), this.target->GetX(), this.target->GetY(), speed / 2, 180);
		if (!angle) // No chance of hitting
		{
			this.Target->Message("$PioneerNotInRange$");
			this.dynamite->RemoveObject();
			return -1;
		}
		this.aim_x = Sin(angle, 500, 10);
		this.aim_y =-Cos(angle, 500, 10);
	},
	Timer = func() {
		if (this.Target->GetAction() != "Walk")
			return;

		if (!this.dynamite || !this.club)
			return -1;

		if (this.stage == 0)
		{
			// Stop moving!
			this.Target->PioneerStopMoving();
			if (this.target->GetX() > this.Target->GetX())
				this.Target->SetDir(DIR_Right);
			else
				this.Target->SetDir(DIR_Left);
			this.stage++;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			this.wait++;
			if (this.wait < 35)
				return FX_OK;
			this.wait = 0;
			// Throw the dynamite into the air (risky business)
			this.dynamite->Incinerate();
			var x = 4;
			var y = -2;
			if (this.aim_x < 0)
				x = -4;
			this.dynamite->Exit(x, y, Random(360), 0, -3, RandomX(-10,10));
			this.club->ControlUseStart(this.Target, this.aim_x, this.aim_y);
			this.club->ControlUseHolding(this.Target, this.aim_x, this.aim_y);
			this.stage++;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			this.wait++;
			if (this.wait < 2)
				return FX_OK;
			this.wait = 0;
			this.stage++;
		}
		if (this.stage == 3)
		{
			// Hit the dynamite as soon as it's in range
			if (this.Target->ObjectDistance(this.dynamite) <= 7)
			{
				this.club->ControlUseStop(this.Target, this.aim_x, this.aim_y);
				this.stage++;
			}
		}
		if (this.stage == 4)
		{
			// Wait a little while before the protection effect starts again
			this.wait++;
			if (this.wait < 10)
				return FX_OK;
			return -1;
		}
	},
	Destruction = func() {
		if (this.dynamite && this.dynamite->Contained() == this.Target)
			this.dynamite->RemoveObject();
		if (this.Target->GetAlive() && this.club)
			if (this.Target->IsAiming())
				this.Target->CancelAiming(this.club);
		if (this.fallback)
			if (this.Target->GetAlive())
				this.Target->PioneerMoveTo(g_pioneer_rally_point[0], g_pioneer_rally_point[1]);
	},
	CalculateClubAcceleration = func(object obj) {
		var div = Club->ClubVelocityPrecision();
		var precision = 6; // The clonk will try to hit the dynamite at distance 7
		// mass/size factor
		var fac1 = 10000 / Max(5, obj->GetMass());
		var fac2 = BoundBy(10-Abs(obj->GetDefCoreVal("Width", "DefCore")-obj->GetDefCoreVal("Height", "DefCore")), 1, 10);
		var speed = (3000 * fac1 * fac2) / 1000 / precision;
		speed = BoundBy(speed, 500, 2000);
		return speed * 10 / div;
	},
	GetBallisticAngle = func(int x, int y, int tx, int ty, int v, int max_angle) // prefers upper_angle over lower_angle
	{
		var dx = tx - x;
		var dy = ty - y;
		// The variable v is in 1/10 pix/frame.
		// The variable gravity is in 1/100 pix/frame^2.
		var g = GetGravity();
		// Correct vertical distance to account for integration error. Engine adds gravity after movement, so
		// targets fly higher than they should. Thus, we aim lower. we don't know the travel time yet, so we
		// assume some 90% of v is horizontal (it's ~2px correction for 200px shooting distance).
		dy += Abs(dx) * g * 10 / (v * 180);
		// The variable q is in 1/10000 (pix/frame)^4 and dy is negative up.
		var q = v**4 - g * (g * dx * dx - 2 * dy * v * v);
		 // Check if target is out of range.
		if (q < 0)
			return nil;
		// Return upper angle if possible.
		var upper_angle = (Angle(0, 0, g * dx, -Sqrt(q) - v * v, 10) + 1800) % 3600 - 1800;
		if (Inside(upper_angle, -10 * max_angle, 10 * max_angle) && this->CheckBallisticPath(x, y, tx, ty, v, upper_angle))
			return upper_angle;
		// Otherwise return lower angle if that one is possible.
		var lower_angle = (Angle(0, 0, g * dx, Sqrt(q) - v * v, 10) + 1800) % 3600 - 1800;
		if (Inside(lower_angle, -10 * max_angle, 10 * max_angle) && this->CheckBallisticPath(x, y, tx, ty, v, lower_angle))
			return lower_angle;
		// No possible shooting angle.
		return nil;
	}
};

static const NPC_Kutor_LastDefender = new Effect {
	Construction = func() {
		this.stage = 0;
	},
	Timer = func() {
		if (this.stage == 0)
		{
			if (this.Target->IsJumping())
				return FX_OK;
			this.Target->SetCommand("Walk", nil, RandomX(2322, 2354), 1090);
			this.stage++;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			if (this.Target->GetCommand())
				return FX_OK;
			AI->AddAI(this.Target);
			AI->SetHome(this.Target, RandomX(2322, 2354), 1090, DIR_Left);
			AI->SetGuardRange(this.Target, 2020, 1050, 400, 200);
			return -1;
		}
	}
};