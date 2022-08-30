//------------------------------------------------------------------------------------------------
class WR_BuildMenuEntry : ScriptedSelectionMenuEntry
{
	protected ChimeraCharacter m_pCharacter;
	protected BuildMenuEntryType m_pEntryType;
	protected string m_pIconPath;
	protected string m_pName;
	protected string m_pDescription;
	
	//------------------------------------------------------------------------------------------------
	//! Callback for when this entry is supposed to be performed
	override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
	{
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(m_pCharacter.FindComponent(SlotManagerComponent));
		Print(slotManager);
		if (!slotManager)
			return;
		
		array<EntitySlotInfo> slotInfos = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slotInfos);
		Print(slotInfos);
		
		foreach (EntitySlotInfo slot : slotInfos)
		{
			IEntity ent = slot.GetAttachedEntity();
			if (!ent)
				continue;
			
			switch (m_pEntryType)
			{
				case BuildMenuEntryType.PLACE:
			
					vector entTransform[4];
					slot.DetachEntity(false);
					ent.GetWorldTransform(entTransform);
					ent.GetParent().RemoveChild(ent);
					ent.SetWorldTransform(entTransform);
					break;
			
				case BuildMenuEntryType.SNAP:
			
					vector entTransform[4];
					vector entAngles[3];
					slot.DetachEntity(false);
					ent.GetWorldTransform(entTransform);
					SCR_TerrainHelper.SnapToTerrain(entTransform, GetGame().GetWorld(), true);
					ent.GetParent().RemoveChild(ent);
					ent.SetWorldTransform(entTransform);
					break;
			
				case BuildMenuEntryType.LEFT:
			
					vector entAngles;
					entAngles = ent.GetAngles();
					Print("old angle: " + entAngles[1]);
					entAngles[1] = ent.GetAngles()[1] - 45;
					ent.SetAngles(entAngles);
					Print("new angle: " + entAngles[1]);
					break;
			
				case BuildMenuEntryType.RIGHT:
			
					vector entAngles;
					entAngles = ent.GetAngles();
					Print("old angle: " + entAngles[1]);
					entAngles[1] = ent.GetAngles()[1] + 45;
					ent.SetAngles(entAngles);
					Print("new angle: " + entAngles[1]);
					break;
			
				default:
					Print("No BuildMenuEntryType found", LogLevel.WARNING);
					break;
			}		
		}
		
		
		
		
		#ifdef WR_BROKEN
		if (!m_pCharacter.GetChildren())
			return;
		
		SCR_SiteSlotEntity buildingSlot = SCR_SiteSlotEntity.Cast( m_pCharacter.GetChildren() );
		
		if (!buildingSlot)
			return;
		
		IEntity ent = buildingSlot.GetOccupant();
		
		if (!ent)
			return;
		
		switch (m_pEntryType)
		{
			case BuildMenuEntryType.PLACE:
			
				vector entTransform[4];
				buildingSlot.SetOccupant(null);
				ent.GetWorldTransform(entTransform);
				ent.GetParent().RemoveChild(ent);
				ent.SetWorldTransform(entTransform);
				break;
			
			case BuildMenuEntryType.SNAP:
			
				vector entTransform[4];
				vector entAngles[3];
				buildingSlot.SetOccupant(null);
				ent.GetWorldTransform(entTransform);
				SCR_TerrainHelper.SnapToTerrain(entTransform, GetGame().GetWorld(), true);
				ent.GetParent().RemoveChild(ent);
				ent.SetWorldTransform(entTransform);
				break;
			
			case BuildMenuEntryType.LEFT:
			
				vector entAngles;
				entAngles = ent.GetAngles();
				Print("old angle: " + entAngles[1]);
				entAngles[1] = ent.GetAngles()[1] - 45;
				ent.SetAngles(entAngles);
				Print("new angle: " + entAngles[1]);
				break;
			
			case BuildMenuEntryType.RIGHT:
			
				vector entAngles;
				entAngles = ent.GetAngles();
				Print("old angle: " + entAngles[1]);
				entAngles[1] = ent.GetAngles()[1] + 45;
				ent.SetAngles(entAngles);
				Print("new angle: " + entAngles[1]);
				break;
			
			default:
				Print("No BuildMenuEntryType found", LogLevel.WARNING);
				break;
		}
		
		SCR_CharacterControllerComponent.Cast(m_pCharacter.FindComponent(SCR_CharacterControllerComponent)).SetDynamicSpeed(1.0);
		
