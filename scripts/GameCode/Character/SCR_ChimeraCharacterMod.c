// --------------------------------------------------------------------
modded class SCR_ChimeraCharacter : ChimeraCharacter
{
	ref EntitySlotInfo m_SlotInfo;
		
	override override void EOnInit(IEntity owner)
	{
		m_SlotInfo = new EntitySlotInfo();
		
		
		
		
		
		
		
		
		
		
		
		#ifdef WR_BROKEN
		Print("Character init");
		if (!GetChildren().IsInherited(SCR_SiteSlotEntity))
		{
			Print("no child slot entity");
			
			// this can be replaced with ESE.SpawnEntity() later;
			EntitySpawnParams spawnParams = new EntitySpawnParams();
        	spawnParams.TransformMode = ETransformMode.WORLD;
        	this.GetWorldTransform(spawnParams.Transform);
			SCR_SiteSlotEntity buildingSlotEntity = SCR_SiteSlotEntity.Cast( GetGame().SpawnEntity(SCR_SiteSlotEntity, GetGame().GetWorld(), spawnParams) );
			
			Print("Slot entity " + buildingSlotEntity + " created");
			
			AddChild(buildingSlotEntity, -1);
			
			vector mat[4];
			buildingSlotEntity.GetLocalTransform(mat);
			mat[3] = Vector(0, 0.25, 3);
			buildingSlotEntity.SetLocalTransform(mat);
		}
		#endif
		
		
		
				
		
		
		#ifdef WR_BROKEN
		Print("Character init");
		if (!this.GetChildren().IsInherited(SCR_SiteSlotEntity))
		{
			Print("Adding building slot to player...");
			ref SCR_SiteSlotEntity buildingSlotEntity = new SCR_SiteSlotEntity(null, this);
			this.AddChild(buildingSlotEntity, -1);
			
			// set local transform of slot to in front of player
			vector mat[4];
			buildingSlotEntity.GetLocalTransform(mat);
			mat[3] = Vector(0, 0.25, 3);
			buildingSlotEntity.SetLocalTransform(mat);
			Print("Building slot successfully added!");
		}
		#endif
	}
};