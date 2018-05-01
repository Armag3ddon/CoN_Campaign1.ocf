// Do not respawn script players

#appendto Rule_BaseRespawn

func OnClonkDeath(object clonk)
{
	if (GetPlayerType(clonk->GetOwner()) == C4PT_Script) return false;

	return _inherited(clonk);
}