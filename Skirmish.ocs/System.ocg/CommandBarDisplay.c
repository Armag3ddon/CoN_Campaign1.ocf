/**
	ControllerCommandBar

	Displays the squad commands during the first battle
*/

#appendto GUI_Controller

// HUD margin and size in tenths of em.
static const GUI_Controller_CommandBar_IconMarginScreenBottom = 5;
static const GUI_Controller_CommandBar_IconSize = 40;
static const GUI_Controller_CommandBar_IconMargin = 5;

static GUI_Controller_Commands_shown;

local command_gui_menu;
local command_gui_id;

local command_buttons;
local command_button;

local command_display;
local all_commands_display;

/* Showing / Hiding */

public func ShowCommandBar()
{
	// Definition call
	if (GetType(this) == C4V_Def)
	{
		GUI_Controller_Commands_shown = true; // for players joining later
		var plr;
		for (var i=0; i<GetPlayerCount(C4PT_User); ++i)
		{
			var plr = GetPlayerByIndex(i, C4PT_User);
			var controller = FindObject(Find_ID(GUI_Controller), Find_Owner(plr));
			if (controller) controller->ShowCommandBar();
		}
	} else {
		if (command_display) return;
		command_gui_id = GuiOpen(command_gui_menu);
		command_display = true;
	}
}

public func HideCommandBar()
{
	// Definition call
	if (GetType(this) == C4V_Def)
	{
		GUI_Controller_Commands_shown = false; // for players joining later
		var plr;
		for (var i=0; i<GetPlayerCount(C4PT_User); ++i)
		{
			var plr = GetPlayerByIndex(i, C4PT_User);
			var controller = FindObject(Find_ID(Library_HUDController->GetGUIControllerID()), Find_Owner(plr));
			if (controller) controller->HideCommandBar();
		}
	} else {
		if (!command_display) return;
		GuiClose(command_gui_id);
		command_gui_id = nil;
		command_display = false;
	}
}

public func IsShowingCommandBar() { return command_display; }

/* Dynamite throwing cooldown */

public func UpdateDynamiteCooldown(int readiness)
{
	// Definition call
	if (GetType(this) == C4V_Def)
	{
		var plr;
		for (var i=0; i<GetPlayerCount(C4PT_User); ++i)
		{
			var plr = GetPlayerByIndex(i, C4PT_User);
			var controller = FindObject(Find_ID(Library_HUDController->GetGUIControllerID()), Find_Owner(plr));
			if (controller) controller->UpdateDynamiteCooldown(readiness);
		}
	} else {
		if (!command_display) return;
		if (!all_commands_display) return;
		var update = {
			buttons = {
				dynamite = {
					cooldown = {
						Right = Format("%d%%", readiness)
					}
				}
			}
		};
		GuiUpdate(update, command_gui_id);
	}
}

/* GUI creation */

// For custom HUD graphics overload the following function as deemed fit.

func AssembleCommandButton(int max_slots, int slot_number, proplist slot_info)
{
	// The gui already exists, only update it with a new submenu
	var pos = CalculateCommandButtonPosition(slot_number, max_slots);

	return
	{
		Target = this,
		hotkey =
		{
			Priority = 4,
			Style = GUI_TextTop,
			Text = slot_info.hotkey,
		},
		cooldown =
		{
			Priority = 3,
			BackgroundColor = RGBa(255, 0, 0, 128),
			Right = "0%"
		},
		command =
		{
			Priority = 2,
			Symbol = Campaign_Icon_Commands,
			GraphicsName = slot_info.icon,
		},
		Style = GUI_NoCrop,
		ID = slot_info.ID,
		Symbol = { Std: Icon_Menu_RectangleRounded, Hover: Icon_Menu_RectangleBrightRounded },
		OnMouseIn = GuiAction_SetTag("Hover"),
		OnMouseOut = GuiAction_SetTag("Std"),
		OnClick = GuiAction_Call(this, slot_info.callback),
		Left = pos.Left, Top = pos.Top, Right = pos.Right, Bottom = pos.Bottom,
	};
}

/* Creation / Destruction */

