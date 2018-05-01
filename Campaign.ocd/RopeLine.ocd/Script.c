/*-- Decorative rope --*/

global func ConnectWithRope(object o1, object o2, array line_attach1, array line_attach2)
{
	if (line_attach1 == nil)
		line_attach1 = [0,0];
	if (line_attach2 == nil)
		line_attach2 = [0,0];

	o1.LineAttach = line_attach1;
	o2.LineAttach = line_attach2;

	var rope = o1->CreateObject(CampaignRopeLine, 0,0);
	rope->SetConnectedObjects(o1, o2);
	return rope;
}

func Initialize()
{
	SetAction("Connect");
	SetVertexXY(0, GetX(), GetY());
	SetVertexXY(1, GetX(), GetY());
	SetProperty("LineColors", [RGB(242, 152, 51), RGB(242, 152, 51)]);
}

public func IsConnectedTo(object obj)
{
	return GetActionTarget(0) == obj || GetActionTarget(1) == obj;
}

public func SetConnectedObjects(obj1, obj2)
{
	SetVertexXY(0, obj1->GetX(), obj1->GetY());
	SetVertexXY(1, obj2->GetX(), obj2->GetY());
	SetActionTargets(obj1, obj2);
}

public func SetConnectedObjectsLineAttach(attach1, attach2)
{
	GetActionTarget(0).LineAttach = attach1;
	GetActionTarget(1).LineAttach = attach2;
}

public func GetConnectedObject(object obj)
{
	if (GetActionTarget(0) == obj)
		return GetActionTarget(1);
	if (GetActionTarget(1) == obj)
		return GetActionTarget(0);
}

public func IsConnecting()
{
	return GetActionTarget(0) && GetActionTarget(1);
}

/* Breaking */

func LineBreak(bool no_msg)
{
	Sound("Objects::Connect");
	if (!no_msg)
		BreakMessage();
}

func BreakMessage()
{
	var line_end = GetActionTarget(0);
	if (line_end->Contained()) line_end = line_end->Contained();

	line_end->Message("$TxtLinebroke$");
}

/* Saving */

public func SaveScenarioObject(props)
{
	if (!inherited(props, ...)) return false;
	if (!IsConnecting()) return false;

	props->AddCall("LineAttach", this, "SetConnectedObjectsLineAttach", GetActionTarget(0).LineAttach, GetActionTarget(1).LineAttach);
	props->AddCall("Connection", this, "SetConnectedObjects", GetActionTarget(0), GetActionTarget(1));
	return true;
}

local ActMap = {
	Connect = {
		Prototype = Action,
		Name = "Connect",
		Procedure = DFA_CONNECT,
		NextAction = "Connect"
	}
};

local Name = "$Name$";