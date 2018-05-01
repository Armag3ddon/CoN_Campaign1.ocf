/*
	Enemy Marker

	Will mark enemies that should stand out in a mass of enemies because they have important objects or are extra dangerous.
*/

local marked;

public func Set(object clonk, string graphics)
{
	marked = clonk;
	SetGraphics(graphics);
	Adjust();
	AddTimer("Adjust", 1);
}

func Adjust()
{
	if (!marked)
		return RemoveObject();
	if (!marked->GetAlive())
		return RemoveObject();

	SetPosition(marked->GetX(), marked->GetY() - 20);
}

local Plane = 901;