/**
	Skirmish
	
	@author Clonkonaut
**/

// Scenario status
static is_initialized, scen_goal;
// Important NPCs
static paula, paulas_dlg, samuel, samuels_dlg;
static g_general, g_g_major, g_g_guard, g_entrance_guard, g_cook, g_gunners, g_gunny, g_prisoner, g_medic, g_corporal, g_soldiers, g_armorer, g_gate_guard, g_pioneers, g_assault;
static g_kutor_frontsoldier;
// Important objects
static g_bridges;
// Other things
static const g_shell_rubble = [[1521,767], [1459,776], [1367,801], [1288,841], [890,978], [1106,815], [1211,759], [1232,752], [798,1073], [680,1073]]; // coordinates where rubble might drop from the ceiling
static g_enemy_guns_effect;
static g_start_position;
// Quest status
static g_talked_to_prisoner;
// Character development status
static g_player_rank, g_question1;
// Building knowledge
static const g_player_knowledge = [
];

// Static save vars (see Objects.c)
// g_headquarters, g_rail, g_blimp, g_spinwheel, g_spinwheel2, g_hatch, g_door1, g_door2, g_door3

// Helpers
static const REPUBLIC = 11; // Player numbers
static const KUTOR = 10;

static g_pioneer_rally_point;
static g_pioneer_last_bridge;

// Debug stuff
static const g_skip_creation = false;
static const g_skip_intro = true;
static const g_skip_prisoner_dlg = true;
static const g_skip_briefing = true;
static const g_skip_tofirstbattle = false;

func Initialize()
{
	g_start_position = [15, 797];

	SetSkyParallax(0, 15, 15, nil, nil, nil, -100);

	if (g_skip_creation)
		return;

	g_talked_to_prisoner = false;
	g_player_rank = 0; // Player starts being a private, see System.ocg/Helpers.c

	CreateRepublicanArmy();
	CreateKutorianArmy();
	SetHostility(REPUBLIC, KUTOR, true, true, true);

	InitObjects();

	var relaunch_rule = GetRelaunchRule();
	relaunch_rule->SetInventoryTransfer(true);
	relaunch_rule->SetFreeCrew(true);
	relaunch_rule->SetRespawnDelay(2);
	relaunch_rule->SetBaseRespawn(true);
	relaunch_rule->SetDefaultRelaunchCount(nil);
	relaunch_rule->SetAllowPlayerRestart(true);
	relaunch_rule->SetLastClonkRespawn(true);
	relaunch_rule->SetInitialRelaunch(false);

	CreateObject(Rule_NoFriendlyFire);
}

