class WR_Statics
{
	#define PRINT_DEBUG
	
	// ==================== STATIC VALUES ====================
	
	// player profile
	static int max_repairKits = 3;
	static int max_jerryCans = 2;
	static int max_medkits = 3;
	
	
	// ====================== FUNCTIONS ======================
	
	// ===================================================================================================
	// ============================================ HIERARCHY ============================================
	// ===================================================================================================
	[Obsolete("Use ESE.GetAllChildren()")]
	static void GetAllChildren(IEntity parent, notnull inout array<IEntity> allChildren)
	{
		if (!parent)
			#ifdef PRINT_DEBUG
			Print("GetAllChildren("+parent+", "+allChildren+") : NULL parent");
			#endif
			return;
		
		IEntity child = parent.GetChildren();
		
		while (child)
		{
			allChildren.Insert(child);
			child = child.GetSibling();
		}
	}
	
	// iterates through all children, outputting an array of all children matching the given class
	[Obsolete("Use ESE.GetAllChildrenByType()")]
	static void GetAllChildrenByClass(IEntity parent, typename childClass, notnull inout array<IEntity> allChildren)
	{
		if (!parent || !childClass)
			#ifdef PRINT_DEBUG
			Print("GetAllChildrenByClass("+parent+", "+childClass+", "+allChildren+") : NULL parent or childClass");
			#endif
			return;
		
		IEntity child = parent.GetChildren();
		
		while (child)
		{
			if (child.IsInherited(childClass))
			{
				allChildren.Insert(child);
			}
			child = child.GetSibling();
		}
	}
	
	// may break if no grandparent exists
	static void GetAllSiblings(IEntity parent, notnull inout array<IEntity> allSiblings)
	{
		if (!parent || !parent.GetParent())
			#ifdef PRINT_DEBUG
			Print("GetAllSiblings("+parent+", "+allSiblings+") : NULL parent or grandparent");
			#endif
			return;
		
		IEntity child = parent.GetParent().GetChildren();
		
		while (child)
		{
			if (child.GetID() != parent.GetID())
			{	
				allSiblings.Insert(child);
				child = child.GetSibling();
			}
		}
	}
	// may break if no grandparent exists
	static void GetAllSiblingsByClass(IEntity parent, typename siblingClass, notnull inout array<IEntity> allSiblings)
	{
		if (!parent || !parent.GetParent())
			#ifdef PRINT_DEBUG
			Print("GetAllSiblingsByClass("+parent+", "+siblingClass+", "+allSiblings+") : NULL parent or siblingClass");
			#endif
			return;
		
		IEntity child = parent.GetParent().GetChildren();
		
		while (child)
		{
			if (child.IsInherited(siblingClass))
			{
				if (child.GetID() != parent.GetID())
				{	
					allSiblings.Insert(child);
					child = child.GetSibling();
				}
			}
		}
	}
	
