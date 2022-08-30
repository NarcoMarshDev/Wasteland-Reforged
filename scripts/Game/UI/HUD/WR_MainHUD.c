class WR_MainHUD : SCR_InfoDisplayExtended
{

	//------------------------------------------------------------------------------------------------
	void CreateLayout(string layout)
	{		
		// Destroy existing layout
		DestroyLayout();
		
		// Create weapon info layout
		SCR_HUDManagerComponent manager = SCR_HUDManagerComponent.GetHUDManager();
		if (manager)
			m_wRoot = manager.CreateLayout(layout, m_eLayer);

		if (!m_wRoot)
			return;
	}
	
	//------------------------------------------------------------------------------------------------
	void DestroyLayout()
	{
		if (m_wRoot)
			m_wRoot.RemoveFromHierarchy();
		
		m_wRoot = null;
	}
}	