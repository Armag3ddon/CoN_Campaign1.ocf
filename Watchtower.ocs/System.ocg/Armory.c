// Special button for cannon production
// Always accept metal

#appendto Armory

public func RejectCollect(id item_id, object item)
{
	if (!_inherited(item_id, item)) return false;

	if (item_id == Metal || (item && item->GetID() == Metal))
		return false;
	return true;
}

public func GetInteractionMenus(object clonk)
{
	var menus = _inherited(clonk, ...) ?? [];
	var cannon_menu =
	{
		title = "$TxtCannon$",
		entries_callback = this.GetCannonMenuEntries,
		callback = "OnCannonSelected",
		callback_hover = "OnCannonBuildHover",
		callback_target = this,
		BackgroundColor = RGB(0, 0, 50),
		Priority = 30
	};
	PushBack(menus, cannon_menu);

	return menus;
}

public func GetCannonMenuEntries(object clonk)
{
	var menu =
	{
		Bottom = "2em",
		container = { // Necessary to encapsule OnMouseIn/Out from top level
		// That took me a day to figure out %(
			BackgroundColor = {Std = 0, Selected = RGB(100, 30, 30)},
			OnMouseIn = GuiAction_SetTag("Selected"),
			OnMouseOut = GuiAction_SetTag("Std"),
			image = { Right = "3em", Style = GUI_TextBottom | GUI_TextLeft },
			text = { Left = "3em", Style = GUI_TextVCenter | GUI_TextLeft },
		}
	};

	if (!MetalGoalAdded())
	{
		menu.container.text.Text = "$TxtGlory$";
	} else {
		menu.container.text.Text = Format("$TxtCannon$");
		menu.container.image.Symbol = Cannon;
	}

	return [{ symbol = Cannon, extra_data = nil, custom = menu }];
}

public func OnCannonBuildHover(symbol, extra_data, desc_menu_target, menu_id)
{
	var text = "";

	if (!MetalGoalAdded())
	{
		text = "$TxtGlory_Desc$";
	} else {
		if (!MetalGoalDone())
		{
			text = Format("$TxtCannon_Cannot$", 20 - ContentsCount(Metal));
		} else {
			text = Format("$TxtCannon_Can$");
		}
	}

	GuiUpdate({ Text = text }, menu_id, 1, desc_menu_target);
}

public func OnCannonSelected(id symbol, string action, object cursor)
{
	if (!MetalGoalAdded() || !MetalGoalDone())
		return Sound("UI::Error", nil, nil, cursor->GetOwner());

	// Double check!
	if (ContentsCount(Metal) < 20)
		return Sound("UI::Error", nil, nil, cursor->GetOwner());

	StartSequence("Cannon", 1, cursor);
}