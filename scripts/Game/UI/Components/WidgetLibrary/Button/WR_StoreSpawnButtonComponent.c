class WR_StoreSpawnButtonComponent: SCR_NavigationButtonComponent
{
	protected IEntity m_CurrentStoreEntity;
	protected WR_StoreComponent m_CurrentStoreComponent;
	protected WR_FieldManualConfigEntry_Store m_CurrentEntry;
	protected WR_PlayerProfile m_LocalPlayerStats;
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		Print("YO IT CLICKED BRO!!!");
		
		int playerId = GetGame().GetPlayerController().GetPlayerId();
		WR_GameModeComponent gmComponent = WR_GameModeComponent.Cast( GetGame().GetGameMode().FindComponent(WR_GameModeComponent) );
		Print(gmComponent.m_PlayerStatsMap);
		foreach (int i, WR_PlayerProfile prof : gmComponent.m_PlayerStatsMap)
		{
			Print("" + i + " : " + prof);
		}
		
		m_LocalPlayerStats = gmComponent.m_PlayerStatsMap.Get(playerId);
		
		m_CurrentStoreEntity = WR_CurrentStorePlayerControllerComponent.Cast( GetGame().GetPlayerController().FindComponent(WR_CurrentStorePlayerControllerComponent) ).GetCurrentStoreEntity();
		m_CurrentStoreComponent = WR_StoreComponent.Cast( m_CurrentStoreEntity.FindComponent(WR_StoreComponent) );
		
		// Get current open entry from field manual UI and check it exists
		SCR_FieldManualUI instance = SCR_FieldManualUI.Cast( GetGame().GetMenuManager().GetTopMenu() );
		WR_FieldManualConfigEntry_Store entry = WR_FieldManualConfigEntry_Store.Cast( instance.GetCurrentEntry() );
		if (entry)
		{
			m_CurrentEntry = entry;
			
		}
		// If player doesn't have enough money, can't buy!
		
		// WR_TODO - Find a way to make this go back to "Confirm Purchase" when you go back a menu, I'm too tired to figure this shit out
		
		if (m_CurrentEntry.m_iPrice > m_LocalPlayerStats.m_MoneyCash)
		{
			SetLabel("Not Enough Money!");
			super.SetLabel("Not Enough Money!");
			return false;
		}
		
		return false;
	}
}