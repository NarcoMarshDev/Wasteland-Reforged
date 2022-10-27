class WR_TestingComponent2Class: ScriptComponentClass
{
};

class WR_TestingComponent2: ScriptComponent
{
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		owner.Update();
	}
}