func CreateRepublicanArmy()
{
	g_general = CreateRepublicanClonk(120, 743, DIR_Left, REPUBLIC, "$RepublicanGeneral$", false, true, true);
	g_general->CreateOfficersHat();
	g_general->RemoveBackpack();
	g_general->SetToWalkingSpeed();
	g_general->SetMeshMaterial("Clonk_General");
	g_general->SetPortrait({Source = Campaign_AltMaterials, Name = "PortraitGeneral", Color = g_general->GetColor()});

	g_g_guard = CreateRepublicanClonk(169, 743, DIR_Right, REPUBLIC, "$PersonalGuard$", true, true, true);
	g_g_guard->RemoveBackpack();
	g_g_guard->CreateContents(Javelin);
	g_g_guard->CreateContents(Helmet)->ControlUse(g_g_guard);
	g_g_guard->SetDialogue("Miller");

	g_g_major = CreateRepublicanClonk(119, 716, DIR_Left, REPUBLIC, "$MajorGeneral$", false, true, true);
	g_g_major->CreateOfficersHat();
	g_g_major->RemoveBackpack();
	g_g_major->CreateContents(Pocketwatch);
	g_g_major->UpdateAttach();
	g_g_major->SetToWalkingSpeed();
	g_g_major->CreateEffect(NPC_Idle_Guinot, 1, 20);
	g_g_major->SetDialogue("Guinot");

	g_entrance_guard = CreateRepublicanClonk(87, 790, DIR_Left, REPUBLIC, "$EntranceGuard$", false, true, true);
	g_entrance_guard->CreateContents(Shield);
	g_entrance_guard->CreateContents(Sword);
	g_entrance_guard->UpdateAttach();
	g_entrance_guard->SetDialogue("Mayer")->SetProperty("NoSpeakerTurn", true);

	g_cook = CreateRepublicanClonk(445, 705, DIR_Right, REPUBLIC, "$Cook$", false, true, true);
	g_cook->SetAlternativeSkin("Doctor");
	g_cook->CreateContents(Sword);
	g_cook->UpdateAttach();
	g_cook->SetAttachTransform(1, Trans_Mul(Trans_Scale(500), Trans_Rotate(90, 1)));
	g_cook->SetToWalkingSpeed();
	g_cook->SetDialogue("Cook")->SetProperty("NoSpeakerTurn", true);

	var wounded = CreateRepublicanClonk(328, 639, Random(2), REPUBLIC, "$Wounded1$", false, true, true);
	wounded->RemoveBackpack();
	wounded->PlayAnimation("Dead", CLONK_ANIM_SLOT_Death, Anim_Const(wounded->GetAnimationLength("Dead")), Anim_Const(1000));

	wounded = CreateRepublicanClonk(304, 639, Random(2), REPUBLIC, "$Wounded3$", true, true, true);
	wounded->RemoveBackpack();
	wounded->PlayAnimation("Dead", CLONK_ANIM_SLOT_Death, Anim_Const(wounded->GetAnimationLength("Dead")), Anim_Const(1000));

	wounded = CreateRepublicanClonk(275, 639, Random(2), REPUBLIC, "$Wounded2$", false, true, true);
	wounded->RemoveBackpack();
	wounded->PlayAnimation("Dead", CLONK_ANIM_SLOT_Death, Anim_Const(wounded->GetAnimationLength("Dead")), Anim_Const(1000));

	wounded = CreateRepublicanClonk(252, 639, Random(2), REPUBLIC, "$Wounded4$", true, true, true);
	wounded->RemoveBackpack();
	wounded->PlayAnimation("Dead", CLONK_ANIM_SLOT_Death, Anim_Const(wounded->GetAnimationLength("Dead")), Anim_Const(1000));

	g_medic = CreateRepublicanClonk(341, 639, DIR_Left, REPUBLIC, "$Medic$", false, true, true);
	g_medic->SetAlternativeSkin("Doctor");
	g_medic->AttachMesh(Barrel, "pos_hand2", "main", Trans_Mul(Trans_Scale(300), Trans_Translate(2000, 2200, 3000)));
	g_medic->SetToWalkingSpeed();
	g_medic->CreateEffect(NPC_Idle_Medic, 1, 10);
	g_medic->SetDialogue("Medic");

	g_gunners = CreateArray(3);

	g_gunners[0] = CreateRepublicanClonk(283, 547, DIR_Right, REPUBLIC, "$Gunner1$", false, true, true);
	var cannon = g_gunners[0]->FindObject(Find_ID(Cannon), Find_AnyLayer(), Sort_Distance());
	cannon->SetObjectLayer(g_gunners[0]);
	cannon->SetCannonAngle(22000);
	g_gunners[0]->ObjectCommand("Grab", cannon);

	g_gunners[1] = CreateRepublicanClonk(324, 554, DIR_Right, REPUBLIC, "$Gunner2$", false, true, true);
	cannon = g_gunners[1]->FindObject(Find_ID(Cannon), Find_AnyLayer(), Sort_Distance());
	cannon->SetObjectLayer(g_gunners[1]);
	cannon->SetCannonAngle(22000);
	g_gunners[1]->ObjectCommand("Grab", cannon);

	g_gunners[2] = CreateRepublicanClonk(354, 564, DIR_Right, REPUBLIC, "$Gunner3$", false, true, true);
	cannon = g_gunners[2]->FindObject(Find_ID(Cannon), Find_AnyLayer(), Sort_Distance());
	cannon->SetObjectLayer(g_gunners[2]);
	cannon->SetCannonAngle(22000);
	g_gunners[2]->ObjectCommand("Grab", cannon);

	g_gunny = CreateRepublicanClonk(192, 546, DIR_Right, REPUBLIC, "$Gunny$", true, true, true);
	g_gunny->SetDialogue("Gunny");

	g_blimp->SetPosition(75, 420);
	g_blimp->TurnAirship(DIR_Left, true);
	var scout = CreateRepublicanClonk(73, 432, DIR_Right, REPUBLIC, "$Scout$", true, true, false);
	g_blimp->SetObjectLayer(g_blimp);
	scout->SetObjectLayer(g_blimp);
	scout->ObjectCommand("Grab", g_blimp);

	ConnectWithRope(g_blimp, g_spinwheel, [28, -15], [-2, -6]);

	g_gunny->CreateEffect(NPC_Idle_Hartman, 1, 35, scout);

	g_gate_guard = CreateRepublicanClonk(700, 753, DIR_Right, REPUBLIC, "$Guard$", true, true, true);
	cannon = g_gate_guard->FindObject(Find_ID(Cannon), Find_AnyLayer(), Sort_Distance());
	cannon->SetObjectLayer(g_gate_guard);
	g_gate_guard->ObjectCommand("Grab", cannon);
	g_gate_guard->SetDialogue("Farina")->SetProperty("NoSpeakerTurn", true);
	g_gate_guard->CreateEffect(NPC_Gate_Guard, 1, 35);

	g_prisoner = CreateKutorianClonk(417, 821, DIR_Right, KUTOR, "$Prisoner$", false, true, true);
	g_prisoner->SetMeshMaterial("Clonk_Prisoner");
	g_prisoner->RemoveBackpack();
	g_prisoner->SetPortrait({Source = Campaign_AltMaterials, Name = "PortraitPrisoner", Color = g_prisoner->GetColor()});
	var shovel = g_prisoner->CreateContents(Shovel);
	shovel->SetMeshMaterial("oldShovel");
	g_prisoner->UpdateAttach();
	g_prisoner->SetToWalkingSpeed();
	g_prisoner->CreateEffect(NPC_Idle_Prisoner, 1, 5);
	g_prisoner->SetDialogue("Prisoner");

	g_corporal = CreateRepublicanClonk(319, 1060, DIR_Right, REPUBLIC, "$ArmoryGuard$", false, true, true);
	g_corporal->RemoveBackpack();
	g_corporal->CreateContents(Javelin);
	g_corporal->CreateContents(Shield);
	g_corporal->UpdateAttach();
	g_corporal->SetDialogue("Smith");
	g_corporal->CreateEffect(NPC_Guard_Smith, 1, 5);

	g_soldiers = CreateArray(4);

	g_soldiers[0] = CreateRepublicanClonk(1593, 731, DIR_Right, REPUBLIC, "$Soldier1$", true, true, true);
	g_soldiers[0]->CreateContents(Blunderbuss);
	g_soldiers[0]->UpdateAttach();
	g_soldiers[0]->SetDialogue("Marie");
	g_soldiers[0]->CreateEffect(NPC_Guard_Fighting, 1, 20);

	g_soldiers[1] = CreateRepublicanClonk(1556, 763, DIR_Left, REPUBLIC, "$Soldier2$", false, true, true);
	g_soldiers[1]->CreateContents(Javelin);
	g_soldiers[1]->CreateContents(Shield);
	g_soldiers[1]->CreateContents(Helmet)->ControlUse(g_soldiers[1]);
	g_soldiers[1]->UpdateAttach();
	g_soldiers[1]->SetDialogue("Mark");

	g_soldiers[2] = CreateRepublicanClonk(228, 921, DIR_Left, REPUBLIC, "$Soldier3$", false, true, true);
	g_soldiers[2]->RemoveBackpack();
	g_soldiers[2]->CreateContents(BigHammer);
	g_soldiers[2]->UpdateAttach();
	g_soldiers[2]->Contents()->ControlUseStart(g_soldiers[2]);
	g_soldiers[2]->SetDialogue("Derek")->SetProperty("NoSpeakerTurn", true);
	var wood = g_soldiers[2]->FindObject(Find_ID(Wood), Find_AnyLayer(), Sort_Distance());
	wood->CreateEffect(NPC_Derek_StopWhenBurnt, 1);

	g_soldiers[3] = CreateRepublicanClonk(1302, 718, DIR_Right, REPUBLIC, "$Soldier4$", false, true, true);
	g_soldiers[3]->CreateContents(Sword);
	g_soldiers[3]->CreateContents(Bow);
	g_soldiers[3]->UpdateAttach();
	g_soldiers[3]->SetDialogue("Flix")->SetProperty("NoSpeakerTurn", true);

	g_armorer = CreateRepublicanClonk(300, 923, DIR_Left, REPUBLIC, "$Lieutenant$", true, true, true);
	g_armorer->CreateOfficersHat();
	g_armorer->CreateContents(Torch);
	g_armorer->UpdateAttach();
	g_armorer->SetDialogue("Bort");
}

