// Class for detecting players in a trigger area, and enabling/disabling sibling SCR_SpawnPoint's within it
//
// This class handles all town spawning, not Wasteland_Town.c
//
//

#define PRINT_DEBUG

[EntityEditorProps(category: "GameScripted/Triggers", description: "Wasteland town player detector")]
class Wasteland_PlayerDetectorClass: SCR_BaseTriggerEntityClass
{
};

class Wasteland_PlayerDetector: SCR_BaseTriggerEntity 
{
	// user script
	ref array<SCR_SpawnPoint> spawnPoints = new array<SCR_SpawnPoint>();
	ref array<string> factionsKeys = {"US", "USSR", "FIA"};
	
	protected string townName;
	
	protected override void EOnInit(IEntity owner)
	{
		townName = GetParent().GetParent().GetName();
		Print("=================================");
		Print("townName: " + townName);
		
		// redundant for now but may get working one day
		// --------------------------------------------------------------------------
		string townNameInternal = Wasteland_Town.Cast(GetParent().GetParent()).GetTownName();	
		Print("townNameInternal: " + townNameInternal);
		
		Wasteland_UIInfo tempUI = new Wasteland_UIInfo();
		tempUI.UpdateName(townNameInternal);
		// --------------------------------------------------------------------------
		
		// Gets siblings of trigger including itself, then filters itself out leaving only spawns.
		// If you want to add other entities to the spawn prefab, make sure they either dont have		
		// hierarchy or are under a sub entity or this will break currently.
				
		GetAllSpawns(GetParent(), spawnPoints);
		
		for (int i = 0; i < spawnPoints.Count(); i++)
		{
			spawnPoints[i].SetFactionKey(factionsKeys[i]);
			// redundant
			spawnPoints[i].LinkInfo(tempUI);
			Print("SCR_SpawnPoint.GetInfo().GetName(): " + spawnPoints[i].GetInfo().GetName());
		}
	}
	// modified GetAllChildren() that sorts non SCR_SpawnPoint entities out of the output array
	static void GetAllSpawns(IEntity parent, notnull inout array<SCR_SpawnPoint> allChildren)
	{
		if (!parent)
			return;
		
		IEntity child = parent.GetChildren();
		
		while (child)
		{
			if (child.ClassName() == "SCR_SpawnPoint")
			{
				SCR_SpawnPoint spawn = SCR_SpawnPoint.Cast(child);
				allChildren.Insert(spawn);
			}
			child = child.GetSibling();
		}
	}
		
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		SCR_ChimeraCharacter cc = SCR_ChimeraCharacter.Cast(ent);
		if (!cc) return false;
		if (!IsAlive(cc)) return false;
		return true;
	}
	
	//-----------------------------------------------------------------------------------------//
	// ======================================= Running ======================================= //
	//-----------------------------------------------------------------------------------------//
	
	// playerCount holds values for number of each faction in trigger, always US, USSR, FIA same as factionsKeys
	ref array<IEntity> allEntities = new array<IEntity>();
	ref array<int> playerCount = {0,0,0};
	int totalPlayerCount = 0;
	
	private void UpdateDetector()
	{
		// Update spawns based on playerCount
		
		if (!totalPlayerCount)
		{
			foreach (SCR_SpawnPoint spawn : spawnPoints)
			{
				spawn.SetFactionKey("Inactive"); // Deactivate
			}
		}
		else
		{
			for (int i = 0; i < spawnPoints.Count(); i++)
			{
				if (playerCount[i] >= Math.Ceil(totalPlayerCount / 2)) spawnPoints[i].SetFactionKey(factionsKeys[i]); // Activate
				else spawnPoints[i].SetFactionKey("Inactive"); // Deactivate
			}
		}
	}
	
	protected override void OnActivate(IEntity ent)
	{
		// get all entities in trigger and find all SCR_ChimeraCharacter's
		// loop over factionsKeys and if the character faction matches the key, add 1 to that faction's playerCount
		playerCount = {0,0,0};
		totalPlayerCount = 0;
		
		GetEntitiesInside(allEntities);
		for (int entCount = 0; entCount < allEntities.Count(); entCount++)
		{
			if (allEntities[entCount].ClassName() == "SCR_ChimeraCharacter")
			{
				totalPlayerCount++;
				for (int i = 0; i < factionsKeys.Count(); i++)
				{
					if (SCR_ChimeraCharacter.Cast(allEntities[entCount]).GetFactionKey() == factionsKeys[i]) playerCount[i] = playerCount[i] + 1; //operator ++ not supported on temporary variables (TODO)
				}
			}
		}
		UpdateDetector();
		#ifdef PRINT_DEBUG
		Print("=========================");
		Print(townName + " Active");
		Print("US: " + playerCount[0] + " USSR: " + playerCount[1] + " FIA: " + playerCount[2]);
		Print("Total Players: " + totalPlayerCount);
		#endif
		
	}
	
	protected override void OnDeactivate(IEntity ent)
	{	
		totalPlayerCount--;
		for (int i = 0; i < factionsKeys.Count(); i++)
				{
					if (SCR_ChimeraCharacter.Cast(ent).GetFactionKey() == factionsKeys[i]) playerCount[i] = playerCount[i] - 1; //operator ++ not supported on temporary variables (TODO)
				}
		UpdateDetector();
		#ifdef PRINT_DEBUG
		Print("=========================");
		Print(townName + " Deactivate: " + ent.ClassName());
		Print("US: " + playerCount[0] + " USSR: " + playerCount[1] + " FIA: " + playerCount[2]);
		Print("Total Players: " + totalPlayerCount);
		#endif
	}

	// Gets and Sets
	void SetTownName(string name){
		townName = name;}
	
	
};

