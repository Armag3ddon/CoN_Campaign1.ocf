#appendto Lantern

/* Lanterns can be shot with acid spit */

public func IsProjectileTarget(object bullet)
{
	if (bullet->GetID() == AcidSpit)
		if (!Contained() && IsLightSource())
			return true;
}

public func OnProjectileHit(object bullet)
{
	if (bullet->GetID() != AcidSpit) return;
	if (!IsLightSource()) return;

	// 20% chance of the lantern being destroyed
	if (Random(5)) return;

	Hit2();
}