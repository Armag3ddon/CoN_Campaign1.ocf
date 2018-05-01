// Very aggressive bats, will always go for clonks and will attack any light source

#appendto Bat

static const DARKFOREST_MIN_Y = 650;
static const DARKFOREST_MIN_X = 1710;

local MaxEnergy = 10000;

/*-- Behavior --*/

func FxCoreBehaviorStart(object target, proplist effect, int temp)
{
	if (temp)
		return FX_OK;
	// No reproduction
	ScheduleCall(this, "RemoveReproductionEffect", 1);

	return _inherited(target, effect, temp);
}

// This timer is called every frame for some basic actions.
func FxCoreBehaviorTimer(object target, proplist effect, int time)
{
	// If the bat is hanging, either start flying or do nothing.
	if (GetAction() == "Hang")
	{
		// Start flying by random.
		if (!Random(100))
			Fly();
		return FX_OK;
	}

	/*-- Basic actions done while flying --*/

	// Move the bat and update its in-flight appearance.
	ChangeMovementVelocity(effect.move_direction[0], effect.move_direction[1]);
	UpdateFlightAppearance();

	// Make flying sounds and sometimes do a sonar wave.
	if (!Random(800))
		Sound("Animals::Bat::Flutter*");
	else if (!Random(800))
		Sound("Screech");
	else if (!Random(800))
		DoSonarWave(false);

	// Update the state of the bat.
	if (effect.time_since_startle > 0)
		effect.time_since_startle--;

	/*-- Random actions and actions based on events --*/

	// Only do these action updates every five frames.
	if ((time % 5) != 0)
		return FX_OK;

	// Stay out of water.
	if (GBackLiquid(0, 4) || GBackLiquid(0, 8))
	{
		effect.move_direction = [RandomX(-1, 1), -1];
		return FX_OK;
	}

	// Stay inside the dark forest
	if (GetY() < DARKFOREST_MIN_Y) {
		effect.move_direction = [RandomX(-1, 1), 1];
	}
	if (GetX() < DARKFOREST_MIN_X) {
		effect.move_direction = [1, RandomX(-1, 1)];
	}

	// Evade sources of light and fire
	for (var danger in FindObjects(Find_Or(Find_OCF(OCF_OnFire), Find_Func("IsLightSource")), Find_Distance(75), Find_Exclude(this), Sort_Distance()))
	{
		if (!PathFree(GetX(), GetY(), danger->GetX(), danger->GetY()))
			continue;
		var x = -Sign(danger->GetX() - GetX() + RandomX(-20, 20));
		var y = -Sign(danger->GetY() - GetY() + RandomX(-20, 20));
		effect.move_direction = [x, y];
		if (!Random(4)) DoSonarWave();
		effect.spit_target = danger;
		// Loose the attack target, because safety is more important.
		effect.attack_target = nil;
		return FX_OK;
	}

	// Use a spit attack against any light source / burning object
	if (effect.spit_target)
	{
		// Left the forest
		if (effect.spit_target->GetX() < DARKFOREST_MIN_X) {
			effect.spit_target = nil;
			return FX_OK;
		}
		if (Distance(GetX(), GetY(), effect.spit_target->GetX(), effect.spit_target->GetY()) > 200 || !PathFree(GetX(), GetY(), effect.spit_target->GetX(), effect.spit_target->GetY()))
		{
			effect.spit_target = nil;
			return FX_OK;
		}
		if (Random(10)) return FX_OK;

		var spit = CreateContents(AcidSpit);
		var angle = Angle(GetX(), GetY(), effect.spit_target->GetX(), effect.spit_target->GetY(), 100);
		var off_x = Sin(angle, 4);
		var off_y = Cos(angle, 4);

		spit->Launch(this, angle, 200, 75, off_x, off_y, 100);
		return FX_OK;
	}

	// Look for clonks to attack and bite for health.
	if (!Random(5))
	{
		var possible_prey = FindObject(Find_ID(Clonk), Find_OCF(OCF_Alive), Find_Distance(100), Find_NoContainer());
		// Check if path free and if there are enough buddies to mount an attack with.
		if (possible_prey && PathFree(GetX(), GetY(), possible_prey->GetX(), possible_prey->GetY()))
		{
			effect.attack_target = possible_prey;
			DoSonarWave(true);
			if (!Random(100))
				Sound("AttackScreech");
		}
		return FX_OK;
	}

	// Fly towards attack target when the bat has one.
	if (effect.attack_target)
	{
		// Don't chase outside the forest
		if (effect.attack_target->GetX() < DARKFOREST_MIN_X) {
			effect.attack_target = nil;
			return FX_OK;
		}
		// Give up chase after a while.
		if (Distance(GetX(), GetY(), effect.attack_target->GetX(), effect.attack_target->GetY()) > 200 || !PathFree(GetX(), GetY(), effect.attack_target->GetX(), effect.attack_target->GetY()))
		{
			effect.attack_target = nil;
			return FX_OK;
		}
		var x = Sign(effect.attack_target->GetX() - GetX());
		var y = Sign(effect.attack_target->GetY() - GetY());
		effect.move_direction = [x, y];

		if (!Random(100))
			Sound("AttackScreech");

		if (Distance(GetX(), GetY(), effect.attack_target->GetX(), effect.attack_target->GetY()) < 10)
		{
			BitePrey(effect.attack_target);
			effect.attack_target = nil;
			SetRandomDirection();
		}
		return FX_OK;
	}

	// Fly towards other bats to loosely make a swarm.
	if (!Random(70) && !effect.is_expat)
	{
		var fellow = FindObject(Find_ID(Bat), Find_OCF(OCF_Alive), Find_Distance(200), Find_Exclude(this));
		if (fellow != nil)
		{
			var x = Sign(fellow->GetX() - GetX());
			var y = Sign(fellow->GetY() - GetY());
			effect.move_direction = [x, y];
		}
		return FX_OK;
	}

	// Explore.
	if (!Random(80))
	{
		SetRandomDirection();
		return FX_OK;
	}

	// Hang on trees
	if (!Random(150))
	{
		var tree = FindObject(Find_AtPoint(), Find_Func("IsTree"), Find_Func("IsStanding"), Find_Not(Find_OCF(OCF_OnFire)));
		if (tree)
		{
			Hang();
			AddEffect("TreeHang", tree, 1, 35, this);
			return FX_OK;
		}
	}

	return FX_OK;
}

