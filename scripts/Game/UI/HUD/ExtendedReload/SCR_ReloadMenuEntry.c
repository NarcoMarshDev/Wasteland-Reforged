//------------------------------------------------------------------------------------------------
class SCR_ReloadMenuEntry : ScriptedSelectionMenuEntry
{
	protected ChimeraCharacter m_pCharacter;
	
	//------------------------------------------------------------------------------------------------
	//! Callback for when this entry is supposed to be performed
	override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
	{	
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(m_pCharacter.FindComponent(SlotManagerComponent));
		Print(slotManager);
		if (!slotManager)
			return;
		
		array<EntitySlotInfo> slots = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slots);
		Print(slots);
		for (int i = 0; i < slots.Count(); i++)
		{
			// thanks to alnyan in the arma discord for this (discord id: 394764165392105472)
			IEntity ent = slots[i].GetAttachedEntity();
			if (!ent)
				continue;
			
			vector entTransform[4];
			slots[i].DetachEntity(false);
			ent.GetWorldTransform(entTransform);
			ent.GetParent().RemoveChild(ent);
			ent.SetWorldTransform(entTransform);
			
		}		
		super.OnPerform(user, sourceMenu);
	}
	//------------------------------------------------------------------------------------------------
	//! Can this entry be shown?
	override bool CanBeShownScript(IEntity user, BaseSelectionMenu sourceMenu)
	{
		return true;
	}
	//------------------------------------------------------------------------------------------------
	//! Can this entry be performed?
	override bool CanBePerformedScript(IEntity user, BaseSelectionMenu sourceMenu)
	{
		if (!m_pCharacter)
			return false;

		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool GetEntryNameScript(out string outName)
	{	
		outName = "GetEntryNameScript";
		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool GetEntryDescriptionScript(out string outDescription)
	{	
		outDescription = "GetEntryDescriptionScript";
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
				img.SetOpacity(0.5);

				Color col = GetLoadColor(50);
				img.SetColor(col);
			}
		}
		return entry;
	}
	
	//------------------------------------------------------------------------------------------------
	
	protected Color GetLoadColor(int loadPercentage)
	{
		if (loadPercentage > 75)
			return Color.White;

		if (loadPercentage > 50)
			return Color.Yellow;

		if (loadPercentage > 25)
			return Color.Orange;

		if (loadPercentage > 0)
			return Color.Red;

		return Color.Black;
	}
	
	//------------------------------------------------------------------------------------------------
	void SCR_ReloadMenuEntry(notnull ChimeraCharacter character)
	{
		m_pCharacter = character;
	}

	//------------------------------------------------------------------------------------------------
	void ~SCR_ReloadMenuEntry()
	{
		m_pCharacter = null;
	}
}