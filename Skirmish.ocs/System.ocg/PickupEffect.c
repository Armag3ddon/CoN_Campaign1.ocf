// Some items are not to be collected and if tried, nearby soldiers will tell the player to cut the crap

static const ImportantGear = new Effect {
	Construction = func (object keeper) {
		this.keeper = keeper;
	},
	Pickup = func(object clonk) { // Called by a clonk that tries to pickup the item (see Clonk.c)
		var dialogue = Dialogue->FindByTarget(this.keeper);
		if (dialogue)
			dialogue->Call(Format("Dlg_%s_Pickup", dialogue.dlg_name), clonk);
	}
};