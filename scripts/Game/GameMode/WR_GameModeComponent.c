class WR_GameModeComponentClass : SCR_BaseGameModeComponentClass
{
}

class WR_GameModeComponent : SCR_BaseGameModeComponent
{
	// ------------------------------------------------ Persistent player ID and profiles ------------------------------------------------ //
	static const string WR_PERSISTENT_ID_FILE_PATH = "$profile:WastelandReforgedPersistentID.txt";
	
	ref map<int, ref WR_PlayerProfile> m_PlayerProfileMap = new map<int, ref WR_PlayerProfile>();
	
	// ---------------------------------------------------------------------------------------------------------------- //
	// Is this unnecessary because the player profiles are strong refs?
	void UpdatePlayerProfile(WR_PlayerProfile prof)
	{
		int playerId = prof.m_PlayerId;
		m_PlayerProfileMap.Set(playerId, prof);
		
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void EOnInit(IEntity owner)
	{

	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPlayerConnected(int playerId)
	{
		// future - if (find profile for player already) { load it }
		auto bApi = GetGame().GetBackendApi();
		ref auto profile = new WR_PlayerProfile(playerId, bApi.GetPlayerUID(playerId), cash: 150000);
		m_PlayerProfileMap.Insert(playerId, profile);
	}	
	
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		//WR_SaveConfigRoot root = SCR_ConfigHelperT<WR_SaveConfigRoot>.GetConfigObject(persistentSavePath);

		
		
		
		// ------------------------------------------------- BUILDING ------------------------------------------------- //
		
		// If player dies while holding slotted entity, find it, snap to terrain, and restore it's materials
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(player.FindComponent(SlotManagerComponent));
		array<EntitySlotInfo> slots = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slots);
		
		EntitySlotInfo buildingSlot = slots[0];
		if (!buildingSlot)
			return;
		
		IEntity ent = buildingSlot.GetAttachedEntity();
		if (!ent)
			return;
		ESE_Entities.RestoreMaterial(ent, true);
		
		vector mat[4];
		ent.GetTransform(mat);
		ESE_Entities.SnapAndOrientToGround(mat, ent, 5, true); // #ESE_TEST
		ent.SetTransform(mat);
		ESE_Entities.EnableCollisions(ent);
		ent.Update();
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		// find build menu component, and set it's controller component reference (yes it's awful)
		WR_BuildingRadialMenuComponent.Cast( GetGame().GetPlayerManager().GetPlayerController(playerId).FindComponent(WR_BuildingRadialMenuComponent) ).SetCharacterController(SCR_CharacterControllerComponent.Cast( controlledEntity.FindComponent(SCR_CharacterControllerComponent) ));
	}
	
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
}