// Important for story development!
// Advanced Telltale features

static const PLAYER_SAVE_NAME_PROGRESS = "CoNCampaign_Progress";
static const PLAYER_SAVE_NAME_DATA = "CoNCampaign_Data";
static const PLAYER_SAVE_NAME_MISSION = "CoNCampaign_Mission";

static const ONE_SECOND = 34; // in frames
static const QUESTION_TIME = 30; // in seconds

/* Memory */

// Saves and restores the character progress across campaign missions

global func InitCharacterDevelopment(int mission_number, int player)
{
	// Check if a mission is played over again
	var progress = GetPlrExtraData(player, PLAYER_SAVE_NAME_PROGRESS) ?? 0;
	if (GetType(progress) == C4V_String)
		progress = ConvertInt(progress);

	// Replaying a scenario
	if (mission_number <= progress)
	{
		// Show a warning message
		CustomMessage("$MissionRepeatWarning$", nil, player, nil, 75, nil, GUI_MenuDeco, Icon_MissionWarning, MSG_Multiple|MSG_HCenter|MSG_VCenter);
	}
	// Skipping at least one scenario
	if (mission_number > progress + 1)
	{
		// Show a warning message
		CustomMessage("$MissionSkipWarning$", nil, player, nil, 75, nil, GUI_MenuDeco, Icon_MissionWarning, MSG_Multiple|MSG_HCenter|MSG_VCenter);
		// In multiplayer: give another player the chance of applying the progress values, so delay the value setting a bit
		Schedule(nil, "DelayedCharacterDevelopment()", 35);
		return;
	}

	// In a multiplayer game, the first joining player fixes the progress
	if (Scenario.Campaign_Init)
	{
		if (progress + 1 == mission_number)
		{
			var vals = GetPlrExtraData(player, PLAYER_SAVE_NAME_DATA);
			if (!vals)
				return;

			vals = RegexSplit(vals, "\\s*[,]\\s*");
			if (ParseInt(vals[0]) != Scenario.Campaign_PaulaVal || ParseInt(vals[1]) != Scenario.Campaign_SamuelVal)
				// Show a warning that the first player has fixed a different progress
				CustomMessage("$MissionDifferentWarning$", nil, player, nil, 75, nil, GUI_MenuDeco, Icon_MissionWarning, MSG_Multiple|MSG_HCenter|MSG_VCenter);
		}
		return;
	}

	Scenario.Campaign_CurrentMission = mission_number;

	// First mission (the watchtower) will always reset the numbers if you play it over again!
	if (mission_number == 1)
	{
		Scenario.Campaign_PaulaVal = 50;
		Scenario.Campaign_SamuelVal = 50;
	} else {
		var vals = GetPlrExtraData(player, PLAYER_SAVE_NAME_DATA);
		if (vals)
			vals = RegexSplit(vals, "\\s*[,]\\s*");
		else
		{} // ????

		Scenario.Campaign_PaulaVal = ParseInt(vals[0]);
		Scenario.Campaign_SamuelVal = ParseInt(vals[1]);
	}

	Scenario.Campaign_Init = true;
}

global func DelayedCharacterDevelopment()
{
	// Already initialised: another player must have set the values
	if (Scenario.Campaign_Init)
		return;
	// Otherwise get random values from the scenario. It should know what values are feasible
	var vals = GameCall("GetRandomProgress");
	Scenario.Campaign_PaulaVal = vals[0];
	Scenario.Campaign_SamuelVal = vals[1];
	Scenario.Campaign_Init = true;
}

global func SaveCharacterDevelopment(string mission_specific)
{
	var mission_progress = Format("%2.2d", Scenario.Campaign_CurrentMission);
	var mission_data = Format("%3.3d,%3.3d", Scenario.Campaign_PaulaVal, Scenario.Campaign_SamuelVal);
	var mission_specific_name = Format("%s%d", PLAYER_SAVE_NAME_MISSION, Scenario.Campaign_CurrentMission);

	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);

		SetPlrExtraData(plr, PLAYER_SAVE_NAME_PROGRESS, mission_progress);
		SetPlrExtraData(plr, PLAYER_SAVE_NAME_DATA, mission_data);
		if (mission_specific)
			SetPlrExtraData(plr, mission_specific_name, mission_specific);
	}
}