func CreateKutorianArmy()
{
	g_kutor_frontsoldier = CreateKutorianClonk(2247, 687, DIR_Left, KUTOR, "$Kutor1$", false, true, true);
	var gatling = CreateObject(GatlingGun, 2242, 687, KUTOR);
	gatling->SetObjectLayer(g_kutor_frontsoldier);
	g_kutor_frontsoldier->ObjectCommand("Grab", gatling);
	g_kutor_frontsoldier->CreateEffect(NPC_Enemy_Gatling, 1, 5);
}

func InitObjects()
{
	// Paula
	paula = CreateObject(Clonk, 43, 797);
	MakePaula(paula, REPUBLIC);
	paula->SetDir(DIR_Right);
	paulas_dlg = paula->SetDialogue("Paula");
	paulas_dlg->SetInteraction(false);

	// Samuel
	samuel = CreateObject(Clonk, 49, 797);
	MakeSamuel(samuel, REPUBLIC);
	samuel->SetDir(DIR_Right);
	samuels_dlg = samuel->SetDialogue("Samuel");
	samuels_dlg->SetInteraction(false);

	// Effects
	CreateEffect(CookingFire, 1, 8);
	FindObject(Find_ID(SteamEngine))->CreateEffect(SteamEngineWork, 1, 15);
	var gear = g_soldiers[1]->FindObjects(Find_Distance(50), Find_Category(C4D_Object), Find_NoContainer());
	for (var item in gear)
		item->CreateEffect(ImportantGear, 1, 0, g_soldiers[1]);
	gear = g_armorer->FindObjects(Find_Or(Find_ID(PowderKeg), Find_ID(Boompack)), Find_Distance(300));
	for (var item in gear)
		item->CreateEffect(ImportantGear, 1, 0, g_armorer);
	gear = g_soldiers[3]->FindObjects(Find_Distance(80), Find_Category(C4D_Object), Find_NoContainer());
	for (var item in gear)
		item->CreateEffect(ImportantGear, 1, 0, g_soldiers[3]);

	// Clouds
	Cloud->Place(10);

	// Visuals
	SetTimeOfDay(0);

	// Entrance to the camp
	var bridge = CreateObject(Campaign_DrawbridgeRight, 746, 833, REPUBLIC);
	bridge->Close(true);
	bridge->AddGaffs(88);
	g_spinwheel2->SetSwitchTarget(bridge);
	CreateObject(TowerCover, 726, 799, REPUBLIC);

	// Make all basements indestructible
	// This, I should have done in the editor. However, I placed far too many basements and forgot many.
	var basements = FindObjects(Find_ID(Basement));
	for (var basement in basements)
		basement->MakeInvincible();

	// Remove radius markers from the steam engine
	ScheduleCall(FindObject(Find_ID(SteamEngine)), "ClearFlagMarkers", 5);

	// Respawn
	var banner = CreateObject(Flagpole, 545, 798, REPUBLIC);
	banner->SetKingdomBanner();
	banner->MakeInvincible();

	// Rules
	CreateObject(Rule_Gravestones, 0,0)->SetFadeOut(3 * 36);

	// Goal
	scen_goal = CreateObject(Goal_Skirmish, 0,0);
	AdvanceGoal();
}

