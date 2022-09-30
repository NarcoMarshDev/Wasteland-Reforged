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
		
		Print("pls");
		array<IEntity> ownerChildren = ESE.GetAllChildren(m_Entity);
		//array<IEntity> ownerChildren = new array<IEntity>();
		//WR_Statics.GetAllChildren(m_Entity, ownerChildren); //#ESE REPLACE
		foreach (IEntity ent : ownerChildren)
		{
			Print("owner: " + ent);
		}
		array<IEntity> userChildren = ESE.GetAllChildren(pUserEntity);
		//array<IEntity> userChildren = new array<IEntity>();
		//WR_Statics.GetAllChildren(pUserEntity, userChildren); //#ESE REPLACE
		foreach (IEntity ent : ownerChildren)
		{
			Print("user: " + ent);
		}
		
						
		ESE_Entities.SetMaterial(pOwnerEntity, ESE_Aliases.AR_MAT_CANBUILD); //WR_Statics.SetMaterial(pOwnerEntity, "{56EBF5038622AC95}Assets/Conflict/CanBuild.emat"); //#ESE REPLACE
		ESE_Entities.DisableCollisions(pOwnerEntity); //WR_Statics.DisableEntityCollisions(pOwnerEntity); //#ESE REPLACE
		WR_Statics.GetEntityBuildingSlot(pUserEntity).AttachEntity(pOwnerEntity);
	}
	
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_Entity)
			return false;
		
		//if (!WR_BuildingRadialMenuComponent.Cast( WR_Statics.GetPlayerControllerFromEntity(user).FindComponent(WR_BuildingRadialMenuComponent) ).GetEnabled()) //#ESE REPLACE
		if (!WR_BuildingRadialMenuComponent.Cast( ESE.GetPlayerControllerFromEntity(user).FindComponent(WR_BuildingRadialMenuComponent) ).GetEnabled())
		{
			return false;
		}
		
		return true;
	}
}