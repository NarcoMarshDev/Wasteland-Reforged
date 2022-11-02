class WR_VehicleManagerComponentClass: SCR_BaseGameModeComponentClass
{
}

class WR_VehicleManagerComponent: SCR_BaseGameModeComponent
{
	[Attribute(defvalue: "100", uiwidget: UIWidgets.Auto, desc: "Maximum number of spawnable vehicles in the world")]
	int m_iMaxVehicleCount;
	
	ESE_Queue<Vehicle> m_VehicleQueue;
	
	void RegisterVehicle(IEntity ent)
	{
		m_VehicleQueue.Enqueue
	}
	
	void WR_VehicleManagerComponent()
	{
		m_VehicleQueue = new ESE_Queue<Vehicle>(m_iMaxVehicleCount);
	}
}