class WR_SpawnPointClass : SCR_SpawnPointClass
{
}

class WR_SpawnPoint : SCR_SpawnPoint
{
	void SetVisibleInDeployMapOnly(bool state)
	{
		m_bShowInDeployMapOnly = state;
	}
}