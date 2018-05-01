// When being hit by acid spit, clonks can lose their currently used light

#appendto Clonk

public func OnProjectileHit(object bullet)
{
	if (bullet->GetID() == AcidSpit)
	{
		var effect_done = false;
		if (GetHandItem(0))
		{
			if (GetHandItem(0)->GetID() == Torch && !Random(2)) // 50% chance of losing a burning torch
			{
				GetHandItem(0)->Exit(9 * GetCalcDir(), 0, RandomX(-2,2), 2 * GetCalcDir(), -1, Random(360));
				effect_done = true;
			}
			if (GetHandItem(0))
				if (GetHandItem(0)->GetID() == Lantern && Random(3)) // 66% chance of a lamp in your hand turning off
				{
					GetHandItem(0)->TurnOff();
					effect_done = true;
				}
		}
		if (!effect_done && ContentsCount(Lantern))
		{
			// Get a random lit lantern (in case somehow someone manages to get more than one?)
			var lantern = FindObject(Find_ID(Lantern), Find_Container(this), Find_Func("IsLightSource"), Sort_Random());
			if (lantern && !Random(3)) // 33% chance of a lamp on you turning off
				lantern->TurnOff();
		}
	}
	return _inherited(bullet);
}