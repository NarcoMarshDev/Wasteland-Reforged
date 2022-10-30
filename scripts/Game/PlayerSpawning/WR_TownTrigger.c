#define PRINT_DEBUG

class WR_TownTriggerClass: SCR_BaseTriggerEntityClass
{
};
// #WR_TODO - Change this so it only requires a friendly in the territory to spawn, not population majority, maybe have it an option though
class WR_TownTrigger: SCR_BaseTriggerEntity 
{
	[Attribute("", UIWidgets.EditBox, "Related entity")]
	string m_sTownName;

	protected ref array<ref WR_TownFactionData> m_FactionData = new array<ref WR_TownFactionData>();
	protected ref array<IEntity> m_AllEntities = new array<IEntity>();
	protected int m_iTotalPlayerCount = 0;

	protected override void EOnInit(IEntity owner)
	{
		IEntity child = GetChildren();
		while (child)
		{
			if (child.IsInherited(WR_SpawnPoint))
			{
				WR_SpawnPoint.Cast(child).GetInfo()._WR_SetName(m_sTownName); // to replace modded SCR_UIName with SetName() method
				//WR_SpawnPoint.Cast(child).SetVisibleInDeployMapOnly(false);
				// create new data object with faction key and spawn point
				ref WR_TownFactionData data = new WR_TownFactionData(WR_SpawnPoint.Cast(child).GetFactionKey(), WR_SpawnPoint.Cast(child)); // not the issue
				Print("data: " + data);
				m_FactionData.Insert(data);
			}
			child = child.GetSibling();
		}
		Print("m_FactionData: " + m_FactionData);
		foreach (WR_TownFactionData data : m_FactionData)
		{
			Print(data.GetFactionKey() + " " + data.GetSpawnPoint() + " " + data.GetPlayerCount());
		}
	}
	
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		SCR_ChimeraCharacter cc = SCR_ChimeraCharacter.Cast(ent);
		if (!cc) return false;
		if (!IsAlive(cc)) return false;
		return true;
	}
	
	protected override void OnActivate(IEntity ent)
	{
		// Find all ChimeraCharacter entities in trigger and check their factions to add to counts
		m_iTotalPlayerCount = 0;
		
		foreach (WR_TownFactionData data : m_FactionData) // this is such a bruh moment please find a better way to do this
		{
			data.ResetPlayerCount();
		}
		GetEntitiesInside(m_AllEntities);
		foreach (IEntity entity : m_AllEntities)
		{
			if (entity.IsInherited(ChimeraCharacter))
			{
				m_iTotalPlayerCount++;
				int time = 0;
				Print("===================");
				foreach (WR_TownFactionData data : m_FactionData)
				{
					//data.ResetPlayerCount(); - old way of doing it
					if (SCR_ChimeraCharacter.Cast(entity).GetFactionKey() == data.GetFactionKey())
					{
						data.IncrementPlayerCount();
					}
					if (data.GetPlayerCount() && data.GetPlayerCount() >= Math.Floor(m_iTotalPlayerCount/2))
					{
						data.EnableSpawn();
					}
					else
					{
						data.DisableSpawn();
					}
					time++;
					Print("time around: " + time);
					Print("Faction: " + data.GetFactionKey() + " | Players: " + data.GetPlayerCount());
				}
				Print("===================");
			}
		}
		Print("Total Player Count: " + m_iTotalPlayerCount);
	}
	
	protected override void OnDeactivate(IEntity ent)
	{
		m_iTotalPlayerCount--;
		foreach (WR_TownFactionData data : m_FactionData)
		{
			if (!m_iTotalPlayerCount)
			{
				data.DisableSpawn();
			}
			else
			{
				if (SCR_ChimeraCharacter.Cast(ent).GetFactionKey() == data.GetFactionKey())
				{
					data.DecrementPlayerCount();
				}
				if (data.GetPlayerCount() && data.GetPlayerCount() >= Math.Floor(m_iTotalPlayerCount/2)) // checking to enable or disable in OnDeactivate() is kind of pointless
				{																    					 // just disable if player count is less than half total or do nothing
					data.EnableSpawn();
				}
				else
				{
					data.DisableSpawn();
				}
			}
			Print("Faction: " + data.GetFactionKey() + " | Players: " + data.GetPlayerCount());
		}
		Print("Total Player Count: " + m_iTotalPlayerCount);
	}
}