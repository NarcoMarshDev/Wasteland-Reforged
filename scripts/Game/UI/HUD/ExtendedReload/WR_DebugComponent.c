class WR_DebugComponentClass : ScriptComponentClass
{
}

class WR_DebugComponent : ScriptComponent
{
	protected override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		Print("YO HE FUCKING SPAWNED BRO!!!");
	}
	
	protected override void EOnFrame(IEntity owner, float timeSlice)
	{
		Print("pos: " + owner.GetOrigin());
	}
}