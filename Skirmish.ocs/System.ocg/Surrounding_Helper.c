// Do not show important gear in the surrounding tab in the interaction menu

#appendto Helper_Surrounding

private func GetPossibleObjects(id limit_definition)
{
	var sort = nil, check_id = nil;
	if (limit_definition != nil)
	{
		sort = Sort_Distance();
		check_id = Find_ID(limit_definition);
	}
	var items = FindObjects(Find_Distance(Radius), Find_NoContainer(), Find_Property("Collectible"), Find_Layer(GetActionTarget()->GetObjectLayer()), check_id, sort);
	var final_items = CreateArray();
	for (var item in items)
		if (!GetEffect("ImportantGear", item))
			PushBack(final_items, item);
	return final_items;
}