func Construction()
{
	command_display = GUI_Controller_Commands_shown;
	all_commands_display = false;

	command_buttons = {
		Target = this,
		back = AssembleCommandButton(4, 1, { ID = 1, hotkey = "1", icon = "Backward", callback = "CommandBackward" }),
		halt = AssembleCommandButton(4, 2, { ID = 2, hotkey = "2", icon = "Halt", callback = "CommandHalt" }),
		forward = AssembleCommandButton(4, 3, { ID = 3, hotkey = "3", icon = "Forward", callback = "CommandForward" }),
		dynamite = AssembleCommandButton(4, 4, { ID = 4, hotkey = "4", icon = "Dynamite", callback = "CommandDynamite" })
	};

	command_button = {
		Target = this,
		commands = AssembleCommandButton(1, 1, { ID = 5, hotkey = "C", icon = "", callback = "ShowCommands" })
	};

	var pos = CalculateCommandBarSize(1);

	command_gui_menu =
	{
		Target = this,
		Player = GetOwner(),
		Style = GUI_Multiple | GUI_NoCrop | GUI_FitChildren,
		buttons = {
		},
		Left = pos.Left, Top = pos.Top, Right = pos.Right, Bottom = pos.Bottom,
	};

	command_gui_menu.buttons = command_button;

	if (command_display) GuiOpen(command_gui_menu);

	return _inherited(...);
}

func Destruction()
{
	if (command_gui_id) command_gui_id = GuiClose(command_gui_id);

	_inherited(...);
}

/* Callbacks */

public func ToggleCommands()
{
	if (!command_display)
		return false;

	if (all_commands_display)
		HideCommands();
	else
		ShowCommands();

	return true;
}

public func ShowCommands()
{
	if (all_commands_display)
		return;

	var pos = CalculateCommandBarSize(4);

	command_gui_menu.buttons = command_buttons;
	command_gui_menu.Left = pos.Left;
	command_gui_menu.Right = pos.Right;
	command_gui_menu.Top = pos.Top;
	command_gui_menu.Bottom = pos.Bottom;

	var cooldown = GetEffect("First_Battle")->GetDynamiteReadiness();
	command_gui_menu.buttons.dynamite.cooldown.Right = Format("%d%%", cooldown);

	GuiClose(command_gui_id);
	command_gui_id = GuiOpen(command_gui_menu);

	all_commands_display = true;
	ScheduleCall(this, "HideCommands", 140);
}

public func HideCommands()
{
	if (!all_commands_display)
		return;

	var pos = CalculateCommandBarSize(1);

	command_gui_menu.buttons = command_button;
	command_gui_menu.Left = pos.Left;
	command_gui_menu.Right = pos.Right;
	command_gui_menu.Top = pos.Top;
	command_gui_menu.Bottom = pos.Bottom;

	GuiClose(command_gui_id);
	command_gui_id = GuiOpen(command_gui_menu);

	all_commands_display = false;
	ClearScheduleCall(this, "HideCommands");
}

public func QuickCommand(int command)
{
	if (!command_display)
		return false;

	if (!all_commands_display)
		return false;

	if (command == 1)
	{
		CommandBackward();
		return true;
	}
	if (command == 2)
	{
		CommandHalt();
		return true;
	}
	if (command == 3)
	{
		CommandForward();
		return true;
	}
	if (command == 4)
	{
		CommandDynamite();
		return true;
	}
	return false;
}

public func CommandBackward()
{
	GameCall("OrderBackwards", GetOwner());
	HideCommands();
}

public func CommandForward()
{
	GameCall("OrderForwards", GetOwner());
	HideCommands();
}

public func CommandHalt()
{
	GameCall("OrderHalt", GetOwner());
	HideCommands();
}

public func CommandDynamite()
{
	GameCall("OrderDynamite", GetOwner());
	HideCommands();
}

/* Display */

// Calculates the position of a specific button and returns a proplist.
func CalculateCommandButtonPosition(int slot_number, int max_slots)
{
	var pos_x = (GUI_Controller_CommandBar_IconSize + GUI_Controller_CommandBar_IconMargin) * (slot_number - 1);
	var pos =
	{
		Left = ToEmString(pos_x),
		Top = "0%",
		Right = ToEmString(pos_x + GUI_Controller_CommandBar_IconSize),
		Bottom = "100%"
	};
	return pos;
}

func CalculateCommandBarSize(int max_slots)
{
	var width = GUI_Controller_CommandBar_IconSize * max_slots + GUI_Controller_CommandBar_IconMargin * (max_slots - 1);
	var pos_y = GUI_Controller_CommandBar_IconMarginScreenBottom;
	var pos =
	{
		Left = Format("50%%%s", ToEmString(-width / 2)),
		Top = Format("100%%%s", ToEmString(-pos_y - GUI_Controller_CommandBar_IconSize)),
		Right = Format("50%%%s", ToEmString(width / 2)),
		Bottom = Format("100%%%s", ToEmString(-pos_y))
	};
	return pos;
}