// The effect which manages the first battle when the pioneers build the bridge

static const PreFirst_Battle = new Effect {
	Construction = func() {
		this.stage = 0;
		this.wait_time = 0;
	},
	Timer = func() {
		if (this.stage == 0)
		{
			paula->SetToWalkingSpeed();
			paula->PioneerMoveTo(1134, 1101);
			paula->Contents()->ControlUseStart(paula, 0,0);
			paula->Contents()->ControlUseStop(paula, 0,0);
			Dialogue->MessageBoxAll("$CommandPaulaPioneers$", paula, true);
			this.stage++;
			return FX_OK;
		}
		if (this.stage == 1)
		{
			FindObject(Find_ID(PioneerBridge))->Forward();
			this.stage++;
			return FX_OK;
		}
		if (this.stage == 2)
		{
			var clonk = FindObject(Find_InRect(1060, 1060, 60, 60));
			if (!clonk)
				return FX_OK;
			var kutorian = GetEffect("First_Battle", nil).kutorian[0];
			Dialogue->MessageBoxAll("$Kutorian1$", kutorian, true);
			this.stage++;
			return FX_OK;
		}
		if (this.stage == 3)
		{
			this.wait_time++;
			if (this.wait_time < 4)
				return FX_OK;
			if (GetPlayerCount(C4PT_User) > 1)
				Dialogue->MessageBoxAll("$CommandPaulaNewbie_Multi$", paula, true);
			else
				Dialogue->MessageBoxAll("$CommandPaulaNewbie_Single$", paula, true);

			for (var pioneer in g_pioneers)
				pioneer->CreateEffect(NPC_Pioneer_Protection, 1, 1);
			return -1;
		}
	}
};

