#include <vector>
#include <iostream>
//#include <algorithm>
#include <windows.h>
#include<string>
using namespace std;

int time[5];	//为演示需要，设置一个全局变量时间

class driver;

class package
{
	//friend boss;
	//friend customer;
	float weight, distance, cost;
	bool payed, valueable, sameday, retrieved, warehouse, delivery, received;	//retrieved是否揽件
	int OrderTime[5], PickupTime[5], WarehouseTime[5], DeliveryTime[5], ReceivedTime[5];
	string PickupAddress, ReceivedAddress;
	string receiver;
	driver *deliverman;
public:
	package(float w, float d, bool v, bool s, string r, string ra, string p = 0) :weight(w), distance(d), valueable(v), sameday(s), receiver(r), ReceivedAddress(ra), PickupAddress(p)
	{
		if (sameday)
			cost = weight * distance * 2;
		else
			cost = weight * distance;
		delivery = warehouse = received = payed = retrieved = false;
		for (int i = 0; i < 5; i++)
		{
			OrderTime[i] = time[i];
		}
	}
	void setpua(string p)
	{
		PickupAddress = p;
	}
	void pickup(driver &dm)
	{
		retrieved = true;
		for (int i = 0; i < 5; i++)
		{
			PickupTime[i] = time[i];
		}
		deliverman = &dm;
	}
	void show()
	{
		cout << "质量：" << weight << "\t距离：" << distance << "\t费用：" << cost << endl;
		cout << "揽件地点：" << PickupAddress << "\n收件地址：" << ReceivedAddress << "\n收件人：" << receiver << endl;
		cout << "包裹属性：";
		if (valueable)
			cout << "贵重\t";
		if(sameday)
			cout << "同日达\t";
		if (payed)
			cout << "已支付\n";
		else
			cout << "未支付\n";
		cout << "包裹状态：\n预定时间：";
		cout << OrderTime[0] << "年" << OrderTime[1] << "月" << OrderTime[2] << "日" << ends << OrderTime[3] << ':' << OrderTime[4] << endl;
		if (!retrieved)
		{
			cout << "正在前往揽件，预估的时间为\n";
			cout << PickupTime[0] << "年" << PickupTime[1] << "月" << PickupTime[2] << "日" << ends << PickupTime[3] << ':' << PickupTime[4] << endl;
		}
		else
		{
			cout << "已揽件，揽件时间：";
			cout << PickupTime[0] << "年" << PickupTime[1] << "月" << PickupTime[2] << "日" << ends << PickupTime[3] << ':' << PickupTime[4] << endl;
			if (!warehouse)
				cout << "正在发往仓库";
			else
			{
				cout << "已入仓，入仓时间：";
				cout << WarehouseTime[0] << "年" << WarehouseTime[1] << "月" << WarehouseTime[2] << "日" << ends << WarehouseTime[3] << ':' << WarehouseTime[4] << endl;
				if (!delivery)
					cout << "正在仓库，准备派发";
				else
				{
					cout << "已派发，派发时间：";
					cout << DeliveryTime[0] << "年" << DeliveryTime[1] << "月" << DeliveryTime[2] << "日" << ends << DeliveryTime[3] << ':' << DeliveryTime[4] << endl;
					if (!received)
						cout << "正在派发途中";
					else
					{
						cout << "已抵达，抵达时间：";
						cout << DeliveryTime[0] << "年" << DeliveryTime[1] << "月" << DeliveryTime[2] << "日" << ends << DeliveryTime[3] << ':' << DeliveryTime[4] << endl;
					}
				}
			}
		}
	}
	bool cancal()
	{
		return !retrieved;
	}
};

