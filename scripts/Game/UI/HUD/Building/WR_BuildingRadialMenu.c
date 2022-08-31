//------------------------------------------------------------------------------------------------

// A big shout out to ex0_ and their "Extended Reload" mod, a lot of the core of this is based on
// that, because radial menus kinda hurt to make and they did a great job!

//------------------------------------------------------------------------------------------------

enum BuildMenuEntryType
{
	// Drop object level to horizon
	PLACE,
	// Drop object and snap to terrain
	SNAP,
	// Rotate object 45 deg left
	LEFT,
	// Rotate object 45 deg right
	RIGHT
}

class WR_BuildingRadialMenuComponentClass : SCR_RadialMenuComponentClass
{
}

//------------------------------------------------------------------------------------------------
class WR_BuildingRadialMenuComponent : SCR_RadialMenuComponent
{
	SCR_CharacterControllerComponent characterController;
	BaseWeaponComponent lastWeapon;
	
	void SetCharacterController(SCR_CharacterControllerComponent controller)
	{
		characterController = controller;
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		if (!m_pRadialMenu)
			return;

		m_pRadialMenu.Init(owner);
		m_pRadialMenu.m_OnActionPerformed.Insert(OnSelection);
	}

	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		if (!m_pRadialMenu)
			return;
		
		bool canOpen = CanOpen(owner);
		
		if (canOpen)
		{			
			characterController.SetDynamicSpeed(0.55);
			
			BaseWeaponComponent currentWeapon = characterController.GetWeaponManagerComponent().GetCurrentWeapon();
			if (currentWeapon)
			{
				lastWeapon = currentWeapon;
			}
			characterController.SelectWeapon(null);
		}
		else if (lastWeapon)
		{
			// for some reason this just doesn't work, lastWeapon exists and is correct, 
			// it returns true saying it's equipped the weapon, but it just doesnt
			Print("lastWeapon: " + lastWeapon);
			
			bool weaponSelected = characterController.SelectWeapon(BaseWeaponComponent.Cast(lastWeapon));
			lastWeapon = null;
		}

		// Disable opening during certain actions
		SCR_RadialMenuInteractions interactions = m_pRadialMenu.GetRadialMenuInteraction();
		if (interactions)
			interactions.SetCanOpenMenu(canOpen);	

		// Always clear entries, this is dirty,
		// but for this version good enough, ticks
		// on local player only
		m_pRadialMenu.ClearEntries(0);

		// If menu shan't be open or we no longer have the input, close it
		// this is a very whacky hacky way around, but hey..
		if (m_pRadialMenu.IsOpen())
		{
			if (!canOpen || GetGame().GetInputManager().GetActionTriggered("CharacterReloadMenuClose"))
			{
				// This is an ugly workaround :\
				if (!GetGame().GetInputManager().IsUsingMouseAndKeyboard())
					m_pRadialMenu.TryPerformCurrentSelection(owner);

				m_pRadialMenu.Close(owner);
				return;
			}
		}

		// No controlled entity, can't manage
		ChimeraCharacter controlledCharacter = GetCharacter(owner);
		if (!controlledCharacter)
		{
			if (m_pRadialMenu.IsOpen())
				m_pRadialMenu.Close(owner);

			return;
		}

		// Fetch the weapon, its muzzle and eventually all magazines from inventory
		// the fact that this is done every frame is less than ideal, but for local
		// player it's gonna be acceptable
		
		
		BaseWeaponComponent weapon = controlledCharacter.GetCharacterController().GetWeaponManagerComponent().GetCurrentWeapon();
		if (weapon)
		{
			WeaponSlotComponent slot = WeaponSlotComponent.Cast(weapon);
			if (slot)
				weapon = WeaponComponent.Cast(slot.GetWeaponEntity().FindComponent(WeaponComponent));

			array<MagazineComponent> magazines = {};
			FetchMagazines(controlledCharacter, weapon, magazines);
			foreach (MagazineComponent magazine : magazines)
			{
				//SCR_ReloadMenuEntry entry = new SCR_ReloadMenuEntry(controlledCharacter, weapon, magazine);
				//SCR_ReloadMenuEntry entry = new SCR_ReloadMenuEntry(controlledCharacter);
				//m_pRadialMenu.AddEntry(entry, 0);
			}
		}
		
