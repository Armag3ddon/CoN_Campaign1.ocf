// Constant boompack attacks pounding at the tower

static const BarrageEffect = {
	Timer = func() {
		if (!Random(2)) return;

		var boompack = CreateObject(Boompack, 9, (58 + Random(28))*8, 10);
		boompack->SetFuel(999999);
		boompack->SetDirectionDeviation(0);
		boompack->Launch(67);
	}
};