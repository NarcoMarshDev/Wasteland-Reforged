class WR_PlayerProfile
{
	int m_PlayerId = -1;
	string m_PlayerGUID = "";
	
	int m_MoneyCash = 0;
	int m_MoneyBank = 0;
	
	int m_Kills = 0;
	int m_Deaths = 0;
	
	bool m_IsAlive = false;
	vector m_LastLocation[4];
	
	protected SCR_ChimeraCharacter m_PlayerCharacter = null;
	// ---------------------------------------------------------------- Methods -------------------------------------------------------- //
	void SetPlayerCharacter(SCR_ChimeraCharacter character)
	{
		m_PlayerCharacter = character;
	}
	// --------------------------------------------------------------------------------------------------------------------------------- //
	
	bool LoadPlayerProfileFromFile(string path)
	{
		array<string> fileOutput = {};
		
		ESE_IO.ReadFileAsArray(path, fileOutput, -1);
		
		return true;
	}
		
	void WR_PlayerProfile(int playerId=-1, string playerGUID="", int cash=0, int bank=0, int kills=0, int deaths=0, SCR_ChimeraCharacter character=null)
	{
		m_PlayerId = playerId;
		m_PlayerGUID = playerGUID;
	
		m_MoneyCash = cash;
		m_MoneyBank = bank;
	
		m_Kills = kills;
		m_Deaths = deaths;
	}
}