static const First_Battle = new Effect {
	Construction = func() {
		this.kutorian = CreateArray();
		this.battle_progress = 0;
		this.kutorian_relaunch = true;

		this.current_door = g_door1;
		this.door_spawn = [1585, 1678];

		this.newbridge = false;
		this.maintainbridge = false;
		this.dynamite_cooldown = 0;
		this.dynamite_cooldown_start = 70;
		this.paula_cooldown = 140;
		this.grenade_spawned = false;
		this->CreateArcher();
	},
	Timer = func() {
		if (this.kutorian_relaunch && GetLength(this.kutorian) < this->GetEnemyStrength())
			if (this->RespawnEnemy())
			{
				if (this.battle_progress == 0)
				{
					this->CreateArcher();
				}
				if (this.battle_progress == 2)
				{
					if (!GetLength(this.kutorian))
						this->CreateArcher();
					else if (!Random(4))
						this->CreateBlunderbuss();
					else
						this->CreateArcher();
				}
			}
		for (var kutorian in this.kutorian) // Resupply enemies with arrows and stuff
		{
			if (this.kutorian_relaunch) // Do not refill ammunition if a door is contested (all ranged enemies have an axe to fight with, in that case)
			{
				if (kutorian->FindContents(Bow)) // New arrows
					if (!FindObject(Find_Container(kutorian), Find_Func("IsArrow")))
					{
						if (Random(10))
							kutorian->CreateContents(Arrow);
						else
							kutorian->CreateContents(FireArrow);
					}
				if (kutorian->FindContents(Blunderbuss)) // New bullets
					if (!FindObject(Find_Container(kutorian), Find_Func("IsBullet")))
						kutorian->CreateContents(LeadBullet);
				if (kutorian->FindContents(Blunderbuss)) // New iron bombs!
				{
					kutorian.battle_grenade_cooldown--; // it takes some times to respawn an iron bomb, because OP
					if (kutorian.battle_grenade_cooldown <= 0)
					{
						kutorian->CreateContents(IronBomb);
						kutorian.battle_grenade_cooldown = 140;
					}
				}
			}
			if (kutorian->Contained()) // Check if clonk is stuck in door - it happens sometimes?!
				if (!kutorian->GetCommand())
					kutorian->Exit(0, 5);
		}
		if (this.battle_progress == 1) // First bridge done, check if position is taken
		{
			if (!GetLength(this.kutorian)) // Taken!
			{
				this->ProgressBattle();
			} else {
				for (var kutorian in this.kutorian) // Kill clonks that have fallen down under the bridge
					if (Inside(kutorian->GetX(), 1200, 1560))
						if (kutorian->GetY() > 1175)
							if (kutorian->GetAlive())
								kutorian->Kill(); // X(
			}
		}
		if (this.newbridge) // Look for pioneers arriving at the rally point, ready to create a bridge
		{
			var found = CreateArray();
			for (var pioneer in g_pioneers)
			{
				if (pioneer == nil)
					continue;
				if (pioneer->PioneerIsMoving())
					continue;
				if (!pioneer->GetAlive())
					continue;
				if (GetEffect("NPC_Pioneer_DynamiteThrowing", pioneer))
					continue;
				if (Distance(pioneer->GetX(), pioneer->GetY(), g_pioneer_rally_point[0], g_pioneer_rally_point[1]) <= 10)
					PushBack(found, pioneer);
			}
			if (GetLength(found) >= 2)
			{
				var bridge = CreateObject(PioneerBridge, g_pioneer_rally_point[0], g_pioneer_rally_point[1], REPUBLIC);
				bridge->LiftUp(found[0]);
				bridge->LiftUp(found[1]);
				bridge->Sound("Structures::Repair");
				this.newbridge = false;
			}
		}
		if (this.maintainbridge) // Assign new pioneers to carry the bridge, former carrier was lost
		{
			var bridge = FindObject(Find_ID(PioneerBridge));
			if (bridge)
			{
				if (bridge->GetCarrierCount() < 2)
				{
					for (var pioneer in g_pioneers)
					{
						if (pioneer == nil)
							continue;
						if (pioneer->PioneerIsMoving())
							continue;
						if (!pioneer->GetAlive())
							continue;
						if (GetEffect("Lifting", pioneer))
							continue;
						if (GetEffect("NPC_Pioneer_DynamiteThrowing", pioneer))
							continue;

						bridge->LiftUp(pioneer);
						break;
					}
				} else {
					this.maintainbridge = false;
				}
			} else {
				this.maintainbridge = false;
				if (!this.newbridge)
					this.newbridge = true;
			}
		}
		// Manage defending pioneers
		var bridge = FindObject(Find_ID(PioneerBridge));
		if (bridge)
		{
			for (var pioneer in g_pioneers)
			{
				if (pioneer == nil)
					continue;
				if (pioneer->PioneerIsMoving())
					continue;
				if (!pioneer->GetAlive())
					continue;
				if (GetEffect("Lifting", pioneer))
					continue;
				if (GetEffect("NPC_Pioneer_DynamiteThrowing", pioneer))
					continue;

				if (bridge->GetCarrierCount() < 2)
				{
					var carrier = bridge->GetCarrier();
					if (carrier)
						this->PioneerDefendingMove(pioneer, carrier);
					else
						continue;
				}

				var nearest_carrier = 0;
				if (pioneer->ObjectDistance(bridge.carriers[0]) > pioneer->ObjectDistance(bridge.carriers[1]))
					nearest_carrier = 1;
				
				// There is a higher chance that a pioneer stays with the nearest carrier
				if (Random(3))
				{
					this->PioneerDefendingMove(pioneer, bridge.carriers[nearest_carrier]);
				} else {
					this->PioneerDefendingMove(pioneer, bridge.carriers[(nearest_carrier - 1) * -1]);
				}
			}
		}
		if (this.dynamite_cooldown > 0)
		{
			this.dynamite_cooldown--;
			GUI_Controller->UpdateDynamiteCooldown(this->GetDynamiteReadiness());
		}
		if (this.paula_cooldown > 0)
		{
			if (!paula->PioneerIsMoving())
				this.paula_cooldown--;
		} else {
			if (this.paula_cooldown == -2)
			{
				var blunderbuss = paula->FindContents(Blunderbuss);
				if (!blunderbuss->FindContents(LeadBullet))
					blunderbuss->CreateContents(LeadBullet);
				blunderbuss->ControlUseStart(paula, 0, 0);
				blunderbuss->ControlUseStop(paula, 0, 0);
				this.paula_cooldown = 141;
			}
			if (this.paula_cooldown == -1)
			{
				var blunderbuss = paula->FindContents(Blunderbuss);
				blunderbuss->ControlUseStop(paula, this.current_door->GetX() - paula->GetX(), this.current_door->GetY() - paula->GetY());
			}
			if (this.paula_cooldown == 0)
			{
				var blunderbuss = paula->FindContents(Blunderbuss);
				blunderbuss->ControlUseStart(paula, this.current_door->GetX() - paula->GetX(), this.current_door->GetY() - paula->GetY());
				blunderbuss->ControlUseHolding(paula, this.current_door->GetX() - paula->GetX(), this.current_door->GetY() - paula->GetY());
			}
			this.paula_cooldown--;
		}
	},
	CreateClonk = func() {
		var g = true;
		if (Random(2))
			g = false;
		var clonk = CreateKutorianClonk(this.current_door->GetX(), this.current_door->GetY(), DIR_Left, KUTOR, "$SoldierName$", g, false, false);
		clonk->Enter(this.current_door);
		AI->AddAI(clonk);
		AI->SetHome(clonk, RandomX(this.door_spawn[0], this.door_spawn[1]), this.current_door->GetY(), DIR_Left);
		AI->SetGuardRange(clonk, this.current_door->GetX() - 500, this.current_door->GetY() - 150, 650, 300);
		clonk->SetCommand("Exit");
		PushBack(this.kutorian, clonk);
		return clonk;
	},
	CreateArcher = func() {
		var clonk = this->CreateClonk();
		clonk->CreateContents(Bow)->CreateContents(Arrow);
		clonk->CreateContents(Axe);
	},
	CreateBlunderbuss = func() {
		var clonk = this->CreateClonk();
		clonk->CreateContents(Blunderbuss)->CreateContents(LeadBullet);
		clonk->CreateContents(Axe);
	},
	CreateSwordsman = func() {
		var clonk = this->CreateClonk();
		clonk->CreateContents(Sword);
		clonk->CreateContents(Shield);
	},
	CreateGrenadier = func() {
		var clonk = this->CreateClonk();
		clonk->CreateContents(GrenadeLauncher)->CreateContents(IronBomb);
		clonk->CreateContents(Sword);
		clonk.battle_grenade_cooldown = 140;
		CreateObject(EnemyMarker, 0,0, KUTOR)->Set(clonk, "Grenade");
	},
	PioneerDefendingMove = func(object pioneer, object carrier) {
		// Pick a random spot around the carrier. Just make sure, it's not just above an abyss
		var x = carrier->GetX() + RandomX(-30, 30);
		var y = carrier->GetY();
		var tries = 0;
		while (PathFree(x, y, x, y + 30))
		{
			tries++;
			if (tries > 5)
				break;
			var x = carrier->GetX() + RandomX(-30, 30);
		}
		if (tries > 5) // better do nothing
			return;
		pioneer->PioneerMoveTo(x, y);
	},
	DynamiteCooldown = func(int cooldown_start) {
		this.dynamite_cooldown = cooldown_start;
		this.dynamite_cooldown_start = cooldown_start;
	},
	OnClonkDeath = func(object clonk) {
		if (IsValueInArray(this.kutorian, clonk))
		{
			while(clonk->Contents())
				clonk->Contents()->RemoveObject();
			RemoveArrayValue(this.kutorian, clonk);
			RemoveHoles(this.kutorian);
		}
	},
	OnBridgeCreated = func() {
		if (this.battle_progress == 0)
		{
			if (this.grenade_spawned)
				return;
			if (GetLength(g_bridges) == 4) // The first time the fourth bridge is placed, spawn a grenadier!
			{
				this.grenade_spawned = true;
				this->CreateGrenadier();
			}
		}
	},
	ProgressBattle = func() {
		this.battle_progress++;
		if (this.battle_progress == 1) // First bridge done
		{
			this.kutorian_relaunch = false;
			var warriors = 3 + GetPlayerCount(C4PT_User)/2;
			for (var i = 0; i < warriors; i++)
				this->CreateSwordsman();
			for (var bridge in g_bridges)
				bridge->MakeInvincible();
			Dialogue->MessageBoxAll("$CommandPaulaCloseCombat$", paula, true);
			for (var pioneer in g_pioneers)
			{
				if (!pioneer)
					continue;
				AI->AddAI(pioneer);
				AI->SetHome(pioneer, this.current_door->GetX(), this.current_door->GetY(), DIR_Right);
				AI->SetGuardRange(pioneer, this.current_door->GetX() - 500, this.current_door->GetY() - 150, 650, 300);
			}
		}
		if (this.battle_progress == 2) // First door taken
		{
			this.kutorian_relaunch = true;
			Dialogue->MessageBoxAll("$CommandPaulaGoodJob$", paula, true);
			for (var pioneer in g_pioneers)
				if (pioneer)
					AI->RemoveAI(pioneer);
			// Close the door
			this.current_door->SetColor(RGB(255, 0, 0));
			this.current_door->SetKey(this.current_door->CreateContents(Key));
			// Open up next door
			this.current_door = g_door2;
			this.door_spawn = [1965, 2095];
			// Start a new bridge
			g_pioneer_last_bridge = CreateObject(Dummy, 1652, 1173, REPUBLIC);
			g_bridges = [g_pioneer_last_bridge];
			SetRallyPoint(1615, 1140);
			for (var pioneer in g_pioneers)
				if (pioneer)
					pioneer->PioneerMoveTo(g_pioneer_rally_point[0], g_pioneer_rally_point[1]);
			this.newbridge = true;
			// Paula moves ahead
			if (this.paula_cooldown < 0)
			{
				var blunderbuss = paula->FindContents(Blunderbuss);
				blunderbuss->ControlUseCancel(paula, 0,0);
				if (!blunderbuss->FindContents(LeadBullet))
					blunderbuss->CreateContents(LeadBullet);
				blunderbuss->SetLoaded();
				this.paula_cooldown = 140;
			}
			paula->SetToRunningSpeed();
			paula->PioneerMoveTo(1659, 1139);
		}
		if (this.battle_progress == 3) // Second bridge done
		{
			Dialogue->MessageBoxAll("$CommandPaulaGoodJob$", paula, true);
		}
	},
	GetDynamiteReadiness = func() {
		return this.dynamite_cooldown * 100 / this.dynamite_cooldown_start;
	},
	GetGrenadier = func() {
		for (var kutorian in this.kutorian)
			if (kutorian->FindContents(GrenadeLauncher))
				return true;
		return false;
	},
	GetEnemyStrength = func() {
		// Take into account the heat of the battle and the amount of players
		if (this.battle_progress == 0)
		{
			return BoundBy(1 + GetPlayerCount(C4PT_User)/2, 2, 4);
		}
		if (this.battle_progress == 2)
		{
			return BoundBy(1 + GetPlayerCount(C4PT_User), 2, 8);
		}
	},
	RespawnEnemy = func() {
		if (!GetLength(this.kutorian)) // Very high chance if no enemies left
			return !Random(3);
		if (this.battle_progress == 0)
		{
			return !Random(50);
		}
		if (this.battle_progress == 2)
		{
			return !Random(25);
		}
	}
};