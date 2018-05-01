// Dark Forest Trees can only be incinerated with the flamethrower

global func Incinerate(
	strength /* strength between 0 and 100 */
	, int caused_by /* the player that caused the incineration */
	, blasted /* whether the object was incinerated by an explosion */
	, incinerating_object /* the object that caused the incineration */
	, flamethrower /* special parameter for the flamethrower */)
{
	if (!this)
		return false;
	// If a bat nest hangs in this tree, inform it
	if (GetEffect("TreeHugging", this)) {
		EffectCall(this, GetEffect("TreeHugging", this), "TreeOnFire", incinerating_object, flamethrower);
		return;
	}
	if (GetEffect("DarkForest_Tree", this)) {
		if (incinerating_object && incinerating_object->GetID() == ImperialFlamethrower && flamethrower)
			return _inherited(strength, caused_by, blasted, incinerating_object);
		return;
	}

	return _inherited(strength, caused_by, blasted, incinerating_object);
}