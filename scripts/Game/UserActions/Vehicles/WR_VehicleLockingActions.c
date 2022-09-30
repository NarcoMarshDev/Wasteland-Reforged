// Base locking class that all locking actions derive from
class WR_BaseVehicleLockingAction: ScriptedUserAction
{
	protected ActionsManagerComponent m_pActionsManagerComponent = null;
	protected Vehicle m_pVehicle = null;
	//protected SCR_BaseCompartmentManagerComponent m_pCompartmentManager = null;
	protected WR_VehicleLockingComponent m_pLockingComponent = null;
			
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_pActionsManagerComponent = ActionsManagerComponent.Cast(pManagerComponent);
		m_pVehicle = Vehicle.Cast(pOwnerEntity);
		//m_pCompartmentManager = SCR_BaseCompartmentManagerComponent.Cast(pOwnerEntity.FindComponent(SCR_BaseCompartmentManagerComponent));
		m_pLockingComponent = WR_VehicleLockingComponent.Cast( GetOwner().FindComponent(WR_VehicleLockingComponent) );
	}
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		World world = GetGame().GetWorld();
		if (!world || !m_pVehicle || m_pVehicle.IsOccupied())
			return;
	}
}
// ------------------------------------------------------------------------------------------------
class WR_LockVehicleAction: WR_BaseVehicleLockingAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		m_pLockingComponent.SetVehicleLock(WR_VehicleLockMode.Locked);
	}
	// --------------------------------------------------------------------------------------------
	// only allow locking if vehicle is unlocked
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		
		if (ESE.GetPlayerId(user) != m_pLockingComponent.GetVehicleOwner())
		{
			return false;
		}
		if (m_pLockingComponent.GetVehicleLock() != WR_VehicleLockMode.Unlocked)
		{
			return false;
		}
		return true;
	}
}
// ------------------------------------------------------------------------------------------------
class WR_UnlockVehicleAction: WR_BaseVehicleLockingAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		m_pLockingComponent.SetVehicleLock(WR_VehicleLockMode.Unlocked);
	}
	// --------------------------------------------------------------------------------------------
	// only allow unlocking if vehicle is locked
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		
		if (ESE.GetPlayerId(user) != m_pLockingComponent.GetVehicleOwner())
		{
			return false;
		}
		if (m_pLockingComponent.GetVehicleLock() == WR_VehicleLockMode.Unlocked)
		{
			return false;
		}
		return true;
	}
}
// ------------------------------------------------------------------------------------------------
class WR_FactionLockVehicleAction: WR_BaseVehicleLockingAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		m_pLockingComponent.SetVehicleLock(WR_VehicleLockMode.FactionUnlocked);
	}
	// --------------------------------------------------------------------------------------------
	// only allow faction locking if vehicle is unlocked
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		
		if (ESE.GetPlayerId(user) != m_pLockingComponent.GetVehicleOwner())
		{
			return false;
		}
		if (m_pLockingComponent.GetVehicleLock() != WR_VehicleLockMode.Unlocked)
		{
			return false;
		}
		return true;
	}
}
// ------------------------------------------------------------------------------------------------
class WR_GroupLockVehicleAction: WR_BaseVehicleLockingAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		m_pLockingComponent.SetVehicleLock(WR_VehicleLockMode.GroupUnlocked);
	}
	// --------------------------------------------------------------------------------------------
	// only allow group locking if vehicle is unlocked
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		
		if (ESE.GetPlayerId(user) != m_pLockingComponent.GetVehicleOwner())
		{
			return false;
		}
		if (m_pLockingComponent.GetVehicleLock() != WR_VehicleLockMode.Unlocked)
		{
			return false;
		}
		return true;
	}
}
// ------------------------------------------------------------------------------------------------