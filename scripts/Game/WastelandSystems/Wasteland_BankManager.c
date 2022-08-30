class Wasteland_BankManagerClass: GenericEntityClass
{
}

class Wasteland_BankManager: GenericEntity
{
	string m_fileName = "enfusion://ResourceManager/~WastelandReforged:accounts.conf";
	ParseHandle m_fileParser;
	
	map<string, int> m_playerAccounts = null;
	
	array<string> m_fileData;
	
	protected override void EOnInit(IEntity owner)
	{
		if (!FileIO.FileExist(m_fileName))
		{
			Print("no file: " + m_fileName);
			return;
		}
		
		m_fileParser = FileIO.BeginParse(m_fileName);
		int uhh = m_fileParser.ParseLine(0, m_fileData);
		Print("uhh: " + uhh);
		Print(m_fileData);
	}
	
	void testFunc()
	{
		void;
	}
}