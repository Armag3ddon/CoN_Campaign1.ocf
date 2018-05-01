/**
	@author Dustin Neß (dness.de)
*/

func Construction()
{
	SetProperty("MeshTransformation", Trans_Mul(Trans_Rotate(RandomX(-40,40),0,20), Trans_Scale(550)));
}

public func IsTree()
{
	return true;
}

public func IsStanding()
{
	return false;
}

local Components = { Wood = 16 }; // Axe will yield 8 wood
local Plane = 99;