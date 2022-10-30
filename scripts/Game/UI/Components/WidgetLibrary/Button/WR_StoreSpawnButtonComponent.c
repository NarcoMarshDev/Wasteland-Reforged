// WR_TODO - Make this work on console, currently the button is not selectable with a controller so actually purchasing something doesn't work

class WR_StoreSpawnButtonComponent: SCR_NavigationButtonComponent
{
	protected IEntity m_CurrentStoreEntity;
	protected WR_StoreComponent m_CurrentStoreComponent;
	protected WR_FieldManualConfigEntry_Store m_CurrentEntry;
	protected WR_PlayerProfile m_LocalPlayerProfile;
		
	protected void ResetLabel(string oldLabel)
	{
		Print("Does this get called?");
		SetLabel(oldLabel);
		super.SetLabel(oldLabel);
		SetEnabled(true);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		Print("YO IT CLICKED BRO!!!");
		
		// find and get player stats object for local player from the server
		int playerId = GetGame().GetPlayerController().GetPlayerId();
		WR_GameModeComponent gmComponent = WR_GameModeComponent.Cast( GetGame().GetGameMode().FindComponent(WR_GameModeComponent) );
		m_LocalPlayerProfile = gmComponent.m_PlayerProfileMap.Get(playerId);
		if (!m_LocalPlayerProfile)
		{
			Print("No player stats found for playerId: " + playerId, LogLevel.ERROR);
			return false;
		}
		
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
		if (m_CurrentEntry.m_iPrice > m_LocalPlayerProfile.m_MoneyCash)
		{
			// change label and disable button until it gets changed back to default to stop input spam
			SetLabel("Not Enough Money!");
			super.SetLabel("Not Enough Money!");
			SetEnabled(false);
			
			// after 3000ms, change label back to confirm purchase and enable the button again
			GetGame().GetCallqueue().CallLater(ResetLabel, 3000, false, "Confirm Purchase");
			return false;
		}
		
		// ------------------------------------------------------------------------------------------------------------------------------- //
		
		vector origin[4];
		Math3D.MatrixIdentity4(origin);				
		IEntity spawnedEntity = ESE_Entities.SpawnPrefab(origin, m_CurrentEntry.m_ResourceToSpawn);
		if (!spawnedEntity)
		{
			// if no entity was spawned, update label to reflect that with the same process for when not having enough money
			Print("Spawning " + m_CurrentEntry.m_ResourceToSpawn + " failed!", LogLevel.WARNING);
			SetLabel("Spawning Failed, Refunding!");
			super.SetLabel("Spawning Failed, Refunding!");
			SetEnabled(false);
			
			GetGame().GetCallqueue().CallLater(ResetLabel, 3000, false, "Confirm Purchase");
			return false;
		}
		// handle if it's an entity that can be picked up or equipped
		if (ESE_Entities.CanPickUpEntity(spawnedEntity))
		{
			// holy shit what a line
			SCR_InventoryStorageManagerComponent manager = SCR_InventoryStorageManagerComponent.Cast(GetGame().GetPlayerManager().GetPlayerControlledEntity(m_LocalPlayerProfile.m_PlayerId).FindComponent(SCR_InventoryStorageManagerComponent));
			if (!manager.TryInsertItem(spawnedEntity, EStoragePurpose.PURPOSE_ANY))
			{
				// the insert failed, so cancel the purchase
				Print("NO SPACE!");
				SetLabel("No free slot, refunding!");
				super.SetLabel("No free slot, refunding!"); // #WR_TODO - see if this super call is needed for the label to update
				SetEnabled(false);
				
				GetGame().GetCallqueue().CallLater(ResetLabel, 3000, false, "Confirm Purchase");
				ESE_Entities.DeleteEntity(spawnedEntity);
				return false;
			}
			Print("YES SPACE!");
		}
		else
		{
			Print("CANT EQUIP");
		
			WR_StoreSpawnSlot spawnSlot = m_CurrentStoreComponent.GetValidSpawnSlot();
			vector mat[4];
			spawnSlot.GetTransform(mat);
			spawnedEntity.SetTransform(mat);
			spawnedEntity.Update();
		
			spawnSlot.SetOccupant(spawnedEntity);
		}
		SetLabel("Purchase Successful!");
		SetEnabled(false);
		GetGame().GetCallqueue().CallLater(ResetLabel, 300, false, "Confirm Purchase");
		// item has spawned, so take the money now and close the menu
		m_LocalPlayerProfile.m_MoneyCash -= m_CurrentEntry.m_iPrice;
		
		gmComponent.UpdatePlayerProfile(m_LocalPlayerProfile);
		//GetGame().GetMenuManager().GetTopMenu().Close(); // enable if you want the menu to auto close after purchase
		return false;
	}
}