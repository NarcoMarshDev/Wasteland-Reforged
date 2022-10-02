// Goto line 83
modded class SCR_GetInUserAction : SCR_CompartmentUserAction
{
	protected SCR_BaseLockComponent m_pLockComp;

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_pLockComp = SCR_BaseLockComponent.Cast(pOwnerEntity.FindComponent(SCR_BaseLockComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!pOwnerEntity || !pUserEntity)
			return;
		
		ChimeraCharacter character = ChimeraCharacter.Cast(pUserEntity);
		if (!character)
			return;
		
		BaseCompartmentSlot targetCompartment = GetCompartmentSlot();
		if (!targetCompartment)
			return;
		
		CompartmentAccessComponent compartmentAccess = CompartmentAccessComponent.Cast(character.FindComponent(CompartmentAccessComponent));
		if (!compartmentAccess)
			return;
		
		if (!compartmentAccess.GetInVehicle(pOwnerEntity, targetCompartment, GetRelevantDoorIndex(pUserEntity)))
			return;
		
		super.PerformAction(pOwnerEntity, pUserEntity);
	}
	
	//------------------------------------------------------------------------------------------------
	override override bool CanBePerformedScript(IEntity user)
	{		
		BaseCompartmentSlot compartment = GetCompartmentSlot();
		if (!compartment)
			return false;
		
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(user);
		if (!character)
			return false;

		CompartmentAccessComponent compartmentAccess = character.GetCompartmentAccessComponent();
		if (!compartmentAccess)
			return false;
		
		IEntity owner = compartment.GetOwner();
		Vehicle vehicle = Vehicle.Cast(SCR_Global.GetMainParent(owner, true));
		if (vehicle)
		{
			Faction characterFaction = character.GetFaction();
			Faction vehicleFaction = vehicle.GetFaction();
			if (characterFaction && vehicleFaction && characterFaction.IsFactionEnemy(vehicleFaction))
			{
				SetCannotPerformReason("#AR-UserAction_SeatHostile");
				return false;
			}
		}
		
		if (compartment.GetOccupant())
		{
			SetCannotPerformReason("#AR-UserAction_SeatOccupied");
			return false;
		}
		
		// Check if the position isn't lock.
		if (m_pLockComp && m_pLockComp.IsLocked(user, compartment))
		{
			SetCannotPerformReason(m_pLockComp.GetCannotPerformReason(user));
			return false;
		}
		
		// Make sure vehicle can be enter via provided door, if not, set reason.
		if (!compartmentAccess.CanGetInVehicleViaDoor(owner, compartment, GetRelevantDoorIndex(user)))
		{
			SetCannotPerformReason("#AR-UserAction_SeatObstructed");
			return false;
		}
		
		// -------------------------------- Modded -------------------------------- //
		// WR_TODO - Find a way to optimise these a bit
		auto lockingComponent = WR_VehicleLockingComponent.Cast( GetCompartmentSlot().GetVehicle().FindComponent(WR_VehicleLockingComponent) );
		if (!lockingComponent)
		{
			return true;
		}
		// check if vehicle is unlocked first, most likely scenario for random vehicles
		if (lockingComponent.GetVehicleLock() == WR_VehicleLockMode.Unlocked)
		{
			return true;
		}
		// check if vehicle is locked
		if (lockingComponent.GetVehicleLock() == WR_VehicleLockMode.Locked)
		{
			SetCannotPerformReason("Vehicle Locked");
			return false;
		}
		// check if vehicle is faction locked and if player faction matches
		if (lockingComponent.GetVehicleLock() == WR_VehicleLockMode.FactionUnlocked && SCR_ChimeraCharacter.Cast(user).GetFaction() != lockingComponent.GetVehicleOwnerFaction())
		{
			SetCannotPerformReason("Vehicle Locked");
			return false;
		}
		// check if vehicle is group locked and if player group matches
		SCR_AIGroup playerGroup = SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(ESE.GetPlayerId(user)); // get player group
		if (lockingComponent.GetVehicleLock() == WR_VehicleLockMode.GroupUnlocked && playerGroup != lockingComponent.GetVehicleOwnerGroup())
		{
			SetCannotPerformReason("Vehicle Locked");
			return false;
		}
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		BaseCompartmentSlot compartment = GetCompartmentSlot();
		if (!compartment)
			return false;
		
		ChimeraCharacter character = ChimeraCharacter.Cast(user);
		if (character && character.IsInVehicle())
			return false;
		
		CompartmentAccessComponent compartmentAccess = character.GetCompartmentAccessComponent();
		if (!compartmentAccess)
			return false;
		
		if (compartmentAccess.IsGettingIn() || compartmentAccess.IsGettingOut())
			return false;
		
		return true;
	}	
};