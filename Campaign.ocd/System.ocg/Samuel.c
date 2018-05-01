// Functions important to create Samuel across scenarios

global func MakeSamuel(object clonk, int owner)
{
	if (owner == nil)
		owner = NO_OWNER;
	clonk->SetOwner(owner);
	clonk->MakeInvincible();
	clonk->MakeNonFlammable();
	clonk->SetSkin(2);
	clonk->SetName("Samuel");
	clonk->SetColor(RGB(77, 66, 151));
	clonk->SetObjectLayer(clonk);
}