// Fly if anything happens to the tree you're hanging in
func FxTreeHangTimer()
{
	if (GetAction() != "Hang")
		return -1;
}

func FxTreeHangDamage(target, effect, int damage)
{
	if (GetAction() == "Hang")
		Fly();

	return damage;
}

func FxTreeHangDestruction()
{
	if (GetAction() == "Hang")
		Fly();
}

// No sounds
func DoSonarWave(bool agressive)
{
	// Handle cool down for the sonar wave.
	if (GetEffect("SonarWaveCoolDown", this))
		return;
	AddEffect("SonarWaveCoolDown", this, 100, 36 * 4, this);
	// Define sonar wave particles.
	var sonar_particle = {
		R = 255,
		B = 255,
		G = 255,
		Alpha = PV_Linear(255, 0),
		Size = PV_Linear(6, 72),
		//Stretch = 1000,
		DampingX = 1000,
		DampingY = 1000,
		BlitMode = 0,
		CollisionVertex = 0,
		OnCollision = PC_Stop(),
	};
	if (agressive)
	{
		sonar_particle.R = 255;
		sonar_particle.G = 0;
		sonar_particle.B = 0;
	}
	// Create the particle.
	CreateParticle("Shockwave", 0, 0, 0, 0, 24, sonar_particle, 1);
	return;
}

// Fast decay
func Death()
{
	// Stop current animation and play dying animation.
	PlayAnimation("Land", 1, Anim_Linear(0, 0, GetAnimationLength("Land"), 19, ANIM_Hold));
	this.MeshTransformation = Trans_Rotate(Random(360), 0, 1, 0);

	// Remove behavior effect and play dead animation.
	RemoveEffect("CoreBehaviour", this);
	SetAction("Dead");
	// Set border bound to zero when dead, also disable contactcalls.
	this.BorderBound = 0;
	this.ContactCalls = false;

	Decay(10);
}