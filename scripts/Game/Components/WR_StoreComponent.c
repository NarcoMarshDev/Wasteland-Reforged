enum WR_StoreType
{
	GunStore = 1,
	VehicleStore = 2,
	GeneralStore = 3
}

class WR_StoreComponentClass: ScriptComponentClass
{
};

class WR_StoreComponent: ScriptComponent
{
	[Attribute(defvalue: SCR_Enum.GetDefault(WR_StoreType.GunStore), uiwidget: UIWidgets.ComboBox, desc: "1: Gun Store, 2: Vehicle Store, 3: General Store", enums: ParamEnumArray.FromEnum(WR_StoreType))]
	protected WR_StoreType m_StoreType;
	
	ref array<IEntity> m_SpawnSlots = new array<IEntity>();
		
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	override void EOnInit(IEntity owner)
	{
		// #WR_TODO - Make the map icon work properly here
		SCR_MapDescriptorComponent desc = SCR_MapDescriptorComponent.Cast(owner.FindComponent(SCR_MapDescriptorComponent));
		//Print(desc);
		if (desc)
		{
			MapItem item = desc.Item();
			//Print(item);
			if (!item)
			{
				return;
			}
			item.SetVisible(true);
			string image = item.GetImageDef();
			Print(image);
			//item.SetImageDef("Slot_Supplies");
			
			MapDescriptorProps props = item.GetProps();
			props.SetIconSize(32, 0.25, 0.25);
			props.SetFrontColor(Color.Red);
			props.SetTextVisible(false);
			props.Activate(true);
		}
	}
		
	WR_StoreType GetStoreType()
	{
		return m_StoreType;
	}
	
	IEntity GetStoreReference()
	{
		return this.GetOwner();
	}
	
	void RegisterSpawnSlot(WR_StoreSpawnSlot slot)
	{
		m_SpawnSlots.Insert(slot);
	}
	
	IEntity GetRandomSpawnSlot()
	{
		return m_SpawnSlots.GetRandomElement();
	}
	// Returns first valid unoccupied spawn slot, if all are occupied it will delete the first slot to make it valid.
	// Because of this behaviour make sure stores have plenty of slots, especially general or building stores so you can buy lots of walls etc. all at once.
	WR_StoreSpawnSlot GetValidSpawnSlot()
	{
		foreach (IEntity ent: m_SpawnSlots)
		{
			WR_StoreSpawnSlot slot = WR_StoreSpawnSlot.Cast(ent);
			IEntity occupant = slot.GetOccupant();
			// if no occupant, it's valid
			if (!occupant)
			{
				return slot;
			}
			// if distance between last occupant and spawn slot it more than 20 meters, set occupant to null and it's valid
			if (vector.Distance( occupant.GetOrigin(), slot.GetOrigin() ) >= 20)
			{
				slot.SetOccupant(null);
				return slot;
			}
		}
		// last resort if none of the slots are valid, get the first one and delete it's last occupant, set it's occupant to null and make it valid
		WR_StoreSpawnSlot slot = WR_StoreSpawnSlot.Cast(m_SpawnSlots[0]);
		ESE_Entities.DeleteEntity(slot.GetOccupant()); // the poor guy with this vehicle will get it deleted :c
		slot.SetOccupant(null);
		return slot;
	}
}