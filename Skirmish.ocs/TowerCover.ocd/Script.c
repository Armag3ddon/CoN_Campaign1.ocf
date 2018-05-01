/*-- Tower Cover --*/

#include Library_Cover

local back;

private func Initialize()
{
	this.cover_area = Shape->Rectangle(-20, -27, 40, 55);
	_inherited(...);
}

func SaveScenarioObject() { return false; }

local Plane = 506;