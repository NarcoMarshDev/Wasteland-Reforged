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
		if (!m_pLockingComponent) 	 {return false;}
		
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
		if (!m_pLockingComponent) 	 {return false;}
		
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
	// only allow faction locking if vehicle is unlocked, and disallow if indipendent faction
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		if (!m_pLockingComponent) 	 {return false;}
		
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
		if (!m_pLockingComponent) 	 {return false;}
		
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
// This will probably be disabled by default, might be a bit too much and ruin the vibe a bit.
// Maybe as an extra you can pay for at a vehicle store, or for mission reward vehicles.
class WR_RequestRemoteVehicleUnlockAction: WR_BaseVehicleLockingAction
{
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.PerformAction(pOwnerEntity, pUserEntity);
		m_pLockingComponent.SendRemoteUnlockRequest(ESE.GetPlayerId(pUserEntity));
	}
	// --------------------------------------------------------------------------------------------
	// only allow unlock request if on same faction or group if independent faction
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_pVehicle) 			 {return false;}
		if (m_pVehicle.IsOccupied()) {return false;}
		if (!m_pLockingComponent) 	 {return false;}
		
		if (ESE.GetPlayerId(user) == m_pLockingComponent.GetVehicleOwner())
		{
			return false;
		}
		if (m_pLockingComponent.GetVehicleLock() == WR_VehicleLockMode.Unlocked)
		{
			return false;
		}
		if (m_pLockingComponent.HasRemoteAccessEnabled())
		{
			SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(user);
			if (character.GetFactionKey() == "FIA")
			{
				if (SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(ESE.GetPlayerId(character)) == m_pLockingComponent.GetVehicleOwnerGroup())
				{
					return true;
				}
				return false;
			}
			else if (character.GetFaction() == m_pLockingComponent.GetVehicleOwnerFaction())
			{
				return true;
			}
			return false;
		}
		return false;
	}
}