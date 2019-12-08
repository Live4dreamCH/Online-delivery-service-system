#include <vector>	//可变数组/向量
#include <iostream>
//#include <algorithm>
#include <windows.h>	//用于控制台输出
#include<string>
using namespace std;
int sumcost = 0;//V3.0 全局变量
int time[5];	//为演示需要，设置一个全局变量，时间：年 月 日 时 分

class driver;	//package类里有driver成员，预先声明

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
	
public:
	//构造函数，申请订单时使用
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
		sumcost += cost;
	}
	void setpua(string p)	//设置取件地址，单独分出来，是因为在主函数中想使用顾客的默认地址
	{
		PickupAddress = p;
	}
	void sendpackage()//V3.0
	{
		received = true;
		
	}
	void pickup(driver &dm)	//取件时对包裹信息的修改（是否已取件、取件时间、取件司机）
	{
		retrieved = true;
		for (int i = 0; i < 5; i++)
		{
			PickupTime[i] = time[i];
		}
		deliverman = &dm;
	}
	void show()	//展示包裹所有信息，几乎在所有地方都有使用
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
	bool cancal()	//为顾客取消订单做准备，返回值为“能否取消这个订单”
	{
		return !retrieved;
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
	//注销用户
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
	/*float sumcost() {
		cout << sumcost();//V3.0将已完成订单放入过往订单，数组下标越界没改出来
		return 0;
	}
	void nowtopast(){
		
		past[past.size()-1] = now[now.size() - 1];
			now[now.size()-1].cancal();
	}*/
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
};

class boss :public customer	//老板掌控全局，需要查看所有包裹信息、当月营收
{
	vector<package> 预订, 已揽件, 在仓库, 已派发, 已收到;
	float month;

};

class driver	//司机负责包裹流通过程中的各个环节：取件、入仓、派送、送达
{
	string position;

};

