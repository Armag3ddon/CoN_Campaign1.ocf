/**
	Bat cave

	Enough for several bats and bat mobiles.
*/

local swarm;

local max_swarm = 10;

local home;

func Init(object tree)
{
	home = tree;
	AddEffect("TreeHugging", tree, 1, nil, this);
	this.Plane = tree.Plane + 1;
}

func Initialize()
{
	swarm = [];
	AddTimer("RebirthBat", 35);
}

// Timer, occassionally respawns a bat
func RebirthBat()
{
	// Remove holes created by dead bats (bat corpses are quickly removed)
	RemoveHoles(swarm);
	if (GetLength(swarm) < max_swarm)
	{
		// Spawning a new bat gets more likely if there are less bats left alive
		if (Random(10000) < (max_swarm - GetLength(swarm))*10)
			SpawnABat();
	}
}

func SpawnABat()
{
	var bat = CreateObject(Bat, 0,0, NO_OWNER);
	// Sneaky player might have clumped a lot of earth in front of the nest!
	while (bat->Stuck())
		bat->SetPosition(bat->GetX(), bat->GetY()-1);
	swarm[GetLength(swarm)] = bat;
}

func SpawnADieingBat()
{
	var bat = CreateObject(Bat, RandomX(-10,10), RandomX(-10,10), NO_OWNER);
	// Poor thing :(
	bat->Incinerate();
}

// The tree this nest hangs onto is invincible
func FxTreeHuggingDamage(target, effect, int damage)
{
	// NO >:(
	return 0;
}

// If the home tree is to be incinerated
func FxTreeHuggingTreeOnFire(object target, proplist effect, incinerating_object, flamethrower)
{
	// Only a direct hit with the flamethrower will destroy this nest
	if (incinerating_object && incinerating_object->GetID() == ImperialFlamethrower && flamethrower)
	{
		ScheduleCall(this, "FieryDeath", 5, nil, incinerating_object);
		RemoveEffect(nil, nil, effect);
	}
}

// Gruesome!
func FieryDeath(object flamethrower)
{
	RemoveTimer("RebirthBat");
	Sound("DeathScreech", true);
	Incinerate();
	if (home)
		home->Incinerate(100, NO_OWNER, false, flamethrower, true);
	ScheduleCall(this, "SpawnADieingBat", 20, 9);
	ScheduleCall(this, "Death", 200);
}

func Death()
{
	GameCall("DeathOfABatNest");
	RemoveObject();
}

local Name = "$Name$";
local Description = "$Description$";