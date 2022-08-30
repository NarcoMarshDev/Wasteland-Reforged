class Wasteland_MoneyBagClass: GenericEntityClass
{
}

class Wasteland_MoneyBag: GenericEntity
{
	int m_money = null;
	
	protected override void EOnInit(IEntity owner)
	{
		m_money = 100;
	}
	
	int getMoney() return m_money;
	
	void setMoney(int money){
		m_money = money;}
}