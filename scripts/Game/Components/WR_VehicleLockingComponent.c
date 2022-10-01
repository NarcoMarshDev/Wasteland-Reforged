enum WR_VehicleLockMode
{
	Locked,
	Unlocked,
	FactionUnlocked,
	GroupUnlocked
}

class WR_VehicleLockingComponentClass: ScriptComponentClass
{
};

class WR_VehicleLockingComponent: ScriptComponent
{
	// ----------------------------------------------------------------
	private WR_VehicleLockMode m_pLockedMode = WR_VehicleLockMode.Unlocked;
	private int m_pOwnerPlayerId = -1;
	private Faction m_pOwnerFaction = null;
	private SCR_AIGroup m_pOwnerGroup = null;
	private bool m_pHasRemoteAccess = false;
	
	override void EOnInit(IEntity owner)
	{
		
	}
	
	bool SendRemoteUnlockRequest(int requestingPlayerId)
	{
		// WR_TODO - Send request to m_pOwnerPlayerId to enable radial menu with info on who requested, and if to accept or deny
		string requestingName = GetGame().GetPlayerManager().GetPlayerName(requestingPlayerId);
		// check if owner is alive, return if not since it's easier to just send again then handle the ui in the respawn menu
		return false;
	}
	// gets & sets
	// ----------------------------------------------------------------
	void SetVehicleLock(WR_VehicleLockMode mode)
	{
		m_pLockedMode = mode;
	}
	// ----------------------------------------------------------------
	WR_VehicleLockMode GetVehicleLock()
	{
		return m_pLockedMode;
	}
	// ----------------------------------------------------------------
	void SetVehicleOwner(int playerId)
	{
		m_pOwnerPlayerId = playerId;
		// also update owner faction when changing vehicle owner
		m_pOwnerFaction = SCR_ChimeraCharacter.Cast( GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId) ).GetFaction();
	}
	// ----------------------------------------------------------------
	int GetVehicleOwner()
	{
		return m_pOwnerPlayerId;
	}
	// ----------------------------------------------------------------
	void SetVehicleOwnerFaction(Faction ownerFaction)
	{
		m_pOwnerFaction = ownerFaction;
	}
	// ----------------------------------------------------------------
	Faction GetVehicleOwnerFaction()
	{
		return m_pOwnerFaction;
	}
	// ----------------------------------------------------------------
	void SetVehicleOwnerGroup(SCR_AIGroup group)
	{
		m_pOwnerGroup = group;
	}
	// ----------------------------------------------------------------
	SCR_AIGroup GetVehicleOwnerGroup()
	{
		return m_pOwnerGroup;
	}
	// ----------------------------------------------------------------
	bool HasRemoteAccessEnabled()
	{
		return m_pHasRemoteAccess;
	}
}