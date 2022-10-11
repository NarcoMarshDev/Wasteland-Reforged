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
	
	array<IEntity> m_SpawnSlots;
	
	override void EOnInit(IEntity owner)
	{
		m_SpawnSlots = ESE.GetAllChildrenByType(GetOwner(), WR_StoreSpawnSlot);
	}
	
	WR_StoreType GetStoreType()
	{
		return m_StoreType;
	}
	
	IEntity GetStoreReference()
	{
		return this.GetOwner();
	}
	
	WR_StoreSpawnSlot GetRandomSpawnSlot()
	{
		return WR_StoreSpawnSlot.Cast( m_SpawnSlots.GetRandomElement() );
	}
}