func PlaceBatches(array item_ids, int n_per_batch, int batch_radius, int n_batches)
{
	// place a number (n_batches) of batches of objects of types item_ids. Each batch has n_per_batch objects.
	// fewer batches and/or objects may be placed if no space is found
	var loc,loc2,n_item_ids=GetLength(item_ids), n_created=0, obj;
	for (var i=0; i<n_batches; ++i)
		if (loc = FindLocation(Loc_Material("Earth")))
			for (var j=0; j<n_per_batch; ++j)
				if (loc2 = FindLocation(Loc_InRect(loc.x-batch_radius,loc.y-batch_radius,batch_radius*2,batch_radius*2), Loc_Material("Earth")))
					if (obj=CreateObjectAbove(item_ids[Random(n_item_ids)],loc2.x,loc2.y))
					{
						obj->SetPosition(loc2.x,loc2.y);
						++n_created;
					}
	return n_created;
}

func DoInit(first_player)
{
	var crew = GetCrew(first_player, 0);
	if (!crew) return;

	if (!g_skip_intro)
	{
		StartSequence("Intro", 0, crew);
	}
	else
	{
		if (!g_skip_creation)
		{
			Sequence->Intro_Stop();
			if (g_skip_prisoner_dlg)
				TalkedToPrisoner();
			if (g_skip_briefing)
			{
				CreatePioneerSquad();
				for (var i = 0; i < GetLength(g_pioneers); i++)
					g_pioneers[i]->SetPosition(800 + RandomX(-10, 10), 1087);

				for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
				{
					var plr = GetPlayerByIndex(i, C4PT_User);
					for (var j = 0; j < GetCrewCount(plr); j++) {
						GetCrew(plr, j)->SetPosition(841 + RandomX(-20,20), 1085);
						GetCrew(plr, j)->SetDir(DIR_Right);
					}
				}

				paula->SetPosition(893, 1085);
				paula->SetDir(DIR_Left);

				Sequence->Briefing_Init();
				Sequence->Briefing_Stop();
				samuel->SetCommand("None");

				g_player_rank++;

				if (g_skip_tofirstbattle)
				{
					FindObject(Find_ID(StoneDoor))->OpenDoor();
					Schedule(nil, "GameCall(\"SkipToFirstBattle\")", 70);
				}
			}
		}
	}

	is_initialized = true;
}

