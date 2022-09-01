class WR_GameModeComponentClass : SCR_BaseGameModeComponentClass
{
}

class WR_GameModeComponent : SCR_BaseGameModeComponent
{
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		// ==================================================== BUILDING ==================================================== //
		
		// If player dies while holding slotted entity, find it, snap to terrain, and restore it's materials
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(player.FindComponent(SlotManagerComponent));
		array<EntitySlotInfo> slots = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slots);
		
		EntitySlotInfo buildingSlot = slots[0];
		if (!buildingSlot)
			return;
		
		IEntity ent = buildingSlot.GetAttachedEntity();
		WR_Statics.RestoreMaterial(ent);
		
		vector mat[4];
		ent.GetTransform(mat);
		SCR_Global.SnapToTerrain(mat, GetGame().GetWorld(), true);		
		ent.SetTransform(mat);
		//WR_Statics.SetEntityCollision(ent, EPhysicsLayerDefs.Default);
	}
	
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		// find build menu component, and set it's controller component reference (yes it's awful)
		WR_BuildingRadialMenuComponent.Cast( GetGame().GetPlayerManager().GetPlayerController(playerId).FindComponent(WR_BuildingRadialMenuComponent) ).SetCharacterController(SCR_CharacterControllerComponent.Cast( controlledEntity.FindComponent(SCR_CharacterControllerComponent) ));
	}
}