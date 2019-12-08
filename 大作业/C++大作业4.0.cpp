#include <vector>	//可变数组/向量
#include <iostream>
#include <iomanip>
//#include <algorithm>
#include <windows.h>	//用于控制台输出
#include<string>
using namespace std;

int mytime[5] = { 2019, 9, 1, 0, 0 };	//为演示需要，设置一个全局变量，时间：年 月 日 时 分

class driver;	//package类里有driver和customer成员，预先声明
class customer;

class package
{
	//friend boss;
	//friend customer;
	float weight, distance, cost;
	bool payed, valueable, sameday, retrieved, warehouse, delivery, received;	//retrieved为揽件；后四个为包裹的流程状态（取件、入仓、派送、送达）
	int OrderTime[5], PickupTime[5], WarehouseTime[5], DeliveryTime[5], ReceivedTime[5];	//流程中的时间节点记录
	string PickupAddress, ReceivedAddress;
	string receiver;
	driver *deliverman;
	customer *owner;
public:
	//构造函数，申请订单时使用
	package(customer &own, int put[], float w, float d, bool v, bool s, string r, string ra, string p) :weight(w), distance(d), valueable(v), sameday(s), receiver(r), ReceivedAddress(ra), PickupAddress(p)
	{
		owner = &own;
		if (sameday)
			cost = weight * distance * 2;
		else
			cost = weight * distance;
		delivery = warehouse = received = payed = retrieved = false;
		for (int i = 0; i < 5; i++)
		{
			PickupTime[i] = put[i];
			OrderTime[i] = mytime[i];
		}
	}
	~package()
	{
		deliverman = 0;
		owner = 0;
	}
	void pickup(driver &dm);	//取件时对包裹信息的修改（是否已取件、取件时间、取件司机）
	void show();	//展示包裹所有信息，几乎在所有地方都有使用
	bool cancal()	//为顾客取消订单做准备，返回值为“能否取消这个订单”
	{
		return !retrieved;
	}
	float getcost()
	{
		return cost;
	}
	bool isown(string n);
	void receive()
	{
		warehouse = delivery = received = true;
		for (int i = 0; i < 5; i++)
		{
			WarehouseTime[i] = mytime[i];
			DeliveryTime[i] = mytime[i];
			PickupTime[i] = mytime[i];
		}
	}
};

class customer
{
	struct business	//如果顾客是公司用户，那么就有这些属性：是否为公司用户，首要联系人，本月账单，上次付钱的时间，本月流水（本月已寄送的包裹）
	{
		bool IsBussiness;
		string PrimaryContactPerson;
		float bill;
		int LastbillTime[5];
		vector<package> month;
	};
	string name, address, password;
	int PhoneNum;
	vector<package> past, now;	//记录顾客的包裹信息：所有已投递完成的、正在投递过程中的
	business b;
public:
	//customer() {};

	//新用户注册时要用
	customer(string n, string a, int pn, string ps, bool i=false, string pcp=0) :name(n), address(a), PhoneNum(pn), password(ps)
	{
		b.IsBussiness = i;
		b.PrimaryContactPerson = pcp;
		b.bill = 0;
		/*for (int i = 0; i < 5; i++)
		{
			b.LastbillTime[i] = mytime[i];
		}*/
		if (b.IsBussiness)
		{
			for (int i = 0; i < 5; i++)
			{
				b.LastbillTime[i] = mytime[i];
			}
		}
	}
	//注销用户
	~customer()
	{
		past.clear();
		now.clear();
		b.month.clear();
	}
	package& order(package &p)	//预约投递服务
	{
		now.push_back(p);
		if(b.IsBussiness)
			b.month.push_back(p);
		return now[now.size() - 1];
	}
	package* canptr(int n)
	{
		if (n > 0 && n <= now.size())
		{
			if (now[n - 1].cancal())
				return &now[n - 1];
		}
		return 0;
	}
	bool cancel(int n)	//取消订单，n为从1开始数第n个包裹
	{
		if (n > 0 && n <= now.size())
		{
			if (now[n - 1].cancal())
			{
				now[n - 1].~package();
				now.erase(now.begin() + n - 1);
				if (b.IsBussiness)
					b.month.erase(b.month.begin() + n - 1);
				return true;
			}
		}
		return false;
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
			cout << endl;
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
			cout << endl;
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
			cout << endl;
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
				cout << endl;
				k = true;
			}
		}
		return k;
	}
	void pickup()	//未完成：取件时对顾客的信息修改函数，修改bill
	{

	}
	bool login(string n, string p)	//登录函数，如果用户名和密码都正确，则返回true
	{
		if (n == name && p == password)
			return true;
		else
			return false;
	}
	bool isbusi()	//判断是否为公司用户
	{
		return b.IsBussiness;
	}
	string getname()	//v4.0展示包裹时可以展示所属客户的姓名
	{
		return name;
	}
	string getadd()
	{
		return address;
	}
};

