// The sequence hides all extra goal displays when playing

#appendto Sequence

public func Start(string name, int progress, ...)
{
	_inherited(name, progress, ...);

	HideExtraGoalTexts();
}

public func Stop(bool no_remove)
{
	_inherited(no_remove, ...);

	ShowExtraGoalTexts();
}