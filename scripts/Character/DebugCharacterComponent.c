class DebugCharacterComponentClass : CharacterControllerComponentClass
{
}

class DebugCharacterComponent : CharacterControllerComponent
{
	override void OnControlledByPlayer(IEntity owner, bool controlled)
	{
		vector pos = GetOwner().GetOrigin();
		Print("character pos: " + pos);
	}
}