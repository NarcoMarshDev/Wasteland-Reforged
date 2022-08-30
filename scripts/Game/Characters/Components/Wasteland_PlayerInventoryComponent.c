class Wasteland_PlayerInventoryComponentClass: ScriptComponentClass
{	
}

class Wasteland_PlayerInventoryComponent: ScriptComponent
{
	int playerId;
	
	int m_money;
	int m_repairKits;
	int m_jerryCans;
	int m_medkits;
	
	override void OnPostInit(IEntity owner)
	{
		void;
	}
	
	// gets
	
	int getMoney() return m_money;
	int getRepairKits() return m_repairKits;
	int getJerryCans() return m_jerryCans;
	int getMedkits() return m_medkits;
	
	// sets
	
	void addMoney(int value)
	{
		m_money += value;
	}
	
	bool addRepairKit(int value)
	{
		int temp = m_repairKits + value;
		if (temp > WR_Statics.max_repairKits)
			return false;
		
		m_repairKits = temp;
		return true;
	}
	
	bool addJerryCan(int value)
	{
		int temp = m_jerryCans + value;
		if (temp > WR_Statics.max_jerryCans)
			return false;
		
		m_jerryCans = temp;
		return true;
	}
	
	bool addMedkit(int value)
	{
		int temp = m_medkits + value;
		if (temp > WR_Statics.max_medkits)
			return false;
		
		m_medkits = temp;
		return true;
	}
	
	// pack and unpack functions for interacting with Wasteland_PlayerProfile
	array<int> PackData()
	{
		array<int> data = {m_money, m_repairKits, m_jerryCans, m_repairKits};
		return data;
	}
	
	void UnpackData(array<int> data)
	{
		m_money      =	data[0];
		m_repairKits = 	data[1];
		m_jerryCans  =	data[2];
		m_repairKits = 	data[3];
	}
}