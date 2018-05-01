// When doing the interaction menu opening callback also deliver 'this'
// So chests can easily close the menu

#appendto GUI_ObjectInteractionMenu

private func DoInteractionMenuOpenedCallback(object obj)
{
	if (!obj) return;
	if (obj._open_interaction_menus == nil)
		obj._open_interaction_menus = 0;
	
	obj._open_interaction_menus += 1;
	
	var is_first = obj._open_interaction_menus == 1;
	obj->~OnShownInInteractionMenuStart(is_first, this);
}