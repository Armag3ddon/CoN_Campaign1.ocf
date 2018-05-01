// Chests are not be opened until being told so.

#appendto Chest

public func OnShownInInteractionMenuStart(bool first, object menu)
{
	if (!g_chests_allowed)
	{
		if (g_chest != this || !g_get_igniter)
		{
			ScheduleCall(menu, "Close", 1);
			samuels_dlg->Dlg_Samuel_LeaveChests(menu.cursor);
			return;
		} else {
			return _inherited(first);
		}
	}

	return _inherited(first);
}