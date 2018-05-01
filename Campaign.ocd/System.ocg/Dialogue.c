// Adds an additional option to the MessageBox command which displays
// short messages above the talker, to inform other players of what's going on.

// Short messages should never be more than two lines of text. The name of the talker is added
// so it's 3 lines in total

// Also adds the option to turn off direction changes when the dialogue starts as it is
// unwanted with certain working clonks.

#appendto Dialogue

local Dialogue_ShortMsg_Duration = 105;

private func MessageBox(string message, object clonk, object talker, int for_player, bool as_message, array options, proplist menu_target, string short_msg)
{
	_inherited(message, clonk, talker, for_player, as_message, options, menu_target);

	if (short_msg == nil)
		return;

	var color = talker->GetColor();
	var name = talker->GetName();
	var yoff = 0;

	var other_dialogues = FindObjects(Find_ID(GetID()), Find_Distance(140), Find_AnyLayer(), Find_Exclude(this));
	for (var other_dialogue in other_dialogues)
		// Only take other dialogues into account which are currently displaying messages
		if (GetEffect("Dialogue_ShortMsg", other_dialogue))
			// Only take dialogues into account that are roughly at the same y level
			if (Inside(GetY(), other_dialogue->GetY() - 20, other_dialogue->GetY() + 20))
				// Show the message higher up
				yoff -= 25;

	CustomMessage(Format("@<c %x>%s:</c>|%s", color, name, short_msg), talker, NO_OWNER, 0, yoff);
	this->CreateEffect(Dialogue_ShortMsg, 2, Dialogue_ShortMsg_Duration, talker);
}

// A simple temporary effect to remember that a message is displayed
local Dialogue_ShortMsg = new Effect {
	Construction = func (object talker) {
		this.talker = talker;
	},
	Timer = func () {
		return -1;
	},
	Destruction = func () {
		// Clear the message from the talker
		if (this.talker)
			CustomMessage("", this.talker);
	},
	Effect = func (string new_name, object talker) {
		// Eat new effects up
		if (new_name == "Dialogue_ShortMsg")
			return -2;
	},
	Add = func (string new_name, int new_timer, object talker) {
		if (talker != this.talker && this.talker)
			CustomMessage("", this.talker);
		this.talker = talker;
		this.Interval = new_timer;
		this.Time = 0;
	}
};

public func SetSpeakerDirs(object speaker1, object speaker2)
{
	if (this.NoSpeakerTurn) return false;
	return _inherited(speaker1, speaker2);
}