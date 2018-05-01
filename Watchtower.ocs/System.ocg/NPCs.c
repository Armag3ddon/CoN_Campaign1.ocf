// Activity effects for NPCs

static const Paula_WorkingEffect = {
	Construction = func (object basem) {
		this.basement = basem;
		this.delta = 0;
		this.last_time = 0;
		this.activity = false;
		this.speed = false;
		this.extra_basements = 0;

		this.swing = false;
		this.swing_back = false;
		this.inspect = false;
		this.inspect_back = false;
		this.chop = false;
		this.chop_back = false;
		this.anim = nil;

		this.Target.noidleactions = true;
	},
	Timer = func (int time) {
		this.delta += time - this.last_time;
		this.last_time = time;

		if (this.Target->GetDir() == DIR_Right)
			this.Target->SetDir(DIR_Left);

		if (this.speed)
		{
			if (this.extra_basements >= 7)
				return -1; // Work done!

			if (!this.activity)
			{
				this.swing = true;
				this.activity = true;
			}
		}
		if (!this.activity)
		{
			if (!Random(2))
				this.swing = true;
			else if (!Random(2))
				this.chop = true;
			else
				this.inspect = true;

			this.activity = true;
			if (!Random(13))
				this.Target->Sound("Clonk::Skin::Steampunk::Singing?", false, 75, nil, nil, 250);
		}

		if (this.swing)
		{
			if (this.anim == nil)
			{
				var speed = 55;
				if (this.speed)
					speed = 10;
				this.anim = this.Target->PlayAnimation("StrikePickaxe", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0,0, this.Target->GetAnimationLength("StrikePickaxe"), speed, ANIM_Hold), Anim_Linear(0, 0, 1000, 5, ANIM_Hold));
				this.delta = 0;
			} else if (this.speed) {
				if (this.delta >= 10 && !this.swing_back)
				{
					this.Target->StopAnimation(this.anim);
					this.anim = this.Target->PlayAnimation("StrikePickaxe", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(this.Target->GetAnimationLength("StrikePickaxe"), this.Target->GetAnimationLength("StrikePickaxe"), this.Target->GetAnimationLength("StrikePickaxe")/2, 10, ANIM_Hold), Anim_Const(1000));
					this.swing_back = true;
					this->Clang();
				}
				if (this.delta >= 20)
				{
					this.basement->Multiply();
					this.extra_basements++;
					this.swing = false;
					this.swing_back = false;
					this.anim = nil;
					this.activity = false;
					this.Target->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, this.Target->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
				}
			} else {
				if (this.delta >= 55 && !this.swing_back)
				{
					this.Target->StopAnimation(this.anim);
					this.anim = this.Target->PlayAnimation("StrikePickaxe", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(this.Target->GetAnimationLength("StrikePickaxe"), this.Target->GetAnimationLength("StrikePickaxe"), this.Target->GetAnimationLength("StrikePickaxe")/2, 55, ANIM_Hold), Anim_Const(1000));
					this.swing_back = true;
					this->Clang();
				}
				if (this.delta >= 110)
				{
					this.swing = false;
					this.swing_back = false;
					this.anim = nil;
					this.activity = false;
					this.Target->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, this.Target->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
				}
			}
		}

		if (this.chop)
		{
			if (this.anim == nil)
			{
				this.anim = this.Target->PlayAnimation("Chop.R", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0,0, 400, 25, ANIM_Hold), Anim_Linear(0, 0, 1000, 5, ANIM_Hold));
				this.delta = 0;
			} else {
				if (this.delta >= 25 && !this.chop_back)
				{
					this.Target->StopAnimation(this.anim);
					this.anim = this.Target->PlayAnimation("Chop.R", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(400, 0, this.Target->GetAnimationLength("Chop.R"), 35, ANIM_Hold), Anim_Const(1000));
					this.chop_back = true;
					this->Clang();
				}
				if (this.delta >= 60)
				{
					this.chop = false;
					this.chop_back = false;
					this.anim = nil;
					this.activity = false;
					this.Target->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, this.Target->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
				}
			}
		}

		if (this.inspect)
		{
			if (this.anim == nil)
			{
				this.anim = this.Target->PlayAnimation("KneelDown", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(this.Target->GetAnimationLength("KneelDown"), this.Target->GetAnimationLength("KneelDown"), 0, 45, ANIM_Hold), Anim_Linear(0, 0, 1000, 5, ANIM_Hold));
				this.delta = 0;
			} else {
				if (this.delta >= 85 && !this.inspect_back)
				{
					this.Target->StopAnimation(this.anim);
					this.anim = this.Target->PlayAnimation("KneelDown", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, this.Target->GetAnimationLength("KneelDown"), 45, ANIM_Hold), Anim_Const(1000));
					this.inspect_back = true;
				}
				if (this.delta >= 130)
				{
					this.Target->StopAnimation(this.anim);
					this.inspect = false;
					this.inspect_back = false;
					this.anim = nil;
					this.activity = false;
					this.Target->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, this.Target->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
				}
			}
		}
	},
	Destruction = func() {
		this.Target->StopAnimation(this.Target->GetRootAnimation(CLONK_ANIM_SLOT_Movement + 1));
		this.Target.noidleactions = nil;
		this.Target->SetHandItemPos(0, 1);

		paulas_dlg->SetInteraction(true);
		paulas_dlg->AddAttention();
	},
	Clang = func() {
		var x = this.basement->GetX() + 13;
		var y = this.basement->GetY() + RandomX(-2,2);
		var spark = Particles_Glimmer();
		this.Target->CreateParticle("StarSpark", this.Target->AbsX(x), this.Target->AbsY(y), PV_Random(-30, 30), PV_Random(-30, 30), PV_Random(10, 50), spark, 30);
		this.Target->Sound("Objects::Pickaxe::Clang?");
	},
	SpeedUp = func() {
		this.activity = false;
		this.swing = false;
		this.swing_back = false;
		this.inspect = false;
		this.inspect_back = false;
		this.chop = false;
		this.chop_back = false;
		if (this.anim) {
			this.Target->StopAnimation(this.anim);
			this.anim = nil;
		}
		this.Target->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, this.Target->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));

		this.speed = true;
	}
};

