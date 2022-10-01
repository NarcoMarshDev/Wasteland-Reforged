class WR_BaseStoreOpenAction: ScriptedUserAction
{
	private ActionsManagerComponent m_pActionsManagerComponent = null;
	
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_pActionsManagerComponent = ActionsManagerComponent.Cast(pManagerComponent);
	}
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		World world = GetGame().GetWorld();
		if (!world)
			return;
		
		// get the store that this action relates to
		WR_StoreComponent store = WR_StoreComponent.Cast( pOwnerEntity.FindComponent(WR_StoreComponent) );
		// send reference of this store to the player's WR_CurrentStorePlayerControllerComponent
		WR_CurrentStorePlayerControllerComponent.Cast(GetGame().GetPlayerController().FindComponent(WR_CurrentStorePlayerControllerComponent)).SetCurrentStoreEntity(pOwnerEntity);
				
		
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.FieldManualDialog);
	}
		
	override event bool CanBeShownScript(IEntity user)
	{
		return true;
	}
}
// ------------------------------------------------------------------------------------------------
// Vehicle store
class WR_VehicleStoreOpenAction: WR_BaseStoreOpenAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		
	}
}
// ------------------------------------------------------------------------------------------------
// General store
class WR_GeneralStoreOpenAction: WR_BaseStoreOpenAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		
	}
}
// ------------------------------------------------------------------------------------------------
// Gun store
class WR_GunStoreOpenAction: WR_BaseStoreOpenAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		
	}
}