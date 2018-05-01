// Start sequence the first time someone tries to move it

#appendto ElevatorCase

func SetMoveDirection(int dir, bool user_requested, bool drill)
{
	if (this.firstmove)
	{
		var clonk = this->GetCasePusher();
		if (clonk) StartSequence("Elevator", 0, clonk);
		return;
	}
	if (this.blocked)
	{
		if (FindObject(Find_ID(Campaign_Crate), Find_InRect(-15,-15,30,30)))
			return Sound("Structures::Elevator::Stop");
		this.blocked = false;
	}
	return _inherited(dir, user_requested, drill);
}