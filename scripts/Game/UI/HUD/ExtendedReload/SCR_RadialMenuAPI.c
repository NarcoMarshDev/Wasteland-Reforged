// From "Extended Reload" by ex0

//------------------------------------------------------------------------------------------------
modded class SCR_RadialMenuHandler : ScriptedSelectionMenu
{
	void TryPerformCurrentSelection(IEntity owner)
	{
		TryPerformEntry(owner);
	}
}