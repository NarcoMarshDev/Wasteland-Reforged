class WR_EventListenerComponentClass: ScriptComponentClass
{	
};
// #WR_OBSOLETE
class WR_EventListenerComponent: ScriptComponent
{
	protected InputManager m_InputManager;
	
	override void EOnInit(IEntity owner)
	{
		m_InputManager = GetGame().GetInputManager();
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		// this really isn't ideal, but not sure an easier way to do it. For now it's fine since only run on client
		if ( m_InputManager.GetActionTriggered("WR_StoreConfirmPurchase") )
		{
			Print("We should have spawned something!");
		}
	}
}