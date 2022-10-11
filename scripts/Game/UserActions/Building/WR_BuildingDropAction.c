// #WR_OBSOLETE
class WR_BuildingDropAction: ScriptedUserAction
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
		
		slots[0].DetachEntity();
	}
	
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_Entity)
			return false;
		
		return true;
	}
}