class customer
{
	struct business	//如果顾客是公司用户，那么就有这些属性
	{
		bool IsBussiness;
		string PrimaryContactPerson;
		float bill;
		int LastbillTime[5];
		vector<package> month;
	};
	string name, address, password;
	int PhoneNum;
	vector<package> past, now;	//记录顾客的包裹信息
	business b;
public:
	//customer() {};
	customer(string n, string a, int pn, string ps, bool i=false, string pcp=0) :name(n), address(a), PhoneNum(pn), password(ps)
	{
		b.IsBussiness = i;
		b.PrimaryContactPerson = pcp;
		b.bill = 0;
		/*for (int i = 0; i < 5; i++)
		{
			b.LastbillTime[i] = time[i];
		}*/
		if (b.IsBussiness)
		{
			for (int i = 0; i < 5; i++)
			{
				b.LastbillTime[i] = time[i];
			}
		}
	}

	~customer()
	{
		past.clear();
		now.clear();
		b.month.clear();
	}
	void order(package &p, bool myaddress)	//预约投递服务
	{
		if (myaddress)
			p.setpua(address);
		now.push_back(p);
		if(b.IsBussiness)
			b.month.push_back(p);
	}
	void cancel(int n)	//取消订单，n为从1开始数第n个包裹
	{
		now[n - 1];
		if (now[n - 1].cancal())
		{
			now.erase(now.begin() + n - 1);
			if (b.IsBussiness)
				b.month.erase(b.month.begin() + n - 1);
		}
	}
	bool showcancel()	//显示可取消订单
	{
		bool k = false;
		for (unsigned int i = 0; i < now.size(); i++)
		{
			cout << i + 1 << endl;
			if (now[i].cancal())
			{
				now[i].show();
				k = true;
			}
			else
				cout << "此订单无法取消！" << endl;
		}
		return k;
	}
	bool shownow()	//显示当前订单
	{
		bool k = false;
		for (unsigned int i = 0; i < now.size(); i++)
		{
			cout << i + 1 << endl;
			now[i].show();
			k = true;
		}
		return k;
	}
	bool showpast()	//显示过往订单
	{
		bool k = false;
		for (unsigned int i = 0; i < past.size(); i++)
		{
			cout << i + 1 << endl;
			past[i].show();
			k = true;
		}
		return k;
	}
	bool showthismonth()	//显示当月订单
	{
		bool k = false;
		if (b.IsBussiness)
		{
			for (unsigned int i = 0; i < b.month.size(); i++)
			{
				cout << i + 1 << endl;
				b.month[i].show();
				k = true;
			}
		}
		return k;
	}
	void pickup()
	{

	}
	bool login(string n, string p)
	{
		if (n == name && p == password)
			return true;
		else
			return false;
	}
	bool isbusi()
	{
		return b.IsBussiness;
	}
};

class boss
{
	vector<package> 预订, 已揽件, 在仓库, 已派发, 已收到;

};

class driver
{
	string position;
};

void SetTime(int t[])
{
	for (int i = 0; i < 5; i++)
	{
		time[i] = t[i];
	}
	cout << "时间设置成功，为：";
	cout << time[0] << "年" << time[1] << "月" << time[2] << "日" << ends << time[3] << ':' << time[4] << endl;
}

