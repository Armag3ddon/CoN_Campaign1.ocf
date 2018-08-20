// Modify the movement AI.
// During the first battle (pioneer bridge building), AI will often just run off and away from the pioneers
// and players and back away far too much. Because the evade behaviour will just give them a com dir and
// then nothing again.

#appendto AI_Movement

// Evade a threat from the given coordinates.
public func ExecuteEvade(effect fx, int threat_dx, int threat_dy)
{
	// Don't override anything if not in the first battle
	if (GetEffect("First_Battle") == nil)
		return _inherited(fx, threat_dx, threat_dy);

	// Don't try to evade if the AI has a commander, if an AI is being commanded
	// it has more important tasks, like staying on an airship.
	if (fx.commander)
		return false;

	// Do not evade if target isn't even close
	if (AbsX(threat_dx) > 70)
		return true;

	// Evade from threat at position delta threat_dx, threat_dy.
	if (threat_dx < 0)
		fx.Target->SetComDir(COMD_Left);
	else
		fx.Target->SetComDir(COMD_Right);
	if (threat_dy >= -5 && !Random(2))
		if (this->ExecuteJump(fx))
			return true;
	// Shield? Todo.
	return true;
}