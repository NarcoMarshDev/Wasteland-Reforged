class WR_MovingObjectTriggerClass: SCR_BaseTriggerEntityClass
{
};

class WR_MovingObjectTrigger: SCR_BaseTriggerEntity 
{
	IEntity m_Parent;
	bool done = false;
	
	protected override void EOnInit(IEntity owner)
	{
		m_Parent = GetParent();
		Print(m_Parent);
	}
	
	protected override void OnFrame(IEntity owner, float timeSlice)
	{
		vector mat[4];
		SetTransform(mat);
		Update();
	}
	
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		return true;
	}
	
	protected override void OnActivate(IEntity ent)
	{
		if (done)
		{
			ent.Update();
			return;
		}
		vector entTransform[4];
		ent.GetWorldTransform(entTransform);
		m_Parent.AddChild(ent, 0);
		//ent.SetWorldTransform(entTransform);
		ent.Update();
		done = true;
	}
	
	protected override void OnDeactivate(IEntity ent)
	{
		vector entTransform[4];
		ent.GetWorldTransform(entTransform);
		m_Parent.RemoveChild(ent);
		//ent.SetWorldTransform(entTransform);
		ent.Update();
	}
}