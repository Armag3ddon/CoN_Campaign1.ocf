#appendto Flagpole

// Show the proud banner of the Broken Kingdom

public func SetKingdomBanner()
{
	SetNeutral(true);
	this.MeshTransformation = Trans_Translate(0, 4000, 0);
	SetMeshMaterial("KingdomFlagBanner", 0);
	this.Name = "$FlagpoleKingdom$";
}