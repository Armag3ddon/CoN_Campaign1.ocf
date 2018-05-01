// Check the armory for metal

static const Metal_GoalEffect = {
	Timer = func () {
		if (this.metal_count == nil || g_armory->ContentsCount(Metal) != this.metal_count) {
			this.metal_count = g_armory->ContentsCount(Metal);
			UpdateMetalGoal(BoundBy(this.metal_count, 0, 20));
		}
	}
};