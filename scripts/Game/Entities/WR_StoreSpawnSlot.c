class WR_StoreSpawnSlotClass: GenericEntityClass
{
};

class WR_StoreSpawnSlot: GenericEntity
{
	WR_StoreComponent m_StoreComponent;
	IEntity m_LastOccupant = null;
	float m_fLastOccupiedTime = 0;
	
	override void EOnInit(IEntity owner)
	{
		m_StoreComponent = WR_StoreComponent.Cast( GetParent().FindComponent(WR_StoreComponent) );
		m_StoreComponent.RegisterSpawnSlot(this);
	}
		
	void SetOccupant(IEntity ent)
	{
		m_LastOccupant = ent;
		// return without updating the last occupied time if occupant is set to null
		if (!ent)
			return;
		
		m_fLastOccupiedTime = GetGame().GetWorld().GetWorldTime();
	}
	
	IEntity GetOccupant()
	{
		return m_LastOccupant;
	}
	
	float GetLastOccupiedTime()
	{
		return m_fLastOccupiedTime;
	}
}