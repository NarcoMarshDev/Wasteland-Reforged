class WR_StoreSpawnSlotClass: GenericEntityClass
{
};

class WR_StoreSpawnSlot: GenericEntity
{
	protected bool m_bIsOccupied = false;
	WR_StoreComponent m_StoreComponent;
	
	override void EOnInit(IEntity owner)
	{
		m_StoreComponent = WR_StoreComponent.Cast( GetParent().FindComponent(WR_StoreComponent) );
		m_StoreComponent.RegisterSpawnSlot(this);
	}
	
	bool IsOccupied()
	{
		return m_bIsOccupied;
	}
	
	void SetOccupied(bool state)
	{
		m_bIsOccupied = state;
	}
}