/**
	Wooden sign
	Not entirely for decoration

	Graphics by Foaly (CC-BY 3.0)
*/

local text = 0;

func Construction()
{
	SetProperty("MeshTransformation", Trans_Mul(Trans_Rotate(90, 0, 1, 0), Trans_Scale(1300)));
}

public func IsInteractable(object clonk)
{
	return true;
}

public func GetInteractionMetaInfo(object clonk)
{
	return { Description = "$MsgRead$", IconName = nil, IconID = nil, Selected = false };
}

public func Interact(object clonk)
{
	clonk->PlayerMessage(clonk->GetOwner(), GetText());
}

public func SaveScenarioObject(proplist props, ...)
{
	if (!_inherited(props, ...)) return false;
	if (text != 0)
		props->AddCall("Text", this, "SetText", text);
	return true;
}

/*-- Messages --*/

public func SetText(int txt)
{
	text = txt;
}

public func GetText()
{
	if (text == 1)
		return "$Msg1$";
	if (text == 2)
		return "$Msg2$";
	if (text == 3)
		return "$Msg3$";
	if (text == 4)
		return "$Msg4$";
	return "";
}

/*-- Display --*/

func Definition(def) {
	SetProperty("PictureTransformation", Trans_Mul(Trans_Rotate(75, 0, 1, 0), Trans_Scale(1300)), def);
}

/*-- Properties --*/

local Name = "$Name$";