// Debug
func SkipToFirstBattle()
{
	for (var pioneer in g_pioneers)
		RemoveEffect("NPC_Pioneer_Warmup", pioneer);
	Sequence->FirstAssault_Stop();
}

func InitializePlayer(int player)
{
	if (GetPlayerType(player) == C4PT_Script)
		return;

	var crew;
	for (var index = 0; crew = GetCrew(player, index); index++) {
		var x = g_start_position[0] + Random(15);
		var y = g_start_position[1];
		crew->SetPosition(x, y);
		crew->SetDir(DIR_Right);
		if (!is_initialized)
			DoInit(player);
	}
	InitCharacterDevelopment(2, player);
	for (var plan in g_player_knowledge)
		SetPlrKnowledge(player, plan);
	SetHostility(player, KUTOR, true, true, true);
}

// Change clonk's skin
func OnClonkRecruitment(object clonk, int plr)
{
	if (GetPlayerType(plr) == C4PT_Script)
		return;

	if (clonk->GetSkin() < 2)
	{
		if (clonk.gender) clonk->SetSkin(3);
		else clonk->SetSkin(2);
	}
}

// Character development

func GetRandomProgress()
{
	// In a random roll of progress you don't get any change from Watchtower question 1 (the optional one)
	// and no bonus from the optional mushrooms.
	// Therefore it's a simple 50:50 chance of either Paula's or Samuel's support
	if (Random(2))
		return [40, 60]; // 40 for Paula, 60 for Samuel
	else
		return [60, 40]; // 60 for Paula, 40 for Samuel
}

func Question1Asked()
{
	return g_question1;
}

// Scenario development

public func AddNPCIdleEffects() // NPCs will start doing what they like when there's no sequence running
{
	g_cook->CreateEffect(NPC_Idle_Cook, 1, 8);
	g_general->CreateEffect(NPC_Idle_General, 1, 6);
}

public func CreateEnemyGuns()
{
	g_enemy_guns_effect = CreateEffect(EnemyGuns, 1, 35); // See System.ocg\EnemyGuns.c
}

public func ShellImpact(proplist shell)
{
	// Impacting artillery shells will cause rubble to fall down in the trenches
	if (Random(3)) return;

	var random = Random(GetLength(g_shell_rubble));
	CreateDustRain(g_shell_rubble[random][0], g_shell_rubble[random][1]);
}

public func TalkedToPrisoner()
{
	// Someone talked to the prisoner and got information about him
	// At this point, Sam will come out of the meeting with the general
	// But still, talking can be done as long as wanted.
	if (g_talked_to_prisoner)
		return;

	g_talked_to_prisoner = true;

	g_headquarters->OpenBarrier();

	samuel->SetCommand("MoveTo", nil, 198, 743);

	Schedule(nil, "GameCall(\"CloseBarrierAgain\")", 20);

	AdvanceGoal();
}

public func CloseBarrierAgain()
{
	g_headquarters->CloseBarrier();

	DoSalute("$Colonel$", g_g_guard);

	var clonks = FindObjects(Find_InRect(102, 727, 70, 28), Find_ID(Clonk), Find_Not(Find_Owner(REPUBLIC)), Find_Exclude(samuel));
	for (var clonk in clonks)
		clonk->SetPosition(123, 715);

	if (!g_skip_briefing)
		Schedule(nil, "GameCall(\"EnsureSamuelExit\")", 50);
}

