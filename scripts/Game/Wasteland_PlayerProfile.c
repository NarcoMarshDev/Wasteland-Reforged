class Wasteland_PlayerProfile
{
	string m_playerUID;
	int m_playerId;
	
	vector m_transform[4];
	
	int m_money = 600;
	int m_repairKits = 0;
	int m_jerryCans = 0;
	int m_medkits = 0;
		
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
	
	// ------------ adds ------------
	void AddMoney(int value)
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
	
	// ------------ sets ------------
	void SetMoney(int value) m_money = value;
	void SetRepairKits(int value) m_repairKits = value;
	void SetJerryCans(int value) m_jerryCans = value;
	void SetMedkits(int value) m_medkits = value;
	
	void SetPlayerUID(string playerUID) m_playerUID = playerUID;
	void SetPlayerId(int playerId) m_playerId = playerId;
			
	// ------------ gets ------------
	int GetMoney() return m_money;
	int getRepairKits() return m_repairKits;
	int getJerryCans() return m_jerryCans;
	int getMedkits() return m_medkits;
	
	string GetPlayerUID() return m_playerUID;
	int GetPlayerId() return m_playerId;
}