void SetTime(int t[])	//修改时间
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
	vector<customer> c;	//存储所有用户的序列
	vector<package> np;	//存储所有正在投递过程中的包裹
	//boss Bill_Wiley;	//老板
	driver d[2];	//司机，不确定到底有几个
	int n;	//用于判断，后面在反复使用它，看起来可能有点烧脑。谨记：我所有判断都是使用的if+else if，所以上一个if中不管n如何改变，下一个else if都不会执行；每次判断n时都只是使用上一次cin>>n的值，与前面无关
	do	//不断循环，从主界面开始
	{
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
			for (int i = 1; i > 0; i--)	//跳转界面，纯属耍帅233
			{
				system("cls");	//清空窗口用
				cout << "注册成功！\n请返回初始界面进行登录，" << i << "s后跳转";
				Sleep(1000);		//使程序暂停运行1000ms
			}
			system("cls");	//清空窗口用
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
						system("cls");	//清空窗口用
						break;
					}
				}
				else	//登陆成功后的操作
				{
					do	//用户界面
					{
						system("cls");	//清空窗口用
						cout << "1.\n寄送\n2.\n取消订单\n3.\n查看正在邮递的包裹\n4.\n查看过往已投递完成的包裹\n5.\n注销账号\n";
						if (c[whichone].isbusi())	//只有用户是公司用户，才显示这一项
						{
							cout << "6.\n查看当月流水\n";
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
							package px(w, d, v, s, r, ra, pu);	//这是一个临时对象，如果确认，就把它加进主函数的np当前包裹序列、客户的now当前包裹序列；如果失败，循环结束后自动析构，无需操作
							system("cls");	//清空窗口用
							cout << "申请订单成功！以下为包裹信息，请核对！\n";
							px.show();
							cout << "\n是否有误？\n1.无误\t2.有误\n";
							int cs;	//前面用于判断的n暂时不能动，所以新建了一个判断变量choose
							cin >> cs;
							if (cs == 1)
							{
								np.push_back(px);	//把它加进主函数的np当前包裹序列
								if (n == 1)
									c[whichone].order(px, true);	//客户的now当前包裹序列、公司用户的month包裹序列
								else
									c[whichone].order(px, false);	//第二个参数为是否使用默认地址
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
								c[whichone].cancel(n);
								cout << "取消成功！" << endl;
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
						else if(n==5)
						{
							c[whichone].~customer();
							c.erase(c.begin() + whichone);
							cout << "账号已注销！\n";
							Sleep(5000);		//使程序暂停运行5000ms
							system("cls");	//清空窗口用
							break;	//循环里所有的break都是返回最初的主界面的意思
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
					whichone = -2;	//将用户下标设为-2，表示用户下线，同时也能返回到主界面
				}
			} while (whichone == -1);
		}
		else if (n == 3)	//V3.0老板
		{
			do
			{
				system("cls");	//清空窗口用
				cout << "1.\n查看正在邮递的包裹\n2.\n查看过往已投递完成的包裹\n3.查看月收入\n";
				cin >> n;

				bool all = true;	//v4.0，用于判断是否所有用户都没有包裹可以展示，如果全都没有，才输出"无过往/当前订单！"
				if (n == 1)
				{
					system("cls");	//清空窗口用
					for (int i = 0; i < c.size(); i++)	//v3.0这里，如果有任何一个顾客无过往订单，就会输出”无当前订单“，然而事实上需要全部顾客都没有订单才需要输出，v4.0有修改
					{
						if (c[i].shownow())	//v4.0只要有人有包裹，就不输出"无当前订单！"
							all = false;
					}
					if (all)
						cout << "无当前订单！\n";
				}
				else if (n == 2)
				{
					system("cls");	//清空窗口用
					for (int i = 0; i < c.size(); i++)
					{
						if (c[i].showpast())
							all = false;
						//cout << "无过往订单！\n";
					}
					if (all)
						cout << "无过往订单！\n";
				}
				else if (n == 3)	//v4.0
				{
					//cout << sumcost;//输出月收入
				}
				cout << "接下来：\n1.继续操作\t2.退出登录\n";
				cin >> n;
				system("cls");	//清空窗口用
			} while (n == 1);
			/*
			system("cls");	//清空窗口用
			cout << "1.\n查看正在邮递的包裹\n2.\n查看过往已投递完成的包裹\n3.查看月收入\n";
			cin >>n;
			
			bool all = true;	//v4.0，用于判断是否所有用户都没有包裹可以展示，如果全都没有，才输出"无过往/当前订单！"
			if (n == 1)
			{
				system("cls");	//清空窗口用
				for (int i = 0; i < c.size(); i++)	//v3.0这里，如果有任何一个顾客无过往订单，就会输出”无当前订单“，然而事实上需要全部顾客都没有订单才需要输出，v4.0有修改
				{
					if (c[i].shownow())	//v4.0只要有人有包裹，就不输出"无当前订单！"
						all = false;
				}
				if (all)
					cout << "无当前订单！\n";
			}
			else if (n == 2)
			{
				system("cls");	//清空窗口用
				for (int i = 0; i < c.size(); i++)
				{
					if (c[i].showpast())
						all = false;
						//cout << "无过往订单！\n";
				}
				if (all)
					cout << "无过往订单！\n";
			}
			else if (n == 3)
			{
					cout << sumcost;//输出月收入
			}
			cout << "接下来：\n1.继续操作\t2.退出登录\n";
			cin >> n;
			system("cls");	//清空窗口用
			*/
		}
		
		 if (n == 4)	//V3.0司机改变包裹状态
		{
			system("cls");	//清空窗口用
			//查看包裹信息
			cout << "\n1.揽件\n2.已送达";
			//1准时到达指定地点，司机揽件，对序列中最后一个包裹
			cin >> n;
			if (n == 1)
			{
				np[np.size() - 1].pickup(d[0]);
				cout << "已揽件！";
			}
			//准时到达收货地点，将货物状态改变
			if (n == 2)
			{
				np[np.size()-1].sendpackage();
				//c[c.size()].nowtopast();//放入过往订单失败，没写出来
				cout << "客户已收货" << endl;
			}
			cout << "接下来：\n1.继续操作\t2.退出登录\n";
			cin >> n;
			system("cls");	//清空窗口用	
		}
		 else	//v4.0
		 {
			 cout << "输入错误！" << endl;
			 Sleep(1000);		//使程序暂停运行1000ms
			 system("cls");	//清空窗口用
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