int main()
{
	vector<customer> c;
	vector<package> np;
	boss Bill_Wiley;
	driver d[2];
	int n;
	do
	{
		cout << "欢迎使用本系统！请问您是:" << endl;
		cout << "1.\n新用户\n2.\n用户\n3.\n老板\n4.\n司机\n";
		cin >> n;
		if (n == 1)
		{
			for (int i = 3; i > 0; i--)
			{
				system("cls");	//清空窗口用
				cout << "请先注册，" << i << "s后跳转";
				Sleep(1000);		//使程序暂停运行1000ms
			}
			system("cls");	//清空窗口用
			string name, passwd[2], addrs;
			int phonb;
			cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
			cout << "姓名：" << endl;
			cin >> name;
			do
			{
				cout << "密码：" << endl;
				cin >> passwd[0];
				system("cls");	//清空窗口用

				cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
				cout << "姓名：" << endl;
				cout << name << endl;
				cout << "密码：" << endl;
				for (int i = 0; i < passwd[0].length(); i++)
				{
					cout << '*';
				}
				cout << "\n请再输入一遍密码：" << endl;
				cin >> passwd[1];
				system("cls");	//清空窗口用
				cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
				cout << "姓名：" << endl;
				cout << name << endl;
				cout << "密码：" << endl;
				for (int i = 0; i < passwd[0].length(); i++)
				{
					cout << '*';
				}
				cout << "\n请再输入一遍密码：" << endl;
				for (int i = 0; i < passwd[1].length(); i++)
				{
					cout << '*';
				}
				cout << endl;
				if (passwd[0] != passwd[1])
				{
					system("cls");	//清空窗口用
					cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
					cout << "姓名：" << endl;
					cout << name << endl;
					cout << "两次输入的密码不一致，请重新输入密码！" << endl;
				}
			} while (passwd[0] != passwd[1]);
			cout << "密码输入正确！" << endl;
			cout << "地址：" << endl;
			cin >> addrs;
			cout << "电话：" << endl;
			cin >> phonb;
			bool is;
			string pcp;
			cout << "请问您是否是公司用户？\n1.是\t2.不是\n";
			do
			{
				cin >> n;
				if (n == 1)
				{
					is = true;
					cout << "请输入贵公司的主要联系人：" << endl;
					cin >> pcp;
				}
				else if (n == 2)
				{
					is = false;
				}
				else
				{
					cout << "输入有误！请重新输入：" << endl;
				}
			} while (n != 1 && n != 2);
			//customer(string n, string a, int pn, string ps, bool i=false, string pcp=0)
			customer x(name, addrs, phonb, passwd[0], is, pcp);
			c.push_back(x);
			for (int i = 3; i > 0; i--)
			{
				system("cls");	//清空窗口用
				cout << "注册成功！\n请返回初始界面进行登录，" << i << "s后跳转";
				Sleep(1000);		//使程序暂停运行1000ms
			}
			system("cls");	//清空窗口用
		}
		else if (n == 2)
		{
			int whichone = -1;
			system("cls");	//清空窗口用
			string nm, pswd;
			do
			{
				cout << "请登录：\n用户名（姓名）：\n";
				cin >> nm;
				cout << "密码：\n";
				cin >> pswd;
				for (int i = 0; i < c.size(); i++)
				{
					if (c[i].login(nm, pswd))
					{
						whichone = i;
						break;
					}
				}
				if (whichone == -1)
				{
					cout << "用户名或密码错误！是否重新输入？\n1.是\t2.否\n";
					cin >> n;
					if (n != 1)
					{
						system("cls");	//清空窗口用
						break;
					}
				}
				else
				{
					do
					{
						system("cls");	//清空窗口用
						cout << "1.\n寄送\n2.\n取消订单\n3.\n查看正在邮递的包裹\n4.\n查看过往已投递完成的包裹\n5.\n注销账号\n";
						if (c[whichone].isbusi())
						{
							cout << "6.\n查看当月流水\n";
						}
						cin >> n;
						if (n == 1)
						{
							system("cls");	//清空窗口用
							float w, d;
							bool v, s;
							string r, ra, pu;
							//	package(float w, float d, bool v, bool s, string r, string ra, string p = 0) :weight(w), distance(d), valueable(v), sameday(s), receiver(r), ReceivedAddress(ra), PickupAddress(p)
							cout << "请输入：\n大致质量：\n";
							cin >> w;
							cout << "大致距离：\n";
							cin >> d;
							cout << "是否贵重：\n";
							cin >> v;
							cout << "是否需要当日达：\n";
							cin >> s;

							cout << "取件地址是否使用您的默认地址？\n1.是\t2.否\n";
							cin >> n;
							if (n != 1)
							{
								cout << "取件地址：\n";
								cin >> pu;
							}
							cout << "收件人：\n";
							cin >> r;
							cout << "收件地址：\n";
							cin >> ra;
							package px(w, d, v, s, r, ra, pu);
							system("cls");	//清空窗口用
							cout << "申请订单成功！以下为包裹信息，请核对！\n";
							px.show();
							cout << "\n是否有误？\n1.无误\t2.有误\n";
							int cs;
							cin >> cs;
							if (cs == 1)
							{
								np.push_back(px);
								if (n == 1)
									c[whichone].order(px, true);
								else
									c[whichone].order(px, false);
							}
							else
							{
								cout << "订单已删除！" << endl;
							}
						}
						else if (n == 2)
						{
							system("cls");	//清空窗口用
							if (c[whichone].showcancel())
							{
								cout << "请问您要取消哪个订单？\n";
								cin >> n;
								c[whichone].cancel(n);
								cout << "取消成功！" << endl;
							}
							else
								cout << "没有可以取消的订单！\n";
						}
						else if (n == 3)
						{
							system("cls");	//清空窗口用
							if(!c[whichone].shownow())
								cout << "无当前订单！\n";
						}
						else if (n == 4)
						{
							system("cls");	//清空窗口用
							if (!c[whichone].showpast())
								cout << "无过往订单！\n";
						}
						else if (n == 5)
						{
							c[whichone].~customer();
							c.erase(c.begin() + whichone);
							cout << "账号已注销！\n";
							Sleep(5000);		//使程序暂停运行5000ms
							system("cls");	//清空窗口用
							break;
						}
						else if (n == 6)
						{
							if (c[whichone].isbusi())
							{
								system("cls");	//清空窗口用
								if (!c[whichone].showthismonth())
									cout << "无当月流水！\n";
							}
							else
								cout << "输入错误！\n";
						}
						else
						{
							cout << "输入错误！\n";
						}
						cout << "接下来：\n1.继续操作\t2.退出登录\n";
						cin >> n;
						system("cls");	//清空窗口用
					} while (n == 1);
					whichone = -2;
					//system("cls");	//清空窗口用
					//cout << "1.\n寄送\n2.\n取消订单\n3.\n查看正在邮递的包裹\n4.\n查看过往已投递完成的包裹\n5.\n注销账号\n";
					//if (c[whichone].isbusi())
					//{
					//	cout << "6.\n查看当月流水\n";
					//}
					//cin >> n;
					//if (n == 1)
					//{
					//	system("cls");	//清空窗口用
					//	float w, d;
					//	bool v, s;
					//	string r, ra, pu;
					//	//	package(float w, float d, bool v, bool s, string r, string ra, string p = 0) :weight(w), distance(d), valueable(v), sameday(s), receiver(r), ReceivedAddress(ra), PickupAddress(p)
					//	cout << "请输入：\n大致质量：\n";
					//	cin >> w;
					//	cout << "大致距离：\n";
					//	cin >> d;
					//	cout << "是否贵重：\n";
					//	cin >> v;
					//	cout << "是否需要当日达：\n";
					//	cin >> s;
					//	cout << "取件地址是否使用您的默认地址？\n1.是\t2.否\n";
					//	cin >> n;
					//	if (n != 1)
					//	{
					//		cout << "取件地址：\n";
					//		cin >> pu;
					//	}
					//	cout << "收件人：\n";
					//	cin >> r;
					//	cout << "收件地址：\n";
					//	cin >> ra;
					//	package px(w, d, v, s, r, ra, pu);
					//	np.push_back(px);
					//	if(n==1)
					//		c[whichone].order(px, true);
					//	else
					//		c[whichone].order(px, false);
					//	cout << "申请订单成功！接下来：\n1.继续操作\t2.退出登录\n";
					//	cin >> n;
					//	if (n == 1)
					//	{

					//	}
					//	else
					//	{
					//		system("cls");	//清空窗口用
					//	}
					//}
				}
			} while (whichone == -1);
		}
		else if (n == 3)
		{
			system("cls");	//清空窗口用

		}
		else if (n == 4)
		{

		}
		else
		{

		}
	} while (true);
	/*
	if (n == 1)
	{	
		for (int i = 3; i > 0; i--)
		{
			system("cls");	//清空窗口用
			cout << "请先注册，" << i << "s后跳转";
			Sleep(1000);		//使程序暂停运行1000ms
		}
		system("cls");	//清空窗口用
		string name, passwd[2], addrs;
		int phonb;
		cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
		cout << "姓名：" << endl;
		cin >> name;
		do
		{
			cout << "密码：" << endl;
			cin >> passwd[0];
			system("cls");	//清空窗口用

			cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
			cout << "姓名：" << endl;
			cout << name << endl;
			cout << "密码：" << endl;
			for (int i = 0; i < passwd[0].length(); i++)
			{
				cout << '*';
			}
			cout << "\n请再输入一遍密码：" << endl;
			cin >> passwd[1];
			system("cls");	//清空窗口用
			cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
			cout << "姓名：" << endl;
			cout << name << endl;
			cout << "密码：" << endl;
			for (int i = 0; i < passwd[0].length(); i++)
			{
				cout << '*';
			}
			cout << "\n请再输入一遍密码：" << endl;
			for (int i = 0; i < passwd[1].length(); i++)
			{
				cout << '*';
			}
			cout << endl;
			if (passwd[0] != passwd[1])
			{
				system("cls");	//清空窗口用
				cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
				cout << "姓名：" << endl;
				cout << name << endl;
				cout << "两次输入的密码不一致，请重新输入密码！" << endl;
			}
		} while (passwd[0] != passwd[1]);
		cout << "密码输入正确！" << endl;
		cout << "地址：" << endl;
		cin >> addrs;
		cout << "电话：" << endl;
		cin >> phonb;
		bool is;
		string pcp;
		cout << "请问您是否是公司用户？\n1.是\t2.不是\n";
		do
		{
			cin >> n;
			if (n == 1)
			{
				is = true;
				cout << "请输入贵公司的主要联系人：" << endl;
				cin >> pcp;
			}
			else if (n == 2)
			{
				is = false;
			}
			else
			{
				cout << "输入有误！请重新输入：" << endl;
			}
		} while (n != 1 && n != 2);
		//customer(string n, string a, int pn, string ps, bool i=false, string pcp=0)
		customer x(name, addrs, phonb, passwd[0], is, pcp);
		c.push_back(x);
		for (int i = 3; i > 0; i--)
		{
			system("cls");	//清空窗口用
			cout << "注册成功！\n请返回初始界面进行登录，" << i << "s后跳转";
			Sleep(1000);		//使程序暂停运行1000ms
		}
		system("cls");	//清空窗口用
	}
	else if (n == 2)
	{
		int whichone = -1;
		system("cls");	//清空窗口用
		string n, pswd;
		do
		{
			cout << "请登录：\n用户名（姓名）：\n";
			cin >> n;
			cout << "密码：\n";
			cin >> pswd;
			for (int i = 0; i < c.size(); i++)
			{
				if (c[i].login(n, pswd))
				{
					whichone = i;
					break;
				}
			}
			if (whichone == -1)
			{
				cout << "用户名或密码错误！请重新输入\n";
			}
			else
			{
				system("cls");	//清空窗口用
				cout << "1.\n寄送\n2.\n取消订单\n3.\n查看正在邮递的包裹\n4.\n查看过往已投递完成的包裹\n5.\n注销账号\n";
				if (c[whichone].isbusi())
				{
					cout << "6.\n查看当月流水\n";
				}

			}
		} while (whichone == -1);
	}
	else if (n == 3)
	{
		system("cls");	//清空窗口用

	}
	else if (n == 4)
	{
		
	}
	else
	{
		
	}
	*/
	system("cls");	//清空窗口用
	Sleep(1000);		//使程序暂停运行1000ms
	system("PAUSE ");	//暂停运行程序，并出现“请按任意键继续. . .”字样，进行等待
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
