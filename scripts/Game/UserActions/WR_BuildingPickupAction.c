class WR_BuildingPickupAction: ScriptedUserAction
{
	ActionsManagerComponent m_ActionsManagerComponent = null;
	IEntity m_Entity = null;
	
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_ActionsManagerComponent = ActionsManagerComponent.Cast(pManagerComponent);
		m_Entity = pOwnerEntity;
	}
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		#ifdef WR_BROKEN
		if (!m_Entity)
			return;
		
		SCR_SiteSlotEntity buildingSlotEntity;
		if (pUserEntity.GetChildren())
		{
			IEntity child = pUserEntity.GetChildren();
			while (child)
			{
				if (child.IsInherited(SCR_SiteSlotEntity))
				{
					buildingSlotEntity = SCR_SiteSlotEntity.Cast(child);
					break;
				}
				child = child.GetSibling();
			}
		}
		
		if (!buildingSlotEntity)
		{
			Print("no valid slot entity");
			return;
		}
		
		if (buildingSlotEntity.IsOccupied())
		{
			Print("building slot occupied");
			return;
		}
		
		buildingSlotEntity.SetOccupant(pOwnerEntity);
		Print("slot occupant set to " + pOwnerEntity);
		#endif
		// ---------------- OLD CODE OBSOLETE ---------------- //
		
		if (!m_Entity)
			return;
		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pUserEntity.FindComponent(SlotManagerComponent));
		array<EntitySlotInfo> slots = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slots);
		
		slots[0].AttachEntity(pOwnerEntity);
	}
	
	override event bool CanBeShownScript(IEntity user)
	{
		if (!m_Entity)
			return false;
		
		return true;
	}
}