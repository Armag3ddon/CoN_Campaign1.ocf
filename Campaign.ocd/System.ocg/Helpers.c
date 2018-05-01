// Random helper function

// Sometimes it is necessary to make sure that clonks are somewhere
// Mainly during sequences in which clonks move around because players might have blocked paths
global func EnsureClonkPosition(int x, int y, int precision)
{
	if (!this) return;
	if (x == nil) x = this->GetX();
	if (y == nil) y = this->GetY();
	if (precision == nil) precision = 5;

	if (!Inside(this->GetX(), x - precision, x + precision) ||
	    !Inside(this->GetY(), y - precision, y + precision))
	{
		this->SetPosition(x, y);
		this->SetCommand("None");
		this->SetComDir(COMD_None);
	}
}

global func EnsureAllCrewPosition(int x, int y, int precision)
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		for (var j = 0; j < GetCrewCount(plr); j++)
		{
			var crew = GetCrew(plr, j);
			crew->EnsureClonkPosition(x, y, precision);
		}
	}
}

global func GetImperialColors()
{
	return RGBa(105,0,255, 255);
}

global func GetKutorianColors()
{
	return RGBa(156,0,0, 255);
}

// In multiplayer games, before starting certain sequences, it is desirable to have a little gathering area
// The area is marked with particles
// The triggering player will see a message, telling that he is waiting for others
// The other players see a message, telling them to go the area
global func CreateMultiplayerWaitingRadius(object triggering, int range, string function_to_call, object triggering_clonk)
{
	// Do not create an effect if all players are already in range
	var in_range = true;
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		var cursor = GetCursor(plr);
		if (!cursor)
			continue;
		if (ObjectDistance(triggering, cursor) > range)
		{
			in_range = false;
			CustomMessage("$MultiplayerWaitingForYou$", nil, plr, nil, 75, nil, GUI_MenuDeco, nil, MSG_Multiple|MSG_HCenter|MSG_VCenter);
		}
	}

	if (in_range)
		return false;

	triggering->CreateEffect(MultiplayerWaitArea, 1, 10, range, function_to_call, triggering_clonk);
	CustomMessage("$MultiplayerWaitForPlayers$", nil, triggering_clonk->GetOwner(), nil, 75, nil, GUI_MenuDeco, nil, MSG_Multiple|MSG_HCenter|MSG_VCenter);
	return true;
}

static const MultiplayerWaitArea = new Effect {
	Construction = func(int range, string func, object trigger) {
		this.range = range;
		this.func = func;
		this.trigger = trigger;
		this.particles =
		{
			Prototype = Particles_Spark(),
			Size = PV_Linear(10, 5),
			R = PV_Random(95, 115),
			G = 0,
			B = PV_Random(230, 255)
		};
	},
	Timer = func() {
		// Check for the triggering object (probably a clonk)
		// If that object moves out of the area, it's probably not important anymore!
		if (ObjectDistance(this.trigger, this.Target) >= this.range + 5)
			return -1;
		// Draw the area
		var rnd = Random(5);
		for (var i = 0; i < 360; i+=5)
		{
			var x = Sin(i + rnd, this.range);
			var y = Cos(i + rnd, this.range);
			this.Target->CreateParticle("MagicFire", x, y, 0,0, 20, this.particles);
		}
		// Check if everyone is in range
		var in_range = true;
		for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
		{
			var plr = GetPlayerByIndex(i, C4PT_User);
			var cursor = GetCursor(plr);
			if (!cursor)
				continue;
			if (ObjectDistance(this.trigger, cursor) > this.range)
			{
				in_range = false;
				
			}
		}
		if (in_range)
		{
			this.Target->Call(this.func, this.trigger);
			return -1;
		}
		// Wait!
		return FX_OK;
	}
};