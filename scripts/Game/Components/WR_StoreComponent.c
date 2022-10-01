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
	[Attribute(desc: "0: Gun Store, 1: Vehicle Store, 2: General Store")]
	protected WR_StoreType m_StoreType;
	
	
	override void EOnInit(IEntity owner)
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
}