class JSONHandler : JsonApiStruct
{
	string clientTokenFile = "{F0F690629330F818}Data/clientTokens.json";
	string serverTokenFile = "{6C050E5B0BA97FD6}Data/serverTokens.json";
	
	protected string m_message = "test";
	
	void Hydrate()
	{
		LoadFromFile(clientTokenFile);
	}
}