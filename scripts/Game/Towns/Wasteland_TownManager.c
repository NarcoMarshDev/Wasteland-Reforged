[EntityEditorProps(category: "GameScripted/Triggers", description: "Wasteland town manager")]
class Wasteland_TownManagerClass: GenericEntityClass
{
};

class Wasteland_TownManager: GenericEntity
{
	ref array<IEntity> towns = new array<IEntity>();
	bool debugFlag;
	
	// needed because game isn't running EOnInit for some reason
	private int forceInit(){
		EOnInit(this);
		return 0;}
	
	int forceInitOutput = forceInit();
	
	protected override void EOnInit(IEntity owner)
	{
		Print("debugFlag: " + debugFlag);
		
		GetAllChildren(this, towns);
		
		for (int townID = 0; townID < towns.Count(); townID++)
		{
			ref array<IEntity> townChildren = new array<IEntity>();
			GetAllChildren(this, townChildren);
			for (int i = 0; i < townChildren.Count(); i++)
			{
				if (townChildren[i].ClassName() == "Wasteland_TeleportTrigger") void;
			}
		}
	}

	//gets
	bool GetDebugFlag() return debugFlag;
	













	void GetAllChildren(IEntity parent, notnull inout array<IEntity> allChildren)
	{
		if (!parent)
			return;
		
		IEntity child = parent.GetChildren();
		
		while (child)
		{
			allChildren.Insert(child);
			child = child.GetSibling();
		}
	}
}