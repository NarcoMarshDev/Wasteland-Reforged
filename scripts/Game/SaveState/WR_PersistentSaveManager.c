class WR_PersistentSaveManagerComponentClass: SCR_BaseGameModeComponentClass
{
};
class WR_PersistentSaveManagerComponent: SCR_BaseGameModeComponent
{
	[Attribute(defvalue: "{5761977075239861}SaveState/WR_PersistentSaveData.conf" ,uiwidget: UIWidgets.Auto, desc: "Persistent save data file (.conf)")]
	ResourceName m_rSaveFilePath;
	
	[Attribute(defvalue: "1", uiwidget: UIWidgets.Auto)]
	bool m_bSaveVehicles;
	
	[Attribute(defvalue: "1", uiwidget: UIWidgets.Auto)]
	bool m_bSaveBuildings;
	
	[Attribute(defvalue: "1", uiwidget: UIWidgets.Auto)]
	bool m_bSavePlayers;
	
	typename m_VehicleTypename = Vehicle;
	typename m_BuildingTypename = WR_BuildingObjectEntity;
	typename m_PlayerTypename = SCR_ChimeraCharacter;
	
	WR_SaveRootStruct m_Root;
	
	ref array<int> m_aPlayerIdPointers;
	BackendApi m_backendApi;
	
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPostInit(IEntity owner) {SetEventMask(owner, EntityEvent.INIT);}
	override void EOnInit(IEntity owner)
	{
		m_aPlayerIdPointers = new array<int>();
		m_backendApi = GetGame().GetBackendApi();
		
		WR_SaveRootStruct root = SCR_ConfigHelperT<WR_SaveRootStruct>.GetConfigObject(m_rSaveFilePath);
		if (!root)
		{
			root = new WR_SaveRootStruct();
		}
		m_Root = root;
		
		
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPlayerConnected(int playerId)
	{
		// this finds the player struct that matches the connected player's uid, then inserts the index of that struct into
		// m_aPlayerIdPointers so you can use playerId to reference the corrent struct
		string uid = m_backendApi.GetPlayerUID(playerId);
		for (int i = 0; i < m_Root.m_aPlayerEntries.Count(); i++)
		{
			if (m_Root.m_aPlayerEntries[i].m_sPlayerUID == uid)
			{
				m_aPlayerIdPointers.Insert(i);
			}
		}
		
		// find vehicles with matching uid and set owner id to current playerId
		
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	void SpawnAllEntities()
	{
		if (m_bSaveVehicles)
		{
			foreach (WR_VehicleSaveStruct vehicleStruct: m_Root.m_aVehicleEntries)
			{
				SpawnVehicle(vehicleStruct);
			}
		}
		if (m_bSaveBuildings)
		{
			foreach (WR_BuildingSaveStruct buildingStruct: m_Root.m_aBuildingEntries)
			{
				SpawnBuilding(buildingStruct);
			}
		}
		if (m_bSavePlayers)
		{
			foreach (WR_PlayerSaveStruct playerStruct: m_Root.m_aPlayerEntries)
			{
				// do something maybe? when a player connects, just find their entry and do m_aPlayerIdPointers.InsertAt(pointerToEntry, playerId);
			}
		}
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	void SpawnVehicle(WR_VehicleSaveStruct saveStruct)
	{
		IEntity ent = ESE_Entities.SpawnPrefab(saveStruct.m_vLastLocation, saveStruct.m_EntityPrefab);
		if (!ent)
			return;
		
		WR_VehicleLockingComponent lockingComponent = ESE_GetComponent<WR_VehicleLockingComponent>.FindFirst(ent);
		if (lockingComponent)
		{
			// #WR_TODO - for now vehicle locking and ownership is not gettin set on server restart due to the owner being a playerId not playerUID
			//lockingComponent.SetVehicleLock(saveStruct.m_LockMode);
			lockingComponent.SetVehicleLock(WR_VehicleLockMode.Unlocked);
		}
		
		SCR_FuelConsumptionComponent fuelComponent = ESE_GetComponent<SCR_FuelConsumptionComponent>.FindFirst(ent);
		if (fuelComponent)
		{
			fuelComponent.GetCurrentFuelTank().SetFuel(saveStruct.m_fFuel);
		}
		
		SCR_DamageManagerComponent damageComponent = ESE_GetComponent<SCR_DamageManagerComponent>.FindFirst(ent);
		if (damageComponent)
		{
			damageComponent.SetHealthScaled(saveStruct.m_fHealth);
		}
		
		SCR_InventoryStorageManagerComponent invComponent = ESE_GetComponent<SCR_InventoryStorageManagerComponent>.FindFirst(ent);
		if (invComponent)
		{
			foreach (ResourceName prefab: saveStruct.m_aInventoryResources)
			{
				invComponent.TrySpawnPrefabToStorage(prefab);
			}
		}
		// #WR_TODO - register entity to vehicle manager here or make it so vehicles auto self insert on init
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	void SpawnBuilding(WR_BuildingSaveStruct saveStruct)
	{
		IEntity ent = ESE_Entities.SpawnPrefab(saveStruct.m_vLastLocation, saveStruct.m_EntityPrefab);
		if (!ent)
			return;
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	void SpawnPlayer(WR_PlayerSaveStruct saveStruct, int playerId)
	{
		IEntity ent = ESE_Entities.SpawnPrefab(saveStruct.m_vLastLocation, saveStruct.m_EntityPrefab);
		if (!ent)
			return;
		
		SCR_PlayerController.Cast( GetGame().GetPlayerManager().GetPlayerController(playerId) ).SetControlledEntity(ent);
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	void RegisterVehicle(Vehicle vehicle)
	{
		WR_VehicleSaveStruct struct = new WR_VehicleSaveStruct(vehicle);
		if (!struct)
			return;
		
		m_Root.m_aVehicleEntries.Insert(struct);
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	void RegisterBuilding(IEntity building)
	{
		WR_BuildingSaveStruct struct = new WR_BuildingSaveStruct(building);
		if (!struct)
			return;
		
		m_Root.m_aBuildingEntries.Insert(struct);
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	// this might need to be changed
	void RegisterPlayerProfile(WR_PlayerProfile profile)
	{
		WR_PlayerSaveStruct struct = new WR_PlayerSaveStruct(profile);
		if (!struct)
			return;
		
		m_Root.m_aPlayerEntries.Insert(struct);
	}
	// ---------------------------------------------------------------------------------------------------------------- //
}