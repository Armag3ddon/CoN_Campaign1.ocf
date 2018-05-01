// All kinds of little visual effects

static const CookingFire = new Effect {
	Construction = func () {
		this.x = 451;
		this.y = 710;
		this.flame = 
		{
			R = PV_KeyFrames(0, 0, 0, 200, 255, 800, 255, 1000, 255),
			G = PV_KeyFrames(0, 0, 0, 200, 210, 800, 70, 1000, 70),
			B = PV_KeyFrames(0, 0, 255, 200, 100, 800, 0, 1000, 0),

			Alpha = PV_KeyFrames(1000, 0, 0, 10, 255, 500, 255, 1000, 0),
			Size = PV_Linear(PV_Random(1, 2), PV_Random(3, 4)),
			Stretch = 1000,
			Phase = PV_Random(0, 4),
			Rotation = PV_Random(0, 359),
			DampingX = 900,
			DampingY = 1000,
			BlitMode = GFX_BLIT_Additive,
		};

		this.kitchen = FindObject(Find_ID(Kitchen));
		this.kitchen->Sound("Liquids::Boiling", nil, 100, nil, +1, 250);
	},
	Timer = func () {
		Smoke(this.x, this.y - 5, 7, RGBa(255,255,255, 150));
		CreateParticle("FireSharp", PV_Random(this.x - 1, this.x + 1), PV_Random(this.y + 5, this.y + 3), PV_Random(-2, 2), PV_Random(-3, -5), 10 + Random(3), this.flame, 12);
	},
	Destruction = func() {
		if (this.kitchen)
			this.kitchen->Sound("Liquids::Boiling", nil, nil, nil, -1);
	}
};

static const SteamEngineWork = new Effect {
	Construction = func () {
		this.anim = this.Target->PlayAnimation("Work", 2, Anim_Linear(0,0, this.Target->GetAnimationLength("Work"), 35, ANIM_Loop), Anim_Const(1000));
		this.Target->Sound("Structures::SteamEngine", nil, 100, nil, +1, 250);
	},
	Timer = func () {
		this.Target->Smoking();
	},
	Destruction = func () {
		this.Target->StopAnimation(this.anim);
		this.Target->Sound("Structures::SteamEngine", nil, nil, nil, -1);
	}
};