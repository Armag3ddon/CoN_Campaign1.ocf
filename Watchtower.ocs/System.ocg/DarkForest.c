/** Dark Forest

	Special stuff needed for the forest
**/

// Creation
// The original forest creation function doesn't create plants before tunnel background
global func PlaceDarkForest(array plants, int x, int y, int width, bool foreground)
{
	// Parameter check
	if (GetLength(plants) == 0) return;
	if (!x) x = 0;
	if (!y) y = LandscapeHeight();
	if (!width) width = LandscapeWidth();
	if (this) { x = AbsX(x); y = AbsY(y); }

	// Roughly 20% of the size (10% per side) are taken for 'forest ending zones'. Plants will be smaller there.
	var end_zone = width * 10 / 100;
	// The width of the standard plants will roughly be the measure for our plant size
	var plant_size = plants[0]->GetDefWidth()/2;

	var growth, y_pos, plant, x_variance, variance = 0, count, j, spot;
	for (var i = plant_size; i < width; i += plant_size)
	{
		growth = 100;
		y_pos = y;
		x_variance = RandomX(-plant_size/2, plant_size/2);
		// End zone check
		if (i < end_zone)
			growth = BoundBy(90 / ((end_zone * 100 / plant_size)/100) * (i/plant_size), 10, 90);
		else if (i > width - end_zone)
			growth = BoundBy(90 / ((end_zone * 100 / plant_size)/100) * ((width-i)/plant_size), 10, 90);
		else if (!Random(10) && GetLength(plants) > 1)
		{
			variance = Random(GetLength(plants)-1)+1;
			// Scatter some other plants
			count = RandomX(2, 4);
			for (j = 0; j < count; j++)
			{
				spot = (plant_size*2 / count) * j + RandomX(-5,5) - plant_size;
				y_pos = y;
				if (!GBackSolid(x + i + spot, y_pos)) continue;
				while (!GBackSky(x + i + spot, y_pos) && y_pos > 0) y_pos--;
				if (y_pos == 0) continue;
				plant = CreateObjectAbove(plants[variance], x + i + spot, y_pos+5, NO_OWNER);
				plant->CreateEffect(DarkForest_Tree, 1);
			}
			continue;
		}
		// No ground at y_pos?
		if (!GBackSolid(x + i + x_variance, y_pos)) continue;
		// Get level ground
		while (GBackSemiSolid(x + i + x_variance, y_pos) && y_pos > 0) y_pos--;
		if (y_pos == 0) continue;
		if (GBackLiquid(x + i + x_variance, y_pos)) continue;

		plant = CreateObjectAbove(plants[0], x + i + x_variance, y_pos+5, NO_OWNER);
		plant->SetCon(growth);
		plant->CreateEffect(DarkForest_Tree, 1);
		if (foreground && !Random(3)) plant.Plane = 510;
		// Every ~7th plant: double plant!
		if (x_variance != 0 && !Random(7))
		{
			y_pos = y;
			if (!GBackSolid(x + i - x_variance, y_pos)) continue;
			while (!GBackSky(x + i - x_variance, y_pos) && y_pos > 0) y_pos--;
			if (y_pos == 0) continue;
			plant = CreateObjectAbove(plants[0], x + i - x_variance, y_pos+5, NO_OWNER);
			plant->SetCon(growth);
			plant->CreateEffect(DarkForest_Tree, 1);
			if (foreground && !Random(3)) plant.Plane = 510;
		}
	}
}