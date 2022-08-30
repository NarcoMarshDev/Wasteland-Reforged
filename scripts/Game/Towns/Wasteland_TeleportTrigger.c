[EntityEditorProps(category: "GameScripted/Triggers", description: "Wasteland town spawn teleporter for random points")]
class Wasteland_TeleportTriggerClass: SCR_BaseTriggerEntityClass
{
};

class Wasteland_TeleportTrigger: SCR_BaseTriggerEntity 
{
	// user script
	SCR_ChimeraCharacter cc;
	ref array<IEntity> teleportPoints = new array<IEntity>();
	
	protected override void EOnInit(IEntity owner)
	{
		GetAllChildren(this, teleportPoints);
	}
		
	private void GetAllChildren(IEntity parent, notnull inout array<IEntity> allChildren)
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
	
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		SCR_ChimeraCharacter temp = SCR_ChimeraCharacter.Cast(ent);
		
		if (temp != null)
		{
			cc = temp;
			return true;
		}
		return false;		
	}
	
	// --------------------------------------------------------------------------- //
	
	vector tpTransform[4];
	
	override void OnActivate(IEntity ent)
	{	
		IEntity temp = teleportPoints.GetRandomElement();
		temp.GetTransform(tpTransform);
		cc.Teleport(tpTransform);
	}

};

