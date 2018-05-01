/**
	Extra Display

	Displays extra information in the lower right corner.
	Reserved for special occasions (such as important questions).
*/

#appendto GUI_Controller

// Does also use constants defined by ControllerWealth to ensure uniform icon sizes.

local extradisplay_gui_menu;
local extradisplay_gui_id;

/* Creation */

func Construction()
{
	var y_begin = -GUI_Controller_Goal_IconSize - GUI_Controller_Goal_IconMargin;

	extradisplay_gui_menu = 
	{
		Target = this,
		Player = NO_OWNER, // Will become visible if there is something to show
		Style = GUI_Multiple | GUI_IgnoreMouse,
		Right = Format("50%%%s", ToEmString(GUI_Controller_Goal_IconMargin + GUI_Controller_Goal_IconSize)),
		Top = Format("100%%%s", ToEmString(y_begin)),
		Bottom = Format("100%%%s", ToEmString(-GUI_Controller_Goal_IconMargin)),
		Priority = 1,
		text = 
		{
			Style = GUI_TextRight | GUI_TextTop,
			Text = nil,
			Priority = 3,
			Right = Format("100%%%s", ToEmString(-GUI_Controller_Goal_IconSize - GUI_Controller_Goal_IconMargin)),
		},
		icon = 
		{
			Symbol = nil,
			Priority = 3,
			Right = "100%",
			Left = Format("100%%%s", ToEmString(-GUI_Controller_Goal_IconSize)),
		}
	};
	extradisplay_gui_id = GuiOpen(extradisplay_gui_menu);

	return _inherited(...);
}

func Destruction()
{
	GuiClose(extradisplay_gui_id);

	_inherited(...);
}

/* Display */

global func UpdateExtraDisplays(string text, icon) // icon can be ID or object
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		UpdateExtraDisplay(text, icon, GetPlayerByIndex(i, C4PT_User));
	}
}

global func UpdateExtraDisplay(string text, icon, int plr)
{
	var controller = FindObject(Find_ID(GUI_Controller), Find_Owner(plr));

	if (controller)
		controller->DoUpdateExtraDisplay(text, icon);
}

public func DoUpdateExtraDisplay(string text, icon)
{
	var plr = GetOwner();
	if (!text) plr = NO_OWNER;

	var update = {
		Player = plr,
		text = {
			Text = text,
		},
		icon = {
			Symbol = icon
		}
	};

	GuiUpdate(update, extradisplay_gui_id);
}