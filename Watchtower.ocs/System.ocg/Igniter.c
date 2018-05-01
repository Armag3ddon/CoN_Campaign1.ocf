// The igniter shouldn't be used (otherwise it's removed)

#appendto Igniter

public func ControlUseStart(object clonk, int x, int y)
{
	if (g_igniter == this && clonk != paula)
		return false;
	return _inherited(clonk, x, y);
}