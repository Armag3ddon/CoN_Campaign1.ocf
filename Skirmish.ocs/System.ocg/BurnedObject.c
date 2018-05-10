// Do not burst into ashes because ashes are immune to explosions and will just make the landscape impassable

#appendto BurnedObject

public func BurstIntoAshes()
{
	var particles =
	{
		Prototype = Particles_Dust(),
		R = 50, G = 50, B = 50,
		Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(2, 10), 1000, 0),
	};

	var r = GetR();
	var size = GetCon() * 110 / 100;

	for(var cnt = 0; cnt < 5; ++cnt)
	{
		var distance = 3;
		var x = Sin(r, distance);
		var y = -Cos(r, distance);

		for(var mirror = -1; mirror <= 1; mirror += 2)
		{
			CreateParticle("Dust", x * mirror, y * mirror, PV_Random(-3, 3), PV_Random(-3, -3), PV_Random(18, 1 * 36), particles, 2);
			//CastPXS("Ashes", 1, 30, x * mirror, y * mirror); ### no ashes!
		}
	}
	RemoveObject();
}