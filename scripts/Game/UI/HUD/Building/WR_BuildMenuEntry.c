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
	// Recursively sets all descendants world position manually to avoid engine bug with children not following parent entities properly when unslotted
	void SetChildTransforms(IEntity ent)
	{
		IEntity child = ent.GetChildren();
		while (child)
		{
			vector localTransform[4];
			child.GetLocalTransform(localTransform);
			Print("localTransform: " + localTransform[3]);
			vector localAngles = localTransform[3].VectorToAngles();
			Print("localAngles: " + localAngles);
						
			vector entAngles = ent.GetAngles();
			Print("entAngles: " + entAngles);
			
			vector sumAngles = WR_Statics.FixVector180( localAngles + entAngles ); //#ESE REPLACE
			Print("sumAngles: " + sumAngles);
			
			float len = localAngles.Length();
			Print("len: " + len);
			
			// use direction from sumAngles and distance from len to move child to correct world position
			
			float x = 0;
			float y = 0;
			float z = 0;
			
			Print("x: " + x);
			Print("y: " + y);
			Print("z: " + z);
			
			child = child.GetSibling();
		}
		
		
		
		#ifdef BROKEN
		IEntity child = ent.GetChildren();
		while (child)
		{
			vector localTransform[4];
			child.GetLocalTransform(localTransform);
			
			vector worldTransform[4];
			child.GetWorldTransform(worldTransform);
			
			vector pos = ent.CoordToParent(localTransform);
			Print("parent pos: " + ent.GetOrigin());
			Print("localTransform: " + localTransform[3]);
			Print("worldTransform: " + worldTransform[3]);
			Print(pos);
			
			child.SetOrigin(pos);
			
			if (child.GetChildren())
			{
				SetChildTransforms(child);
			}
			child = child.GetSibling();
		}
		
		
		
		IEntity child = ent.GetChildren();
		while (child)
		{
			vector localTransform[4];
			child.GetLocalTransform(localTransform);
			Print("localTransform: " + localTransform[3]);
			vector newPos = child.GetParent().GetOrigin() + localTransform[3];
			child.SetOrigin(newPos);
			
			if (child.GetChildren())
			{
				SetChildTransforms(child);
			}
			child = child.GetSibling();
		}
		#endif
	}
	
	//------------------------------------------------------------------------------------------------
	//! Callback for when this entry is supposed to be performed
	override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
	{
		EntitySlotInfo slot = WR_Statics.GetEntityBuildingSlot(m_pCharacter);
		IEntity ent = slot.GetAttachedEntity();
		if (!ent)
			return;
		
		switch (m_pEntryType)
		{
			case BuildMenuEntryType.PLACE:
			
				vector entTransform[4];
				slot.DetachEntity(false);
				ent.GetWorldTransform(entTransform);
				ent.GetParent().RemoveChild(ent);
				ent.SetWorldTransform(entTransform);
				WR_Statics.RestoreMaterial(ent); //#ESE REPLACE
				WR_Statics.EnableEntityCollisions(ent); //#ESE REPLACE
				break;
			
			case BuildMenuEntryType.SNAP:
			
				vector entTransform[4];
				slot.DetachEntity(false);
				ent.GetWorldTransform(entTransform);
				SCR_TerrainHelper.SnapToTerrain(entTransform, GetGame().GetWorld(), true);
				ent.GetParent().RemoveChild(ent);
				ent.SetWorldTransform(entTransform);
				
				SetChildTransforms(ent);			
				
				WR_Statics.RestoreMaterial(ent); //#ESE REPLACE
				WR_Statics.EnableEntityCollisions(ent); //#ESE REPLACE
				break;
			
			case BuildMenuEntryType.LEFT:
				
				vector mat[4];
				ent.GetWorldTransform(mat);
				vector entAngles = Math3D.MatrixToAngles(mat);
				entAngles[0] = entAngles[0] - 22.5; // these angles are 22.5 instead of 45 because this seems to run twice with auto closing the menu on selection disabled
				if 		(entAngles[0] > 180) 	{entAngles[0] = entAngles[0] - 360;} // replace with WR_Statics.FixFloat180(); //#ESE REPLACE
				else if (entAngles[0] < -180) 	{entAngles[0] = entAngles[0] + 360;}
					
				Math3D.AnglesToMatrix(entAngles, mat);
				ent.SetWorldTransform(mat);
				break;
			
			case BuildMenuEntryType.RIGHT:
			
				vector mat[4];
				ent.GetWorldTransform(mat);
				vector entAngles = Math3D.MatrixToAngles(mat);

				entAngles[0] = entAngles[0] + 22.5;
				if 		(entAngles[0] > 180) 	{entAngles[0] = entAngles[0] - 360;} //#ESE REPLACE - ESE_Math.FixFloat180();
				else if (entAngles[0] < -180) 	{entAngles[0] = entAngles[0] + 360;}
					
				Math3D.AnglesToMatrix(entAngles, mat);
				ent.SetWorldTransform(mat);
				break;		
			
			default:
				Print("No BuildMenuEntryType found", LogLevel.WARNING);
				break;
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