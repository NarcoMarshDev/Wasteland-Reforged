enum WR_StoreType
{
	GunStore,
	VehicleStore,
	GeneralStore
}

class WR_StoreComponentClass: ScriptComponentClass
{
};

class WR_StoreComponent: ScriptComponent
{
	[Attribute(defvalue: SCR_Enum.GetDefault(WR_StoreType.GunStore), uiwidget: UIWidgets.ComboBox, desc: "0: Gun Store, 1: Vehicle Store, 2: General Store", enums: ParamEnumArray.FromEnum(WR_StoreType))]
	protected WR_StoreType m_StoreType;
	
	ref array<IEntity> m_SpawnSlots = new array<IEntity>();
		
	override void OnPostInit(IEntity owner)
	{
		
	}
		
	WR_StoreType GetStoreType()
	{
		return m_StoreType;
	}
	
	IEntity GetStoreReference()
	{
		return this.GetOwner();
	}
	
	void RegisterSpawnSlot(WR_StoreSpawnSlot slot)
	{
		m_SpawnSlots.Insert(slot);
	}
	
	IEntity GetRandomSpawnSlot()
	{
		return m_SpawnSlots.GetRandomElement();
	}
}