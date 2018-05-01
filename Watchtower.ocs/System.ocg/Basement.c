#appendto Basement

// Create new and exiting basements above or below (used for paula's building sequence)
public func Multiply()
{
	var dir = 1; // below
	if (Random(2))
		dir = -1; // above
	var y = 8 * dir;
	while (FindObject(Find_ID(GetID()), Find_AtPoint(0, y)))
		y += 8 * dir;

	var other = CreateObject(Basement, 0, y);
	other->SetWidth(25);
	other->MakeInvincible();
	Sound("Objects::WallKit::Click");
}