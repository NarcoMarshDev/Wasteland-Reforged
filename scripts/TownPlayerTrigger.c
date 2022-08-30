class TownPlayerDetector_Class: SCR_BaseTriggerEntity 
{
	// user script
	SCR_ChimeraCharacter cc;
	ref array<SCR_SpawnPoint> spawnPoints = new array<SCR_SpawnPoint>();
	SCR_SpawnPoint SpawnPointUS;
	SCR_SpawnPoint SpawnPointUSSR;
	SCR_SpawnPoint SpawnPointFIA;
	
	string playerFaction;
	
	int countUS = 0;
	int countUSSR = 0;
	int countFIA = 0;
	
	bool done = false;
	
	private void GetAllChildren(IEntity parent, notnull inout array<SCR_SpawnPoint> allChildren) // modified to cast to spawn point entities
	{
		if (!parent)
			return;
		
		SCR_SpawnPoint child = SCR_SpawnPoint.Cast(parent.GetChildren());
		
		while (child)
		{
			allChildren.Insert(child);
			child = SCR_SpawnPoint.Cast(child.GetSibling());
		}
	}
	
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (!done)
		{
			GetAllChildren(this, spawnPoints);
			Print(spawnPoints);
			vector trans[4];
			for (int i = 0; i < 3; ++i)
			{	
				Print(spawnPoints.IsEmpty());
				Print(i);
				SCR_SpawnPoint temp = spawnPoints.Get(i);
				Print(temp);
				if (temp.GetFactionKey() == "US") SpawnPointUS = temp;
				else if (temp.GetFactionKey() == "USSR") SpawnPointUSSR = temp;
				else if (temp.GetFactionKey() == "FIA") SpawnPointFIA = temp;
				else Print("nothing");
				temp.GetTransform(trans);
				Print(trans);
			}
			done = true;
		
			vector trans2[4]
			SpawnPointUS.GetTransform(trans2);
			Print(trans2);
			
		}
		SCR_ChimeraCharacter temp = SCR_ChimeraCharacter.Cast(ent);
		if (temp != null)
		{
			cc = temp;
			playerFaction = cc.GetFactionKey();
			return true;
		}
		else return false;		
	}
	
	override void OnActivate(IEntity ent)
	{
		if(playerFaction == "US") countUS++;
		else if(playerFaction == "USSR") countUSSR++;
		else if(playerFaction == "FIA") countFIA++;
		
		int total = countUS + countUSSR + countFIA;
		int half = total / 2;
		Print(countUS);
		Print(half);
		Print(SpawnPointUS);
		if (countUS >= half) SpawnPointUS.Activate(); // currently allows spawning with 1 friendly and 2 enemies, 2 with 3, 3 with 4, etc.
		if (countUSSR >= half) SpawnPointUSSR.Activate();
		if (countFIA >= half) SpawnPointFIA.Activate();
		Print(countUS);
	}
	
	override void OnDeactivate(IEntity ent)
	{
		if(playerFaction == "US") countUS--;
		else if(playerFaction == "USSR") countUSSR--;
		else if(playerFaction == "FIA") countFIA--;
	
		int total = countUS + countUSSR + countFIA;
		int half = total / 2;
		if (countUS < half) SpawnPointUS.Deactivate();
		if (countUSSR < half) SpawnPointUSSR.Deactivate();
		if (countFIA < half) SpawnPointFIA.Deactivate();
		Print(countUS);
	}

};

