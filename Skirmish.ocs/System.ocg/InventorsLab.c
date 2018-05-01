/* The lab functions as military headquarters */

#appendto InventorsLab

local hq = false;
local barrier;

public func CloseBarrier()
{
	barrier = CreateObject(InventorBarrier, 38, 29, GetOwner());
}

public func OpenBarrier()
{
	if (barrier)
		barrier->RemoveObject();
}

public func SetHeadquarters()
{
	hq = true;

	SetMeshMaterial("militaryLab", 0);
	SetMeshMaterial("militaryLabTeleScope", 1);
	SetMeshMaterial("militaryLabNoCull", 2);
	SetMeshMaterial("militaryLabDetail", 3);

	var cover = CreateObject(InventorCover, 0,0, GetOwner());
	cover->Set(this);
}

public func SaveScenarioObject(proplist props, ...)
{
	if (!_inherited(props, ...)) return false;
	if (hq)
		props->AddCall("Headquarters", this, "SetHeadquarters");
	return true;
}