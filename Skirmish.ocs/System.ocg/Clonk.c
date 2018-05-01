// The clonk is not allowed to pickup all the stuff in this scenario

#appendto Clonk

private func TryToCollect(object item)
{
	if (GetEffect("ImportantGear", item))
	{
		GetEffect("ImportantGear", item)->Pickup(this);
		return false;
	}

	return _inherited(item);
}