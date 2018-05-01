// If all nests have been destroyed, bats still respawn but only a few

static const BatSpawnEffect = {
	Timer = func() {
		var bat_count = ObjectCount(Find_ID(Bat));
		if (bat_count < 5)
		{
			if (Random(10000) < (5 - bat_count) * 20)
			{
				var bat = CreateObject(Bat, RandomX(1870, 2670), 710, NO_OWNER);
				while (bat->Stuck())
					bat->SetPosition(bat->GetX(), bat->GetY()-1);
			}
		}
	}
};