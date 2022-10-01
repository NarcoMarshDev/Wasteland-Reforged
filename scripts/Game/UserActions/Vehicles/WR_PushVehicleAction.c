class WR_PushVehicleAction: ScriptedUserAction
{
	private ActionsManagerComponent m_pActionsManagerComponent = null;
	private Vehicle m_pVehicle = null;
	private WR_VehicleLockingComponent m_pLockingComponent = null;
	
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
		
		// get normalised vector pointing from player position to vehicle position
		vector dir = vector.Direction(pUserEntity.GetOrigin(), pOwnerEntity.GetOrigin());
		dir.Normalize();
		// scale vector based on vehicle mass * 3
		Physics phys = pOwnerEntity.GetPhysics();
		dir *= phys.GetMass() * 3;
		phys.ApplyImpulse(dir);
	}
		
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 				{return false;}
		if (m_pVehicle.IsOccupied())	{return false;}
		if (!m_pLockingComponent) 		{return false;}
		
		if (m_pLockingComponent.GetVehicleLock() != WR_VehicleLockMode.Unlocked)
		{
			return false;
		}
		
		return true;
	}
}