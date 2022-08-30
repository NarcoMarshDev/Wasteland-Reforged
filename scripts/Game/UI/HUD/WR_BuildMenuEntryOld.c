class WR_BuildMenuEntryOld : ScriptedSelectionMenuEntry
{
	protected ChimeraCharacter m_pCharacter;

	override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
	{
		
		// do action here
		
		super.OnPerform(user, sourceMenu);
	}

	override bool CanBeShownScript(IEntity user, BaseSelectionMenu sourceMenu)
	{
		return true;
	}

	override bool CanBePerformedScript(IEntity user, BaseSelectionMenu sourceMenu)
	{
		if (!m_pCharacter)
			return false;

		return true;
	}

	override bool GetEntryNameScript(out string outName)
	{
		outName = "outName";
		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool GetEntryDescriptionScript(out string outDescription)
	{
		outDescription = "outDescription";
		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool GetEntryIconPathScript(out string outIconPath)
	{
		outIconPath = "{A3D157619860A564}UI/Textures/Editor/Attributes/Arsenal/Attribute_Arsenal_Magazines.edds";
		return true;
	}
	//------------------------------------------------------------------------------------------------
	/*
	 TODO: This is a big ugly hack, there is no other callback that would work
	*/
	override SCR_SelectionEntryWidgetComponent GetEntryComponent()
	{
		SCR_SelectionEntryWidgetComponent entry = super.GetEntryComponent();
		if (entry)
		{
			ImageWidget img = entry.GetIcon();
			if (img)
			{
				float opacity = 1.0;
				img.SetOpacity(opacity);

				Color col = GetLoadColor();
				img.SetColor(col);
			}
		}
		return entry;
	}
	//------------------------------------------------------------------------------------------------
	protected Color GetLoadColor()
	{
		return Color.Black;
	}
	//------------------------------------------------------------------------------------------------
	protected string GetLoadString()
	{
		return "EMPTY";
	}
	
	//------------------------------------------------------------------------------------------------
	void WR_BuildMenuEntryOld(notnull ChimeraCharacter character, notnull BaseWeaponComponent weapon, notnull MagazineComponent magazine)
	{
		m_pCharacter = character;
	}

	//------------------------------------------------------------------------------------------------
	void ~WR_BuildMenuEntryOld()
	{
		m_pCharacter = null;
	}
}