// Changes the attitude a character has towards you.
// Can only be between 0 and 100
// 0 means the characters doesn't like you very much
// 100 means you're practically best friends with that character

global func CharacterPaulaChange(int value)
{
	Scenario.Campaign_PaulaVal = BoundBy(Scenario.Campaign_PaulaVal + value, 0, 100);
}

global func CharacterSamuelChange(int value)
{
	Scenario.Campaign_SamuelVal = BoundBy(Scenario.Campaign_SamuelVal + value, 0, 100);
}

global func GetPaulaAttitude()
{
	return Scenario.Campaign_PaulaVal;
}

global func GetSamuelAttitude()
{
	return Scenario.Campaign_SamuelVal;
}

global func ConvertInt(string number)
{
	var decade = GetChar(number, 0);
	var num = GetChar(number, 1);
	return (decade - 48) * 10 + (num - 48);
}

/* Question asking */

// These questions mark important dialogues during the campaign and will decide how important character interact with you

global func GetImportantQuestionColor() { return "ebd024"; }

global func AskImportantQuestion(object talker, string question, string answer1, string answer2, string callback_1, string callback_2)
{
	if (!this || this->GetID() != Sequence)
		return;

	this.q_first_win = callback_1;
	this.q_second_win = callback_2;
	this.q_outcome = [0,0];

	this.q_running = true;
	this.q_time = 0;
	this.q_talker = talker;

	this.q_question = question;
	this.q_answer1 = answer1;
	this.q_answer2 = answer2;

	for(var i = 0; i < GetPlayerCount(C4PT_User); ++i)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		var clonk = GetCrew(plr, 0);
		QuestionBox(question, clonk, talker, plr, [[answer1, "FirstAnswer"], [answer2, "SecondAnswer"]], this);
	}

	//talker->Message("@$QuestionMsg$", QUESTION_TIME);
	UpdateExtraDisplays(Format("$QuestionMsg$", QUESTION_TIME), talker);

	ScheduleCall(this, "QuestionTick", ONE_SECOND, nil, talker);
}

global func QuestionTick(object talker)
{
	this.q_time++;

	if (this.q_time >= QUESTION_TIME)
		return ForceQuestionEnd();

	//talker->Message("@$QuestionMsg$", QUESTION_TIME - this.q_time);
	UpdateExtraDisplays(Format("$QuestionMsg$", QUESTION_TIME - this.q_time), talker);

	ScheduleCall(this, "QuestionTick", ONE_SECOND, nil, talker);
}

global func QuestionBox(string message, object clonk, object talker, int for_player, array options, proplist menu_target)
{
	if (!this.q_repeat)
		message = Format("<c %x>%s:</c> %s|$ImportantMsg$", talker->GetColor(), talker->GetName(), message);
	else
		message = Format("<c %x>%s:</c> %s|$ImportantRepeat$", talker->GetColor(), talker->GetName(), message);

	var portrait = talker->~GetPortrait();

	var cmd;
	if (menu_target) cmd = "MenuOK";

	clonk->CreateMenu(menu_target->GetID(), menu_target, C4MN_Extra_None, nil, nil, C4MN_Style_Dialog, false, menu_target->GetID());
	var menu_item_offset = 0;

	// Add NPC portrait.
	if (portrait)
		menu_item_offset += clonk->AddMenuItem("", nil, Dialogue, nil, clonk, nil, C4MN_Add_ImgPropListSpec, portrait);
	else
		menu_item_offset += clonk->AddMenuItem("", nil, Dialogue, nil, clonk, nil, C4MN_Add_ImgObject, talker);

	// Add NPC message.
	menu_item_offset += clonk->AddMenuItem(message, nil, nil, nil, clonk, nil, C4MN_Add_ForceNoDesc);

	// Add answers.
	for (var option in options)
	{
		var option_text, option_command;
		if (GetType(option) == C4V_Array)
		{
			// Text+Command given
			option_text = option[0];
			option_command = option[1];
			if (!WildcardMatch(option_command, "*(*")) option_command = Format("%s(Object(%d))", option_command, clonk->ObjectNumber());
		}
		clonk->AddMenuItem(option_text, option_command, nil, nil, clonk, nil, C4MN_Add_ForceNoDesc);
	}

	// Set menu decoration.
	clonk->SetMenuDecoration(GUI_MenuDeco);

	var name = talker->GetName();
	var n_length;
	while (GetChar(name, n_length))
		n_length++;
	clonk->SetMenuTextProgress(n_length + 1);
}