public func EnsureSamuelExit()
{
	var diff = Distance(samuel->GetX(), samuel->GetY(), 198, 743);
	if (diff >= 10)
		samuel->SetPosition(198, 743);

	AddImportantQuestionMark(samuels_dlg, "Question1Asked");
	samuels_dlg->SetInteraction(true);
}

public func CreatePioneerSquad()
{
	g_pioneers = CreateArray();

	for (var i = 0; i < 5; i++)
	{
		var gender = true;
		if (Random(2))
			gender = false;

		g_pioneers[i] = CreateRepublicanClonk(278, 1060, DIR_Right, REPUBLIC, "$PioneerName$", gender, true, false);
		g_pioneers[i]->CreateContents(Helmet)->ControlUse(g_pioneers[i]);
		g_pioneers[i]->CreateContents(Club);
		g_pioneers[i]->CreateContents(DynamiteBox);
		g_pioneers[i]->SetHandItemPos(0, 1);
		g_pioneers[i]->SetQuickSwitchSlot(2);
	}
}

public func CreateAssaultTeam()
{
	g_assault = CreateArray();

	for (var i = 0; i < 5; i++)
	{
		var gender = true;
		if (Random(2))
			gender = false;

		g_assault[i] = CreateRepublicanClonk(685 - i*20, 815, DIR_Right, REPUBLIC, "$SoldierName$", gender, false, false);
		g_assault[i]->CreateContents(Blunderbuss);
		g_assault[i]->CreateContents(Shield);
		g_assault[i]->UpdateAttach();
	}
}

public func AddNPCPreparationEffects()
{
	for (var i = 0; i < GetLength(g_pioneers); i++)
		g_pioneers[i]->CreateEffect(NPC_Pioneer_Warmup, 1, 35 + Random(10), i);
}

// First Battle

// This will always be the point the pioneers will retreat to when either:
// The current bridge is destroyed
// The current bridge has been placed
global func SetRallyPoint(int x, int y)
{
	g_pioneer_rally_point = [x, y];
}

public func OrderBackwards(int plr)
{
	var cursor = GetCursor(plr);
	var bridge = FindObject(Find_ID(PioneerBridge));

	if (cursor && bridge)
		Dialogue->MessageBoxAll("$PioneerBack$", cursor, true);

	if (bridge)
		bridge->Backward();
	else if (cursor)
		cursor->PlayerMessage(plr, "$PioneerNoBridge$");
}

public func OrderForwards(int plr)
{
	var cursor = GetCursor(plr);
	var bridge = FindObject(Find_ID(PioneerBridge));

	if (cursor && bridge)
		Dialogue->MessageBoxAll("$PioneerForward$", cursor, true);

	if (bridge)
		bridge->Forward();
	else if (cursor)
		cursor->PlayerMessage(plr, "$PioneerNoBridge$");
}

public func OrderHalt(int plr)
{
	var cursor = GetCursor(plr);
	var bridge = FindObject(Find_ID(PioneerBridge));

	if (cursor && bridge)
		Dialogue->MessageBoxAll("$PioneerHalt$", cursor, true);

	if (bridge)
		bridge->Halt();
	else if (cursor)
		cursor->PlayerMessage(plr, "$PioneerNoBridge$");
}

public func OrderDynamite(int plr)
{
	var cursor = GetCursor(plr);
	var bridge = FindObject(Find_ID(PioneerBridge));

	if (GetEffect("First_Battle")->GetDynamiteReadiness() > 0)
	{
		cursor->PlayerMessage(plr, "$PioneerDynamiteNotReady$");
		return;
	}

	if (cursor && bridge)
		Dialogue->MessageBoxAll("$PioneerDynamite$", cursor, true);

	if (bridge)
	{
		for (var pioneer in g_pioneers)
		{
			if (pioneer == nil)
				continue;
			if (!pioneer->GetAlive())
				continue;
			if (GetEffect("Lifting", pioneer))
					continue;

			pioneer->CreateEffect(NPC_Pioneer_DynamiteThrowing, 1, 1);
		}
		GetEffect("First_Battle")->DynamiteCooldown(140); // About 20 seconds
	}
	else if (cursor)
		cursor->PlayerMessage(plr, "$PioneerNoBridge$");
}

