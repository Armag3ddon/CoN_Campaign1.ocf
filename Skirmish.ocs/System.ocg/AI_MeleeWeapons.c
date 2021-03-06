// Modify the melee weapon AI.
// During the first battle (pioneer bridge building), AI clonk might find themselves in the situation
// of being without ammunition but with a melee weapon. However, storming into battle like the standard
// AI behaviour will probably end the clonk up in lava. That's silly.

// ExecuteEvade!

#appendto AI_MeleeWeapons

private func ExecuteMelee(effect fx)
{
	// Don't override anything if not in the first battle
	if (GetEffect("First_Battle") == nil)
		return _inherited(fx);

	// Still carrying the melee weapon?
	if (fx.weapon->Contained() != fx.Target)
	{
		fx.weapon = nil;
		return false;
	}
	// Are we in range?
	var x = fx.Target->GetX(), y = fx.Target->GetY(), tx = fx.target->GetX(), ty = fx.target->GetY();
	var dx = tx - x, dy = ty - y;
	if (Abs(dx) <= 10 && PathFree(x, y, tx, ty))
	{
		if (dy >= -15)
		{
			// Target is under us - sword slash downwards!
			if (!this->CheckHandsAction(fx))
				return true;
			// Stop here.
			fx.Target->SetCommand("None");
			fx.Target->SetComDir(COMD_None);
			// Cooldown?
			if (!fx.weapon->CanStrikeWithWeapon(fx.Target))
			{
				// While waiting for the cooldown, we try to evade...
				this->ExecuteEvade(fx, dx, dy);
				return true;
			}
			// OK, do a strike.
			this->SelectItem(fx, fx.weapon);
			// First try to do a single control use call.
			if (!fx.weapon->~HoldingEnabled() && fx.weapon->~ControlUse(fx.Target, tx, ty))
				return true;
			// Assume the weapon must be held longer for a strike.
			if (fx.weapon->~ControlUseStart(fx.Target, tx, ty))
			{
				fx.weapon->~ControlUseHolding(fx.Target, tx, ty);
				fx.weapon->~ControlUseStop(fx.Target, tx, ty);
			}
		}
		// Clonk is above us - jump there, but only if not commanded.
		if (!fx.commander)
		{
			this->ExecuteJump(fx);
			if (dx < -5)
				fx.Target->SetComDir(COMD_Left);
			else if (dx > 5)
				fx.Target->SetComDir(COMD_Right);
			else
				fx.Target->SetComDir(COMD_None);
		}
	}

	// Only 'walk there' if it's save!
	if (Abs(dx) > 70) // Distance is bigger than a wooden bridge, so there might be a gap between us and the target!
		if (!GetEffect("First_Battle")->SafeCrossing())
			return true; // Better do nothing (with a chance of looking stupid)

	// Not in range. Walk there, but only if not being commanded.
	if (!fx.commander && (!fx.Target->GetCommand() || !Random(10)))
		fx.Target->SetCommand("MoveTo", fx.target);
	return true;
}