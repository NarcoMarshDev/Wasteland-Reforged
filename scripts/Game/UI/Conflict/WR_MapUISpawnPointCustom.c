#ifdef WASTELAND_BROKEN
modded class SCR_MapUISpawnPoint : SCR_MapUIElement
{
	protected TextWidget m_wFriendlyCount;
	protected TextWidget m_wEnemyCount;
	protected bool m_IsHandlerAttached = false;
			
	void UpdatePlayerCounts(int friendlies, int enemies)
	{
		m_wFriendlyCount.SetText("Friendlies: " + friendlies);
		m_wEnemyCount.SetText("Enemies: " + enemies);
	}
	//------------------------------------------------------------------------------
	// should set icon color to orange if spawn point is disabled
	void UpdateIconColor()
	{
		if (!m_SpawnPoint)
		{
			Print("Update Color Failed!");
			return;
		}
		string img = string.Format("%1_%2", m_sFactionKey, m_sSpawnPoint);
		string selectionImg = string.Format("%1_%2", img, m_sSelection);
		
		if (WR_SpawnPoint.Cast(m_SpawnPoint).GetEnabled())
		{
			m_wImage.SetColor(GetColorForFaction(m_sFactionKey));
		}
		else
		{
			m_wImage.SetColor(new Color(1.0, 0.5, 0.0, 1.0));
		}
		SetImage(img);
		m_wSelectImg.LoadImageFromSet(0, m_sImageSet, selectionImg);
	}
	//------------------------------------------------------------------------------
	override override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
				
		m_IsHandlerAttached = true;
		
		//UpdateIconColor();
		
		// send reference of the ui back to the town trigger so it can periodically update the ui with new player counts
		//if(m_SpawnPoint.IsInherited(WR_SpawnPoint))
		//{
		//	WR_TownTrigger trigger = WR_TownTrigger.Cast( m_SpawnPoint.GetParent() );
		//	trigger.SendUIReference(WR_SpawnPoint.Cast(m_SpawnPoint), this);
		//}
		
		m_wImageOverlay = w.FindAnyWidget("Overlay");
		m_wSpawnPointName = TextWidget.Cast(w.FindAnyWidget("Name"));
		m_wFriendlyCount = TextWidget.Cast(w.FindAnyWidget("Friendlies"));
		m_wEnemyCount = TextWidget.Cast(w.FindAnyWidget("Enemies"));
		
		Print("w: " + w);
		Print("m_wFriendlyCount: " + m_wFriendlyCount.GetText());
		Print("m_wEnemyCount: " + m_wEnemyCount.GetText());
		
		SCR_SelectSpawnPointSubMenu.Event_OnSpawnPointChanged.Insert(OnSelected);
		SCR_SpawnPoint.Event_SpawnPointRemoved.Insert(Remove);
	}
	
	override override void HandlerDeattached(Widget w)
	{
		super.HandlerDeattached(w);
		m_IsHandlerAttached = false;
		
		Print("closed" + this);
		
		SCR_SelectSpawnPointSubMenu.Event_OnSpawnPointChanged.Remove(OnSelected);
		SCR_SpawnPoint.Event_SpawnPointRemoved.Remove(Remove);
	}
}
#endif