public func PioneerBridgePlaced()
{
	if (GetEffect("First_Battle")->GetGrenadier())
		Dialogue->MessageBoxAll("$PaulaKillGrenadier$", paula, true);
	else
		Dialogue->MessageBoxAll("$PaulaBridgeDestroyed$", paula, true);
	GetEffect("First_Battle").newbridge = true;
	PioneersFallBack();
}

public func PioneerBridgeDestroyed()
{
	Dialogue->MessageBoxAll("$PaulaBridgeDestroyed$", paula, true);
	GetEffect("First_Battle").newbridge = true;
	PioneersFallBack();
}

public func WoodenBridgeDestroyed(object bridge)
{
	if (!g_bridges)
		return;
	if (!IsValueInArray(g_bridges, bridge))
		return;

	Dialogue->MessageBoxAll("$PaulaBridgeHole$", paula, true);

	if (bridge == g_pioneer_last_bridge) // Latest bridge got destroyed, no panic, no hole
	{
		var i;
		for (var i = GetLength(g_bridges) - 1; i > -1; i--)
			if (g_bridges[i] != nil)
				if (g_bridges[i] != bridge)
					break;
		g_pioneer_last_bridge = g_bridges[i];
	} else {
		// Uh oh. The bridges will fall!
		var passed = false;
		var last_one = g_bridges[0];
		for (var i = 0; i < GetLength(g_bridges); i++)
		{
			if (g_bridges[i] == bridge) // After this bridge, every other fall break apart
			{
				passed = true;
				g_pioneer_last_bridge = last_one;
			} else if (passed)
			{
				if (g_bridges[i])
					g_bridges[i]->ShakeToDestruction();
			} else if (g_bridges[i]) {
				last_one = g_bridges[i];
			}
		}
	}
}

public func PioneersFallBack()
{
	for (var pioneer in g_pioneers)
		if (pioneer)
			pioneer->PioneerMoveTo(g_pioneer_rally_point[0], g_pioneer_rally_point[1]);
}

public func OnClonkDeath(object clonk)
{
	if (g_pioneers)
		if (IsValueInArray(g_pioneers, clonk))
		{
			while(clonk->Contents())
				clonk->Contents()->RemoveObject();
			RelaunchPioneer();
		}
	if (GetEffect("First_Battle"))
		GetEffect("First_Battle")->OnClonkDeath(clonk);
}

func RelaunchPioneer()
{
	var gender = true;
	if (Random(2))
		gender = false;

	var clonk = CreateRepublicanClonk(630, 1014, DIR_Right, REPUBLIC, "$PioneerName$", gender, false, false);
	var door = FindObject(Find_ID(RoomDoor), Find_AtPoint(630, 1014));
	clonk->Enter(door);
	clonk->CreateContents(Helmet)->ControlUse(clonk);
	clonk->CreateContents(Club);
	clonk->CreateContents(DynamiteBox);
	clonk->SetHandItemPos(0, 1);
	clonk->SetQuickSwitchSlot(2);
	clonk->CreateEffect(NPC_Pioneer_MoveIntoAction, 1, 10);
}

public func AddNewPioneer(object clonk)
{
	PushBack(g_pioneers, clonk);
	RemoveHoles(g_pioneers);
}

public func CreateWoodenBridge()
{
	var bridge = CreateObject(WoodenBridge, g_pioneer_last_bridge->GetX() + 72, g_pioneer_last_bridge->GetY(), REPUBLIC);
	var i;
	for (i = 0; i < GetLength(g_bridges); i++)
		if (g_bridges[i] == nil)
			break;
	g_bridges[i] = bridge;
	bridge->Sound("Objects::Connect");
	g_pioneer_last_bridge = bridge;

	if (GetLength(g_bridges) >= 5) // Bridge complete!
	{
		// Double check
		var bridges = 0;
		for (var i = 0; i < GetLength(g_bridges); i++)
			if (g_bridges[i] != nil)
				bridges++;
		if (bridges < 5) // There is a hole in the bridge, not done!
			return;

		GetEffect("First_Battle")->ProgressBattle();
		return true;
	} else {
		GetEffect("First_Battle")->OnBridgeCreated();
	}
	return false;
}

// The end

func EndScenario()
{
	//SaveCharacterDevelopment();
	GainMissionAccess("CoNCamp2");
	GainScenarioAchievement("Done");

	GameOver();
}