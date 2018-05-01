#appendto Torch

/* Updated description */

local Description = "$DescTorch$";

/* Torches can be shot with acid spit */

public func IsProjectileTarget(object bullet)
{
	if (bullet->GetID() == AcidSpit)
		if (!Contained() && IsLightSource())
			return true;
}

public func OnProjectileHit(object bullet)
{
	if (bullet->GetID() != AcidSpit) return;
	if (!IsLightSource()) return;

	// 50% chance of the torch being extinguished
	if (Random(2)) return;

	var effect = GetEffect("IntBurning", this);
	if (effect)
	{
		if (effect.on_tree)
			DetachFromTree();
		else
			DetachFromWall();
	}
}

/* Torches can't be put up on the background of the Dark Forest */

public func ControlUse(object clonk)
{
	// Attach the torch if the clonk stands in front of tunnel material.
	if (GetMaterial() == Material("Tunnel"))
		if (GetTexture() != "tunnel_transp")
		{
			// Do an attach animation. 
			clonk->DoKneel(); // For now kneel.
			// Attach the torch to the wall.
			AttachToWall();
			return true;
		}
	var tree;
	if (tree = FindObject(Find_AtPoint(), Find_Func("IsTree"), Find_Func("IsStanding")))
	{
		clonk->DoKneel();
		AttachToTree(tree);
		return true;
	}
	// Otherwise log a message about where one can attach torches.
	Message("$MsgTorchAttach$");
	return true;
}

/* Torches can be attached to trees */

public func Interact(object clonk)
{
	var effect = GetEffect("IntBurning", this);
	if (effect && effect.on_tree) {
		clonk->DoKneel();
		DetachFromTree();
		clonk->Collect(this, false, nil, true);
		return true;
	}
	return _inherited(clonk);
}

public func AttachToTree(object tree)
{
	if (!tree) return;
	// Exit the torch and make it a non-collectible static back, also change its state.
	if (Contained()) Exit(0, 3);
	SetPosition(tree->GetX(), GetY());

	SetCategory(C4D_StaticBack);
	this.Collectible = false;
	state = TRCH_Attached;
	this.Plane = tree.Plane + 1;
	// Rotate the head of the torch a little into the screen.
	this.MeshTransformation = Trans_Rotate(-20, 1, 0, 0);
	// Add a burning effect if not already done.
	if (!GetEffect("IntBurning", this))
		AddEffect("IntBurning", this, 100, 4, this);
	GetEffect("IntBurning", this).tree = tree;
	GetEffect("IntBurning", this).on_tree = true;

	return;
}

public func DetachFromTree()
{
	// Make the torch a collectible object, also change its state.
	SetCategory(C4D_Object);
	this.Collectible = true;
	state = TRCH_Normal;
	// Remove the burning effect if active.
	if (GetEffect("IntBurning", this))
		RemoveEffect("IntBurning", this);
	return;
}

/* Torches can be held in the hand and are lit! */

func Selection()
{
	if (!GetEffect("IntBurning", this))
		AddEffect("IntBurning", this, 100, 4, this);
}

func Deselection()
{
	if (GetEffect("IntBurning", this))
		RemoveEffect("IntBurning", this);
}

func FxIntBurningTimer (object target, effect fx, int time)
{
	if (state == TRCH_InHand)
	{
		if (!Contained()) return; // ??

		if(!Contained()->~IsJumping() && !Contained()->~IsWalking())
			return;

		var dir = Contained()->GetCalcDir();
		var dist = 4;
		if (dir == -1) dist = 5;
		var lower_limit = Min(dir*dist - 1, dir*dist + 2);
		var upper_limit = Max(dir*dist - 1, dir*dist + 2);
		// Fire effects.
		Contained()->CreateParticle("FireSharp", PV_Random(lower_limit, upper_limit), PV_Random(-6, -9), PV_Random(-2, 2), PV_Random(-3, -5), 10 + Random(3), fx.flame, 12);
		// Smoke effects.
		Contained()->CreateParticle("Smoke", PV_Random(lower_limit, upper_limit), PV_Random(-13, -15), PV_Random(-2, 2), PV_Random(-2, 2), 24 + Random(12), fx.smoke, 4);
		// Interval jitter
		if (!Random(10)) fx.Interval = 3+Random(3);
	}

	if (fx.on_tree)
	{
		if (!fx.tree)
			return DetachFromTree();
		if (!fx.tree->~IsStanding())
			return DetachFromTree();
	}

	return _inherited(target, fx, time);
}