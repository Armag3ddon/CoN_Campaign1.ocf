#appendto Clonk

// Needed for SetToWalkingSpeed (NPCs.c) to work
func GetCurrentWalkAnimation()
{
	if (!this.shouldwalk)
		return _inherited();

	if(Contained())
	{
		if(Contained()->GetCategory() & C4D_Structure)
		{
			return Clonk_WalkInside;
		}
		return;
	}
	else SetProperty("PictureTransformation", Trans_Mul(Trans_Translate(0,1000,5000), Trans_Rotate(70,0,1,0)), this);
	var velocity = Distance(0,0,GetXDir(),GetYDir());
	if(velocity < 1) return Clonk_WalkStand;
	return Clonk_WalkWalk;
}

// Prevent clonk from performing idle actions (visually conflicts with other animations cycles)
func FxIntWalkTimer(pTarget, effect)
{
	// Test Waterlevel
	if(InLiquid() && GBackLiquid(0, -5) && !Contained())
	{
		SetAction("Swim");
		if(GetComDir() == COMD_Left)
			SetComDir(COMD_UpLeft);
		else if(GetComDir() == COMD_Right)
			SetComDir(COMD_UpRight);
		else if(GetComDir() != COMD_Down && GetComDir() != COMD_DownLeft && GetComDir() != COMD_DownRight)
			SetComDir(COMD_Up);
		return;
	}
	if(lAnim.backwardsSpeed != nil)
		UpdateBackwardsSpeed();
	if(effect.idle_animation_time)
	{
		effect.idle_animation_time--;
		if(effect.idle_animation_time == 0)
			effect.animation_name = nil;
	}
	var anim = GetCurrentWalkAnimation();
	if(anim != effect.animation_name)
	{
		effect.animation_name = anim;
		effect.idle_time = 0;
		effect.animation_id = PlayAnimation(anim, CLONK_ANIM_SLOT_Movement, GetWalkAnimationPosition(anim, GetAnimationPosition(effect.animation_id), effect.animation_name), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
	}
	// The clonk has to stand, not making a pause animation yet and not doing other actions with the hands (e.g. loading the bow)
	else if(anim == Clonk_WalkStand && !GetHandAction() && GetMenu() == nil && !this.noidleactions)
	{
		if (effect.footstop_time) effect.footstep_time = 0;
		if(!effect.idle_animation_time)
		{
			effect.idle_time++;
			if(effect.idle_time > 300+effect.idle_offset)
			{
				effect.idle_time = 0;
				effect.idle_offset = Random(300);
				var rand = Random(GetLength(Clonk_IdleActions));
				PlayAnimation(Clonk_IdleActions[rand][0], CLONK_ANIM_SLOT_Movement, Anim_Linear(0, 0, GetAnimationLength(Clonk_IdleActions[rand][0]), Clonk_IdleActions[rand][1], ANIM_Remove), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
				effect.idle_animation_time = Clonk_IdleActions[rand][1]-5;
				if (!Random(10))
					this->PlaySoundIdle();
			}
		}
	}
	else
	{
		effect.idle_time = 0;
		if(effect.idle_animation_time)
		{
			effect.animation_name = nil;
			effect.idle_animation_time = 0;
		}
		if (anim == Clonk_WalkRun)
		{
			// There are roughly two animation positions in Run when a foot touches the ground:
			// 550 and 1700
			// This here is trying to trigger a footstep as close as possible to these two moments.
			var pos = GetAnimationPosition(effect.animation_id);
			if (pos < 550 && effect.footstep_time)
				effect.footstep_time = 0;
			if (Inside(pos, 550, 1699) && effect.footstep_time != 1)
			{
				Footstep();
				effect.footstep_time = 1;
			}
			if (pos >= 1700 && effect.footstep_time != 2)
			{
				Footstep();
				effect.footstep_time = 2;
			}
		}
		else if(effect.footstep_time) effect.footstep_time = 0;
	}
}

// Include the lift plate from the mining elevator in the interactions
public func GetExtraInteractions()
{
	var functions = _inherited(...) ?? [];

	// call elevator cases
	var elevators = FindObjects(Find_ID(ElevatorLiftPlate), Find_InRect(-ELEVATOR_CALL_DISTANCE, AbsY(0), ELEVATOR_CALL_DISTANCE * 2, GetY() + AbsY(LandscapeHeight())), Find_Func("Ready", this));
	for (var elevator in elevators)
		PushBack(functions, { Fn = "CallCase", Object=elevator, Description=elevator->GetCallDescription(), Priority=0 });
	return functions;
}