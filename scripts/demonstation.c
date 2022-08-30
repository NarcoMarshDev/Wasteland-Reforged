



ref array<IEntity> children = new array<IEntity>();

GetAllChildren(this, children);

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

