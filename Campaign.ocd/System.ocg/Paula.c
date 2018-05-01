// Functions important to create Paula across scenarios

global func MakePaula(object clonk, int owner)
{
	if (owner == nil)
		owner = NO_OWNER;
	clonk->SetOwner(owner);
	clonk->MakeInvincible();
	clonk->MakeNonFlammable();
	clonk->SetSkin(3);
	clonk->SetName("Paula");
	clonk->SetColor(RGB(77, 151, 66));
	clonk->SetObjectLayer(clonk);
}