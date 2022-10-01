class WR_LockpickVehicleAction: ScriptedUserAction
{
	protected ActionsManagerComponent m_pActionsManagerComponent = null;
	protected Vehicle m_pVehicle = null;
	protected WR_VehicleLockingComponent m_pLockingComponent = null;
			
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_pActionsManagerComponent = ActionsManagerComponent.Cast(pManagerComponent);
		m_pVehicle = Vehicle.Cast(pOwnerEntity);
		m_pLockingComponent = WR_VehicleLockingComponent.Cast( GetOwner().FindComponent(WR_VehicleLockingComponent) );
	}
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		World world = GetGame().GetWorld();
		if (!world || !m_pVehicle || m_pVehicle.IsOccupied())
			return;
		
		m_pLockingComponent.SetVehicleLock(WR_VehicleLockMode.Unlocked);
	}
		
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		if (!m_pLockingComponent) 	 {return false;}
		// can't lockpick your own vehicle
		if (ESE.GetPlayerId(user) == m_pLockingComponent.GetVehicleOwner())
		{
			return false;
		}
		// can't lockpick an already unlocked vehicle
		if (m_pLockingComponent.GetVehicleLock() == WR_VehicleLockMode.Unlocked)
		{
			return false;
		}
		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		// if (can't find lockpick item in inventory) -> return false;
		array<typename> componentQuery = {WR_ItemLockpickComponent};
		IEntity lockpick = inventory.FindItemWithComponents(componentQuery);
		if (!lockpick)
		{
			return false;
		}		
		return true;
	}
}