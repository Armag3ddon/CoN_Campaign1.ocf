// In this scenario, explosions don't BlastFree

global func DoExplosion(int x, int y, int level, object inobj, int cause_plr, object layer, bool silent, int damage_level)
{
	// Zero-size explosion doesn't affect anything
	if (level <= 0) return;
	// If the object is contained, loop through all parent containers until the blast is contained or no container is found.
	// Blast the containers it passes through and the objects inside them.
	var container = inobj;
	var prev_container = nil;
	while (container)
	{
		// Determine first whether the container has a parent and if it contains the blast.
		// This is needed because the container might be exploded 
		var contains_blast = container.ContainBlast;
		var parent_container = container->Contained();
		// Blast the current container, but not the previous container.
		BlastObjects(x + GetX(), y + GetY(), level, container, cause_plr, damage_level, layer, prev_container);
		// Break the blasting if this container contains the blast.
		if (contains_blast)
			break;
		// Move one container up if possible.
		prev_container = container;
		container = parent_container;
	}
	
	// Explosion outside: Explosion effects.
	if (!container)
	{
		// Blast objects outside if there was no final container containing the blast.
		BlastObjects(x + GetX(), y + GetY(), level, container, cause_plr, damage_level, layer, prev_container);
		// Incinerate oil.
		if (!IncinerateLandscape(x, y, cause_plr))
			if (!IncinerateLandscape(x, y - 10, cause_plr))
				if (!IncinerateLandscape(x - 5, y - 5, cause_plr))
					IncinerateLandscape(x + 5, y - 5, cause_plr);
		// Graphic effects.
		Call("ExplosionEffect", level, x, y, 0, silent, damage_level);
		// Landscape destruction. Happens after BlastObjects, so that recently blown-free materials are not affected
		//BlastFree(x, y, level, cause_plr);
		//##########
	}

	return true;
}