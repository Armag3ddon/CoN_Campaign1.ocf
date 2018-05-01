/*-- Inventor Cover --*/

#include Library_Cover

local back;

private func Initialize()
{
	this.cover_area = Shape->Rectangle(-25, 6, 64, 34);
	_inherited(...);
}

public func Set(object inv)
{
	back = inv;
	AddTimer("CheckPosition", 5);
}

private func CheckPosition()
{
	if (!back) return RemoveObject();
	if (GetX() != back->GetX()+1 || GetY() != back->GetY()+1)
		SetPosition(back->GetX()+1, back->GetY()+1);
}

func EditCursorMoved()
{
	// Move main trunk along with front in editor mode
	if (back) back->SetPosition(GetX()-1, GetY()-1);
	return true;
}

func SaveScenarioObject() { return false; }

local Plane = 505;