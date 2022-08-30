[EntityEditorProps(category: "GameScripted/Triggers", description: "Wasteland town")]
class Wasteland_TownClass: GenericEntityClass
{
};

class Wasteland_Town: GenericEntity
{
	// user Script
	
	[Attribute("Town", UIWidgets.EditBox, "Name of Wasteland_Town."), RplProp(onRplName: "testFunc2")]
	private string m_sTownName;
	
	string mapName;
	int forceInitOutput = forceInit();
	
	protected override void EOnInit(IEntity owner)
	{
		mapName = GetName();
		int charsReplaced = mapName.Replace("Town", "");
	}

	
	
	private int forceInit(){
		EOnInit(this);
		return 0;}
	
	// gets
	string GetTownName() return mapName;
	
	void testFunc2() void;
}