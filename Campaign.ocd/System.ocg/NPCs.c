// General functions useful for NPCs

/*-- Creation --*/

global func CreateRepublicanClonk(int x, int y, int dir, int owner, string name, bool female, bool invincible, bool, own_layer)
{
	var clonk = CreateObject(Clonk, x, y, owner);
	clonk->MakeCrewMember(owner);

	clonk->SetDir(dir);
	clonk->SetColor(GetImperialColors());

	if (!female)
		clonk->SetSkin(2);
	else
		clonk->SetSkin(3);

	if (name)
		clonk->SetName(name);

	if (invincible)
	{
		clonk->MakeInvincible();
		clonk->MakeNonFlammable();
	}

	if (own_layer)
		clonk->SetObjectLayer(clonk);

	return clonk;
}

global func CreateKutorianClonk(int x, int y, int dir, int owner, string name, bool female, bool invincible, bool, own_layer)
{
	var clonk = CreateObject(Clonk, x, y, owner);
	clonk->MakeCrewMember(owner);

	clonk->SetDir(dir);
	clonk->SetColor(GetKutorianColors());

	if (!female)
		clonk->SetSkin(0);
	else
		clonk->SetSkin(1);

	if (name)
		clonk->SetName(name);

	if (invincible)
	{
		clonk->MakeInvincible();
		clonk->MakeNonFlammable();
	}

	if (own_layer)
		clonk->SetObjectLayer(clonk);

	return clonk;
}

/*-- Visuals --*/

global func CreateOfficersHat(object clonk)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;

	var hat = clonk->CreateContents(Hat);
	hat->SetMeshMaterial("OfficersHat");
	hat->SetColor(GetImperialColors());

	clonk->AttachMesh(hat, "skeleton_head", "main", Trans_Mul(Trans_Translate(5200), Trans_Scale(900), Trans_Rotate(15, 0, 1,1)));
	return hat;
}

/*-- Animations --*/

// Use a slow walk instead of always running
global func SetToWalkingSpeed(object clonk)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;
	if (GetType(clonk) != C4V_C4Object)
		return;
	clonk.ActMap = { Prototype = Clonk.ActMap, Walk = { Prototype = Clonk.ActMap.Walk } };
	clonk.ActMap.Walk.Speed /= 4;
	clonk->SetAction("Walk");
	clonk.shouldwalk = true;
}

global func SetToRunningSpeed(object clonk)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;
	if (GetType(clonk) != C4V_C4Object)
		return;
	//clonk.ActMap = { Prototype = Clonk.ActMap, Walk = { Prototype = Clonk.ActMap.Walk } };
	clonk.ActMap.Walk.Speed *= 4;
	clonk->SetAction("Walk");
	clonk.shouldwalk = false;
}

// Let's the clonk neatly sit down
global func DoSitDown(object clonk, int time)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;
	if (!time)
		time = 15; // in frames

	// Idle actions would interrupt the sitting
	clonk.noidleactions = true;
	// Visuals
	clonk->DetachHandItem(0);
	clonk->DetachHandItem(1);

	clonk->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, clonk->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(1000,1000,0, time, ANIM_Remove));
	clonk->PlayAnimation("Drive", CLONK_ANIM_SLOT_Movement + 1, Anim_Const(1000), Anim_Linear(0,0,1000, time, ANIM_Hold));
	clonk->CreateEffect(SittingTransform, 1, 1, 1, time);
}

global func DoSitUp(object clonk, int time)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;
	if (!time)
		time = 15; // in frames

	clonk->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, clonk->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0,0,1000, time, ANIM_Remove));
	clonk->CreateEffect(SittingTransform, 1, 1, -1, time);
	Schedule(clonk, "this.noidleactions = false", time);
	ScheduleCall(clonk, "UpdateAttach", time);
}

// Slowly transforms the clonk down or up when sitting down/up
static const SittingTransform = new Effect {
	Construction = func (int direction, int time) {
		this.direction = direction;
		this.layer = 8; // See SetMeshTransformation in the clonk's script
		this.total_time = time;
	},
	Timer = func() {
		var percentage = (this.Time * 100) / this.total_time;
		var trans = percentage * 3000 / 100;
		if (this.direction == -1)
			trans = 3000 - trans;

		Target->SetMeshTransformation(Trans_Translate(0, trans), this.layer);

		if (this.Time >= this.total_time)
			return -1;
	}
};

// Properly greet your superior officers
global func DoSalute(string message, object clonk, int time)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;
	if (!time)
		time = 25; // in frames

	// Idle actions would interrupt the salute
	clonk.noidleactions = true;
	// Visuals
	clonk->DetachHandItem(0);

	clonk->PlayAnimation("IdleScratch", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0,0, 800, 15, ANIM_Hold), Anim_Linear(0,0,1000, 5, ANIM_Hold));
	ScheduleCall(clonk, "UndoSalute", 15 + time);
	if (message != nil)
		CustomMessage(Format("<c %x>%s:</c>|%s", clonk->GetColor(), clonk->GetName(), message), clonk, NO_OWNER);
}

global func UndoSalute(object clonk)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;

	clonk->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, clonk->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0,0,1000, 15, ANIM_Remove));
	Schedule(clonk, "this.noidleactions = false", 15);
	ScheduleCall(clonk, "UpdateAttach", 15);
}

// Care for wounded clonks
global func DoKneelComfort(object clonk, int time)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;
	if (!time)
		time = 35; // in frames

	// Idle actions would interrupt the kneel
	clonk.noidleactions = true;
	// Visuals
	clonk->DetachHandItem(0);
	clonk->DetachHandItem(1);

	clonk->PlayAnimation("KneelDown", CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(clonk->GetAnimationLength("KneelDown"),clonk->GetAnimationLength("KneelDown"), 198, 15, ANIM_Hold), Anim_Linear(0,0,1000, 5, ANIM_Hold));
	ScheduleCall(clonk, "UndoKneelComfort", 15 + time);
}

global func UndoKneelComfort(object clonk)
{
	if (!clonk && !this) return;
	if (!clonk)
		clonk = this;

	clonk->PlayAnimation(Clonk_WalkStand, CLONK_ANIM_SLOT_Movement + 1, Anim_Linear(0, 0, clonk->GetAnimationLength(Clonk_WalkStand), 95, ANIM_Loop), Anim_Linear(0,0,1000, 15, ANIM_Remove));
	Schedule(clonk, "this.noidleactions = false", 15);
	ScheduleCall(clonk, "UpdateAttach", 15);
}

/*-- Movement --*/

// A turning (SetDir) function which can be called using the "Call" command
global func CommandedTurn(dummy, int dir)
{
	if (!this) return;
	this->SetDir(dir);
}

global func FxTurnAfterWalkingStart(object target, proplist effect, int temp, int dir)
{
	effect.dir = dir;
}

global func FxTurnAfterWalkingTimer(object target, proplist effect)
{
	if (target->GetCommand()) return FX_OK;

	target->SetDir(effect.dir);
	return -1;
}