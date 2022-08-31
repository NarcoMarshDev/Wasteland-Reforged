//------------------------------------------------------------------------------------------------
class WR_BuildMenuEntry : ScriptedSelectionMenuEntry
{
	protected ChimeraCharacter m_pCharacter;
	protected BuildMenuEntryType m_pEntryType;
	protected string m_pIconPath;
	protected string m_pName;
	protected string m_pDescription;
	protected Color m_IconColor;
	
	//------------------------------------------------------------------------------------------------
	//! Callback for when this entry is supposed to be performed
	override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
	{
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(m_pCharacter.FindComponent(SlotManagerComponent));
		if (!slotManager)
			return;
		
		array<EntitySlotInfo> slotInfos = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slotInfos);
		
		// this may be able to be replaced by just referencing slotInfos[0] instead of using a normally single loop foreach loop
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
					WR_Statics.RestoreMaterial(ent);
					break;
			
				case BuildMenuEntryType.SNAP:
			
					vector entTransform[4];
					vector entAngles[3];
					slot.DetachEntity(false);
					ent.GetWorldTransform(entTransform);
					SCR_TerrainHelper.SnapToTerrain(entTransform, GetGame().GetWorld(), true);
					ent.GetParent().RemoveChild(ent);
					ent.SetWorldTransform(entTransform);
					WR_Statics.RestoreMaterial(ent);
					break;
			
				case BuildMenuEntryType.LEFT:
					
					vector mat[4];
					ent.GetWorldTransform(mat);
					vector entAngles = Math3D.MatrixToAngles(mat);

					entAngles[0] = entAngles[0] - 22.5; // these angles are 22.5 instead of 45 because this seems to run twice with auto closing the menu on selection disabled
					if 		(entAngles[0] > 180) 	{entAngles[0] = entAngles[0] - 360;}
					else if (entAngles[0] < -180) 	{entAngles[0] = entAngles[0] + 360;}
					
					Math3D.AnglesToMatrix(entAngles, mat);
					ent.SetWorldTransform(mat);
					break;
			
				case BuildMenuEntryType.RIGHT:
			
					vector mat[4];
					ent.GetWorldTransform(mat);
					vector entAngles = Math3D.MatrixToAngles(mat);

					entAngles[0] = entAngles[0] + 22.5;
					if 		(entAngles[0] > 180) 	{entAngles[0] = entAngles[0] - 360;}
					else if (entAngles[0] < -180) 	{entAngles[0] = entAngles[0] + 360;}
					
					Math3D.AnglesToMatrix(entAngles, mat);
					ent.SetWorldTransform(mat);
					break;		
			
				default:
					Print("No BuildMenuEntryType found", LogLevel.WARNING);
					break;
			}	
		}
		SCR_CharacterControllerComponent.Cast( m_pCharacter.FindComponent(SCR_CharacterControllerComponent) ).SetDynamicSpeed(1.0);
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

				Color col = m_IconColor;
				img.SetColor(col);
			}
		}
		return entry;
	}
		
	//------------------------------------------------------------------------------------------------
	void WR_BuildMenuEntry(notnull ChimeraCharacter character, BuildMenuEntryType entryType)
	{
		m_pCharacter = character;
		m_pEntryType = entryType;
		m_pDescription = "Left Click To Select";
		
		// set entry icon, name and color depending on entry type
		
		switch (m_pEntryType)
		{
			case BuildMenuEntryType.PLACE:
				m_pIconPath = "{8657510D7940031F}UI/Textures/Workshop/DownloadManager/DownloadManager_DownloadingIcon_UI.edds";
				m_pName = "Place As Is";
				m_IconColor = Color.Green;
				break;
			
			case BuildMenuEntryType.SNAP:
				m_pIconPath = "{BBA44B0A0EEFB9D9}UI/Textures/icon_snaptosurface.edds";
				m_pName = "Snap To Surface";
				m_IconColor = Color.Yellow;
				break;
			
			case BuildMenuEntryType.LEFT:
				m_pIconPath = "{93C33F624F012AAA}UI/Textures/icon_right.edds";
				m_pName = "Rotate Anti-Clockwise";
				m_IconColor = Color.Orange;
				break;
			
			case BuildMenuEntryType.RIGHT:
				m_pIconPath = "{5D33E0725DB71976}UI/Textures/icon_left.edds";
				m_pName = "Rotate Clockwise";
				m_IconColor = Color.Orange;
				break;
			
			default:
				m_pIconPath = "{A3D157619860A564}UI/Textures/Editor/Attributes/Arsenal/Attribute_Arsenal_Magazines.edds";
				m_IconColor = Color.Black;
				break;
		}
	}

	//------------------------------------------------------------------------------------------------
	void ~WR_BuildMenuEntry()
	{
		m_pCharacter = null;
	}
}