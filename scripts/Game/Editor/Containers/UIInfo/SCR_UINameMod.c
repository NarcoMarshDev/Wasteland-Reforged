/*!
Class used to hold UI name.
Data are intentionally *READ ONLY*, because the class is often used on prefabs, not instances.
Instead of adding SetXXX() functions here, consider using specialized inherited class.
*/
[BaseContainerProps(), BaseContainerCustomTitleField("Name")]
modded class SCR_UIName
{
	void SetName(LocalizedString str)
	{
		Name = str;
	}
};