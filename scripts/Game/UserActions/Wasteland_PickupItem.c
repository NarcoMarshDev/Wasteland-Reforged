class Wasteland_PickupItem: ScriptedUserAction
{
	ActionsManagerComponent m_ActionsManagerComponent = null;
	IEntity m_Entity = null;
	Wasteland_PlayerInventoryComponent m_inventoryComponent = null;
	string m_itemType;
	
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_ActionsManagerComponent = ActionsManagerComponent.Cast(pManagerComponent);
		m_Entity = pOwnerEntity;
	}
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!m_Entity)
			return;
		
		// pick up item action
		m_inventoryComponent = Wasteland_PlayerInventoryComponent.Cast(pUserEntity.FindComponent(Wasteland_PlayerInventoryComponent));
		
		if (pOwnerEntity.IsInherited(Wasteland_MoneyBag))
		{
			m_inventoryComponent.addMoney(Wasteland_MoneyBag.Cast(pOwnerEntity).getMoney());			
			RplComponent.DeleteRplEntity(pOwnerEntity, false);
			Print("after: " + m_inventoryComponent.getMoney());
		}
		else Print("not money bag");
		
	}
	
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_Entity)
			return false;
		
		return true;
	}
}