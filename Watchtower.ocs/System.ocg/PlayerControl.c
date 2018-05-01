// Override for the final sequence

global func PlayerControl(int plr, int ctrl, id spec_id, int x, int y, int strength, bool repeat, int status)
{
	var release = status == CONS_Up;

	if (g_fireatwill)
	{
		if (ctrl == CON_Use && release)
		{
			return Sequence->CannonShot(plr, x, y);
		}
	}

	return _inherited(plr, ctrl, spec_id, x, y, strength, repeat, status);
}