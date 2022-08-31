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
		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pUserEntity.FindComponent(SlotManagerComponent));
		array<EntitySlotInfo> slots = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slots);
		
		// what the actual fuck? replace with ESE.GetPlayerControllerFromEntity(IEntity ent); or even ESE.FindPlayerControllerComponent(IEntity ent);
		WR_BuildingRadialMenuComponent menu = WR_BuildingRadialMenuComponent.Cast( GetGame().GetPlayerManager().GetPlayerController( GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity) ).FindComponent(WR_BuildingRadialMenuComponent) );
				
		array<ResourceName> resNames = {};
		
		IEntity child = pOwnerEntity.GetChildren();
		while (child)
		{
			resNames.Insert(child.GetVObject().GetResourceName());
			child = child.GetSibling();
		}
				
		menu.SetCurrentModelPaths(resNames);
		WR_Statics.SetMaterial(pOwnerEntity, "{56EBF5038622AC95}Assets/Conflict/CanBuild.emat");
		
		slots[0].AttachEntity(pOwnerEntity);		
	}
	
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_Entity)
			return false;
		
		return true;
	}
}