		// TODO move all these to EOnInit, they're static so don't need to be remade every frame;
		
		WR_BuildMenuEntry placeEntry 	= new WR_BuildMenuEntry(controlledCharacter, BuildMenuEntryType.PLACE);
		WR_BuildMenuEntry snapEntry 	= new WR_BuildMenuEntry(controlledCharacter, BuildMenuEntryType.SNAP);
		WR_BuildMenuEntry leftEntry		= new WR_BuildMenuEntry(controlledCharacter, BuildMenuEntryType.LEFT);
		WR_BuildMenuEntry rightEntry 	= new WR_BuildMenuEntry(controlledCharacter, BuildMenuEntryType.RIGHT);
		
		m_pRadialMenu.AddEntry(placeEntry, 0);
		m_pRadialMenu.AddEntry(rightEntry, 0);
		m_pRadialMenu.AddEntry(snapEntry,  0);
		m_pRadialMenu.AddEntry(leftEntry,  0);
	}

	//------------------------------------------------------------------------------------------------
	override void OnDelete(IEntity owner)
	{
		if (!m_pRadialMenu)
			return;
		m_pRadialMenu.m_OnActionPerformed.Remove(OnSelection);

		super.OnDelete(owner);
	}

	//------------------------------------------------------------------------------------------------
	protected void OnSelection(BaseSelectionMenuEntry e, int i)
	{
		//if (m_pRadialMenu.IsOpen())
		//	m_pRadialMenu.Close(null);
	}

	//------------------------------------------------------------------------------------------------
	protected ChimeraCharacter GetCharacter(IEntity owner)
	{
		PlayerController playerController = PlayerController.Cast(owner);
		if (!playerController)
			return null;

		IEntity controlledEntity = playerController.GetControlledEntity();
		if (!controlledEntity)
			return null;

		ChimeraCharacter character = ChimeraCharacter.Cast(controlledEntity);
		return character;
	}

	//------------------------------------------------------------------------------------------------
	protected bool CanOpen(IEntity owner)
	{
		if (!owner)
			return false;
		
		ChimeraCharacter character = GetCharacter(owner);
		if (!character)
			return false;

		SlotManagerComponent slotManager = SlotManagerComponent.Cast(character.FindComponent(SlotManagerComponent));
		if (!slotManager)
			return false;
		
		array<EntitySlotInfo> slots = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slots);
				
		foreach (EntitySlotInfo slot : slots)
		{
			if (!slot.GetAttachedEntity())
			{
				return false;
			}
		}
		
		CharacterControllerComponent controller = character.GetCharacterController();
		if (!controller)
			return false;

		if (controller.IsDead())
			return false;

		if (controller.IsReloading())
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void FetchMagazines(ChimeraCharacter character, BaseWeaponComponent weapon, array<MagazineComponent> magazines)
	{
		magazines.Clear();
		if (!character || !weapon || !weapon.GetCurrentMuzzle())
			return;

		InventoryStorageManagerComponent inventory = character.GetCharacterController().GetInventoryStorageManager();
		if (!inventory)
			return;

		BaseMuzzleComponent currentMuzzle = weapon.GetCurrentMuzzle();
		if (!currentMuzzle)
			return;

		array<typename> components = {};
		array<IEntity> foundItems = {};
        components.Insert(MagazineComponent);

		BaseMagazineWell targetWell = currentMuzzle.GetMagazineWell(); // Well, well, well
		if (!targetWell)
			return;

		for (int i = inventory.FindItemsWithComponents(foundItems,components, EStoragePurpose.PURPOSE_ANY) -1; i > -1; i--)
		{
			MagazineComponent magComp = MagazineComponent.Cast(foundItems[i].FindComponent(MagazineComponent));
			if (!magComp)
				continue;
			
			BaseMagazineWell well = magComp.GetMagazineWell();
			if (!well)
				continue;
			
			if (!well.Type().IsInherited(targetWell.Type()))
				continue;

			magazines.Insert(magComp);
		}
	}
}