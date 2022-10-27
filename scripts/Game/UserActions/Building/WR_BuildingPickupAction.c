class WR_BuildingPickupAction: ScriptedUserAction
{
	ActionsManagerComponent m_ActionsManagerComponent = null;
	IEntity m_Entity = null;
	
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_ActionsManagerComponent = ActionsManagerComponent.Cast(pManagerComponent);
		m_Entity = pOwnerEntity;
	}
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!m_Entity)
			return;
										
		ESE_Entities.SetMaterial(pOwnerEntity, ESE_Aliases.AR_MAT_CANBUILD);
		ESE_Entities.DisableCollisions(pOwnerEntity);
		WR_Statics.GetEntityBuildingSlot(pUserEntity).AttachEntity(pOwnerEntity);
	}
	
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_Entity)
			return false;
		
		if (!WR_BuildingRadialMenuComponent.Cast( ESE.GetPlayerControllerFromEntity(user).FindComponent(WR_BuildingRadialMenuComponent) ).GetEnabled())
		{
			return false;
		}
		
		if (WR_Statics.GetEntityBuildingSlot(user).GetAttachedEntity())
		{
			return false;
		}
		
		return true;
	}
}