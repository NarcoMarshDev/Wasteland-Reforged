class Trigger_RandomSpawnTeleport_Class: SCR_BaseTriggerEntity 
{
	// user script
	SCR_ChimeraCharacter cc;
	//ref array<IEntity> entities = new array<IEntity>();
	array<SCR_SpawnPoint> spawnPoints = SCR_SpawnPoint.GetSpawnPoints();
	
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		SCR_ChimeraCharacter temp = SCR_ChimeraCharacter.Cast(ent);
		if (temp != null)
		{
			cc = temp;
			return true;
		}
		else return false;
		
		//Print("swag");
		//Print(cc);
		//if (!cc) x = false;
		//if (!IsAlive(cc)) x = false;
		//if (cc.GetFactionKey() != "US") x = false;
		//return x;
	}
	
	protected ref RandomGenerator m_pRandomGenerator = new RandomGenerator();
	int random;
	vector tpTransform[4];
	
	override void OnActivate(IEntity ent)
	{
		//testMissionEnd();
		
		random = Math.RandomInt(0, spawnPoints.Count());
		Print(random);
		Print(cc);
		spawnPoints[random].GetTransform(tpTransform);
		Print(tpTransform);
		cc.Teleport(tpTransform);
		
		//vector mat[4];
		//Math3D.MatrixIdentity4( mat );

		//cc.SetTransform(mat);
	
		//vector outmat[4];
		//cc.GetTransform(outmat );
		//Print( outmat );
	}
	
	void testMissionEnd()
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode()); // Get the game mode for the end script
        Faction faction = GetGame().GetFactionManager().GetFactionByKey("US"); // Get the winning faction Key
        int usIndex = GetGame().GetFactionManager().GetFactionIndex(faction); // Get the winning faction key's index
        gameMode.EndGameMode(SCR_GameModeEndData.CreateSimple(SCR_GameModeEndData.ENDREASON_EDITOR_FACTION_VICTORY, -1, usIndex));
	}

};

