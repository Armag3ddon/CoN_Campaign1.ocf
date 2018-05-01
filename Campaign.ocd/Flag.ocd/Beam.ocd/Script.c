/**
	More or less a dummy definition.
	The flag will change def if SetBeam() is called into this one.
	This has a different Offset, so the rotation point will be different.
	The beam is used in conjunction with the drawbridge.
*/

func SaveScenarioObject(props)
{
	return false;
}

public func SetTurnPosition(int angle)
{
	this.MeshTransformation = Trans_Mul(Trans_Rotate(60,0,1,0), Trans_Rotate(angle, 1));
}

/*-- Display --*/

func Definition(def)
{
	SetProperty("MeshTransformation", Trans_Rotate(60,0,1,0),def);
}

/*-- Properties --*/

local Name = "$Name$";