// During the first battle, resupply the chests

static const ResupplyChest = new Effect {
	Construction = func () {
		// Remember the contents of the chest
		var contents = CreateArray();
		for (var i = 0; i < this.Target->ContentsCount(); i++)
			PushBack(contents, this.Target->Contents(i)->GetID());
		RemoveDuplicates(contents);
		this.contents = contents;
	},
	Timer = func() {
		// Check if any of the original contents are depleted
		for (var content in this.contents)
		{
			if (!this.Target->FindContents(content))
				this.Target->CreateContents(content, Random(4)+1);
		}
	}
};