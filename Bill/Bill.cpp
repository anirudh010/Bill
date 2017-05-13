// Bill.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

//types of Users
enum UserType
{
	EmployeeType,
	AffiliateType,
	SpecialCustomerType,
	CustomerType
};

//Factory of Users
class User
{
private:
protected:
	float _discount;
public:
	virtual float GetDiscount() = 0;

};
class Employee :public User
{
public:
	Employee() { _discount = 30; };
	float GetDiscount() { return _discount; };
};
class Affiliate :public User
{
public:
	Affiliate() { _discount = 10; };
	float GetDiscount() { return _discount; };
};
class SpecialCustomer :public User
{
public:
	SpecialCustomer() { _discount = 5; };
	float GetDiscount() { return _discount; };
};
class Customer :public User
{
public:
	Customer() { _discount = 0; };
	float GetDiscount() { return _discount; };
};

class UserFactory
{
private:
public:
	static User* CreateUser(UserType type)
	{
		if (type == EmployeeType)
		{
			return new Employee();
		}
		else if (type == AffiliateType)
		{
			return new Affiliate();
		}
		else if (type == SpecialCustomerType)
		{
			return new SpecialCustomer();
		}
		else if (type == CustomerType)
		{
			return new Customer();
		}
		else
		{
			return nullptr;
		}
	}
};

enum ItemType
{
	GroceryType,
	NonGroceryType
};
//Factory of Items
class Items
{
private:
protected:
	float _price;
public:
	virtual float GetPrice(float discount = 0) = 0;

};
class Grocery :public Items
{
public:
	Grocery(float price) { _price = price; };
	float GetPrice(float discount = 0)
	{
		return _price; //do not apply discount on Grocery item
	}
};
class NonGrocery :public Items
{
public:
	NonGrocery(float price) { _price = price; };
	float GetPrice(float discount = 0)
	{
		return (_price - discount); //apply discount on NonGrocery item
	}

};

class ItemFactory
{
private:
public:
	static Items* CreateItem(ItemType type, float price)
	{
		if (type == GroceryType)
		{
			return new Grocery(price);
		}
		else if (type == NonGroceryType)
		{
			return new NonGrocery(price);
		}
		else
		{
			return nullptr;
		}
	}
};

class Bill
{
private:
	User* _user;
	vector<Items*> _items;
	float CalculateTotalPrice()
	{
		float totalCost = 0;
		vector<Items*>::iterator itr;
		for (itr = _items.begin(); itr != _items.end(); itr++)
		{
			float discount = float((*itr)->GetPrice()*_user->GetDiscount()) / 100; //this class should not care about the type of user 
			totalCost += (*itr)->GetPrice(discount) ; //this class should not care about the type of item 
		}
		return totalCost;
	}
public:
	Bill(User* user, vector<Items*> items)
	{
		_user = user;
		_items = items;
	}
	float GetNetAmountPayable()
	{
		float totalAmt = CalculateTotalPrice();
		int discount = int(CalculateTotalPrice() / 100);
		discount = discount * 5;
		float netAmt = totalAmt - discount;
		return netAmt;
	}
};

int main()
{
	ifstream file;//file Cases.txt contains data in following format 
////////////////////////////////////////////////////////////////////
//// Employee
///  Grocery 10
//// Grocery 20
//// NonGrocery 30

/* 1. First Line indicates type of user, possible values are Employee, Affiliate, SpecialCustomer, Customer  */
/* 2. Second Line contains item type and price(separated by space), possible values of item type are Grocery, NonGrocery   */

///////////////////////////////////////////////////////////////////////
	
	file.open("Cases.txt");
	
	UserFactory* userFactory = new UserFactory(); //Factory to create users
	ItemFactory* itemFactory = new ItemFactory();//Factory to create items
	User* user = nullptr; // Read first line from file, decide type of user  and create using factory 
	vector<Items*> items;//Read rest of the file and choose item type and price 	
	
	UserType userType = EmployeeType;//default

	string line;
	//read the fisrt line = Get User type
	if (file.is_open())
	{
		getline(file, line);
		//Get the correct enum type		
		if (!line.compare("Employee"))
		{
			userType = EmployeeType;
			user = userFactory->CreateUser(EmployeeType);
		}
		else if (!line.compare("Affiliate"))
		{
			userType = AffiliateType;
			user = userFactory->CreateUser(AffiliateType);
		}
		else if (!line.compare("SpecialCustomer"))
		{
			userType = SpecialCustomerType;
			user = userFactory->CreateUser(SpecialCustomerType);
		}
		else if (!line.compare("Customer"))
		{
			userType = CustomerType;
			user = userFactory->CreateUser(CustomerType);
		}
		else
		{
			//cout << "Incorrect User Type" << endl;
		}

	}
	else
	{
		cout << "File Not Found" << endl;
	}
	
	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, line);
			istringstream iss(line);
			string itemType;
			string itemPrice;
			iss >> itemType;
			iss >> itemPrice;
			ItemType itemTypeEnum = GroceryType; //by default
			if (!itemType.compare("Grocery"))
			{
				itemTypeEnum = GroceryType;
				float price = atof(itemPrice.c_str());
				Items* item = itemFactory->CreateItem(itemTypeEnum, price);
				items.push_back(item);
			}
			else if (!itemType.compare("NonGrocery"))
			{
				itemTypeEnum = NonGroceryType;
				float price = atof(itemPrice.c_str());
				Items* item = itemFactory->CreateItem(itemTypeEnum, price);
				items.push_back(item);
			}
			else
			{
				//cout << "Incorrect Item Type" << endl;
			}

		}
	}	
	
	Bill bill(user, items);
	float netAmount = bill.GetNetAmountPayable();
	cout << "Net Payable Amount " << netAmount << endl;
	//clean up 
	vector<Items*>::iterator itr;
	for (itr = items.begin(); itr != items.end(); itr++)
	{
		Items* obj = *itr;
		delete obj;
	}

	file.close();
	delete user;
	delete userFactory;
	getchar();
	return 0;
}

