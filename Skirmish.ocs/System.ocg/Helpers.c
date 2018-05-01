// Returns the player's current rank name

global func GetNewbiesRank()
{
	if (g_player_rank == 0) return "$PlayerRankFirst$";
	if (g_player_rank == 1) return "$PlayerRankSecond$";
	if (g_player_rank == 2) return "$PlayerRankThird$";
	return "";
}

// Constant shelling effects for the bunkers

global func CreateDustRain(int x, int y)
{
	// Dust rain is 21 pixels wide
	for (var i = -10; i < 11; i++)
	{
		// Determine ceiling height
		var ceiling_y = y;
		while (!GBackSolid(x + i, ceiling_y) && y - ceiling_y < 20)
			ceiling_y--;
		if (y - ceiling_y >= 20)
			continue;
		while (GBackSolid(x + i, ceiling_y + 1) && ceiling_y - y < 20)
			ceiling_y++;
		if (ceiling_y - y >= 20)
			continue;

		var color = GetAverageTextureColor(GetTexture(x + i, ceiling_y));

		var particle =
		{
			Prototype = Particles_DustRain(),
			R = (color >> 16) & 0xff,
			G = (color >> 8) & 0xff,
			B = color & 0xff,
			ForceY = PV_Gravity(50 + Cos(i * 9, 100) + Random(20))
		};

		CreateParticle("Dust", x+i, ceiling_y+1, 0, 0, 70, particle, 1);
		CreateParticle("Dust", x+i, ceiling_y+1, 0, 5, 70, particle, 1);
		CreateParticle("Dust", x+i, ceiling_y+1, 0, 10, 70, particle, 1);
	}

	var dummy = CreateObject(Dummy, x, y, NO_OWNER);
	dummy.Visibility = VIS_All;
	dummy->Sound("DustRain", false, 100, nil, nil, 250);
	Schedule(dummy, "RemoveObject()", 150);
}

global func Particles_DustRain()
{
	return
	{
		Size = PV_Random(1, 4),
		Phase = 1,
		Rotation = PV_Random(0, 360),
		ForceX = PV_Wind(20),
		CollisionVertex = 0,
		OnCollision = PC_Die()
	};
}