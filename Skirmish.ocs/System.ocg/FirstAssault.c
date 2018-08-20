// The effects which manage the first battle when the pioneers build the bridge

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
				if (this.battle_progress == 3)
				{
					this->CreateFinalStageClonk();
				}
			}
		for (var kutorian in this.kutorian) // Resupply enemies with arrows and stuff
		{
			if (this.kutorian_relaunch) // Do not refill ammunition if a door is contested (all ranged enemies have an axe to fight with, in that case)
			{
				if (kutorian->FindContents(Bow)) // New arrows
					if (!FindObject(Find_Container(kutorian), Find_Func("IsArrow")))
					{
						if (Random(BoundBy(11 - GetPlayerCount(C4PT_User), 3, 11)))
							kutorian->CreateContents(Arrow);
						else
							kutorian->CreateContents(FireArrow);
					}
				if (kutorian->FindContents(Blunderbuss)) // New bullets
					if (!FindObject(Find_Container(kutorian), Find_Func("IsBullet")))
						kutorian->CreateContents(LeadBullet);
				if (kutorian->FindContents(GrenadeLauncher)) // New iron bombs!
				{
					kutorian.battle_grenade_cooldown--; // it takes some time to respawn an iron bomb, because OP
					if (kutorian.battle_grenade_cooldown <= 0)
					{
						kutorian->CreateContents(IronBomb);
						kutorian->GetAI().strategy = AI.ExecuteRanged;
						kutorian->GetAI().weapon = kutorian->FindContents(GrenadeLauncher);
						kutorian->GetAI().ranged = true;
						kutorian->GetAI().ammo_check = AI.HasBombs;
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
				for (var pioneer in g_pioneers) // Check for newly spawn pioneers, add AI
					if (pioneer && !pioneer->PioneerIsMoving())
						if (!pioneer->~GetAI())
						{
							AI->AddAI(pioneer);
							AI->SetHome(pioneer, this.current_door->GetX(), this.current_door->GetY(), DIR_Right);
							AI->SetGuardRange(pioneer, this.current_door->GetX() - 500, this.current_door->GetY() - 150, 650, 300);
						}
			}
		}
		if (this.battle_progress == 3) // Second bridge done, check if prisoners are freed
		{
			if (!this.freed)
			{
				if (!this.final_key && !this.key_spawned)
				{
					// Close the prison door, as soon as all new enemies are out
					var enemy_in = false;
					for (var i = 0; i < this.current_door->ContentsCount(); i++)
						if (IsValueInArray(this.kutorian, this.current_door->Contents(i)))
							enemy_in = true;
					if (!enemy_in)
					{
						while(this.current_door->Contents())
							this.current_door->Contents()->Exit();
						this.final_key = this.current_door->CreateContents(Key);
						this.current_door->SetKey(this.final_key);
						this.key_spawned = true;
					}
				}
				if (!this.final_key && this.key_spawned)
				{
					// The door has been opened!
					this->FreePrisoners();
				}

				// Get the player clonk most advanced forward
				var advanced = this->GetAdvancedClonk();
				for (var pioneer in g_pioneers)
				{
					if (!pioneer)
						continue;
					if (pioneer->PioneerIsMoving())
						continue;
					if (!pioneer->GetAlive())
						continue;
					if (!pioneer->~GetAI())
					{
						AI->AddAI(pioneer);
						AI->SetHome(pioneer, advanced->GetX(), advanced->GetY(), DIR_Right);
						AI->SetGuardRange(pioneer, this.current_door->GetX() - 600, this.current_door->GetY() - 150, 750, 300);
					} else if (pioneer->ObjectDistance(advanced) > 150)
					{
						AI->SetHome(pioneer, advanced->GetX(), advanced->GetY(), DIR_Right);
					}
				}
			} else {
				// Time to get back!
				for (var pioneer in g_pioneers)
				{
					if (!pioneer)
						continue;
					if (pioneer->PioneerIsMoving())
						continue;
					if (!pioneer->GetAlive())
						continue;
					// Remove AI
					if (pioneer->~GetAI())
					{
						AI->RemoveAI(pioneer);
						pioneer->PioneerMoveTo(1124 + RandomX(-20, 0), 1101);
					}
				}
				if (!this.paula_retreated && !paula->PioneerIsMoving())
				{
					var advanced = this->GetAdvancedClonk();
					if (advanced->GetY() <= paula->GetY())
					{
						// Paula moves back
						if (this.paula_cooldown < 0)
						{
							var blunderbuss = paula->FindContents(Blunderbuss);
							blunderbuss->ControlUseCancel(paula, 0,0);
						}
						paula->PioneerMoveTo(1124 + RandomX(-20, 0), 1101);
						this.paula_cooldown = -4; // Deactivate Paula's shooting
						this.paula_retreated = true;
					}
				} else if (this.paula_retreated && !paula->PioneerIsMoving())
				{
					if (!this.pilots_arrived)
					{
						// Check for the battle to end
						var pilots_moving = false;
						for (var pilot in g_pilots)
							if (pilot && pilot->PioneerIsMoving())
								pilots_moving = true;
						if (!pilots_moving)
							this.pilots_arrived = true;
					} else if (!this.waiting_clonk) {
						var player_clonk = FindObject(Find_Allied(REPUBLIC), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_Not(Find_Owner(REPUBLIC)), Sort_Distance(paula->GetX(), paula->GetY()));
						if (player_clonk && paula->ObjectDistance(player_clonk) < 100)
						{
							this.waiting_clonk = player_clonk;
							paulas_dlg->CheckForBattleFinished(player_clonk);
						}
					} else {
						if (!GetEffect("MultiplayerWaitArea", paulas_dlg))
							this.waiting_clonk = nil;
					}
				}
				while (this.current_door->Contents())
					this.current_door->Contents()->Exit();
			}
		}
		// Bridge building parts
		if (this.battle_progress == 0 || this.battle_progress == 2)
		{
			// Look for pioneers arriving at the rally point, ready to create a bridge
			if (this.newbridge)
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
			var bridge = FindObject(Find_ID(PioneerBridge));
			if (bridge)
			{
				// Assign new pioneers to carry the bridge if former carrier was lost
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
				}
				// Manage defending pioneers
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
			} else if (!this.newbridge)
					this.newbridge = true;
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

				// In the end, Paula will give hints about what to do
				if (this.battle_progress == 3 && !this.freed)
				{
					if (this.keeper)
						Dialogue->MessageBoxAll("$CommandPaulaKillKeeper$", paula, true);
					else if (this.final_key && !this.final_key->Contained())
						Dialogue->MessageBoxAll("$CommandPaulaTakeKey$", paula, true);
					else if (this.final_key && this.final_key->Contained())
						Dialogue->MessageBoxAll("$CommandPaulaOpenDoor$", paula, true);
				}
			}
			if (this.paula_cooldown == -1)
			{
				var x = this.current_door->GetX() - paula->GetX();
				var y = this.current_door->GetY() - paula->GetY();
				if (this.battle_progress == 3)
				{
					x = 2320 - paula->GetX();
					y = 1090 - paula->GetY();
				}
				var blunderbuss = paula->FindContents(Blunderbuss);
				blunderbuss->ControlUseStop(paula, x, y);
			}
			if (this.paula_cooldown == 0)
			{
				var x = this.current_door->GetX() - paula->GetX();
				var y = this.current_door->GetY() - paula->GetY();
				if (this.battle_progress == 3)
				{
					x = 2320 - paula->GetX();
					y = 1090 - paula->GetY();
				}
				var blunderbuss = paula->FindContents(Blunderbuss);
				blunderbuss->ControlUseStart(paula, x, y);
				blunderbuss->ControlUseHolding(paula, x, y);
			}
			if (this.paula_cooldown > -10)
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
	CreateFinalStageClonk = func() {
		var g = true;
		if (Random(2))
			g = false;
		var clonk = CreateKutorianClonk(2453, 761, DIR_Left, KUTOR, "$SoldierName$", g, false, false);
		if (Random(3))
		{
			clonk->CreateContents(Bow)->CreateContents(Arrow);
			clonk->CreateContents(Axe);
		} else {
			clonk->CreateContents(Blunderbuss)->CreateContents(LeadBullet);
			clonk->CreateContents(Axe);
		}
		PushBack(this.kutorian, clonk);
		clonk->CreateEffect(NPC_Kutor_LastDefender, 1, 1);
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
	CreateDungeonKeeper = func() {
		var clonk = this->CreateClonk();
		clonk->CreateContents(Sword);
		clonk->CreateContents(Shield);
		clonk->CreateContents(Helmet)->ControlUse(clonk);
		CreateObject(EnemyMarker, 0,0, KUTOR)->Set(clonk, "Key");
		// This one we need to save
		this.keeper = clonk;
	},
	PioneerDefendingMove = func(object pioneer, object carrier) {
		if (!carrier)
			return;
		if (!pioneer)
			return;
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
	ClearArrows = func() {
		var arrows = FindObjects(Find_ID(Arrow), Find_InRect(1120, 980, 1000, 350), Find_NoContainer());
		for (var arrow in arrows)
			if (!arrow->OnFire())
				arrow->FadeOut(100, true);
	},
	FreePrisoners = func() {
		this.freed = true;
		g_pilots = [];

		for (var i = 0; i < 5; i++)
		{
			var g = true;
			if (Random(2))
				g = false;
			var clonk = CreateRepublicanClonk(this.current_door->GetX(), this.current_door->GetY(), DIR_Left, REPUBLIC, "$PilotName$", g, false, false);
			clonk.MaxEnergy = 300000;
			clonk->DoEnergy(3000);
			Schedule(clonk, "PioneerMoveTo(1124 + RandomX(-20, 0), 1101)", 15 + i * 20);
			PushBack(g_pilots, clonk);
		}

		Dialogue->MessageBoxAll("$CommandPaulaSafety$", paula, true);
	},
	OnClonkDeath = func(object clonk) {
		if (IsValueInArray(this.kutorian, clonk))
		{
			while(clonk->Contents())
				clonk->Contents()->RemoveObject();
			RemoveArrayValue(this.kutorian, clonk);
			RemoveHoles(this.kutorian);

			// The dungeon keeper was killed, spawn the key
			if (this.keeper == clonk)
			{
				if (this.final_key)
				{
					this.final_key->Exit();
					this.final_key->SetPosition(clonk->GetX(), clonk->GetY());
				} else { // The player was very quick and killed the dungeon keeper before the key could be created
					this->FreePrisoners();
				}
				this.keeper = nil;
			}
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
			this->ClearArrows();
		}
		if (this.battle_progress == 2) // First door taken
		{
			this.kutorian_relaunch = true;
			Dialogue->MessageBoxAll("$CommandPaulaGoodJob$", paula, true);
			for (var pioneer in g_pioneers)
				if (pioneer)
					AI->RemoveAI(pioneer);
			// Close the door
			while(this.current_door->Contents())
				this.current_door->Contents()->Exit();
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
			Dialogue->MessageBoxAll("$CommandPaulaFreePrisoners$", paula, true);
			this->ClearArrows();
			for (var bridge in g_bridges)
				bridge->MakeInvincible();
			// Close the door
			while(this.current_door->Contents())
				this.current_door->Contents()->Exit();
			this.current_door->SetColor(RGB(255, 0, 0));
			this.current_door->SetKey(this.current_door->CreateContents(Key));
			// Set up the final door
			this.current_door = g_door3;
			this.door_spawn = [2455, 2520];
			// Pioneers move ahead
			SetRallyPoint(1990, 1148);
			for (var pioneer in g_pioneers)
				if (pioneer)
					pioneer->PioneerMoveTo(g_pioneer_rally_point[0], g_pioneer_rally_point[1]);
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
			paula->PioneerMoveTo(1972, 1148);
			// Spawn final enemies
			this->CreateDungeonKeeper();
			this->CreateBlunderbuss();
			for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
				this->CreateSwordsman();
			this->CreateFinalStageClonk();
			// Create a new respawn flag
			var banner = CreateObject(Flagpole, 1667, 1113, REPUBLIC);
			banner->SetKingdomBanner();
			banner->MakeInvincible();
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
		if (this.battle_progress == 3)
		{
			return BoundBy(1 + GetPlayerCount(C4PT_User), GetPlayerCount(C4PT_User) + 3, GetPlayerCount(C4PT_User) * 2 + 3);
		}
	},
	GetAdvancedClonk = func() {
		var furthest = paula;
		for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
		{
			if (GetCrew(GetPlayerByIndex(i, C4PT_User), 0)->GetX() > furthest->GetX())
				furthest = GetCrew(GetPlayerByIndex(i, C4PT_User), 0);
		}
		return furthest;
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
		if (this.battle_prohress == 3)
		{
			return !Random(40);
		}
	},
	SafeCrossing = func() {
		// An AI controlled clonk is asking if it's cool to advance on an enemy
		if (this.battle_progress == 0 || this.battle_progress == 2)
			// Crossing is unsafe during bridge construction!
			return false;
		return true;
	}
};