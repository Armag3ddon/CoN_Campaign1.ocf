/*--
	Commands for the pioneers
--*/

global func PlayerControl(int plr, int ctrl)
{
	if (ctrl >= CON_QuickCommand1 && ctrl <= CON_QuickCommand4)
	{
		var controller = FindObject(Find_ID(GUI_Controller), Find_Owner(plr));
		if (!controller) return false;
		if (controller->QuickCommand(ctrl - CON_QuickCommand0))
			return true;
	}
	if (ctrl == CON_ToggleCommands)
	{
		var controller = FindObject(Find_ID(GUI_Controller), Find_Owner(plr));
		if (!controller) return false;
		if (controller->ToggleCommands())
			return true;
	}
	return _inherited(plr, ctrl, ...);
}