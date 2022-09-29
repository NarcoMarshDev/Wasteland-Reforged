class WR_BuildingObjects
{
	private array<ResourceName> m_pResourceArray = {
	
	"{F0EC223F2A1094C0}Prefabs/Props/Military/Sandbags/Sandbag_01_bunker_burlap.et",
	"{CD67070EFAFC28C7}Prefabs/Props/Military/Sandbags/Sandbag_01_wall_burlap.et",
	"{9C9C4BED9E19C374}Prefabs/Props/Military/Sandbags/Sandbag_01_wall_solid_burlap.et",
	"{7AF4B627D5C90235}Prefabs/Props/Military/Sandbags/Sandbag_01_round_high_burlap.et",
	"{569C0E9792A6D39A}Prefabs/Props/Military/Training/Obstacles/ObstacleRamp_01_start.et",
	"{B8576ECF78CDBCD7}Prefabs/Props/Military/Training/Obstacles/ObstacleBridge_01_start.et",
	"{DDF59362051B28BC}Prefabs/Props/Military/Fortification/BarbedTape_KnifeRest.et",
	};
	
	static array<ResourceName> GetAllAsNames()
	{
		return m_pResourceArray;
	}
	
	static array<Resource> GetAllAsResources()
	{
		array<Resource> resArray = {};
		foreach (ResourceName name : m_pResourceArray)
		{
			resArray.Insert( Resource.Load(name) );
		}
		return resArray;
	}
	
	static bool IsBuildingObject(ResourceName name)
	{
		if (m_pResourceArray.Contains(name))
			return true;
		
		return false;
	}
	
	
}