global func FirstAnswer(object clonk)
{
	if (!clonk || !this.q_running) return;

	this.q_outcome[0]++;

	CheckQuestionFinished();
}

global func SecondAnswer(object clonk)
{
	if (!clonk || !this.q_running) return;

	this.q_outcome[1]++;

	CheckQuestionFinished();
}

global func CheckQuestionFinished()
{
	var answers = this.q_outcome[0] + this.q_outcome[1];
	if (answers >= GetPlayerCount(C4PT_User))
		EndImportantQuestion();
}

global func ForceQuestionEnd()
{
	// Every remaining player just votes for whatever answer is selected

	for(var i = 0; i < GetPlayerCount(C4PT_User); ++i)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		var clonk = GetCrew(plr, 0);
		var answer = clonk->GetMenuSelection();
		if (answer != nil)
		{
			if (answer == 2) // First answer
				this.q_outcome[0]++;
			if (answer == 3) // Second answer
				this.q_outcome[1]++;
			clonk->CloseMenu();
		}
	}

	EndImportantQuestion();
}

global func EndImportantQuestion()
{
	this.q_running = nil;
	//this.q_talker->Message("");
	UpdateExtraDisplays("", nil);
	ClearScheduleCall(this, "QuestionTick");

	if (this.q_outcome[0] > this.q_outcome[1])
	{
		this->Call(this.q_first_win);

		this.q_first_win = nil;
		this.q_second_win = nil;
		this.q_outcome = nil;
		this.q_question = nil;
		this.q_answer1 = nil;
		this.q_answer2 = nil;
	} else if (this.q_outcome[1] > this.q_outcome[0])
	{
		this->Call(this.q_second_win);

		this.q_first_win = nil;
		this.q_second_win = nil;
		this.q_outcome = nil;
		this.q_question = nil;
		this.q_answer1 = nil;
		this.q_answer2 = nil;
	} else {
		// Repeat the question once on a tie
		if (!this.q_repeat)
		{
			this.q_repeat = true;
			AskImportantQuestion(this.q_talker, this.q_question, this.q_answer1, this.q_answer2, this.q_first_win, this.q_second_win);
		} else {
			// Two ties in a row! Stalemate. Pick an answer at random :(
			if (Random(2))
				this->Call(this.q_first_win);
			else
				this->Call(this.q_second_win);

			this.q_first_win = nil;
			this.q_second_win = nil;
			this.q_outcome = nil;
			this.q_question = nil;
			this.q_answer1 = nil;
			this.q_answer2 = nil;
		}
	}
}

/* Display the yellow question mark when an important question is available */

global func AddImportantQuestionMark(object dlg_object, string gamecall_check)
{
	return dlg_object->CreateEffect(ImportantQuestionMarkEffect, 1, 5, gamecall_check);
}

static const ImportantQuestionMarkEffect = {
	Construction = func(string callback) {
		this.gamecall = callback;

		var mark = CreateObject(Icon_QuestionMark, 0,0, NO_OWNER);
		mark->SetObjectLayer(mark);
		mark->SetGraphics("Ingame");
		mark->SetCon(40);
		mark.Plane = 899;

		this.mark = mark;
	},
	Timer = func(int time) {
		if (!this.mark)
			return -1;
		// Remove once the question has been asked and answered
		if (GameCall(this.gamecall))
			return -1;

		var x_pos = this.Target->GetX();
		if (this.Target.dlg_attention)
			x_pos += 10;
		var y_pos = this.Target->GetY() - 28;

		y_pos += Sin(time%360*5, 3);

		this.mark->SetPosition(x_pos, y_pos);
	},
	Destruction = func() {
		if (this.mark)
			this.mark->RemoveObject();
	}
};