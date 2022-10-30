/*!
Class used to hold UI name.
Data are intentionally *READ ONLY*, because the class is often used on prefabs, not instances.
Instead of adding SetXXX() functions here, consider using specialized inherited class.
*/

// #WR_MODDED - Breaking the rules here because casting to SCR_OverridableUIInfo and using SetName() is not working
// #ESE_REPLACE - Add to ESE_ModdedClasses.c with _ESE_SetName() method
[BaseContainerProps(), BaseContainerCustomTitleField("Name")]
modded class SCR_UIName
{
	void _WR_SetName(LocalizedString str)
	{
		Name = str;
	}
};