static const Samuel_WorkingEffect = {
	Construction = func() {
		samuel->SetToWalkingSpeed();

		this.delta = 0;
		this.last_time = 0;
		this.activity = false;

		this.inspect = nil;
		this.inspect_back = nil;
		this.walk = nil;
		this.wait = nil;
		this.anim = nil;

		this.Target.noidleactions = true;
	},
	Timer = func(int time) {
		this.delta += time - this.last_time;
		this.last_time = time;

		if (!this.activity)
		{
			if (Random(8)) {
				if (Random(3)) {
					this.wait = 35 + Random(35);
					this.delta = 0;
				} else {
					this.walk = true;
					this.Target->SetCommand("MoveTo", nil, 810 + Random(60), this.Target->GetY());
				}
			} else {
				this.inspect = true;
			}

			this.activity = true;
		}

		if (this.wait)
		{
			if (this.delta >= this.wait) {
				this.wait = nil;
				this.activity = false;
				if (!Random(6))
					this->Chatter();
			}
		}

		if (this.walk)
		{
			if (!this.Target->GetCommand()) {
				this.walk = nil;
				this.activity = false;
			}
		}

		if (this.inspect)
		{
			if (this.anim == nil)
			{
				this.anim = this.Target->PlayAnimation("AimArmsGeneric.R", CLONK_ANIM_SLOT_Movement, Anim_Const(1200), Anim_Linear(0, 0, 1000, 25, ANIM_Hold));
				this.delta = 0;
			} else {
				if (this.delta >= 45 && !this.inspect_back)
				{
					this.Target->SetAnimationPosition(this.anim, Anim_Linear(1200, 1200, 700, 35, ANIM_Hold));
					this.inspect_back = 1;
				}
				if (this.delta >= 110 && this.inspect_back != 2)
				{
					this.Target->SetAnimationWeight(this.anim, Anim_Linear(1000, 1000, 0, 20, ANIM_Remove));
					this.anim = this.Target->PlayAnimation("Stand", CLONK_ANIM_SLOT_Movement, Anim_Const(1000), Anim_Linear(0, 0, 1000, 20, ANIM_Remove));
				}
				if (this.delta >= 140) {
					this.Target->StopAnimation(this.anim);
					this.inspect = false;
					this.inspect_back = 0;
					this.anim = nil;
					this.activity = false;
				}
			}
		}
	},
	Destruction = func() {
		samuel->SetToRunningSpeed();
		this.Target.noidleactions = false;
		if (this.anim != nil)
		{
			this.Target->StopAnimation(this.anim);
			this.Target->PlayAnimation("Stand", CLONK_ANIM_SLOT_Movement, Anim_Const(1000), Anim_Linear(0, 0, 1000, 20, ANIM_Remove));
		}
	},
	Chatter = func() {
		if (Random(9)) {
			var rnd = Random(10);
			var msg = "";
			if (rnd == 0)
				msg = "$RndSamuelSR1$";
			if (rnd == 1)
				msg = "$RndSamuelSR2$";
			if (rnd == 2)
				msg = "$RndSamuelSR3$";
			if (rnd == 3)
				msg = "$RndSamuelSR4$";
			if (rnd == 4)
				msg = "$RndSamuelSR5$";
			if (rnd == 5)
				msg = "$RndSamuelSR6$";
			if (rnd == 6)
				msg = "$RndSamuelSR7$";
			if (rnd == 7)
				msg = "$RndSamuelSR8$";
			if (rnd == 8)
				msg = "$RndSamuelSR9$";
			if (rnd == 9)
				msg = "$RndSamuelSR10$";

			this.Target->Message(msg);
		} else {
			if (Random(2))
				this.Target->Sound("Clonk::Skin::Alchemist::Confirm3", false, 75);
			else
				this.Target->Sound("Clonk::Skin::Alchemist::Decline6", false, 75);
		}
	}
};

// NPCs check the intermediate goals

static const Paula_GoalEffect = {
	Timer = func () {
		var lorries = paula->FindObjects(Find_InRect(-50, -50, 150, 65), Find_ID(Lorry), Find_Layer());
		var most_filled = nil;
		for (var lorry in lorries) {
			if (!most_filled || most_filled->ContentsCount(Rock) < lorry->ContentsCount(Rock))
				most_filled = lorry;
		}
		if (most_filled) {
			if (!this.rock_count || this.rock_count != BoundBy(most_filled->ContentsCount(Rock), 0, 40)) {
				this.rock_count = BoundBy(most_filled->ContentsCount(Rock), 0, 40);
				UpdatePaulaGoal(this.rock_count);
			}
		} else if (this.rock_count) {
			this.rock_count = 0;
			UpdatePaulaGoal(this.rock_count);
		}
	}
};

static const Samuel_GoalEffect = {
	Timer = func() {
		var chests = FindObjects(Find_InRect(700, 400, 160, 60), Find_ID(Chest));
		var breads = 0;
		for (var chest in chests)
			breads += chest->ContentsCount(Bread);
		breads = BoundBy(breads, 0, 25);
		if (!this.bread_count || this.bread_count != breads) {
			this.bread_count = breads;
			UpdateSamuelGoal(this.bread_count);
		}
	}
};