/* Automatically created objects file */

static g_chest, g_armory, g_door, g_door2, g_door3, g_door4, g_elevator, g_lorry, g_lorry2, g_cannon;

func InitializeObjects()
{
	var Grass001 = CreateObjectAbove(Grass, 1124, 566);
	var Grass002 = CreateObjectAbove(Grass, 1129, 566);
	var Grass003 = CreateObjectAbove(Grass, 1132, 566);
	var Grass004 = CreateObjectAbove(Grass, 1135, 567);
	var Grass005 = CreateObjectAbove(Grass, 1141, 566);
	var Grass006 = CreateObjectAbove(Grass, 1146, 566);
	var Grass007 = CreateObjectAbove(Grass, 1152, 565);
	var Grass008 = CreateObjectAbove(Grass, 1154, 565);
	var Grass009 = CreateObjectAbove(Grass, 1160, 566);
	var Grass010 = CreateObject(Grass, 1164, 565);
	var Grass011 = CreateObjectAbove(Grass, 1168, 566);

	var Torch001 = CreateObjectAbove(Torch, 1418, 803);
	Torch001->AttachToWall(true);
	var Torch002 = CreateObjectAbove(Torch, 829, 797);
	Torch002->AttachToWall(true);
	var Torch003 = CreateObjectAbove(Torch, 376, 1328);
	Torch003->AttachToWall(true);
	var Torch004 = CreateObjectAbove(Torch, 893, 403);
	Torch004->AttachToWall(true);
	var Torch005 = CreateObjectAbove(Torch, 893, 528);
	Torch005->AttachToWall(true);
	var Torch006 = CreateObjectAbove(Torch, 714, 1008);
	Torch006->AttachToWall(true);
	var Torch007 = CreateObjectAbove(Torch, 981, 687);
	Torch007->AttachToWall(true);
	var Torch008 = CreateObjectAbove(Torch, 1010, 963);
	Torch008->AttachToWall(true);
	var Torch009 = CreateObjectAbove(Torch, 962, 996);
	Torch009->AttachToWall(true);
	var Torch010 = CreateObjectAbove(Torch, 1019, 435);
	Torch010->AttachToWall(true);
	var Torch011 = CreateObjectAbove(Torch, 1057, 553);
	Torch011->AttachToWall(true);
	var Torch012 = CreateObjectAbove(Torch, 719, 500);
	Torch012->AttachToWall(true);
	var Torch013 = CreateObjectAbove(Torch, 1072, 790);
	Torch013->AttachToWall(true);

	CreateObjectAbove(Campaign_TreeTrunks, 1344, 815);
	CreateObjectAbove(Campaign_TreeTrunks, 1301, 815);

	var Rule_TeamAccount001 = CreateObject(Rule_TeamAccount, 111, 53);

	var Rule_NoPowerNeed001 = CreateObject(Rule_NoPowerNeed, 167, 54);

	var Branch001 = CreateObject(Branch, 1396, 729);
	Branch001->SetR(-98);
	Branch001->SetPosition(1396, 729);
	var Branch002 = CreateObject(Branch, 1445, 708);
	Branch002->SetR(-2);
	Branch002->SetPosition(1445, 708);
	var Branch003 = CreateObject(Branch, 1451, 746);
	Branch003->SetR(97);
	Branch003->SetPosition(1451, 746);
	var Branch004 = CreateObject(Branch, 1451, 767);
	Branch004->SetR(95);
	Branch004->SetPosition(1451, 767);
	var Branch005 = CreateObject(Branch, 1457, 713);
	Branch005->SetR(54);
	Branch005->SetPosition(1457, 713);

	var Trunk001 = CreateObject(Trunk, 1529, 794);
	Trunk001->SetR(-10);
	Trunk001->SetPosition(1529, 794);
	CreateObjectAbove(Trunk, 1572, 808);
	CreateObjectAbove(Trunk, 1623, 794);
	var Trunk002 = CreateObject(Trunk, 2525, 788);
	Trunk002->SetR(45);
	Trunk002->SetPosition(2525, 788);

	var Flag001 = CreateObjectAbove(Flag, 759, 271, 11);
	Flag001->SetColor(0xff6900ff);
	var Flag002 = CreateObjectAbove(Flag, 1107, 296, 11);
	Flag002->SetColor(0xff6900ff);
	var Flag003 = CreateObjectAbove(Flag, 570, 440, 11);
	Flag003->SetColor(0xff6900ff);
	var Flag004 = CreateObjectAbove(Flag, 356, 721, 11);
	Flag004->SetColor(0xff6900ff);
	var Flag005 = CreateObjectAbove(Flag, 1428, 720, 11);
	Flag005->SetColor(0xff6900ff);
	Flag005->SetSlow();

	CreateObjectAbove(EnvPack_WineBarrel, 947, 448);
	CreateObjectAbove(EnvPack_WineBarrel, 921, 449);

	CreateObjectAbove(EnvPack_Bag, 917, 569);
	CreateObjectAbove(EnvPack_Bag, 930, 568);

	CreateObjectAbove(EnvPack_Painting, 858, 434);
	var MetalLadder001 = CreateObjectAbove(MetalLadder, 787, 921);
	MetalLadder001->SetLength(25);
	MetalLadder001->SetPosition(787, 921);

	var Deco_MetalBeam001 = CreateObject(Deco_MetalBeam, 671, 643);
	Deco_MetalBeam001->SetCon(204);
	Deco_MetalBeam001->SetR(40);
	Deco_MetalBeam001->SetPosition(671, 643);
	var Deco_MetalBeam002 = CreateObject(Deco_MetalBeam, 758, 642);
	Deco_MetalBeam002->SetCon(205);
	Deco_MetalBeam002->SetR(141);
	Deco_MetalBeam002->SetPosition(758, 642);

	CreateObjectAbove(EnvPack_Bag, 677, 696);
	CreateObjectAbove(EnvPack_BridgeRustic, 702, 696);

	var EnvPack_TreeTrunks001 = CreateObject(EnvPack_TreeTrunks, 749, 696);
	EnvPack_TreeTrunks001->SetPosition(749, 696);

	CreateObjectAbove(EnvPack_Candle, 713, 658);
	CreateObjectAbove(EnvPack_Candle, 734, 659);
	CreateObjectAbove(EnvPack_WineBarrel, 683, 697);

	CreateObjectAbove(EnvPack_Painting, 925, 618);
	CreateObjectAbove(EnvPack_Painting, 861, 617);
	var EnvPack_Painting001 = CreateObjectAbove(EnvPack_Painting, 891, 599);
	EnvPack_Painting001->SetClrModulation(0xff330081);
	EnvPack_Painting001->SetObjectBlitMode(GFX_BLIT_Mod2);

	var Deco_WoodenSign001 = CreateObjectAbove(Deco_WoodenSign, 805, 686);
	Deco_WoodenSign001->SetText(4);

	var Deco_MetalBeam003 = CreateObject(Deco_MetalBeam, 787, 684);
	Deco_MetalBeam003->SetCon(209);
	Deco_MetalBeam003->SetR(150);
	Deco_MetalBeam003->SetPosition(787, 684);

	CreateObjectAbove(EnvPack_Bag, 1075, 448);
	CreateObjectAbove(EnvPack_Bag, 783, 448);

	var Branch006 = CreateObject(Branch, 361, 1318);
	Branch006->SetCon(158);
	Branch006->SetR(164);
	Branch006->SetPosition(361, 1318);

	CreateObjectAbove(EnvPack_Rail, 697, 771);
	CreateObjectAbove(EnvPack_Rail, 729, 755);
	CreateObjectAbove(EnvPack_Rail, 769, 771);

	var EnvPack_Guidepost2001 = CreateObjectAbove(EnvPack_Guidepost2, 1461, 816);
	EnvPack_Guidepost2001->SetInscription("Silva Tenebris >>");
	var EnvPack_Guidepost2002 = CreateObjectAbove(EnvPack_Guidepost2, 332, 819);
	EnvPack_Guidepost2002->SetInscription("$Guidepost$");

	var MetalLadder002 = CreateObjectAbove(MetalLadder, 970, 738);
	MetalLadder002->SetLength(10);
	MetalLadder002->SetPosition(970, 738);
	var Keypad001 = CreateObjectAbove(Keypad, 682, 615);
	Keypad001->SetClrModulation(0xffc8c8c8);
	var Flag006 = CreateObject(Flag, 1211, 448, 11);
	Flag006->SetR(68);
	Flag006->SetColor(0xff6900ff);
	Flag006->SetPosition(1211, 448);
	Flag006->SetSlow();

	var Deco_Tree_Deciduous2_Burned001 = CreateObject(Deco_Tree_Deciduous2_Burned, 993, 866);
	Deco_Tree_Deciduous2_Burned001->SetCon(187);
	Deco_Tree_Deciduous2_Burned001->SetR(83);
	Deco_Tree_Deciduous2_Burned001->SetClrModulation(0x78ffffff);
	Deco_Tree_Deciduous2_Burned001->SetPosition(993, 866);
	Deco_Tree_Deciduous2_Burned001->MakeInvincible();

	var Flag007 = CreateObject(Flag, 1222, 443, 11);
	Flag007->SetR(68);
	Flag007->SetColor(0xff6900ff);
	Flag007->SetPosition(1222, 443);
	Flag007->SetSlow();
	var Flower001 = CreateObjectAbove(Flower, 1126, 573);
	Flower001->SetSeedChance(0);
	Flower001->SetSeedAmount(0);
	var Flower002 = CreateObjectAbove(Flower, 1154, 573);
	Flower002->SetSeedChance(0);
	Flower002->SetSeedAmount(0);
	var Flower003 = CreateObject(Flower, 1140, 569);
	Flower003->SetCon(49);
	Flower003->SetPosition(1140, 569);

	var MetalLadder003 = CreateObjectAbove(MetalLadder, 1209, 515);
	MetalLadder003->SetLength(10);
	var Cotton001 = CreateObjectAbove(Cotton, 1130, 568);
	Cotton001->SetSeedAmount(0);
	Cotton001->SetSeedArea(0);
	Cotton001->GrowBranch(true, 0);
	var Cotton002 = CreateObjectAbove(Cotton, 1159, 568);
	Cotton002->SetSeedAmount(0);
	Cotton002->SetSeedChance(0);

	var Branch007 = CreateObject(Branch, 888, 955);
	Branch007->SetR(8);
	Branch007->SetPosition(888, 955);
	var Branch008 = CreateObject(Branch, 862, 955);
	Branch008->SetCon(97);
	Branch008->SetR(-8);
	Branch008->SetPosition(862, 955);

	var Basement001 = CreateObject(Basement, 1041, 942);
	Basement001->SetPosition(1041, 942);
	var Basement002 = CreateObject(Basement, 1040, 949);
	Basement002->SetPosition(1040, 949);
	var Basement003 = CreateObject(Basement, 1018, 982);
	Basement003->SetPosition(1018, 982);
	var Basement004 = CreateObject(Basement, 893, 1044);
	Basement004->SetWidth(60);
	Basement004->SetPosition(893, 1044);
	var Basement005 = CreateObject(Basement, 846, 1045);
	Basement005->SetWidth(60);
	Basement005->SetPosition(846, 1045);

	var Hatch001 = CreateObject(Hatch, 382, 1353);
	Hatch001->SetCategory(C4D_StaticBack);
	Hatch001->SetPosition(382, 1353);
	Hatch001->MakeInvincible();

	var HatchBasement001 = CreateObject(HatchBasement, 382, 1356);
	HatchBasement001->SetParent(Hatch001);
	HatchBasement001->SetPosition(382, 1356);
	HatchBasement001->MakeInvincible();

	var Sawmill001 = CreateObjectAbove(Sawmill, 1136, 814);
	Sawmill001->MakeInvincible();

	var Basement006 = CreateObject(Basement, 1135, 818);
	Basement006->SetParent(Sawmill001);
	Basement006->SetPosition(1135, 818);
	Basement006->MakeInvincible();

	var Basement007 = CreateObject(Basement, 1019, 990);
	Basement007->SetPosition(1019, 990);

	var ChemicalLab001 = CreateObjectAbove(ChemicalLab, 785, 519);
	ChemicalLab001->MakeInvincible();

	var Foundry001 = CreateObjectAbove(Foundry, 847, 1040);
	Foundry001->MakeInvincible();
	var Foundry002 = CreateObjectAbove(Foundry, 899, 1039);
	Foundry002->MakeInvincible();

	var Kitchen001 = CreateObjectAbove(Kitchen, 989, 447);
	Kitchen001->MakeInvincible();

	var ToolsWorkshop001 = CreateObjectAbove(ToolsWorkshop, 967, 567);
	ToolsWorkshop001->MakeInvincible();

	g_chest = CreateObjectAbove(Chest, 800, 447);
	g_chest->SetMeshMaterial("MetalChest", 0);
	g_chest.StaticSaveVar = "g_chest";
	g_chest->MakeInvincible();
	var Chest001 = CreateObjectAbove(Chest, 881, 447);
	Chest001->MakeInvincible();
	var Chest002 = CreateObjectAbove(Chest, 752, 447);
	Chest002->MakeInvincible();
	var Chest003 = CreateObjectAbove(Chest, 822, 383);
	Chest003->MakeInvincible();
	var Chest004 = CreateObjectAbove(Chest, 800, 383);
	Chest004->MakeInvincible();
	var Chest005 = CreateObjectAbove(Chest, 825, 448);
	Chest005->MakeInvincible();

	g_armory = CreateObjectAbove(Armory, 871, 695);
	g_armory.StaticSaveVar = "g_armory";
	g_armory->MakeInvincible();

	g_door = CreateObject(StoneDoor, 1100, 796);
	g_door->SetComDir(COMD_Down);
	g_door.StaticSaveVar = "g_door";
	g_door->MakeInvincible();
	g_door->SetPosition(1100, 796);
	var StoneDoor001 = CreateObject(StoneDoor, 652, 796);
	StoneDoor001->SetComDir(COMD_Down);
	StoneDoor001->SetPosition(652, 796);
	g_door2 = CreateObject(StoneDoor, 1428, 796);
	g_door2->SetComDir(COMD_Down);
	g_door2.StaticSaveVar = "g_door2";
	g_door2->MakeInvincible();
	g_door2->SetPosition(1428, 796);
	var StoneDoor002 = CreateObject(StoneDoor, 356, 796);
	StoneDoor002->SetComDir(COMD_Down);
	StoneDoor002->SetPosition(356, 796);
	g_door3 = CreateObject(StoneDoor, 3252, 759);
	g_door3.StaticSaveVar = "g_door3";
	g_door3->SetPosition(3252, 759);
	var StoneDoor003 = CreateObject(StoneDoor, 780, 676);
	StoneDoor003->SetComDir(COMD_Down);
	StoneDoor003->SetPosition(780, 676);
	var StoneDoor004 = CreateObject(StoneDoor, 428, 1332);
	StoneDoor004->SetComDir(COMD_Down);
	StoneDoor004->SetPosition(428, 1332);
	StoneDoor004->MakeInvincible();
	g_door4 = CreateObject(StoneDoor, 773, 428);
	g_door4->SetComDir(COMD_Down);
	g_door4->SetPosition(773, 428);
	g_door4.StaticSaveVar = "g_door4";
	var StoneDoor006 = CreateObject(StoneDoor, 1100, 428);
	StoneDoor006->SetComDir(COMD_Down);
	StoneDoor006->SetPosition(1100, 428);

	var Idol001 = CreateObject(Idol, 2099, 888);
	Idol001->SetR(-149);
	Idol001->SetMeshMaterial("IdolGrayColor", 0);
	Idol001->SetPosition(2099, 888);
	var Idol002 = CreateObject(Idol, 731, 739);
	Idol002->SetR(13);
	Idol002->SetPosition(731, 739);
	var Idol003 = CreateObjectAbove(Idol, 696, 769);
	Idol003->SetClrModulation(0xffff00dc);
	Idol003->SetMeshMaterial("IdolGrayColor", 0);
	var Idol004 = CreateObject(Idol, 767, 755);
	Idol004->SetR(1);
	Idol004->SetClrModulation(0xffec9366);
	Idol004->SetMeshMaterial("IdolGrayColor", 0);
	Idol004->SetPosition(767, 755);
	var Idol005 = CreateObjectAbove(Idol, 1178, 448);
	Idol005->SetClrModulation(0xff97faff);
	Idol005->SetMeshMaterial("IdolGrayColor", 0);
	Idol005->SetObjectLayer(Idol005);

	var Compensator001 = CreateObject(Compensator, 710, 616);
	Compensator001->SetStoredPower(0);
	Compensator001->MakeInvincible();
	Compensator001->SetPosition(710, 616);

	var Hatch002 = CreateObject(Hatch, 796, 818);
	Hatch002->SetCategory(C4D_StaticBack);
	Hatch002->SetPosition(796, 818);
	Hatch002->MakeInvincible();

	var Loom001 = CreateObjectAbove(Loom, 709, 567);
	Loom001->SetClrModulation(0xff717171);
	Loom001->MakeInvincible();
	var Deco_MetalBeam004 = CreateObject(Deco_MetalBeam, 1098, 433);
	Deco_MetalBeam004->SetCon(207);
	Deco_MetalBeam004->SetR(175);
	Deco_MetalBeam004.Plane = 201;
	Deco_MetalBeam004->SetPosition(1098, 433);

	g_elevator = CreateObjectAbove(Elevator, 1017, 319);
	g_elevator.StaticSaveVar = "g_elevator";
	g_elevator->CreateShaft(6);
	g_elevator->SetCasePosition(313);
	var Elevator001 = CreateObjectAbove(Elevator, 993, 895);
	Elevator001->SetDir(DIR_Right);
	Elevator001->CreateShaft(49);
	Elevator001->SetCasePosition(932);
	var Switch001 = CreateObject(Switch, 350, 1331);
	Switch001->SetR(90);
	Switch001->SetPosition(350, 1331);
	Switch001->SetSwitchTarget(StoneDoor004);
	Switch001->SetSwitchDir(1);

	var Lorry001 = CreateObject(Lorry, 989, 807);
	Lorry001->SetClrModulation(0xffff806e);
	Lorry001->SetPosition(989, 807);
	var Lorry002 = CreateObjectAbove(Lorry, 957, 815);
	Lorry002->SetClrModulation(0xffff00dc);
	Lorry002->SetMeshMaterial("RuinedLorry", 0);
	var Lorry003 = CreateObject(Lorry, 915, 807);
	Lorry003->SetPosition(915, 807);
	var Lorry004 = CreateObject(Lorry, 921, 1022);
	Lorry004->SetR(-55);
	Lorry004->SetPosition(921, 1022);
	g_lorry = CreateObjectAbove(Lorry, 3288, 775);
	g_lorry.StaticSaveVar = "g_lorry";
	g_lorry2 = CreateObjectAbove(Lorry, 3324, 775);
	g_lorry2->SetMeshMaterial("RuinedLorry", 0);
	g_lorry2.StaticSaveVar = "g_lorry2";

	g_cannon = CreateObjectAbove(Cannon, 462, 815);
	g_cannon->SetMeshMaterial("KingdomCannon", 0);
	g_cannon.StaticSaveVar = "g_cannon";

	CreateObjectAbove(Catapult, 718, 691);
	var Catapult001 = CreateObjectAbove(Catapult, 607, 447);
	Catapult001->SetDir(DIR_Right);

	CreateObjectAbove(Airship_Burnt, 682, 447);

	CreateObjectAbove(StrawMan, 1143, 567);
	CreateObjectAbove(StrawMan, 1321, 815);
	CreateObjectAbove(StrawMan, 1281, 814);
	var Fish001 = CreateObjectAbove(Fish, 235, 1141);
	Fish001->Unstick(7);
	var Fish002 = CreateObjectAbove(Fish, 189, 1136);
	Fish002->Unstick(7);
	var Fish003 = CreateObjectAbove(Fish, 233, 1131);
	Fish003->Unstick(7);
	var Fish004 = CreateObjectAbove(Fish, 103, 1251);
	Fish004->Unstick(7);
	var Fish005 = CreateObjectAbove(Fish, 88, 1255);
	Fish005->Unstick(7);
	var Fish006 = CreateObjectAbove(Fish, 79, 1268);
	Fish006->Unstick(7);

	var Bone001 = CreateObject(Bone, 1653, 886);
	Bone001->SetCon(96);
	Bone001->SetR(107);
	Bone001->SetPosition(1653, 886);
	var Bone002 = CreateObject(Bone, 1684, 829);
	Bone002->SetR(-10);
	Bone002->SetPosition(1684, 829);
	var Bone003 = CreateObject(Bone, 1761, 825);
	Bone003->SetCon(98);
	Bone003->SetR(-45);
	Bone003->SetPosition(1761, 825);
	var Bone004 = CreateObject(Bone, 2189, 824);
	Bone004->SetCon(92);
	Bone004->SetR(164);
	Bone004->SetPosition(2189, 824);
	var Bone005 = CreateObject(Bone, 2487, 799);
	Bone005->SetCon(92);
	Bone005->SetR(126);
	Bone005->SetPosition(2487, 799);
	var Bone006 = CreateObject(Bone, 2273, 960);
	Bone006->SetCon(92);
	Bone006->SetR(152);
	Bone006->SetPosition(2273, 960);

	var Skull001 = CreateObject(Skull, 1894, 816);
	Skull001->SetCon(97);
	Skull001->SetR(-37);
	Skull001->SetPosition(1894, 816);

	//var LavaCore001 = CreateObjectAbove(LavaCore, 2602, 848);
	//LavaCore001->SetCon(50);
	//LavaCore001->Unstick(7);
	var Rock001 = Lorry001->CreateContents(Rock);
	var Rock002 = Lorry001->CreateContents(Rock);
	var Rock003 = Lorry001->CreateContents(Rock);
	var Rock004 = Lorry002->CreateContents(Rock);
	var Rock005 = CreateObjectAbove(Rock, 413, 1351);
	Rock005->Unstick(7);
	var Rock006 = CreateObjectAbove(Rock, 370, 1350);
	Rock006->Unstick(7);

	var Coal001 = Lorry003->CreateContents(Coal);
	var Coal002 = Lorry002->CreateContents(Coal);

	var Wood001 = Lorry002->CreateContents(Wood);
	var Wood002 = Lorry003->CreateContents(Wood);
	var Wood003 = Lorry003->CreateContents(Wood);
	var Wood004 = Lorry003->CreateContents(Wood);

	var Crate001 = CreateObjectAbove(Crate, 403, 1319);
	Crate001->Unstick(7);

	var Metal001 = Crate001->CreateContents(Metal);
	var Metal002 = Crate001->CreateContents(Metal);
	var Metal003 = Crate001->CreateContents(Metal);
	var Metal004 = Crate001->CreateContents(Metal);
	var Metal005 = Crate001->CreateContents(Metal);

	var Barrel001 = CreateObject(Barrel, 856, 444);
	Barrel001->SetR(85);
	Barrel001->SetColor(0xff000000);
	Barrel001->SetPosition(856, 444);
	Barrel001->Unstick(7);
	var Barrel002 = CreateObject(Barrel, 848, 441);
	Barrel002->SetR(22);
	Barrel002->SetColor(0xff000000);
	Barrel002->SetPosition(848, 441);
	Barrel002->Unstick(7);
	var Barrel003 = CreateObjectAbove(Barrel, 910, 448);
	Barrel003->SetColor(0xff000000);
	var Barrel004 = CreateObjectAbove(Barrel, 633, 447);
	Barrel004->SetColor(0xff000000);
	Barrel004->Unstick(7);
	var Barrel005 = CreateObject(Barrel, 791, 380);
	Barrel005->SetR(-105);
	Barrel005->SetColor(0xff000000);
	Barrel005->SetPosition(791, 380);
	var Barrel006 = CreateObjectAbove(Barrel, 1011, 448);
	Barrel006->SetColor(0xff000000);

	CreateObjectAbove(PowderKeg, 419, 919);
	var PowderKeg001 = CreateObject(PowderKeg, 460, 915);
	PowderKeg001->SetR(116);
	PowderKeg001->SetPosition(460, 915);
	var PowderKeg002 = CreateObject(PowderKeg, 436, 914);
	PowderKeg002->SetR(-22);
	PowderKeg002->SetPosition(436, 914);
	CreateObjectAbove(PowderKeg, 404, 919);
	var PowderKeg003 = CreateObject(PowderKeg, 416, 870);
	PowderKeg003->SetPosition(416, 870);
	var PowderKeg004 = CreateObject(PowderKeg, 437, 871);
	PowderKeg004->SetPosition(437, 871);
	var PowderKeg005 = CreateObject(PowderKeg, 457, 893);
	PowderKeg005->SetR(41);
	PowderKeg005->SetPosition(457, 893);
	var PowderKeg006 = CreateObjectAbove(PowderKeg, 904, 695);
	PowderKeg006->Unstick(7);

	CreateObjectAbove(Crate, 862, 815);
	var Crate002 = CreateObjectAbove(Crate, 883, 815);
	Crate002->SetClrModulation(0xff784726);
	Crate002->SetObjectBlitMode(GFX_BLIT_Mod2);
	Crate002->Unstick(7);
	var Crate003 = CreateObjectAbove(Crate, 899, 815);
	Crate003->Unstick(7);
	var Crate004 = CreateObjectAbove(Crate, 932, 695);
	Crate004->Unstick(7);
	var Crate005 = CreateObjectAbove(Crate, 897, 447);
	Crate005->Unstick(7);

	var MetalBarrel001 = CreateObject(MetalBarrel, 829, 692);
	MetalBarrel001->SetR(75);
	MetalBarrel001->SetPosition(829, 692);
	var MetalBarrel002 = CreateObject(MetalBarrel, 834, 689);
	MetalBarrel002->SetR(-177);
	MetalBarrel002->SetPosition(834, 689);
	MetalBarrel002->Unstick(7);

	var Bucket001 = CreateObjectAbove(Bucket, 938, 695);
	Bucket001->Unstick(7);

	var Bread001 = Crate005->CreateContents(Bread);
	var Bread002 = Crate005->CreateContents(Bread);

	var Seeds001 = CreateObjectAbove(Seeds, 928, 567);
	Seeds001->Unstick(7);
	var Seeds002 = CreateObjectAbove(Seeds, 909, 567);
	Seeds002->Unstick(7);

	var Flour001 = CreateObjectAbove(Flour, 967, 447);
	Flour001->Unstick(7);
	var Flour002 = CreateObjectAbove(Flour, 964, 447);
	Flour002->Unstick(7);
	CreateObjectAbove(Flour, 957, 447);

	var Dynamite001 = CreateObjectAbove(Dynamite, 681, 519);
	Dynamite001->Unstick(7);
	var Dynamite002 = CreateObject(Dynamite, 685, 514);
	Dynamite002->SetR(27);
	Dynamite002->SetPosition(685, 514);
	Dynamite002->Unstick(7);
	var Dynamite003 = CreateObjectAbove(Dynamite, 689, 519);
	Dynamite003->Unstick(7);
	var Dynamite004 = CreateObject(Dynamite, 699, 518);
	Dynamite004->SetR(-96);
	Dynamite004->SetPosition(699, 518);
	Dynamite004->Unstick(7);

	var Hammer001 = Chest001->CreateContents(Hammer);
	var Hammer002 = Chest001->CreateContents(Hammer);
	var Hammer003 = Chest001->CreateContents(Hammer);

	var Shovel001 = Chest002->CreateContents(Shovel);
	var Shovel002 = Chest002->CreateContents(Shovel);
	var Shovel003 = Chest002->CreateContents(Shovel);

	var Sickle001 = Chest003->CreateContents(Sickle);
	var Sickle002 = Chest003->CreateContents(Sickle);
	var Sickle003 = Chest003->CreateContents(Sickle);

	var Axe001 = Chest004->CreateContents(Axe);
	var Axe002 = Chest004->CreateContents(Axe);
	var Axe003 = Chest004->CreateContents(Axe);

	var Pickaxe001 = Chest005->CreateContents(Pickaxe);
	var Pickaxe002 = CreateObject(Pickaxe, 359, 1349);
	Pickaxe002->SetR(68);
	Pickaxe002->SetPosition(359, 1349);

	var Helmet001 = CreateObjectAbove(Helmet, 695, 673);
	Helmet001->SetCategory(C4D_StaticBack);
	Helmet001->SetClrModulation(0xff6900ff);

	var Bow001 = CreateObjectAbove(Bow, 750, 675);
	Bow001->SetCategory(C4D_StaticBack);
	var Bow002 = CreateObjectAbove(Bow, 742, 674);
	Bow002->SetCategory(C4D_StaticBack);

	var Javelin001 = CreateObjectAbove(Javelin, 708, 671);
	Javelin001->SetCategory(C4D_StaticBack);
	var Javelin002 = CreateObjectAbove(Javelin, 705, 672);
	Javelin002->SetCategory(C4D_StaticBack);
	var Javelin003 = CreateObjectAbove(Javelin, 701, 673);
	Javelin003->SetCategory(C4D_StaticBack);

	CreateObjectAbove(Seaweed, 91, 1279);
	CreateObjectAbove(Seaweed, 187, 1263);
	CreateObjectAbove(Seaweed, 196, 1159);
	CreateObjectAbove(Seaweed, 240, 1151);
	CreateObjectAbove(Seaweed, 183, 1151);
	CreateObjectAbove(Seaweed, 57, 1303);

	var Sword001 = CreateObject(Sword, 1189, 425);
	Sword001->SetCategory(C4D_StaticBack);
	Sword001->SetR(21);
	Sword001->SetMeshMaterial("LaserSword", 0);
	Sword001->SetObjectLayer(Sword001);
	Sword001->SetPosition(1189, 425);
	var Firestone001 = CreateObject(Firestone, 2369, 1199);
	Firestone001->SetPosition(2369, 1199);
	var Firestone002 = CreateObject(Firestone, 2389, 1187);
	Firestone002->SetPosition(2389, 1187);
	var Firestone003 = CreateObject(Firestone, 2385, 1173);
	Firestone003->SetPosition(2385, 1173);
	var Firestone004 = CreateObject(Firestone, 2554, 1078);
	Firestone004->SetPosition(2554, 1078);
	var Firestone005 = CreateObject(Firestone, 2534, 1098);
	Firestone005->SetPosition(2534, 1098);
	var Firestone006 = CreateObject(Firestone, 2543, 1117);
	Firestone006->SetPosition(2543, 1117);
	var Firestone007 = CreateObject(Firestone, 2552, 1105);
	Firestone007->SetPosition(2552, 1105);
	var Firestone008 = CreateObject(Firestone, 2636, 1077);
	Firestone008->SetPosition(2636, 1077);
	var Firestone009 = CreateObject(Firestone, 2643, 1101);
	Firestone009->SetPosition(2643, 1101);
	var Firestone010 = CreateObject(Firestone, 2598, 1149);
	Firestone010->SetPosition(2598, 1149);
	var Firestone011 = CreateObject(Firestone, 2593, 1174);
	Firestone011->SetPosition(2593, 1174);
	var Firestone012 = CreateObject(Firestone, 2602, 1197);
	Firestone012->SetPosition(2602, 1197);
	var Firestone013 = CreateObject(Firestone, 2612, 1206);
	Firestone013->SetPosition(2612, 1206);
	var Firestone014 = CreateObject(Firestone, 2604, 1176);
	Firestone014->SetPosition(2604, 1176);
	var Firestone015 = CreateObject(Firestone, 2326, 1053);
	Firestone015->SetPosition(2326, 1053);
	var Firestone016 = CreateObject(Firestone, 2334, 1063);
	Firestone016->SetPosition(2334, 1063);
	var Firestone017 = CreateObject(Firestone, 2212, 1208);
	Firestone017->SetPosition(2212, 1208);
	var Firestone018 = CreateObject(Firestone, 2215, 1122);
	Firestone018->SetPosition(2215, 1122);
	var Firestone019 = CreateObject(Firestone, 2181, 1226);
	Firestone019->SetPosition(2181, 1226);
	var Firestone020 = CreateObject(Firestone, 1945, 1161);
	Firestone020->SetPosition(1945, 1161);
	var Firestone021 = CreateObject(Firestone, 1948, 1153);
	Firestone021->SetPosition(1948, 1153);
	var Firestone022 = CreateObject(Firestone, 2008, 1258);
	Firestone022->SetPosition(2008, 1258);
	var Firestone023 = CreateObject(Firestone, 2020, 1269);
	Firestone023->SetPosition(2020, 1269);
	var Firestone024 = CreateObject(Firestone, 2032, 1247);
	Firestone024->SetPosition(2032, 1247);

	var Deco_TunnelSupport001 = CreateObjectAbove(Deco_TunnelSupport, 714, 1017);
	Deco_TunnelSupport001->Extend(75);
	var Deco_TunnelSupport002 = CreateObjectAbove(Deco_TunnelSupport, 700, 697);
	Deco_TunnelSupport002->Extend(0);
	var Deco_TunnelSupport003 = CreateObjectAbove(Deco_TunnelSupport, 744, 696);
	Deco_TunnelSupport003->Extend(0);
	var Deco_TunnelSupport004 = CreateObjectAbove(Deco_TunnelSupport, 722, 696);
	Deco_TunnelSupport004->Extend(25);
	var Deco_TunnelSupport005 = CreateObjectAbove(Deco_TunnelSupport, 452, 1352);
	Deco_TunnelSupport005->Extend(13);
	var Deco_TunnelSupport006 = CreateObjectAbove(Deco_TunnelSupport, 1009, 978);
	Deco_TunnelSupport006->Extend(62);
	var Deco_TunnelSupport007 = CreateObjectAbove(Deco_TunnelSupport, 696, 521);
	Deco_TunnelSupport007->Extend(0);
	var Deco_TunnelSupport008 = CreateObjectAbove(Deco_TunnelSupport, 739, 521);
	Deco_TunnelSupport008->Extend(0);
	var Deco_TunnelSupport009 = CreateObjectAbove(Deco_TunnelSupport, 873, 1041);
	Deco_TunnelSupport009->Extend(75);

	var EnvPack_Rail001 = CreateObjectAbove(EnvPack_Rail, 1056, 816);
	EnvPack_Rail001.Plane = 710;
	var EnvPack_Rail002 = CreateObjectAbove(EnvPack_Rail, 1057, 696);
	EnvPack_Rail002.Plane = 710;
	var EnvPack_Rail003 = CreateObjectAbove(EnvPack_Rail, 1056, 320);
	EnvPack_Rail003.Plane = 710;
	var EnvPack_Rail004 = CreateObjectAbove(EnvPack_Rail, 1056, 449);
	EnvPack_Rail004.Plane = 710;
	var EnvPack_Rail005 = CreateObjectAbove(EnvPack_Rail, 1056, 568);
	EnvPack_Rail005.Plane = 710;
	return true;
}