class WR_AquireVehicleOwnershipAction: ScriptedUserAction
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
		
		m_pLockingComponent.SetVehicleOwner(ESE.GetPlayerId(pUserEntity));
	}
		
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		
		if (ESE.GetPlayerId(user) == m_pLockingComponent.GetVehicleOwner())
		{
			return false;
		}
		return true;
	}
}