	// modified GetAllChildren() that sorts non SCR_SpawnPoint entities out of the output array
	[Obsolete("Use SCR_SpawnPoint.GetSpawnPoints()")]
	static void GetAllSpawns(IEntity parent, notnull inout array<SCR_SpawnPoint> allChildren)
	{
		if (!parent)
			#ifdef PRINT_DEBUG
			Print("GetAllChildren("+parent+", "+allChildren+") : NULL parent");
			#endif
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
	[Obsolete("Use ESE_Entities.DeleteEntity()")]
	static void DeleteEntity(IEntity ent)
	{
		RplComponent.DeleteRplEntity(ent, false);
	}
	
	// ===================================================================================================
	// =========================================== DATA TYPES ============================================
	// ===================================================================================================
	#ifdef ADDITIONS
	static bool CastToNewArray(array<Class> oldArray, array<Class> newArray, typename T)
	{
		int size = oldArray.Count();
		
		if (!size || newArray.Count())
			return false;
		
		// for each element of this array, cast it's value to the new type and insert
		for (int i = 0; i < size; i++)
		{
			newArray.Insert(T.Cast( oldArray.Get(i) ));
		}
		
		return true;
	}
	#endif
	
	// ===================================================================================================
	// ============================================== MATH ===============================================
	// ===================================================================================================
	
	// See SCR_Global.FixVector180() - All these do the same thing for different data types
	[Obsolete("Use ESE_Math.FixFloat180() or ESE_Math.FastFixFloat180")]
	static float FixFloat180(float val)
	{
		if 		(val > 180)  {val -= 360;}
		else if (val < -180) {val += 360;}
		return val;
	}
	[Obsolete("Use ESE_Math.FixInt180() or ESE_Math.FastFixInt180")]
	static int FixInt180(int val)
	{
		if 		(val > 180)  {val -= 360;}
		else if (val < -180) {val += 360;}
		return val;
	}
	[Obsolete("Use ESE_Math.FixVector180() or ESE_Math.FastFixVector180")]
	static vector FixVector180(vector vec)
	{
		for (int a = 0; a < 3; a++)
		{
			float v = vec[a];
			while (v > 180)
				v -= 360;
			while (v < -180)
				v += 360;
			vec[a] = v;
		}
		return vec;
	}
	
	// ===================================================================================================
	// =========================================== ITEM SPAWNS ===========================================
	// ===================================================================================================
	
	static void SpawnMoneyBag(IEntity player, int money, ResourceName prefabName)
	{
		Resource prefab = Resource.Load(prefabName);
		
		// generate spawn parameters based on player location
		EntitySpawnParams spawnParams = new EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        player.GetWorldTransform(spawnParams.Transform);
		
		// spawn prefab
		Wasteland_MoneyBag moneyBagEntity = Wasteland_MoneyBag.Cast( GetGame().SpawnEntityPrefab(prefab, null, spawnParams) ); // was player.GetWorld() instead of null
		
		// set money bag stats
		moneyBagEntity.setMoney(money);
		moneyBagEntity.SetScale(0.5);
		Physics entPhysics = moneyBagEntity.GetPhysics();
		entPhysics.SetVelocity("0 -0.1 0");
		
		Print("MONEY BAG ENTITY: " + moneyBagEntity);

        // Destroy the money bag after 300 seconds.
        GetGame().GetCallqueue().CallLater(WR_Statics.DeleteEntity, 300000, false, moneyBagEntity); //#ESE REPLACE
	}
	
	// thanks to Chad (Discord ID 110143440817229824)
	[Obsolete("Use ESE_Entities.SpawnPrefabOnPlayer()")]
	static IEntity SpawnPrefabOnPlayer(IEntity player, ResourceName prefabName)
	{
		Print("=====SpawnPrefabOnPlayer=====");
		Print("player: " + player);
		Print("prefabName: " + prefabName);
		
		if (!prefabName)
        {
            Print("Missing Prefab: " + prefabName, LogLevel.ERROR);
            return null;
        }
		
		Resource prefab = Resource.Load(prefabName);
		Print("prefab: " + prefab);
		
		// generate spawn parameters based on player location
		EntitySpawnParams spawnParams = new EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        player.GetWorldTransform(spawnParams.Transform);
		Print("spawnParams: " + spawnParams);
		
		// spawn prefab and return it
		return GetGame().SpawnEntityPrefab(prefab, player.GetWorld(), spawnParams);
	}
	
	// ===================================================================================================
	// ============================================== OTHER ==============================================
	// ===================================================================================================
	[Obsolete("Use ESE.GetPlayerControllerFromEntity()")]
	static PlayerController GetPlayerControllerFromEntity(IEntity ent)
	{
		return GetGame().GetPlayerManager().GetPlayerController( GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(ent) );
	}
	
	// ===================================================================================================
	// ============================================= ENTITIES ============================================
	// ===================================================================================================
	
	// #ESE_REPLACE
	// fixed SCR_TerrainHelper.OrientToTerrain()
	static bool OrientToTerrain(out vector transform[4], BaseWorld world = null, bool noUnderwater = false)
	{
		//--- Get world
		if (!world)
			world = GetGame().GetWorld();

		if (!world)
			return false;

		//--- Get surface basis
		vector surfaceBasis[4];
		if (!SCR_TerrainHelper.GetTerrainBasis(transform[3], surfaceBasis, world, noUnderwater))
			return false;

		//--- Reset pitch and roll, but preserve yaw
		vector angles = Math3D.MatrixToAngles(transform);
		Math3D.AnglesToMatrix(Vector(angles[0], 0, 0), transform);

		//--- Combine surface and entity transformations
		Math3D.MatrixMultiply3(surfaceBasis, transform, transform);

		return true;
	}
	// #ESE_REPLACE
	// fixed SCR_TerrainHelper.SnapAndOrientToTerrain()
	static bool SnapAndOrientToTerrain(out vector transform[4], BaseWorld world = null, bool noUnderwater = false, float height = 0)
	{
		//--- Get world
		if (!world)
			world = GetGame().GetWorld();

		if (!world)
			return false;

		//--- Get surface basis
		vector surfaceBasis[4];
		if (!SCR_TerrainHelper.GetTerrainBasis(transform[3], surfaceBasis, world, noUnderwater))
			return false;

		//--- Set position to surface
		transform[3] = surfaceBasis[3];

		//--- Reset pitch and roll, but preserve yaw
		vector angles = Math3D.MatrixToAngles(transform);
		Math3D.AnglesToMatrix(Vector(angles[0], 0, 0), transform);

		//--- Combine surface and entity transformations
		Math3D.MatrixMultiply3(surfaceBasis, transform, transform);

		return true;
	}
	
	[Obsolete("Use ESE_Entities.DisableCollisions()")]
	static void DisableEntityCollisions(IEntity ent)
	{
		Physics parentPhys = ent.GetPhysics();
		if (parentPhys)
		{
			parentPhys.Destroy();
		}
		IEntity child = ent.GetChildren();
		int i = 0;
		while (child)
		{
			Physics p = child.GetPhysics();
			if (!p)
			{
				Print("child: " + child + " no p " + i);
				i++;
				child = child.GetSibling();
				continue;
			}
			Print("child: " + child + " yes p " + i);
			i++;
			p.Destroy();
			child = child.GetSibling();
		}
	}
	[Obsolete("Use ESE_Entities.EnableCollisions()")]
	static void EnableEntityCollisions(IEntity ent, int layerMask = 0xffffffff)
	{
		Physics.CreateStatic(ent, layerMask);
		IEntity child = ent.GetChildren();
		while (child)
		{
			Physics.CreateStatic(child, layerMask);
			child = child.GetSibling();
		}
	}
	
	static EntitySlotInfo GetEntityBuildingSlot(IEntity ent)
	{
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(ent.FindComponent(SlotManagerComponent));
		if (!slotManager)
			return null;
		
		array<EntitySlotInfo> slotArray = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slotArray);
		return slotArray[0];
	}
	[Obsolete("Use ESE_Entities.GetMaterial")]
	static void GetMaterial(IEntity entity, out string materials[], out int numMaterials)
	{
		VObject mesh = entity.GetVObject();
		if (mesh)
		{
			numMaterials = mesh.GetMaterials(materials);
		}
	}
	[Obsolete("Use ESE_Entities.SetMaterial()")]
	static void SetMaterial(IEntity entity, ResourceName material, bool recursively = true)
	{
		//--- Remap textures of the current mesh
		VObject mesh = entity.GetVObject();
		if (mesh)
		{
			string remap;
			string materials[256];
			int numMats = mesh.GetMaterials(materials);
			for (int i = 0; i < numMats; i++)
			{
				remap += string.Format("$remap '%1' '%2';", materials[i], material);
			}
			entity.SetObject(mesh, remap);
		}
		
		//--- Iterate through children
		if (recursively)
		{
			IEntity child = entity.GetChildren();
			while (child)
			{
				SetMaterial(child, material);
				child = child.GetSibling();
			}
		}
	}
	
