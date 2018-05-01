#appendto Crate

public func RejectContentsMenu()
{
	if (GetEffect("ImportantGear", this))
		return true;
	return false;
}