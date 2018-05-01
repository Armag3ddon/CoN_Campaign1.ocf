/**
	The Watchtower
	
	@author Clonkonaut
**/

// Scenario status
static is_initialized, scen_goal;
// NPCs
static paula, paulas_dlg, samuel, samuels_dlg, tower_guard;
// Important objects
static g_dynamite, locking_basement, g_igniter, g_scout, g_stairs, g_barricade, g_nests_down;
// Quest status
static g_chests_allowed, g_get_igniter, g_forward_open, g_mushrooms, g_bunker_spotted, g_destruction, g_playershots, g_fireatwill;
// Character development status
static g_question1, g_question2;
// Building knowledge
static const g_player_knowledge = [
	Basement, Chest, Foundry, Sawmill, Windmill, WoodenBridge,
	Bread, CookedMushroom, Flour,
	Loam,
	Crate, Dynamite, DynamiteBox, Hammer, Torch,
	Bow, Arrow, FireArrow
];

// Static save vars (see Objects.c)
// g_elevator, g_lorry, g_lorry2, g_cannon, g_chest, g_door, g_door2, g_door3, g_door4, g_armory

// Debug stuff
static const g_skip_intro = false;
static const g_igniter_ready = false;
static const g_skip_attack = false;
static const g_skip_elevator = false;
static const g_skip_mainquest = false;
static const g_open_doors = false;

func Initialize()
{
	InitObjects();
	FixOwnership();

	SetSkyParallax(0, 15, 15, nil, nil, nil, -100);

	var relaunch_rule = GetRelaunchRule();
	relaunch_rule->SetInventoryTransfer(true);
	relaunch_rule->SetFreeCrew(true);
	relaunch_rule->SetRespawnDelay(2);
	relaunch_rule->SetBaseRespawn(true);
	relaunch_rule->SetDefaultRelaunchCount(nil);
	relaunch_rule->SetAllowPlayerRestart(true);
	relaunch_rule->SetLastClonkRespawn(true);
	relaunch_rule->SetInitialRelaunch(false);

	g_question1 = false;
	g_question2 = false;
	g_nests_down = 0;
	g_mushrooms = 0;
}

