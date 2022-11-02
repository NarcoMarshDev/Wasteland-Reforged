class WR_BuildingObjectEntityClass : GenericEntityClass
{
};

class WR_BuildingObjectEntity : GenericEntity
{
	override void EOnInit(IEntity owner)
	{
		Activate();
	}
}