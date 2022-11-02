// ---------------------------------------------------------------------------------------------------------------- //
[BaseContainerProps(configRoot: true)]
class WR_SaveRootStruct
{	
	[Attribute(uiwidget: UIWidgets.Auto, desc: "Save Data Categories")]
	ref array<ref WR_PlayerSaveStruct> m_aPlayerEntries;
	
	[Attribute(uiwidget: UIWidgets.Auto, desc: "Save Data Categories")]
	ref array<ref WR_VehicleSaveStruct> m_aVehicleEntries;
	
	[Attribute(uiwidget: UIWidgets.Auto, desc: "Save Data Categories")]
	ref array<ref WR_BuildingSaveStruct> m_aBuildingEntries;
	
	void WR_SaveRootStruct()
	{
		m_aPlayerEntries 	= new array<ref WR_PlayerSaveStruct>();
		m_aVehicleEntries 	= new array<ref WR_VehicleSaveStruct>();
		m_aBuildingEntries 	= new array<ref WR_BuildingSaveStruct>();
	}
}
// ---------------------------------------------------------------------------------------------------------------- //
[BaseContainerProps(configRoot: true)]
class WR_BaseSaveStruct
{
	[Attribute()]
	vector m_vLastLocation[4];
	
	[Attribute()]
	ResourceName m_EntityPrefab;
}
// ---------------------------------------------------------------------------------------------------------------- //
[BaseContainerProps(configRoot: true)]
class WR_PlayerSaveStruct: WR_BaseSaveStruct
{
	[Attribute()]
	string m_sPlayerUID;
	
	[Attribute()]
	int m_iMoneyCash;
	
	[Attribute()]
	int m_iMoneyBank;
	
	[Attribute()]
	int m_iKills;
	
	[Attribute()]
	int m_iDeaths;
	
	[Attribute()]
	bool m_bIsAlive;
	
	[Attribute()]
	ref array<ResourceName> m_aInventoryResources;
	
	void WR_PlayerSaveStruct(WR_PlayerProfile profile)
	{
		if (!profile)
		{
			Print("WR_PlayerSaveStruct: Invalid player profile!", LogLevel.ERROR);
			return;
		}
		m_sPlayerUID 	= profile.m_PlayerGUID;
		m_iMoneyCash 	= profile.m_MoneyCash;
		m_iMoneyBank 	= profile.m_MoneyBank;
		m_iKills 		= profile.m_Kills;
		m_iDeaths 		= profile.m_Deaths;
		m_bIsAlive 		= profile.m_IsAlive;
		m_vLastLocation	= profile.m_LastLocation;
		m_aInventoryResources = new array<ResourceName>();
		
		m_EntityPrefab = "{37578B1666981FCE}Prefabs/Characters/Core/Character_Base.et";
		
		ChimeraCharacter char = ESE.GetPlayerEntity(profile.m_PlayerId);
		if (!char)
			return;
		
		auto invComponent = ESE_InventoryHelper.GetInventoryStorageManager(char);
		if (!invComponent)
			return;
		
		m_aInventoryResources = ESE_InventoryHelper.GetItemResources(invComponent);
		
		Print(m_aInventoryResources);		
	}
}
// ---------------------------------------------------------------------------------------------------------------- //
[BaseContainerProps(configRoot: true)]
class WR_VehicleSaveStruct: WR_BaseSaveStruct
{
	[Attribute()]
	string m_sOwnerUID;
	
	[Attribute()]
	float m_fFuel;
	
	[Attribute()]
	float m_fHealth;
	
	[Attribute()]
	WR_VehicleLockMode m_LockMode;
	
	[Attribute()]
	ref array<ResourceName> m_aInventoryResources;
	
	void WR_VehicleSaveStruct(Vehicle ent)
	{
		WR_VehicleLockingComponent lockingComponent = ESE_GetComponent<WR_VehicleLockingComponent>.FindFirst(ent);
		if (lockingComponent)
		{
			m_sOwnerUID = GetGame().GetBackendApi().GetPlayerUID( lockingComponent.GetVehicleOwner() );
			m_LockMode = lockingComponent.GetVehicleLock();
		}
		
		SCR_FuelConsumptionComponent fuelComponent = ESE_GetComponent<SCR_FuelConsumptionComponent>.FindFirst(ent);
		if (fuelComponent)
		{
			m_fFuel = fuelComponent.GetCurrentFuelTank().GetFuel();
		}
		
		SCR_DamageManagerComponent damageComponent = ESE_GetComponent<SCR_DamageManagerComponent>.FindFirst(ent);
		if (damageComponent)
		{
			m_fHealth = damageComponent.GetHealth();
		}
		
		SCR_InventoryStorageManagerComponent invComponent = ESE_GetComponent<SCR_InventoryStorageManagerComponent>.FindFirst(ent);
		if (invComponent)
		{
			m_aInventoryResources = ESE_InventoryHelper.GetItemResources(invComponent);
		}
		
		m_EntityPrefab = ent.GetPrefabData().GetPrefabName();
		
		ent.GetWorldTransform(m_vLastLocation);
	}
}
// ---------------------------------------------------------------------------------------------------------------- //
[BaseContainerProps(configRoot: true)]
class WR_BuildingSaveStruct: WR_BaseSaveStruct
{
	// #WR_TODO
	void WR_BuildingSaveStruct(IEntity ent)
	{
		m_EntityPrefab = ent.GetPrefabData().GetPrefabName();
		ent.GetWorldTransform(m_vLastLocation);
	}
}