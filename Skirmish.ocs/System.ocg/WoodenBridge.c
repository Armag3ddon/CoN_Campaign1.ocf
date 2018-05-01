#appendto WoodenBridge

local shakes = 0;

func Destruction()
{
	Campaign_Debris->CreateDebris("BridgeLeft",  GetX() - 15, GetY(), -1, 0, -10, 0, 70)->SetCon(130);
	Campaign_Debris->CreateDebris("BridgeRight", GetX() + 15, GetY(), +1, 0, +10, 0, 70)->SetCon(130);
	Sound("Environment::Tree::Crack");

	GameCall("WoodenBridgeDestroyed", this);
}

public func ShakeToDestruction()
{
	AddTimer("Shakey", 2);
}

func Shakey()
{
	SetPosition(GetX() + RandomX(-1, 1), GetY() + RandomX(-1, 1));
	shakes++;

	if (!Random(3))
		Sound("Hits::Materials::Wood::WoodCreak?");

	if (shakes > 25)
		RemoveObject();
}