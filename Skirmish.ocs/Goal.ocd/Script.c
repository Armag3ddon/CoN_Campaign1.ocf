#include Goal_Script

local current_goal = -1;

global func AdvanceGoal() {
	var goal = FindObject(Find_ID(Goal_Skirmish));
	if (!goal) return;
	goal->Advance();
}

public func Advance() {
	current_goal++;
	if (current_goal == 0) {
		this.Name = "$Goal_Name0$";
		this.Description = "$Goal_Desc0$";
		SetGraphics("Wait");
	}
	if (current_goal == 1) {
		this.Name = "$Goal_Name1$";
		this.Description = "$Goal_Desc1$";
		SetGraphics("Talk");
	}
	if (current_goal == 2) {
		this.Name = "$Goal_Name2$";
		this.Description = "$Goal_Desc2$";
		SetGraphics("Samuel");
	}
	if (current_goal == 3) {
		this.Name = "$Goal_Name3$";
		this.Description = "$Goal_Desc3$";
		SetGraphics("Bridge");
	}
	if (current_goal == 4) {
		this.Name = "$Goal_Name4$";
		this.Description = "$Goal_Desc4$";
		SetGraphics("BridgeFight");
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
	return _inherited(plr);
}

func UpdateExtraGoals()
{
	var text = "";
	var new_line = false;

	UpdateExtraGoalTexts(text);
}