func InitObjects()
{
	// Add some nice plants and make our scary, dark forest
	PlaceGrass(75, 1111, 1520);
	PlaceGrass(75, 220, 1520);
	PlaceDarkForest([Tree_Coniferous, Tree_Coniferous2, Fern, Mushroom], 1650, 860, 1200, true);
	PlaceDarkForest([Tree_Coniferous, Tree_Coniferous2, Fern, Mushroom], 1650, 860, 1200, true);
	CreateObject(Ambience_DarkForest,0,0,NO_OWNER);

	// Create screecher nests
	var tree1 = nil;
	var tree2 = nil;
	for (var i = 0; i < 3; i++)
	{
		var tree = FindObject(Find_InRect(1825, 630, 1000, 300), Find_Func("IsTree"), Find_OCF(OCF_Fullcon), Find_Exclude(tree1), Find_Exclude(tree2), Sort_Random());
		if (!tree) continue; // ???
		if (i == 0)
			tree1 = tree;
		if (i == 1)
			tree2 = tree;

		var nest = CreateObject(BatCave, tree->GetX(), tree->GetY()-20, NO_OWNER);
		nest->Init(tree);
	}

	// Create little cave mushrooms
	CreateObjectAbove(Campaign_LittleCaveMushroom, 1804, 1090, NO_OWNER);
	CreateObjectAbove(Campaign_LittleCaveMushroom, 2077, 1088, NO_OWNER);
	CreateObjectAbove(Campaign_LittleCaveMushroom, 1520, 984, NO_OWNER);
	CreateObjectAbove(Campaign_LittleCaveMushroom, 3328, 672, NO_OWNER);
	CreateObjectAbove(Campaign_LittleCaveMushroom, 2273, 959, NO_OWNER);

	// Light all torches
	for (var torch in FindObjects(Find_ID(Torch)))
		torch->AttachToWall(true);

	// Paula
	paula = CreateObjectAbove(Clonk, 880, 1036);
	MakePaula(paula);
	paula->SetDir(DIR_Left);
	paulas_dlg = paula->SetDialogue("Paula");
	paulas_dlg->SetInteraction(false);

	// Samuel
	samuel = CreateObjectAbove(Clonk, 860, 446);
	MakeSamuel(samuel);
	samuel->SetDir(DIR_Left);
	samuels_dlg = samuel->SetDialogue("Samuel");
	samuels_dlg->SetInteraction(false);

	// Tower guard
	tower_guard = CreateObjectAbove(Clonk, 432, 810, 11);
	tower_guard->MakeInvincible();
	tower_guard->MakeNonFlammable();
	tower_guard->SetSkin(2);
	tower_guard->SetName("Eric");
	tower_guard->SetDir(DIR_Right);
	tower_guard->SetObjectLayer(tower_guard);
	tower_guard->CreateContents(Javelin);
	tower_guard->CreateContents(Helmet)->ControlUse(tower_guard);

	// Respawn
	var banner = CreateObject(Flagpole, 800, 780, 11);
	banner->SetKingdomBanner();
	banner->MakeInvincible();

	// Place the igniter
	g_igniter = g_chest->CreateContents(DynamiteBox);
	while (g_igniter->Contents())
		g_igniter->Contents()->RemoveObject();
	g_igniter->ChangeToIgniter();

	// Chests are not accessible
	g_chests_allowed = false;
	g_get_igniter = false;

	// Blast corridor
	DrawMaterialQuad("tunnel-tunnel", 475,929, 475,931, 663,984, 663,982, DMQ_Sub);
	var dynamite1 = CreateObject(Dynamite, 479, 932);
	dynamite1->SetR(-45);
	var dynamite2 = CreateObject(Dynamite, 515, 942);
	dynamite2->SetR(-45);
	var dynamite3 = CreateObject(Dynamite, 551, 952);
	dynamite3->SetR(-45);
	var dynamite4 = CreateObject(Dynamite, 585, 962);
	dynamite4->SetR(-45);
	var dynamite5 = CreateObject(Dynamite, 622, 972);
	dynamite5->SetR(-45);
	var dynamite6 = CreateObject(Dynamite, 663, 986);
	dynamite6->SetR(45);
	Fuse->Create(dynamite1, dynamite2);
	Fuse->Create(dynamite2, dynamite3);
	Fuse->Create(dynamite3, dynamite4);
	Fuse->Create(dynamite4, dynamite5);
	Fuse->Create(dynamite5, dynamite6);
	g_dynamite = dynamite6;

	// Players shouldn't interfere with dynamite at this point
	var dynamites = FindObjects(Find_ID(Dynamite));
	for (var dynamite in dynamites)
		dynamite.Collectible = false;

	// Scouting post
	g_scout = CreateObject(InventorsLab, 842, 279, 11);
	g_stairs = CreateObject(Campaign_Stairs, 836, 301, 11);
	g_stairs.Plane = g_scout.Plane + 1;

	// Two filled caves (late additions)
	CastObjects(Wood, 7, 30, 361,1189);
	CreateObject(Diamond_Socket, 371, 1184, NO_OWNER);
	CreateObject(Diamond_Socket, 354, 1190, NO_OWNER);
	CastObjects(Bone, 15, 30, 1437,1276);
	CastObjects(OldBook, 1, 30, 1437,1276);
	CreateObject(Pickaxe, 1406, 1305, NO_OWNER)->SetR(116);
	CreateObject(Mushroom, 1451, 1284, NO_OWNER);

	// Random earth material
	PlaceBatches([Rock, Firestone, Loam], 8, 150, 13);

	PlaceCracks(25);

	// Goal
	scen_goal = CreateObject(Goal_Watchtower, 0,0);
	AdvanceGoal();

	// Ultimate goal
	for (var i = 0; i < 20; i++)
	{
		g_lorry->CreateContents(Metal)->SetGraphics("Old");
		g_lorry2->CreateContents(Metal)->SetGraphics("Old");
	}

	// Set elevator right
	g_elevator->CreateShaft(600);
	g_elevator.case->SetPosition(g_elevator.case->GetX(), 806);
	g_elevator->MakeInvincible();
	g_elevator.case->MakeInvincible();

	// Give knowledge to friendly nonhuman player
	for (var plan in g_player_knowledge)
		SetPlrKnowledge(11, plan);

	// Debugging
	if (!g_skip_elevator)
		g_elevator.case.firstmove = true;

	locking_basement = CreateObject(Basement, 1035, 332, NO_OWNER);
	locking_basement->MakeInvincible();
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

func PlaceCracks(int amount)
{
	var loc, n_created=0, obj;
	for (var i=0; i < amount; ++i)
		if (loc = FindLocation(Loc_InRect(1696, 792, 1664, 264), Loc_Material("Earth")))
			if (obj=CreateObjectAbove(TunnelBreaker,loc.x,loc.y))
			{
				obj->SetPosition(loc.x,loc.y);
				++n_created;
			}
	return n_created;
}

func FixOwnership()
{
	for (var flag in FindObjects(Find_Or(Find_ID(Flagpole), Find_Category(C4D_Structure))))
		flag->SetOwner(11);
}

func DoInit(first_player)
{
	var crew = GetCrew(first_player, 0);
	if (!crew) return;

	if (!g_skip_intro)
		StartSequence("Intro", 0, crew);
	else
	{
		paulas_dlg->SetInteraction(true);
		paulas_dlg->AddAttention();
		samuels_dlg->SetInteraction(true);
		AdvanceGoal();
	}

	// Debugging
	if (g_igniter_ready)
	{
		var igniter = paula->CreateObject(DynamiteBox);
		while (igniter->Contents())
			igniter->Contents()->RemoveObject();
		igniter->ChangeToIgniter();
		igniter->SetObjectLayer();
	}

	if (g_skip_attack)
	{
		AdvanceGoal();
		Sequence->Attack_37();
		Sequence->Attack_Stop();
	}

	if (g_skip_mainquest)
	{
		RemoveEffect("Samuel_WorkingEffect", samuel);
		RemoveEffect("Paula_WorkingEffect", paula);
		AdvanceGoal();
		AdvanceGoal();
		AddMetalGoal();
		Sequence->MainQuest_31();
		samuels_dlg->SetDialogueProgress(1, "MainQuest", true);
		paulas_dlg->SetDialogueProgress(1, "MainQuest", true);
	}

	if (g_open_doors)
	{
		g_door->OpenDoor();
		g_door2->OpenDoor();
	}

	is_initialized = true;
}

func InitializePlayer(int player)
{
	if (GetPlayerType(player) == C4PT_Script)
		return;

	var crew;
	for (var index = 0; crew = GetCrew(player, index); index++) {
		var x = 850 + Random(50);
		var y = 805;
		crew->SetPosition(x, y);
		if (!is_initialized)
			DoInit(player);
	}
	InitCharacterDevelopment(1, player);
	for (var plan in g_player_knowledge)
		SetPlrKnowledge(player, plan);
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

func Question1Asked()
{
	return g_question1;
}

func Question2Asked()
{
	return g_question2;
}

// Scenario development

func DeathOfABatNest()
{
	// The player oblitareted a bat nest with the flamethrower
	g_nests_down++;

	if (g_nests_down == 3) // All nests are down, occasionally respawn a bat with an effect
		Scenario->CreateEffect(BatSpawnEffect, 1, 35);
}

// The end

func EndScenario()
{
	SaveCharacterDevelopment();
	GainMissionAccess("CoNCamp1");
	GainScenarioAchievement("Done");
	if (g_mushrooms > 0 && g_mushrooms < 5)
		GainScenarioAchievement("Mushrooms", 1);
	if (g_mushrooms == 5)
		GainScenarioAchievement("Mushrooms", 2);

	GameOver();
}