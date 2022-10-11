class WR_StoreSpawnSlotClass: GenericEntityClass
{
};

class WR_StoreSpawnSlot: GenericEntity
{
	protected bool m_bIsOccupied = false;
	
	bool IsOccupied()
	{
		return m_bIsOccupied;
	}
	
	void SetOccupied(bool state)
	{
		m_bIsOccupied = state;
	}
}