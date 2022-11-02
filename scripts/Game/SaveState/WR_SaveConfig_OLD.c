[BaseContainerProps(configRoot: true)]
class WR_SaveConfigRoot
{
	[Attribute(defvalue: "Save", uiwidget: UIWidgets.Auto, desc: "Save state name")]
	string m_sTitle;

	[Attribute(uiwidget: UIWidgets.Auto, desc: "Save Data Categories")]
	ref array<ref WR_SaveConfigCategory> m_aCategories;
}

//------------------------------------------------------------------------------------------------

[BaseContainerProps(configRoot: true)]
class WR_SaveConfigCategory
{
	[Attribute(defvalue: "Entity Category", uiwidget: UIWidgets.Auto, desc: "Entity category name")]
	string m_sTitle;
	
	[Attribute(uiwidget: UIWidgets.Auto, desc: "Entity to be saved")]
	ref array<ref WR_SaveConfigEntry> m_aEntries;
}

//------------------------------------------------------------------------------------------------

[BaseContainerProps(insertable: false)]
class WR_SaveConfigEntry
{
	[Attribute(uiwidget: UIWidgets.Object)]
	IEntity m_Entity;

	
}

//------------------------------------------------------------------------------------------------