	// this is fucking sick
	[Obsolete("Use ESE_Entities.RestoreMaterial()")]
	static void RestoreMaterial(IEntity entity)
	{
		IEntity child = entity;
		while (child)
		{
			VObject vobj = child.GetVObject();
			if (!vobj)
			{
				child = child.GetChildren();
				continue;
			}
			ResourceName resName;
			string remap;
			SCR_Global.GetModelAndRemapFromResource(vobj.GetResourceName(), resName, remap);
			child.SetObject( Resource.Load(resName).GetResource().ToVObject(), "");
			child = child.GetChildren();
		}
	}
	[Obsolete("Use ESE_Entities.IsEntityAlive()")]
	static bool IsEntityAlive(IEntity entity)
	{
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(entity.FindComponent(DamageManagerComponent));
		if (damageManager)
			return damageManager.GetState() != EDamageState.DESTROYED;
		else
			return true;
	}
	//[Obsolete("Use ESE_Entities.IsEntityOnScreen()")] #ESE ADD
	static bool IsEntityOnScreen(IEntity ent)
	{
		WorkspaceWidget workspace = GetGame().GetWorkspace();
		float width, height;
		workspace.GetScreenSize(width, height);
		
		vector worldPos = ent.GetOrigin();
		if (!worldPos)
			return false;
		
		vector screenPos = workspace.ProjWorldToScreenNative(worldPos, GetGame().GetWorld());
		return screenPos[2] > 0 && screenPos[0] > 0 && screenPos[0] < width && screenPos[1] > 0 && screenPos[1] < height);
	}
	
	// ===================================================================================================
	// ========================================== FILE HANDLING ==========================================
	// ===================================================================================================
	[Obsolete("Use ESE_IO.ReadFileAsArray()")]
	static void ReadFileAsArray(string path, notnull inout array<string> fileArray)
	{
		FileHandle file =  FileIO.OpenFile(path, FileMode.READ);
		
		if (!file)
			Print("can not find file: " + file);
			return;
		
		string line;
		int chars = file.FGets(line);
		
		while (chars != -1)
		{
			fileArray.Insert(line);
			chars = file.FGets(line);
		}
		file.CloseFile();
	}
}