		super.OnPerform(user, sourceMenu);
		
		#endif
				
		// ---------------- OLD CODE OBSOLETE ---------------- //
		
		#ifdef WR_OBSOLETE
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
			
			if (m_pEntryType == BuildMenuEntryType.PLACE)
			{
				vector entTransform[4];
				slots[i].DetachEntity(false);
				ent.GetWorldTransform(entTransform);
				ent.GetParent().RemoveChild(ent);
				ent.SetWorldTransform(entTransform);
			}
			
			else if (m_pEntryType == BuildMenuEntryType.SNAP)
			{
				vector entTransform[4];
				vector entAngles[3];
				slots[i].DetachEntity(false);
				ent.GetWorldTransform(entTransform);
				SCR_TerrainHelper.SnapToTerrain(entTransform, GetGame().GetWorld(), true);
				ent.GetParent().RemoveChild(ent);
				ent.SetWorldTransform(entTransform);
			}
			
			else if (m_pEntryType == BuildMenuEntryType.LEFT)
			{
				vector entAngles;
				entAngles = ent.GetAngles();
				Print("old angle: " + entAngles[1]);
				entAngles[1] = ent.GetAngles()[1] - 45;
				ent.SetAngles(entAngles);
				Print("new angle: " + entAngles[1]);
			}
			
			else if (m_pEntryType == BuildMenuEntryType.RIGHT)
			{
				vector entAngles;
				entAngles = ent.GetAngles();
				Print("old angle: " + entAngles[1]);
				entAngles[1] = ent.GetAngles()[1] + 45;
				ent.SetAngles(entAngles);
				Print("new angle: " + entAngles[1]);
			}
		}
		
		SCR_CharacterControllerComponent.Cast(m_pCharacter.FindComponent(SCR_CharacterControllerComponent)).SetDynamicSpeed(1.0);
		
		super.OnPerform(user, sourceMenu);
		#endif
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
		outName = m_pName;
		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool GetEntryDescriptionScript(out string outDescription)
	{	
		outDescription = m_pDescription;
		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool GetEntryIconPathScript(out string outIconPath)
	{	
		outIconPath = m_pIconPath;
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

				Color col = GetColor();
				img.SetColor(col);
			}
		}
		return entry;
	}
	
	//------------------------------------------------------------------------------------------------
	
	protected Color GetColor()
	{
		if (m_pEntryType == BuildMenuEntryType.PLACE)
			return Color.Green;

		if (m_pEntryType == BuildMenuEntryType.SNAP)
			return Color.Yellow;

		if (m_pEntryType == BuildMenuEntryType.LEFT)
			return Color.Orange;

		if (m_pEntryType == BuildMenuEntryType.RIGHT)
			return Color.Orange;

		return Color.Black;
	}
	
	//------------------------------------------------------------------------------------------------
	void WR_BuildMenuEntry(notnull ChimeraCharacter character, BuildMenuEntryType entryType)
	{
		m_pCharacter = character;
		m_pEntryType = entryType;
		
		m_pDescription = "Left Click To Select";
		
		// set entry icon and name depending on entry type
		
		if (m_pEntryType == BuildMenuEntryType.PLACE)
		{
			m_pIconPath = "{8657510D7940031F}UI/Textures/Workshop/DownloadManager/DownloadManager_DownloadingIcon_UI.edds";
			m_pName = "Place As Is";
		}
		else if (m_pEntryType == BuildMenuEntryType.SNAP)
		{
			m_pIconPath = "{BBA44B0A0EEFB9D9}UI/Textures/icon_snaptosurface.edds";
			m_pName = "Snap To Surface";
		}
		else if (m_pEntryType == BuildMenuEntryType.LEFT)
		{
			m_pIconPath = "{93C33F624F012AAA}UI/Textures/icon_right.edds";
			m_pName = "Rotate Anti-Clockwise";
		}
		else if (m_pEntryType == BuildMenuEntryType.RIGHT)
		{
			m_pIconPath = "{5D33E0725DB71976}UI/Textures/icon_left.edds";
			m_pName = "Rotate Clockwise";
		}
		
		
		
		else
		{
			m_pIconPath = "{A3D157619860A564}UI/Textures/Editor/Attributes/Arsenal/Attribute_Arsenal_Magazines.edds";
		}
	}

	//------------------------------------------------------------------------------------------------
	void ~WR_BuildMenuEntry()
	{
		m_pCharacter = null;
	}
}