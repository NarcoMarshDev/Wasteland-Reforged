class WR_FlipVehicleAction: ScriptedUserAction
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
		
		vector vehicleTransform[4];
		pOwnerEntity.GetTransform(vehicleTransform);
		WR_Statics.OrientToTerrain(vehicleTransform, world, true); //#ESE REPLACE
		// move 0.5 meters up
		vehicleTransform[3][1] = vehicleTransform[3][1] + 0.5;
		
		pOwnerEntity.SetTransform(vehicleTransform);
		// apply impulse down to settle back on ground
		pOwnerEntity.GetPhysics().ApplyImpulse("0 -1 0");
	}
		
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 				{return false;}
		if (m_pVehicle.IsOccupied())	{return false;}
		
		return true;
	}
}