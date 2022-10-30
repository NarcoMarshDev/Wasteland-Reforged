modded class SCR_OpenVehicleStorageAction : SCR_InventoryAction
{
#ifndef DISABLE_INVENTORY
	[Attribute("1")]
	protected bool m_bShowFromOutside;

	[Attribute("1")]
	protected bool m_bShowInside;

	protected IEntity m_Vehicle;
	protected WR_VehicleLockingComponent m_LockingComponent;

	//------------------------------------------------------------------------------------------------
	/**
	override protected void PerformActionInternal(SCR_InventoryStorageManagerComponent manager, IEntity pOwnerEntity, IEntity pUserEntity)
	{
		manager.SetStorageToOpen(m_Vehicle);
		manager.OpenInventory();
	}
	*/
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if (!user || !m_Vehicle)
			return false;

		ChimeraCharacter character = ChimeraCharacter.Cast(user);

		CompartmentAccessComponent compAccess = CompartmentAccessComponent.Cast(character.FindComponent(CompartmentAccessComponent));
		if (compAccess && compAccess.IsGettingIn() || compAccess.IsGettingOut())
			return false;

		if (!m_bShowFromOutside && !character.IsInVehicle())
			return false;

		if (!m_bShowInside && character.IsInVehicle())
			return false;

		//--------------------------------------------------------------------------------------------
		// modded, removed base game faction checks
		return true;
	}

	override bool CanBePerformedScript(IEntity user)
	{
		if (!m_LockingComponent)
			return true;
		
		// check if vehicle is unlocked first, most likely scenario for random vehicles
		if (m_LockingComponent.GetVehicleLock() == WR_VehicleLockMode.Unlocked)
		{
			return true;
		}
		// check if vehicle is locked
		if (m_LockingComponent.GetVehicleLock() == WR_VehicleLockMode.Locked)
		{
			SetCannotPerformReason("Vehicle Locked");
			return false;
		}
		// check if vehicle is faction locked and if player faction matches
		if (m_LockingComponent.GetVehicleLock() == WR_VehicleLockMode.FactionUnlocked && SCR_ChimeraCharacter.Cast(user).GetFaction() != m_LockingComponent.GetVehicleOwnerFaction())
		{
			SetCannotPerformReason("Vehicle Locked");
			return false;
		}
		// check if vehicle is group locked and if player group matches
		SCR_AIGroup playerGroup = SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(ESE.GetPlayerId(user)); // get player group
		if (m_LockingComponent.GetVehicleLock() == WR_VehicleLockMode.GroupUnlocked && playerGroup != m_LockingComponent.GetVehicleOwnerGroup())
		{
			SetCannotPerformReason("Vehicle Locked");
			return false;
		}
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		if (!Vehicle.Cast(pOwnerEntity))
		{
			m_Vehicle = pOwnerEntity.GetParent();
		}
		else
		{
			m_Vehicle = pOwnerEntity;
		}
		m_LockingComponent = WR_VehicleLockingComponent.Cast( m_Vehicle.FindComponent(WR_VehicleLockingComponent) );
	}
#endif
};