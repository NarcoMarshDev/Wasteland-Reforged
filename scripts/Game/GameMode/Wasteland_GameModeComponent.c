class Wasteland_GameModeComponentClass: SCR_BaseGameModeComponentClass
{
}

class Wasteland_GameModeComponent: SCR_BaseGameModeComponent
{	
	// =================== RESOURCE LOADING ====================
	
	ResourceName resName_moneyBag = "{927D8B7B6B11BA38}Prefabs/Items/Wasteland_MoneyBag.et";
	//ResourceName resName_moneyBag = "{A81F501D3EF6F38E}Prefabs/Items/Medicine/FieldDressing_US_01.et";

	// =================== PLAYER MANAGEMENT ===================
	
	BackendApi b_api = GetGame().GetBackendApi();
	ref array<ref Wasteland_PlayerProfile> playerProfileArray = new array<ref Wasteland_PlayerProfile>();
	
	// ======================== METHODS ========================
		
	Wasteland_PlayerProfile GetPlayerProfileWithIndex(int playerId, inout int index)
	{
		for (int i = 0; i < playerProfileArray.Count(); i++)
		{
			if (playerProfileArray[i].GetPlayerId() == playerId)
			{
				index = i;
				return playerProfileArray[i];
			}
		}
		return null;
	}
	
	Wasteland_PlayerProfile GetPlayerProfile(int playerId)
	{
		foreach (Wasteland_PlayerProfile profile : playerProfileArray)
		{
			if (profile.GetPlayerId() == playerId)
			{
				return profile;
			}
		}
		return null;
	}
	
	SCR_ChimeraCharacter GetPlayerEntity(int playerId)
	{
		return SCR_ChimeraCharacter.Cast(GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId));
	}
	
	Wasteland_PlayerInventoryComponent GetPlayerInventory(IEntity player)
	{
		return Wasteland_PlayerInventoryComponent.Cast(player.FindComponent(Wasteland_PlayerInventoryComponent));
	}
	
	// ======================= OVERRIDES ========================
	
	override void OnPlayerConnected(int playerId) // DONE AS OF 06/06/22
	{	
		string playerUID = b_api.GetPlayerUID(playerId);
		if (playerUID.Length() == 0)
		{
			playerUID = playerId.ToString();
		}
			
		// try find matching player profile and update playerId
		bool profileFound = false;
		foreach (Wasteland_PlayerProfile profile : playerProfileArray)
		{
			if (profile.GetPlayerUID() == playerUID)
			{
				profile.SetPlayerId(playerId);
				profileFound = true;
				Print("Player profile for user (" + playerUID + ") found, assigned temporary playerId (" + playerId + ")");
				break;
			}
		}
			
		if (!profileFound)
		{
			Print("No player profile for user (" + playerUID + ") found, creating new profile...");			
			Wasteland_PlayerProfile newProfile = new Wasteland_PlayerProfile();
			newProfile.SetPlayerUID(playerUID);
			newProfile.SetPlayerId(playerId);
			playerProfileArray.Insert(newProfile);
			Print("New profile created, assigned temporary playerId (" + playerId + ")");
		}
	}
	
	override void OnPlayerDisconnected(int playerId)
	{
		SCR_ChimeraCharacter player = GetPlayerEntity(playerId);
		if (WR_Statics.IsEntityAlive(player))
		{			
			int arrayIndex;
			Wasteland_PlayerProfile profileToSave = GetPlayerProfileWithIndex(playerId, arrayIndex);
			if (profileToSave.GetPlayerId().ToString() != profileToSave.GetPlayerUID())
			{
				// unpack data into profileToSave, data is packed data from GetPlayerInventory()
				profileToSave.UnpackData(GetPlayerInventory(player).PackData());
				playerProfileArray[arrayIndex] = profileToSave;
				return;
			}
			// if playerUID and playerId are the same due to invalid playerUID on connection, remove entry on disconnect
			playerProfileArray.Remove(arrayIndex);
			return;
		}
		
	}
	
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		array<int> data = GetPlayerProfile(playerId).PackData();
		GetPlayerInventory(controlledEntity).UnpackData(data); // will crash if player loadout doesn't have Wasteland_PlayerInventoryComponent
	}
	
	// money
	int bonusKillMoney = 100;
	
	void SpawnMoneyBag(ResourceName resName, IEntity player, int money)
	{
		Resource res = Resource.Load(resName);
		
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		vector mat[4];
		player.GetWorldTransform(mat);
		spawnParams.Transform = mat;
		
		IEntity item = GetGame().SpawnEntityPrefab(res, null, spawnParams);
		Wasteland_MoneyBag.Cast(item).setMoney(money);
		
		Physics entPhysics = item.GetPhysics();
		entPhysics.SetVelocity("0 -0.05 0");
	}
	
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		int playerMoney = GetPlayerInventory(player).getMoney();
		SpawnMoneyBag(resName_moneyBag, player, playerMoney);
		
		// if not suicide, check if team kill and either add or subtract bonus money
		if (player != killer)
		{
			if (SCR_ChimeraCharacter.Cast(player).GetFaction() == SCR_ChimeraCharacter.Cast(killer).GetFaction())
			{
				GetPlayerInventory(killer).addMoney(-bonusKillMoney);
				return;
			}
			GetPlayerInventory(killer).addMoney(bonusKillMoney);
		}
	}
}