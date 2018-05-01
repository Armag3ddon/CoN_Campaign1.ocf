// Controls the different times the scenario is playing in

static g_the_time;

// Time is either:
// 0 - afternoon
// 1 - evening
// 2 - night
global func SetTimeOfDay(int time)
{
	if (time == 0)
	{
		// Desurate the sky
		SetSkyAdjust(RGBa(255,255,255,200));
		// Set the time for the pocket watch
		g_the_time = 14 * 60 + 45;
		// Bright day
		SetGamma(100,100,100, 3);
	}
	if (time == 1)
	{
		// Make the sky a bit red
		SetSkyAdjust(RGBa(170,100,100,220));
		g_the_time = 20 * 60 + 23;
		SetGamma(78, 70, 70, 3);
	}
	if (time == 2)
	{
		// Night time!
		SetSkyAdjust(RGBa(35,35,35,255));
		g_the_time = 0 * 60 + 37;
		SetGamma(55,60,65, 3);
	}
}