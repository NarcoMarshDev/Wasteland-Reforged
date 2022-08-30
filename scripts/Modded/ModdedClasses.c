#ifdef ADDITIONS
modded class array<Class T> : Managed
{
	bool CastToNewArray(inout array newArray, typename T)
	{
		int size = Count();
		
		if (!size || newArray.Count())
			return false;
		
		// for each element of this array, cast it's value to the new type and insert
		for (int i = 0; i < size; i++)
		{
			newArray.Insert(T.Cast( Get(i) ));
		}
		
		return true;
	}
}
#endif

// WASTELAND SPECIFIC MODDED CLASSES

