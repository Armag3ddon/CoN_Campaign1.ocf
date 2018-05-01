// The trees of the dark forest are very resistant to fire

static const TREES_FIRE_RES = 150;

static const DarkForest_Tree = {
	Construction = func() {
		this.fire_buffer = 0;
		this.temp_buffer = 0;
	},
	Damage = func(int damage, int cause) {
		if (cause == FX_Call_DmgFire || cause == FX_Call_DmgBlast) {
			if (this.fire_buffer >= TREES_FIRE_RES) {
				if (this.Target->GetDamage() + damage > this.Target->MaxDamage())
					this->Respawn();
				return damage;
			}

			// put damage in an internal buffer before actually damaging the tree
			this.fire_buffer += damage;
			this.temp_buffer += damage;

			// extinguish the tree
			if (this.temp_buffer > TREES_FIRE_RES / 3) {
				this.temp_buffer = 0;
				this.Target->Extinguish();
			}
			return 0;
		}
		if (cause == FX_Call_DmgChop) {
			if (this.Target->GetDamage() + damage > this.Target->MaxDamage())
				this->Respawn();
		}

		return damage;
	},
	Respawn = func() {
		if (this.spawned)
			return;
		var new_tree = this.Target->CreateObjectAbove(this.Target->GetID(), 0, this.Target->GetObjHeight() / 2, NO_OWNER);
		new_tree->SetCon(2);
		new_tree->StartGrowth(10, RandomX(900, 1050));
		new_tree->CreateEffect(DarkForest_Tree, 1);
		this.spawned = true;
	}
};