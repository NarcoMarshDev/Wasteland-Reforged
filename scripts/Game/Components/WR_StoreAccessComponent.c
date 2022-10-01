class WR_CurrentStorePlayerControllerComponentClass: ScriptComponentClass
{
};
// Goes on player controller so menu UI can know what store's infomation to load, see SCR_FieldManualUI_Modded.c -> OnMenuOpen();
class WR_CurrentStorePlayerControllerComponent: ScriptComponent
{
	protected IEntity m_CurrentStoreEntity;
	
	IEntity GetCurrentStoreEntity()
	{
		return m_CurrentStoreEntity;
	}
	
	void SetCurrentStoreEntity(IEntity storeRef)
	{
		m_CurrentStoreEntity = storeRef;
	}
}