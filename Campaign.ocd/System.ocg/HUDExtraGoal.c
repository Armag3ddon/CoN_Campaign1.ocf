/**
	ExtraGoal Display

	Displays extra information below the goal icon.
*/

#appendto GUI_Controller

// Does also use constants defined by ControllerWealth to ensure distance between both icons.

local extragoal_gui_menu;
local extragoal_gui_id;

local extragoal_visible = true;
local extragoal_set = false;

/* Creation */

func Construction()
{
	var y_begin = GUI_Controller_Goal_IconMargin * 2 + GUI_Controller_Goal_IconSize;
	var y_end = y_begin + GUI_Controller_Goal_IconSize;
	var x_end = GUI_Controller_Goal_IconMargin;

	extragoal_gui_menu = 
	{
		Target = this,
		Player = NO_OWNER, // Will become visible if there is something to show
		Style = GUI_Multiple | GUI_IgnoreMouse,
		Right = Format("100%%%s", ToEmString(-x_end)),
		Top = ToEmString(y_begin),
		Bottom = ToEmString(y_end),
		Priority = 1,
		text = 
		{
			Style = GUI_TextRight | GUI_TextTop,
			Text = nil,
			Priority = 3,
		}
	};
	extragoal_gui_id = GuiOpen(extragoal_gui_menu);

	return _inherited(...);
}

func Destruction()
{
	GuiClose(extragoal_gui_id);

	_inherited(...);
}

/* Display */

global func UpdateExtraGoalTexts(string text)
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		UpdateExtraGoalText(text, GetPlayerByIndex(i, C4PT_User));
	}
}

global func UpdateExtraGoalText(string text, int plr)
{
	var controller = FindObject(Find_ID(GUI_Controller), Find_Owner(plr));

	if (controller)
		controller->DoUpdateExtraGoalText(text);
}

public func DoUpdateExtraGoalText(string text)
{
	var plr = GetOwner();
	if (!text || !extragoal_visible) plr = NO_OWNER;

	extragoal_set = true;
	if (!text)
		extragoal_set = false;

	var update = {
		Player = plr,
		text = {
			Text = text,
		},
	};

	GuiUpdate(update, extragoal_gui_id);
}

global func HideExtraGoalTexts()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		ToggleExtraGoalText(GetPlayerByIndex(i, C4PT_User), false);
	}
}

global func ShowExtraGoalTexts()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		ToggleExtraGoalText(GetPlayerByIndex(i, C4PT_User), true);
	}
}

global func ToggleExtraGoalText(int plr, bool visible)
{
	var controller = FindObject(Find_ID(GUI_Controller), Find_Owner(plr));

	if (controller)
		controller->ToggleExtraGoalText(visible);
}

public func ToggleExtraGoalText(bool visible)
{
	extragoal_visible = visible;

	var plr = GetOwner();
	if (!extragoal_visible || !extragoal_set)
		plr = NO_OWNER;

	GuiUpdate({ Player = plr }, extragoal_gui_id);
}