class boss	//老板掌控全局，需要查看所有包裹信息、当月营收
{
	vector<package *> now, past, month;
	float bill;
public:
	boss():bill(0){}
	~boss()
	{
		now.clear();
		past.clear();
		month.clear();
	}
	void addnow(package &n)
	{
		now.push_back(&n);
	}
	bool cancel(package *cp)
	{
		bool t[2];
		for (int i = 0; i < now.size(); i++)
		{
			if (now[i] == cp)
			{
				now.erase(now.begin() + i);
				t[0] = true;
				break;
			}
		}
		for (int i = 0; i < month.size(); i++)
		{
			if (month[i] == cp)
			{
				month.erase(month.begin() + i);
				t[1] = true;
				break;
			}
		}
		if (t[0] == true && t[1] == true)
			return true;
		return false;
	}
	void addpast(package &n)
	{
		past.push_back(&n);
	}
	void addmonth(package &n)
	{
		month.push_back(&n);
	}
	bool shownow()
	{
		bool k = false;
		for (unsigned int i = 0; i < now.size(); i++)
		{
			cout << i + 1 << endl;
			now[i]->show();
			cout << endl;
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
			past[i]->show();
			cout << endl;
			k = true;
		}
		return k;
	}
	bool showmonth()	//显示当月订单
	{
		bool k = false;
		for (unsigned int i = 0; i < month.size(); i++)
		{
			cout << i + 1 << endl;
			month[i]->show();
			cout << endl;
			k = true;
		}
		return k;
	}
	float getbill()
	{
		return bill;
	}
	void cancus(string n)
	{
		for (int i = 0; i < now.size(); i++)
		{
			if (now[i]->isown(n))
			{
				now.erase(now.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < past.size(); i++)
		{
			if (past[i]->isown(n))
			{
				past.erase(past.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < month.size(); i++)
		{
			if (month[i]->isown(n))
			{
				month.erase(month.begin() + i);
				i--;
			}
		}
	}
	vector<package *>& getnow()
	{
		return now;
	}
	void addbill(float c)
	{
		bill += c;
	}
	void turnpast()
	{
		while (!now.empty())
		{
			past.push_back(now.back());
			now.pop_back();
		}
	}
};

class driver	//司机负责包裹流通过程中的各个环节：取件、入仓、派送、送达
{
	string name;
public:
	driver(string n="Tom"):name(n){}
};

void package::show()	//展示包裹所有信息，几乎在所有地方都有使用
{
	//v4.0
	cout << endl;
	cout << "包裹所属的用户为：\n" << owner->getname() << endl;
	cout << "\n质量：\n" << weight << "\n距离：\n" << distance << "\n费用：\n" << cost << endl;
	cout << "\n揽件地点：\n" << PickupAddress << "\n收件地址：\n" << ReceivedAddress << "\n收件人：\n" << receiver << endl;
	cout << "\n包裹属性：";
	if (valueable)
		cout << "贵重\t";
	if (sameday)
		cout << "同日达\t";
	if (payed)
		cout << "已支付\n";
	else
		cout << "未支付\n";
	cout << "\n申请订单时间：\n";
	cout << OrderTime[0] << "年" << OrderTime[1] << "月" << OrderTime[2] << "日" << ends;
	cout << setfill('0') << setw(2) << OrderTime[3] << ':' << setw(2) << OrderTime[4] << endl;
	//	cout << setfill('0') << setw(2) << mytime[3] << ':' << setw(2) << mytime[4] << endl;
	cout << endl;
	if (!retrieved)
	{
		cout << "正在前往揽件，预估的时间为：\n";
		cout << PickupTime[0] << "年" << PickupTime[1] << "月" << PickupTime[2] << "日" << ends << setfill('0') << setw(2) << PickupTime[3] << ':' << setw(2) << PickupTime[4] << endl;
	}
	else
	{
		cout << "已揽件，揽件时间：\n";
		cout << PickupTime[0] << "年" << PickupTime[1] << "月" << PickupTime[2] << "日" << ends << setfill('0') << setw(2) << PickupTime[3] << ':' << setw(2) << PickupTime[4] << endl;
		if (!warehouse)
			cout << "正在发往仓库\n";
		else
		{
			cout << "已入仓，入仓时间：\n";
			cout << WarehouseTime[0] << "年" << WarehouseTime[1] << "月" << WarehouseTime[2] << "日" << ends << setfill('0') << setw(2) << WarehouseTime[3] << ':' << setw(2) << WarehouseTime[4] << endl;
			if (!delivery)
				cout << "正在仓库，准备派发\n";
			else
			{
				cout << "已派发，派发时间：\n";
				cout << DeliveryTime[0] << "年" << DeliveryTime[1] << "月" << DeliveryTime[2] << "日" << ends << setfill('0') << setw(2) << DeliveryTime[3] << ':' << setw(2) << DeliveryTime[4] << endl;
				if (!received)
					cout << "正在派发途中\n";
				else
				{
					cout << "已抵达，抵达时间：\n";
					cout << DeliveryTime[0] << "年" << DeliveryTime[1] << "月" << DeliveryTime[2] << "日" << ends << setfill('0') << setw(2) << DeliveryTime[3] << ':' << setw(2) << DeliveryTime[4] << endl;
				}
			}
		}
	}
}
bool package::isown(string n)
{
	if (n == owner->getname())
		return true;
	return false;
}
void package::pickup(driver &dm)	//取件时对包裹信息的修改（是否已取件、取件时间、取件司机）
{
	retrieved = true;
	if (!this->owner->isbusi())
		payed = true;
	for (int i = 0; i < 5; i++)
	{
		PickupTime[i] = mytime[i];
	}
	deliverman = &dm;
}



//v4.0小时和分钟现在可以正常显示
void SetTime(int t[])	//修改时间
{
	for (int i = 0; i < 5; i++)
	{
		mytime[i] = t[i];
	}
	cout << "时间设置成功，为：";
	cout << mytime[0] << "年" << mytime[1] << "月" << mytime[2] << "日" << ends;
	cout << setfill('0') << setw(2) << mytime[3] << ':' << setw(2) << mytime[4] << endl;
	//	cout << setfill('0') << setw(2) << 1234 << endl;
}

int main()
{
	vector<customer> c;	//存储所有用户的序列
	boss Bill_Wiley;	//老板
	driver d("Tom");	//司机，不确定到底有几个
	int n;	//用于判断，后面在反复使用它，看起来可能有点烧脑。谨记：我所有判断都是使用的if+else if，所以上一个if中不管n如何改变，下一个else if都不会执行；每次判断n时都只是使用上一次cin>>n的值，与前面无关
	
	//v4.0
	cout << "本系统由冯泽旭、李程浩、王永裕制作，侵权必究！";
	Sleep(3000);		//使程序暂停运行3000ms

	do	//不断循环，从主界面开始
	{
		system("cls");	//清空窗口用

		//v4.0新增，修改时间功能
		cout << "是否修改时间？\n1.是\t2.否" << endl;
		cin >> n;
		if (n == 1)
		{
			cout << "请输入当前时间，以供校准：（仅数字）" << endl;
			int t[5];
			for (int i = 0; i < 5; i++)
			{
				cin >> t[i];
			}
			SetTime(t);
		}
		else
		{
			cout << "当前时间为：";
			cout << mytime[0] << "年" << mytime[1] << "月" << mytime[2] << "日" << ends;
			cout << setfill('0') << setw(2) << mytime[3] << ':' << setw(2) << mytime[4] << endl;
		}
		system("PAUSE ");	//暂停运行程序，并出现“请按任意键继续. . .”字样，进行等待
		system("cls");	//清空窗口用

		cout << "欢迎使用本系统！请问您是:" << endl;	//主界面
		cout << "1.\n新用户\n2.\n用户\n3.\n老板\n4.\n司机\n";
		cin >> n;
		if (n == 1)	//新用户注册流程
		{
			for (int i = 3; i > 0; i--)	//跳转界面，纯属耍帅233
			{
				system("cls");	//清空窗口用
				cout << "请先注册，" << i << "s后跳转";
				Sleep(1000);		//使程序暂停运行1000ms
			}
			system("cls");	//清空窗口用
			string name, passwd[2], addrs;	//申请新用户所需数据
			int phonb;
			cout << "请依次输入您的姓名（或公司名称）、密码、地址、电话" << endl;
			cout << "姓名：" << endl;
			cin >> name;
			do	//此处循环用于两次密码输入不一致时，反复输入直到一致
			{
				cout << "密码：" << endl;
				cin >> passwd[0];
				system("cls");	//清空窗口用
				//下面这段的意思：我想把密码用****遮起来，但只能把窗口清空，然后重新输出上面的所有字符，然后把密码这一行输出*****来实现
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
				//同理，再次清空然后变******
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
			bool is;	//是否是否是公司用户
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
			customer x(name, addrs, phonb, passwd[0], is, pcp);	//新建顾客对象
			c.push_back(x);	//把顾客对象加进所有顾客的序列里
			for (int i = 3; i > 0; i--)	//跳转界面，纯属耍帅233
			{
				system("cls");	//清空窗口用
				cout << "注册成功！\n请返回初始界面进行登录，" << i << "s后跳转";
				Sleep(1000);		//使程序暂停运行1000ms
			}
		}
		else if (n == 2)	//用户登录以及使用界面
		{
			int whichone = -1;	//用户序列的下标，登录了哪个用户，whichone就存储那个用户在用户序列c里的下标，之后用c[whichone]操作那个用户
			system("cls");	//清空窗口用
			string nm, pswd;
			do
			{
				cout << "请登录：\n用户名（姓名）：\n";
				cin >> nm;
				cout << "密码：\n";
				cin >> pswd;
				for (int i = 0; i < c.size(); i++)	//遍历用户序列，假如有哪个用户的用户名和密码都匹配，就登录为ta
				{
					if (c[i].login(nm, pswd))
					{
						whichone = i;
						break;
					}
				}
				if (whichone == -1)	//假如用户序列都不匹配，就错了
				{
					cout << "用户名或密码错误！是否重新输入？\n1.是\t2.否\n";
					cin >> n;
					if (n != 1)
					{
						break;
					}
				}
				else	//登陆成功后的操作
				{
					do	//用户界面，v4.0新增退出登录功能
					{
						system("cls");	//清空窗口用
						cout << "1.\n寄送\n2.\n取消订单\n3.\n查看正在邮递的包裹\n4.\n查看过往已投递完成的包裹\n5.\n退出登录\n6.\n永久注销账号\n";
						if (c[whichone].isbusi())	//只有用户是公司用户，才显示这一项
						{
							cout << "7.\n查看当月流水\n";
						}
						cin >> n;
						if (n == 1)	//申请订单界面
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

							cout << "请输入预约的取件时间：（仅数字）" << endl;
							int put[5];
							for (int i = 0; i < 5; i++)
							{
								cin >> put[i];
							}
							cout << "取件地址是否使用您的默认地址？\n1.是\t2.否\n";
							cin >> n;
							if (n != 1)
							{
								cout << "取件地址：\n";
								cin >> pu;
							}
							else
								pu = c[whichone].getadd();
							cout << "收件人：\n";
							cin >> r;
							cout << "收件地址：\n";
							cin >> ra;
							package px(c[whichone], put, w, d, v, s, r, ra, pu);	//这是一个临时对象，如果确认，就把它加进主函数的np当前包裹序列、客户的now当前包裹序列；如果失败，循环结束后自动析构，无需操作
							system("cls");	//清空窗口用
							cout << "申请订单成功！以下为包裹信息，请核对！\n";
							px.show();
							cout << "\n是否有误？\n1.无误\t2.有误\n";
							cin >> n;
							if (n == 1)
							{
								Bill_Wiley.addnow(c[whichone].order(px));	//v4.0客户的now当前包裹序列、公司用户的month包裹序列，并且加入老板的当前序列
								Bill_Wiley.addmonth(c[whichone].order(px));
								cout << "申请订单成功！"<<endl;
							}
							else
							{
								cout << "订单已删除！" << endl;
							}
						}
						else if (n == 2)	//取消订单界面
						{
							system("cls");	//清空窗口用
							if (c[whichone].showcancel())	//if判断括号里面的函数输出能够取消的订单，并返回值：是否有能够取消的订单
							{
								cout << "请问您要取消哪个订单？\n";
								cin >> n;
								if (Bill_Wiley.cancel(c[whichone].canptr(n)) && c[whichone].cancel(n))
									cout << "取消成功！" << endl;
								else
									cout << "取消失败！" << endl;
							}
							else
								cout << "没有可以取消的订单！\n";
						}
						else if (n == 3)
						{
							system("cls");	//清空窗口用
							if(!c[whichone].shownow())	//customer的这几个show函数原理一样，都有两个功能：输出、判断
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
							break;
						}
						else if (n == 6)	//注销，先析构这个时候登录的用户，再从用户序列里删除ta
						{
							Bill_Wiley.cancus(c[whichone].getname());
							c[whichone].~customer();
							c.erase(c.begin() + whichone);
							cout << "账号已注销！\n";
							Sleep(5000);		//使程序暂停运行5000ms
							system("cls");	//清空窗口用
							break;	
						}
						else if (n == 7)
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
					} while (n == 1);
					whichone = -2;	//将用户下标设为-2，表示用户下线，同时也能返回到主界面
				}
			} while (whichone == -1);
		}
		else if (n == 3)	//V4.0老板重写了，要面向对象啊
		{
			do
			{
				system("cls");	//清空窗口用
				cout << "1.\n查看正在邮递的包裹\n2.\n查看过往已投递完成的包裹\n3.查看当月流水\n";
				cin >> n;

				if (n == 1)
				{
					system("cls");	//清空窗口用
					if (!Bill_Wiley.shownow())
						cout << "无当前订单！\n";
				}
				else if (n == 2)	
				{
					system("cls");	//清空窗口用
					if (!Bill_Wiley.showpast())
						cout << "无过往订单！\n";
				}
				else if (n == 3)	//v4.0未实现
				{
					system("cls");	//清空窗口用
					if (!Bill_Wiley.showmonth())
						cout << "无当月订单！\n" << endl;
					cout << "当月总收入为：" << Bill_Wiley.getbill() << endl;
				}
				else
				{
					cout << "输入错误！" << endl;
					Sleep(1000);		//使程序暂停运行1000ms
				}
				cout << "接下来：\n1.继续操作\t2.退出登录\n";
				cin >> n;
			} while (n == 1);
		}
		else if (n == 4)	//V3.0司机改变包裹状态
		{
			do
			{
				system("cls");	//清空窗口用
				//查看包裹信息
				cout << "1.揽件\n2.已送达\n";
				//1准时到达指定地点，司机揽件，对序列中最后一个包裹
				cin >> n;
				if (n == 1)
				{
					if (!Bill_Wiley.shownow())
						cout << "无当前订单！\n";
					else
					{
						cout << "以上为所有当前订单，是否确认揽件？\n1.是\t2.否" << endl;
						cin >> n;
						if (n == 1)
						{
							for (int i = 0; i < Bill_Wiley.getnow().size(); i++)
							{
								Bill_Wiley.getnow()[i]->pickup(d);
								Bill_Wiley.addbill(Bill_Wiley.getnow()[i]->getcost());
							}
							cout << "已揽件！";
						}
					}
				}
				else if (n == 2)
				{
					if (!Bill_Wiley.shownow())
						cout << "无当前订单！\n";
					else
					{
						cout << "以上为所有当前订单，是否确认收件人已收货？\n1.是\t2.否" << endl;
						cin >> n;
						if (n == 1)
						{
							for (int i = 0; i < Bill_Wiley.getnow().size(); i++)
							{
								Bill_Wiley.getnow()[i]->receive();
								Bill_Wiley.turnpast();
							}
							cout << "客户已收货" << endl;
						}
					}
				}
				else
				{
					cout << "输入错误！" << endl;
					Sleep(1000);		//使程序暂停运行1000ms
				}
				cout << "接下来：\n1.继续操作\t2.退出登录\n";
				cin >> n;
			} while (n == 1);
		}
		else	//v4.0
		{
			cout << "输入错误！" << endl;
			Sleep(1000);		//使程序暂停运行1000ms
		}
	} while (true);	//不停重复，以返回到主界面
	//以下为控制台里有用的功能
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
