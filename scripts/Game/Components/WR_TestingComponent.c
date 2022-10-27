class WR_TestingComponentClass: ScriptComponentClass
{
};

class WR_TestingComponent: ScriptComponent
{
	override void EOnContact(IEntity owner, IEntity other, Contact contact)
	{
		Print("Contact!");
	}
	
	override void EOnTouch(IEntity owner, IEntity other, int touchTypesMask)
	{
		Print("Touched!");
	}
}