/**
	Dark Forest

	Gradually reduce the view range of players entering the forest.
*/

local Name = "$Name$";
local Description = "$Description$";

private func Initialize() {
	AddTimer("CheckEnterForest", 1);
}

private func CheckEnterForest() {
	for (var clonk in FindObjects(Find_ID(Clonk), Find_InRect(AbsX(1650),AbsY(0), 1000,LandscapeHeight())))
		if (!GetEffect("DarkForest", clonk))
			AddEffect("DarkForest", clonk, 1, 1, this);
}

private func FxDarkForestStart(target, effect, temp) {
	if (temp) return;
	
}

private func FxDarkForestTimer(target) {
	var x = target->GetX();
	if (x < 1650) return -1;

	if (!g_bunker_spotted && x >= 3100) {
		if (Inside(target->GetY(), 0, 900)) {
			g_bunker_spotted = true;
		}
	}

	x -= 1650;
	var light = 300 - x;
	if (light < 0) light = 0;
	target->SetLightRange(light, light/4);
}

private func FxDarkForestStop(target, effect, reason, temp) {
	if (temp) return;
	target->SetLightRange(300,80);
}