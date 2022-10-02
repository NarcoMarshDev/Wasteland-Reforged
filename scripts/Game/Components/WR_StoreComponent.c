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