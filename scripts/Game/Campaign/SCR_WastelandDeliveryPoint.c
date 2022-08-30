[EntityEditorProps(category: "GameScripted/Campaign", description: "Campaign delivery point allowing asset acquisition", color: "0 0 255 255")]
class SCR_WastelandDeliveryPointClass: GenericEntityClass
{
};

// Modified version of SCR_CampaignDeliveryPoint to add wasteland store functionality

//------------------------------------------------------------------------------------------------
//! Delivery point type (vehicle depot etc.)
enum EWastelandServicePointType
{
	VEHICLE_DEPOT,
	SUPPLY_DEPOT,
	FUEL_DEPOT,
	ARMORY,
	REPAIR_DEPOT,
	FIELD_HOSPITAL,
	BARRACKS,
	RADIO_ANTENNA,
	GUN_STORE,
	VEHICLE_STORE,
	GENERAL_STORE
};

//------------------------------------------------------------------------------------------------
//! Serves as template for all spots where players can request assets (vehicle depots etc.) in Campaign
class SCR_WastelandDeliveryPoint : GenericEntity
{
	// Attributes
	[Attribute(defvalue: "0", uiwidget: UIWidgets.ComboBox, desc: "Type", enums: ParamEnumArray.FromEnum(ECampaignServicePointType))]
	protected EWastelandServicePointType m_eType;
	
	// Member variables 
	protected SCR_CampaignBase m_Base;
	protected RplComponent m_RplComponent;
	protected ResourceName m_sCurrentSpawnedPrefab;
	protected SCR_SiteSlotEntity m_Slot;
	protected BaseGameTriggerEntity m_VehicleSpawnpoint;
	protected float m_fLastVehicleSpawnTimestamp;
	
	// Synced variables
	[RplProp()]
	protected ref array<int> m_aAssetStock = new array<int>();
	[RplProp(onRplName: "OnBuiltChanged")]
	protected bool m_bIsBuilt = false;
	
