[BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("m_sTitle", "Standard: %1")]
class WR_FieldManualConfigEntry_Store : SCR_FieldManualConfigEntry
{
	[Attribute(uiwidget: UIWidgets.ResourceNamePicker, params: "et")]
	ResourceName m_ResourceToSpawn;
	
	[Attribute(uiwidget: UIWidgets.Auto, params: "price")]
	int m_iPrice;
	
	[Attribute(defvalue: "{E7BD479E6A46BCC4}UI/layouts/Menus/FieldManual/Entries/FieldManual_Entry_Standard.layout", uiwidget: UIWidgets.EditBoxWithButton, params: "layout")]
	ResourceName m_Layout;



	//------------------------------------------------------------------------------------------------
	void WR_FieldManualConfigEntry_Store()
	{
		if (m_Layout.Trim().IsEmpty())
			m_Layout = "{E7BD479E6A46BCC4}UI/layouts/Menus/FieldManual/Entries/FieldManual_Entry_Standard.layout";
	}

	//------------------------------------------------------------------------------------------------
	override Widget CreateWidget(notnull Widget parent)
	{
		return CreateWidgetFromLayout(m_Layout, parent);
	}
};
