//#ifdef WASTELAND_BROKEN
//------------------------------------------------------------------------------------------------
modded class SCR_RespawnSystemComponentClass: RespawnSystemComponentClass
{
};

//! Scripted implementation that handles spawning and respawning of players.
//! Should be attached to a GameMode entity.
[ComponentEditorProps(icon: HYBRID_COMPONENT_ICON)]
modded class SCR_RespawnSystemComponent : RespawnSystemComponent
{
	protected override override GenericEntity RequestSpawn(int playerId)
	{
		// Catch illicit requests,
		// TODO@AS:
		// TODO@LK:
		// We should probably make it so RequestRespawn()
		// is not even called from client if !CanPlayerRespawn(playerId)
		// and only resort to this as a safety measure
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (gameMode && !gameMode.CanPlayerRespawn(playerId) && !m_bCustomRespawn)
		{
			Print("gameMode: " + gameMode);
			Print("gameMode.CanPlayerRespawn(playerId): " + gameMode.CanPlayerRespawn(playerId));
			Print("m_bCustcomRespawn: " + m_bCustomRespawn);
			
			
			Print("Requested spawn denied! GameMode returned false in CanPlayerRespawn() for playerId=" + playerId, LogLevel.WARNING);
			return null;
		}
		
		if (m_bCustomRespawn)
		{
			m_CustomSpawnedEntity = DoSpawn(m_sCustomRespawnPrefab, m_vCustomRespawnPos, m_vCustomRespawnRot);
			if (!m_CustomSpawnedEntity)
				return null;

			FactionAffiliationComponent affiliationComp = FactionAffiliationComponent.Cast(m_CustomSpawnedEntity.FindComponent(FactionAffiliationComponent));
			if (affiliationComp)
			{
				Faction faction = affiliationComp.GetAffiliatedFaction();
				if (faction)
					DoSetPlayerFaction(playerId, GetFactionIndex(faction));
			}

			return m_CustomSpawnedEntity;
		}

		SCR_BasePlayerLoadout loadout = GetPlayerLoadout(playerId);
		if (!loadout)
		{
			Print(LOG_HEAD+" No valid entity to spawn could be returned in RequestSpawn. Are there valid loadouts for the target player faction?", LogLevel.ERROR);
			return null;
		}

		SCR_SpawnPoint spawnPoint = GetPlayerSpawnPoint(playerId);
		if (!spawnPoint)
		{
			Print(LOG_HEAD+" No valid spawn point available in RequestSpawn. Player will not spawn!", LogLevel.ERROR);
			return null;
		}
		
		SCR_PlayerSpawnPoint playerSpawnPoint = SCR_PlayerSpawnPoint.Cast(spawnPoint);
		if (playerSpawnPoint)
		{
			if (!CanSpawnOnPlayerSpawnPoint(playerSpawnPoint))
			{
				string playerFactionKey;
				Faction faction = GetPlayerFaction(playerId);
				if (faction)
					playerFactionKey = faction.GetFactionKey();
				
				SCR_SpawnPoint nearestSpawnPoint = FindNearestAvailableSpawnPoint(playerSpawnPoint.GetOrigin(), playerFactionKey, playerId);
				if (nearestSpawnPoint)
					spawnPoint = nearestSpawnPoint;
			}
		}
		
		vector spawnPosition = vector.Zero;
		vector spawnRotation = vector.Zero;
		
		// ================ MODDED START ================
		
		if (spawnPoint)
		{
			Print("there's a spawn point bro");
			IEntity parentTrigger = spawnPoint.GetParent();
			if (!parentTrigger || !parentTrigger.IsInherited(WR_TownTrigger))
			{
				spawnPoint.GetPositionAndRotation(spawnPosition, spawnRotation);
			}
			else
			{
				float triggerRadius = WR_TownTrigger.Cast(parentTrigger).GetSphereRadius();
				
				vector coords = spawnPoint.GetOrigin();
				
				float x = coords[0] + Math.RandomFloatInclusive(-triggerRadius, triggerRadius);
				float z = coords[2] + Math.RandomFloatInclusive(-triggerRadius, triggerRadius);
				
				vector newCoords[4];
				Math3D.MatrixIdentity4( newCoords );
				newCoords[3] = Vector(x,coords[1],z);
				SCR_TerrainHelper.SnapToTerrain(newCoords, GetGame().GetWorld(), true);
				
				spawnPosition = newCoords[3];
			}
			m_pGameMode.OnSpawnPointUsed(spawnPoint, playerId);
		}
		
		// ================ MODDED END ================

		GenericEntity spawned = DoSpawn(loadout.GetLoadoutResource(), spawnPosition, spawnRotation);
		loadout.OnLoadoutSpawned(spawned, playerId);
		if (spawnPoint)
			spawnPoint.EOnPlayerSpawn(spawned);
		
		return spawned;
	}
};
//#endif