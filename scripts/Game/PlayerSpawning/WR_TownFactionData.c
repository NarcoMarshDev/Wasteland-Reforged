class WR_TownFactionData
{
	private string m_sFaction;
	private int m_iPlayerCount;
	private WR_SpawnPoint m_SpawnPoint;
	private SCR_MapUISpawnPoint m_UI;
	
	string GetFactionKey() 			{return m_sFaction;}
	int GetPlayerCount() 			{return m_iPlayerCount;}
	WR_SpawnPoint GetSpawnPoint() 	{return m_SpawnPoint;}
	SCR_MapUISpawnPoint GetUI() 	{return m_UI;}
	
	void IncrementPlayerCount() 	{m_iPlayerCount++;}
	void DecrementPlayerCount()		{m_iPlayerCount--;}
	void ResetPlayerCount()			{m_iPlayerCount = 0;}
	
	void EnableSpawn()				{m_SpawnPoint.SetFactionKey(m_sFaction);}
	void DisableSpawn() 			{m_SpawnPoint.SetFactionKey("DISABLED");}
	
	void WR_TownFactionData(string faction, WR_SpawnPoint spawn, SCR_MapUISpawnPoint ui=null)
	{
		m_sFaction = faction;
		m_iPlayerCount = 0;
		m_SpawnPoint = spawn;
		m_UI = ui;
	}
	
	void ~WR_TownFactionData()
	{
		
	}
}