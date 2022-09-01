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
		array<IEntity> ownerChildren = new array<IEntity>();
		WR_Statics.GetAllChildren(m_Entity, ownerChildren);
		foreach (IEntity ent : ownerChildren)
		{
			Print("owner: " + ent);
		}
		array<IEntity> userChildren = new array<IEntity>();
		WR_Statics.GetAllChildren(pUserEntity, userChildren);
		foreach (IEntity ent : ownerChildren)
		{
			Print("user: " + ent);
		}
		
						
		WR_Statics.SetMaterial(pOwnerEntity, "{56EBF5038622AC95}Assets/Conflict/CanBuild.emat");
		//WR_Statics.SetEntityCollision(pOwnerEntity, EPhysicsLayerDefs.None); This crashes the entire workbench when run, wont even bring up the crash reporter	
	
		WR_Statics.GetEntityBuildingSlot(pUserEntity).AttachEntity(pOwnerEntity);
		WR_Statics.DisableEntityCollisions(pOwnerEntity);
	}
	
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_Entity)
			return false;
		
		return true;
	}
}