	//------------------------------------------------------------------------------------------------
	//! Returns the type of this delivery point
	ECampaignServicePointType GetServiceType()
	{
		return m_eType;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns the base which contains this delivery point
	SCR_CampaignBase GetParentBase()
	{
		return m_Base;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSlot(SCR_SiteSlotEntity slot)
	{
		if (!slot)
			return;
		
		m_Slot = slot;
		
		if (m_Base)
		{
			float dist = vector.DistanceSqXZ(GetOrigin(), slot.GetOrigin());
		
			if (dist > 100)
				Print(string.Format("%1 in %2 is too far from its slot (%3m)", typename.EnumToString(ECampaignServicePointType, m_eType), m_Base.GetBaseName(), Math.Sqrt(dist)), LogLevel.WARNING);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void SetIsBuilt(bool isBuilt)
	{
		m_bIsBuilt = isBuilt;
		Replication.BumpMe();
		OnBuiltChanged()
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsBuilt()
	{
		return m_bIsBuilt;
	}
	
	//------------------------------------------------------------------------------------------------
	void OnBuiltChanged()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateCompositionPrefab(ResourceName builtFirst = ResourceName.Empty)
	{
		SetIsBuilt(true);
		
		if (builtFirst != ResourceName.Empty)
		{
			m_sCurrentSpawnedPrefab = builtFirst;
			return;
		}
		
		if (!m_Slot || !m_Base)
			return;
		
		SCR_CampaignFaction owningFaction = m_Base.GetOwningFaction();
		
		if (!owningFaction)
			return;
		
		IEntity occupant = m_Slot.GetOccupant();
		ECampaignCompositionType compType = CalculateSupplyDepotCompositionType(m_Base.GetSuppliesMax(), m_Base.GetSupplies());
		ResourceName compName = owningFaction.GetBuildingPrefab(compType);
		
		if (!compName.IsEmpty() && compName != m_sCurrentSpawnedPrefab)
		{
			Resource newComposition = Resource.Load(compName);
			
			if (newComposition)
			{
				if (occupant)
				{
					RplComponent.DeleteRplEntity(occupant, false);
				}
				
				m_Slot.SpawnEntityInSlot(newComposition);
				m_sCurrentSpawnedPrefab = compName;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add assets to the stock
	//! \param firstTime True means that the stock is being filled at the start of the scenario
	void FillAssetPool(bool firstTime = false, int overrideAmount = -1)
	{
		if (!m_Base)
			return;
		
		SCR_CampaignFaction baseOwningFaction = m_Base.GetOwningFaction();
		
		if (!baseOwningFaction)
			return;
		
		// Get data for available vehicles from the list saved in FactionManager
		array<ref SCR_CampaignVehicleAssetInfo> assetList = new array<ref SCR_CampaignVehicleAssetInfo>();
		SCR_CampaignFactionManager.GetInstance().GetVehicleAssetList(assetList);
		int assetListCnt = assetList.Count();
		
		if (firstTime)
			m_aAssetStock.Clear();
		
		// Prepare array size of stock
		if (m_aAssetStock.Count() == 0)
		{
			for (int i = 0; i < assetListCnt; i++)
			{
				m_aAssetStock.Insert(0)
			}
		}
		
		Replication.BumpMe();
		
		// Add amount of assets specified in the asset list, don't go over limit
		string sBaseFactionKey = baseOwningFaction.GetFactionKey();
		
		foreach (int i, SCR_CampaignVehicleAssetInfo assetInfo: assetList)
		{
			if (assetInfo.GetFactionKey() != sBaseFactionKey)
				continue;
			
			if (overrideAmount != -1)
			{
				m_aAssetStock[i] = overrideAmount;
				continue;
			}
			
			int curAmount = m_aAssetStock[i];
			int curAmountOpposite = 0;
			
			// Enemy assets can share the space inside the delivery point
			foreach (int i2, SCR_CampaignVehicleAssetInfo assetInfo2: assetList)
			{
				if (assetInfo.GetPrefabOpposite() == assetInfo2.GetPrefab())
				{
					curAmountOpposite = m_aAssetStock[i2];
					break;
				};
			}
			
			int toAdd;
			
			if (firstTime)
				toAdd = assetInfo.GetStartingAmount();
			else
				toAdd = assetInfo.GetReinforcementsAmount();
			
			toAdd = Math.Min(toAdd, assetInfo.GetMaximumAmount() - curAmount - curAmountOpposite);
			m_aAssetStock[i] = curAmount + toAdd;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Checks if the session is run as client
	protected bool IsProxy()
	{
		return (m_RplComponent && m_RplComponent.IsProxy());
	}
	
	//------------------------------------------------------------------------------------------------
	//! How many assets of given ID are in stock
	//! \param assetID Unique asset ID (its index in asset list)
	int GetStockSize(int assetID)
	{
		if (m_aAssetStock.Count() <= assetID || assetID < 0)
		{
			return -1;	// Out of bounds index
		} else {
			return m_aAssetStock[assetID];
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Remove one unit of an asset from stock
	//! \param assetID Unique asset ID (its index in asset list)
	void DepleteAsset(int assetID)
	{
		m_aAssetStock[assetID] = (m_aAssetStock[assetID]) - 1;
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	void HandleVehicleSpawn()
	{
		if (!m_Base)
			return;
		
		SCR_CampaignFaction faction = m_Base.GetOwningFaction();
		
		if (!faction)
			return;
		
		if (!m_Slot)
			return;
		
		IEntity occupant = m_Slot.GetOccupant();
		
		if (!occupant)
			return;
		
		SCR_CampaignServiceCompositionComponent comp = SCR_CampaignServiceCompositionComponent.Cast(occupant.FindComponent(SCR_CampaignServiceCompositionComponent));
		
		if (!comp)
			return;
		
		if (!comp.IsServiceOperable())
			return;
		
		// Register spawn point if not done yet
		if (!m_VehicleSpawnpoint)
		{
			IEntity compoParent = m_Slot.GetOccupant();
			
			if (!compoParent)
				return;
			
			IEntity child = compoParent.GetChildren();
			
			while (!m_VehicleSpawnpoint && child)
			{
				m_VehicleSpawnpoint = BaseGameTriggerEntity.Cast(child);
				
				child = child.GetSibling();
			}
		}
		
		if (!m_VehicleSpawnpoint)
			return;
		
		if (Replication.Time() < m_fLastVehicleSpawnTimestamp + SCR_GameModeCampaignMP.GARAGE_VEHICLE_SPAWN_INTERVAL)
			return;
		
		m_VehicleSpawnpoint.QueryEntitiesInside();
		array<IEntity> inside = {};
		m_VehicleSpawnpoint.GetEntitiesInside(inside);
		
		if (inside.Count() != 0)
			return;
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		m_VehicleSpawnpoint.GetWorldTransform(params.Transform);
		Resource res = Resource.Load(faction.GetDefaultTransportPrefab());
		
		if (!res)
			return;
		
		Vehicle veh = Vehicle.Cast(GetGame().SpawnEntityPrefab(res, null, params));
	}
	
	//------------------------------------------------------------------------------------------------
	static ECampaignCompositionType CalculateSupplyDepotCompositionType(int maxSupplies, int curSupplies)
	{
		float fraction = Math.InverseLerp(0, maxSupplies, curSupplies);
		
		if (fraction == 0)
			return ECampaignCompositionType.SUPPLIES_EMPTY;
		else
			if (fraction == 1)
				return ECampaignCompositionType.SUPPLIES_FULL;
			else
				if (fraction < 0.5)
					return ECampaignCompositionType.SUPPLIES_LOW;
		
		return ECampaignCompositionType.SUPPLIES_HIGH;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Check if the given player can request assets at this depot
	//! \param player Player trying to request an asset
	//! \param assetID Unique asset ID (its index in asset list)
	int CanRequest(int playerID, int assetID)
	{
		if (!GetGame().AreGameFlagsSet( EGameFlags.SpawnVehicles ))
			return 0;
		
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
		if (!campaign)
			return 0;
		
		// Check the current stock size for given vehicle
		int stockSize = GetStockSize(assetID);
		
		// Index out of bounds
		if (stockSize == -1)
			return 0;
		
		SCR_PlayerController playerController;
		
		if (IsProxy())
			playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		else
			playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerID));
		
		if (!playerController)
			return 0;
		
		IEntity player;
		
		if (IsProxy())
			player = playerController.GetMainEntity();
		else
			player = GetGame().GetPlayerManager().GetPlayerController(playerID);

		if (!player || !m_Base)
			return 0;

		SCR_CampaignNetworkComponent campaignNetworkComponent = SCR_CampaignNetworkComponent.Cast(playerController.FindComponent(SCR_CampaignNetworkComponent));

		if (!campaignNetworkComponent || !GetWorld() || !campaign)
			return 0;

		// Check if the base is within HQ radio signal range
		SCR_CampaignFaction playerFaction;
		
		if (IsProxy())
			playerFaction = SCR_CampaignFaction.Cast(SCR_RespawnSystemComponent.GetLocalPlayerFaction());
		else
			playerFaction = SCR_CampaignFaction.Cast(campaign.GetPlayerFaction(playerID));
		
		if (!playerFaction || playerFaction != m_Base.GetOwningFaction())
			return 0;

		if (m_Base != playerFaction.GetMainBase() && !(m_Base.IsBaseInFactionRadioSignal(playerFaction)))
			return SCR_CampaignAssetRequestDeniedReason.NO_SIGNAL;

		SCR_CampaignFactionManager factionManager = SCR_CampaignFactionManager.GetInstance();
		
		// Check if the asset is still in stock
		if (stockSize == 0)
		{
			// Show only faction-appropriate depleted vehicles
			if (factionManager.GetVehicleAssetFactionKey(assetID) == playerFaction.GetFactionKey())
				return SCR_CampaignAssetRequestDeniedReason.OUT_OF_STOCK;
			else
				return 0;
		}

		// Check if the player hasn't requested something recently
		ECharacterRank rank;
		
		if (IsProxy())
			rank = SCR_CharacterRankComponent.GetCharacterRank(player);
		else
			rank = factionManager.GetRankByXP(campaignNetworkComponent.GetPlayerXP());

		// Check if the player has high enough rank
		if ((factionManager.GetVehicleAssetMinimumRank(assetID) > rank) && (!campaign.CanRequestWithoutRank()))
			return SCR_CampaignAssetRequestDeniedReason.RANK_LOW;

		// If player's rank is high enough, check the time remaining
		float fTimeout = campaignNetworkComponent.GetLastRequestTimestamp() + (float)factionManager.GetRankRequestCooldown(rank);
		if (fTimeout > Replication.Time())
			return SCR_CampaignAssetRequestDeniedReason.COOLDOWN;
		
		return 1;
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{	
		m_RplComponent = RplComponent.Cast(FindComponent(RplComponent));
		OnBuiltChanged();
		ClearFlags(EntityFlags.ACTIVE, true);
	}
	
	//------------------------------------------------------------------------------------------------
	// Constructor
	void SCR_CampaignDeliveryPoint(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
		SetFlags(EntityFlags.ACTIVE, true);
		
		// Register parent base
		if (parent && parent.Type() == SCR_CampaignBase)
			m_Base = SCR_CampaignBase.Cast(parent);
		
		if (m_eType == ECampaignServicePointType.VEHICLE_DEPOT || m_eType == ECampaignServicePointType.SUPPLY_DEPOT)
			SetIsBuilt(true);
	}
	
	//------------------------------------------------------------------------------------------------
	void ~SCR_WastelandDeliveryPoint()
	{
		m_aAssetStock = null;
	}
};