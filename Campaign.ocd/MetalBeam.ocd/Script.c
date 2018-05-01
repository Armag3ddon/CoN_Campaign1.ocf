/**
	Metal beam
	Purely for decoration

	Graphics by Foaly (CC-BY 3.0)
*/

func Construction()
{
	SetProperty("MeshTransformation", Trans_Mul(Trans_Rotate(RandomX(-15,15), 1, 0, 0), Trans_Rotate(RandomX(-15,15), 0, 1, 0)));
}

/*-- Properties --*/

local Name = "$Name$";