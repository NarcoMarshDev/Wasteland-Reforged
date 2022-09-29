class WR_PushVehicleAction: ScriptedUserAction
{
	private ActionsManagerComponent m_pActionsManagerComponent = null;
	private Vehicle m_pVehicle = null;
	
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_pActionsManagerComponent = ActionsManagerComponent.Cast(pManagerComponent);
		m_pVehicle = Vehicle.Cast(pOwnerEntity);
	}
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		World world = GetGame().GetWorld();
		if (!world || !m_pVehicle || m_pVehicle.IsOccupied())
			return;
		
		vector dir = vector.Direction(pUserEntity.GetOrigin(), pOwnerEntity.GetOrigin());
		dir.Normalize();
		Physics phys = pOwnerEntity.GetPhysics();
		dir *= phys.GetMass() * 3;
		phys.ApplyImpulse(dir);
	}
		
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 				{return false;}
		if (m_pVehicle.IsOccupied())	{return false;}
		
		return true;
	}
}