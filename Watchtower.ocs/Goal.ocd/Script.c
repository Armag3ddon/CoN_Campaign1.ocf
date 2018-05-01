#include Goal_Script

local current_goal = -1;

local paula_added = false;
local samuel_added = false;
local metal_added = false;
local mushroom_added = false;

local paula_count = 0;
local samuel_count = 0;
local metal_count = 0;

local paula_done = false;
local samuel_done = false;
local metal_done = false;

local paula_finished = false;
local samuel_finished = false;
local metal_finished = false;

// done = players have put all items to where they belong
// finished = players have triggered the dialogue/action to finish the goal

global func AdvanceGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	goal->Advance();
}

global func AddPaulaGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	if (PaulaGoalAdded()) return;

	goal->AddPaula();
	Scenario->CreateEffect(Paula_GoalEffect, 1, 20);
}

global func AddSamuelGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	if (SamuelGoalAdded()) return;

	goal->AddSamuel();
	Scenario->CreateEffect(Samuel_GoalEffect, 1, 20);
}

global func AddMetalGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	if (MetalGoalAdded()) return;

	goal->AddMetal();
	Scenario->CreateEffect(Metal_GoalEffect, 1, 20);
}

global func AddMushroomGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	if (MushroomGoalAdded()) return;

	goal->AddMushroom();
}

global func PaulaGoalAdded() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.paula_added;
}

global func SamuelGoalAdded() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.samuel_added;
}

global func MetalGoalAdded() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.metal_added;
}

global func MushroomGoalAdded() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.mushroom_added;
}

global func UpdatePaulaGoal(int count) {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	goal->UpdatePaula(count);
}

global func UpdateSamuelGoal(int count) {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	goal->UpdateSamuel(count);
}

global func UpdateMetalGoal(int count) {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	goal->UpdateMetal(count);
}

global func PaulaGoalDone() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.paula_done;
}

global func SamuelGoalDone() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.samuel_done;
}

global func MetalGoalDone() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.metal_done;
}

global func FinishPaulaGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	if (PaulaGoalFinished()) return;

	goal->FinishPaula();
}

global func FinishSamuelGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	if (SamuelGoalFinished()) return;

	goal->FinishSamuel();
}

global func FinishMetalGoal() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	if (MetalGoalFinished()) return;

	goal->FinishMetal();
}

global func PaulaGoalFinished() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.paula_finished;
}

global func SamuelGoalFinished() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.samuel_finished;
}

global func MetalGoalFinished() {
	var goal = FindObject(Find_ID(Goal_Watchtower));
	if (!goal) return;
	return goal.metal_finished;
}

public func Advance() {
	current_goal++;
	if (current_goal == 0) {
		this.Name = "$Goal_Name0$";
		this.Description = "$Goal_Desc0$";
		SetGraphics("Tower");
	}
	if (current_goal == 1) {
		this.Name = "$Goal_Name1$";
		this.Description = "$Goal_Desc1$";
		SetGraphics("Basement");
	}
	if (current_goal == 2) {
		this.Name = "$Goal_Name2$";
		this.Description = "$Goal_Desc2$";
		SetGraphics("Igniter");
	}
	if (current_goal == 3) {
		this.Name = "$Goal_Name3$";
		this.Description = "$Goal_Desc3$";
		SetGraphics("Supplies");
	}
	if (current_goal == 4) {
		this.Name = "$Goal_Name4$";
		this.Description = "$Goal_Desc4$";
		SetGraphics("Samuel");
	}
	if (current_goal == 5) {
		this.Name = "$Goal_Name5$";
		this.Description = "$Goal_Desc5$";
		SetGraphics("Metal");
	}
	GameCallEx("OnGoalUpdate", this);
}

public func GetDescription(int plr)
{
	if (current_goal == 3)
	{
		var txt = this.Description;
		if (paula_added && !paula_done && !paula_finished)
			txt = Format("%s|$Goal_Paula_Verbose$", txt, paula_count);
		if (paula_done && !paula_finished)
			txt = Format("%s|$Goal_Paula_Done$", txt);
		if (samuel_added && !samuel_done && !samuel_finished)
			txt = Format("%s|$Goal_Samuel_Verbose$", txt, samuel_count);
		if (samuel_done && !samuel_finished)
			txt = Format("%s|$Goal_Samuel_Done$", txt);

		return txt;
	}

	if (current_goal == 5)
	{
		var txt = this.Description;
		if (metal_done)
			txt = Format("%s||$Goal_Metal_Done$", txt);

		if (mushroom_added)
			txt = Format("%s||$Goal_Mushroom_Verbose$", txt);

		return txt;
	}

	return _inherited(plr);
}

public func AddPaula()
{
	if (paula_added) return;

	paula_added = true;
	UpdateExtraGoals();
}

public func AddSamuel()
{
	if (samuel_added) return;

	samuel_added = true;
	UpdateExtraGoals();
}

public func AddMetal()
{
	if (metal_added) return;

	metal_added = true;
	UpdateExtraGoals();
}

public func AddMushroom()
{
	if (mushroom_added) return;

	mushroom_added = true;
	UpdateExtraGoals();
}

public func UpdatePaula(int count)
{
	paula_count = count;
	if (paula_count >= 40 && !paula_done) {
		paula_done = true;
		paulas_dlg->AddAttention();
	}
	else if (paula_done) {
		paula_done = false;
		paulas_dlg->RemoveAttention();
	}
	UpdateExtraGoals();
}

public func UpdateSamuel(int count)
{
	samuel_count = count;
	if (samuel_count >= 25 && !samuel_done) {
		samuel_done = true;
		samuels_dlg->AddAttention();
	}
	else if (samuel_done) {
		samuel_done = false;
		samuels_dlg->RemoveAttention();
	}
	UpdateExtraGoals();
}

public func UpdateMetal(int count)
{
	metal_count = count;
	if (metal_count >= 20 && !metal_done) {
		metal_done = true;
	}
	else if (metal_done) {
		metal_done = false;
	}
	UpdateExtraGoals();
}

public func FinishPaula()
{
	paula_finished = true;
	UpdateExtraGoals();

	if (BothFinished()) {
		AdvanceGoal();
		AddImportantQuestionMark(samuels_dlg, "Question2Asked");
	}
}

public func FinishSamuel()
{
	samuel_finished = true;
	UpdateExtraGoals();

	if (BothFinished()) {
		AdvanceGoal();
		AddImportantQuestionMark(samuels_dlg, "Question2Asked");
	}
}

public func FinishMetal()
{
	metal_finished = true;
	UpdateExtraGoals();
}

func BothFinished()
{
	return paula_finished && samuel_finished;
}

func UpdateExtraGoals()
{
	var text = "";
	var new_line = false;
	if (paula_added && !paula_finished) {
		text = Format("$Goal_Paula$", paula_count);
		new_line = true;
	}
	if (samuel_added && !samuel_finished) {
		if (new_line)
			text = Format("%s\n$Goal_Samuel$", text, samuel_count);
		else
			text = Format("%s$Goal_Samuel$", text, samuel_count);
	}
	if (metal_added && !metal_finished) {
		if (metal_done)
			text = Format("$Goal_Metal_Cannon$");
		else
			text = Format("$Goal_Metal$", metal_count);
		if (mushroom_added)
		{
			text = Format("%s\n$Goal_Mushroom$", text);
		}
	}

	UpdateExtraGoalTexts(text);
}