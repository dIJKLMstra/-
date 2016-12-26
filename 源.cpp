#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
int M, N, R, K, T;
int dHP, nHP, iHP, lHP, wHP; //初始生命值
int dA, nA, iA, lA, wA; //攻击力
string redorder[5] = { "iceman","lion","wolf","ninja","dragon" }; //武士一共有五种
string blueorder[5] = { "lion","dragon","ninja","iceman","wolf" };
string cinorder[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string quartername[2] = { "red", "blue" };
bool warFin ; //战争结束

class city;
class Headquarter;
class wartime //时间
{
public:
	int hour;   //记录小时
	int minute; //记录分钟
	wartime(int _h, int _m) :hour(_h), minute(_m){}
	void addhour()  //进位
	{
		if (minute >= 60)
		{
			minute -= 60;
			hour++;
		}
	}
	void printtime()       //每次都要输出的时间
	{
		cout << setw(3) << setfill('0') << hour << ":";
		cout << setw(2) << setfill('0') << minute << " ";
	}
	bool isFin()  //判断是否达到结束时间
	{
		if (hour * 60 + minute > T) return true;
		else  return false;
	}
	void reset()  //重置
	{
		hour = minute = 0;
	}
}; wartime warclock(0, 0);


class sword //剑
{
public:
	int atk;
	sword(int _waratk):atk(_waratk/5){} //初始值为拥有它的武士的攻击力的20%
};

class arrow //箭
{
public:
	int atk;
	int usetime; //耐久度
	arrow() :atk(R),usetime(3){}  //arrow有一个攻击力值R
};

class bomb //炸弹
{
public:
	bool usebomb;  //是否使用bomb
	bomb() :usebomb(false){}
};

class warrior //武士
{
public:	
	sword* s;  //是否有武器
	arrow* a;
	bomb* b;
	int loyal; //忠诚度
	int warnum;//武士编号
	int atk; //攻击力
	int HP; //生命值
	int pos;//所在城市编号
	int movestep;//为雪人准备的移动步数
	int beforewarHP;//为狮子准备的战前血量
	double morale; //士气
	string type; //种类
	bool dead; //是否死亡
	string color; //红还是蓝

	//好长的构造函数、、
	warrior(int num, string _type, string _color);
	void lionrunaway(city &c);
	void goForward();
	void shoot(warrior* w);
	void attack(warrior* w);
	void fightback(warrior* w);
	bool will_be_killed(warrior* w);
	void usebomb(warrior* w);
	void win(warrior* w);
	void draw(warrior* w);
	int get_atk();
	int get_back_atk();
};

class city //城市
{
public:
	int citynum; //城市编号
	int cityelem; //城市生命元
	warrior* redwar; //是否有红色武士
	warrior* bluewar;//蓝色武士
	string lastwinner;//上一场的赢家
	string flag;//旗帜颜色
	city() :citynum(0), cityelem(0), redwar(NULL), bluewar(NULL),lastwinner("none"),flag("none"){}
	void fight(); // 管理战争部分
	void clearbody(); //清除尸体
}; city *City;

class output  //管理各种输出
{
public:
	//武士降生部分输出
	void born(warrior* w)  
	{
		warclock.printtime();
		cout << w -> color << " " << w -> type << " " << w -> warnum << " born" << endl;
		if (w->type == "dragon")   //造出dragon
			cout << "Its morale is " << setiosflags(ios::fixed)<<setprecision(2)<<w->morale<<endl;//四舍五入到小数点后两位
		if (w->type == cinorder[3])    //造出lion
			cout << "Its loyalty is " << w->loyal << endl;
	}
	//lion逃跑部分输出
	void runaway(warrior* w) 
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " ran away" << endl;
	}
	//武士前进部分输出
	void cforward(warrior* w,int citynum) //抵达城市
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " marched to city " << citynum << " with " << w->HP << " elements and force " << w->atk<<endl;
	}
	void hforward(warrior* w, string color)//抵达司令部
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " reached " << color << " headquarter with " << w->HP << " elements and force " << w->atk << endl;
	}
	void headtaken(string color) //司令部被占领
	{
		warclock.printtime();
		cout << color << " headquarter was taken" << endl;
	}
	//武士获取生命元部分输出
	void getelements(warrior* w,int elements)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " earned " << elements << " elements for his headquarter" << endl;
	}
	//武士放箭部分输出
	void shoot(warrior* w1, warrior* w2)
	{
		if (w2->HP <= 0)  //如果杀死敌人
		{
			w2->dead = true;
			warclock.printtime();
			cout << w1->color << " " << w1->type << " " << w1->warnum << " shot and killed "<<w2 ->color<<" "<<w2 ->type<<" "<<w2 -> warnum<<endl;
		}
		else
		{
			warclock.printtime(); //未杀死
			cout << w1->color << " " << w1->type << " " << w1->warnum << " shot" << endl;
		}
	
	}
	//武士使用bomb部分输出
	void bomb(warrior* w1,warrior* w2)
	{
		warclock.printtime();
		cout << w1->color << " " << w1->type << " " << w1->warnum << " used a bomb and killed " << w2->color << " " << w2->type << " " << w2->warnum << endl;
	}
	//武士主动进攻部分输出
	void attack(warrior* w1,warrior* w2)
	{
		warclock.printtime();
		cout << w1->color << " " << w1->type << " " << w1->warnum << " attacked " << w2->color << " " << w2->type << " " << w2->warnum << " in city " << w1->pos << " with " << w1->HP << " elements and force " << w1->atk<< endl;
	}
	//武士反击部分输出
	void fightback(warrior*w1, warrior* w2)
	{
		warclock.printtime();
		cout << w1->color << " " << w1->type << " " << w1->warnum << " fought back against " << w2->color << " " << w2->type << " " << w2->warnum << " in city " << w1->pos << endl;
	}
	//武士战死部分输出
	void killed(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " was killed in city " << w->pos << endl;
	}
	//武士欢呼部分输出
	void yell(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " yelled in city " << w->pos << endl;
	}
	//旗帜升起部分输出
	void raiseflag(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " flag raised in city " << w->pos << endl;
	}
	//司令部报告生命元数量部分输出
	void headelements(Headquarter &h);
	//武器报告部分输出
	void weapon(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " has ";
		if (w->a == NULL && w->b == NULL && w->s == NULL)  //如果没武器
			cout << "no weapon" << endl;
		else  //如果有武器
		{
			//if (w ->s != NULL && w->s->atk == 0) w->s = NULL;
			if (w->a != NULL)
				cout << "arrow(" << w->a->usetime << ")";
			if (w->a == NULL && w->b != NULL)
				cout << "bomb";
			if (w->a != NULL && w->b != NULL)
				cout << ",bomb";
			if (w->a == NULL && w->b == NULL && w->s != NULL)
				cout << "sword("<<w ->s ->atk<<")";
			if ((w -> a != NULL || w -> b != NULL)&& w -> s != NULL)
				cout << ",sword(" << w->s->atk << ")";
			cout << endl;
		}
	}
}; output Output;


class Headquarter   //司令部
{
public:
	int element; //生命元
	string name;//红蓝
	int prodnum;//生产过的武士数量
	int enemycnt;//到达司令部敌人的数量
	bool newcomer;//判断敌人是不是这回合来的
	warrior* newwar; //司令部的新武士
	warrior* enemy; //第一个到达司令部的敌人
	warrior* secenemy; //第二个到达司令部的敌人
	Headquarter(int n, string s) :element(n), name(s),prodnum(0),newwar(NULL),newcomer(false),enemycnt(0),enemy(NULL),secenemy(NULL){}
	void reset() //重置
	{ 	
		element = M; 
		prodnum = 0;
		newwar = NULL;
		newcomer = false;
		enemycnt = 0;
		enemy = NULL;
		secenemy = NULL;
	}  
	void produceWa(string warname, int _HP)
	{
		if (_HP <= element)   //如果足够生成武士
		{
			prodnum++;     //武士数量增加
			element -= _HP;  //生命元减少
			newwar = new warrior(prodnum, warname, name);
			Output.born(newwar);
		}
	}
	void givebirth()  //武士降生
	{
		newwar = NULL;//清零
			if (name == quartername[0])
			{
				switch (prodnum % 5)
				{
				case 0: produceWa(redorder[0], iHP); break;
				case 1: produceWa(redorder[1], lHP); break;
				case 2:produceWa(redorder[2], wHP); break;
				case 3:produceWa(redorder[3], nHP); break;
				case 4:produceWa(redorder[4], dHP); break;
				}
			}
			if (name ==quartername[1])
			{
				switch (prodnum % 5)
				{
				case 0: produceWa(blueorder[0], lHP); break;
				case 1: produceWa(blueorder[1], dHP); break;
				case 2:produceWa(blueorder[2], nHP); break;
				case 3:produceWa(blueorder[3], iHP); break;
				case 4:produceWa(blueorder[4], wHP); break;
				}
			}
		}

}; Headquarter red(M, quartername[0]), blue(M, quartername[1]);



warrior::warrior(int num, string _type, string _color) :warnum(num), type(_type), color(_color), dead(false), loyal(0), morale(0), s(NULL), a(NULL), b(NULL)
{
	if (_color == "red")		pos = 0;  //红魔军的司令部算作编号为0的城市
	if (_color == "blue")    pos = N + 1; //蓝魔军的司令部算作编号为N+1的城市

	if (_type == cinorder[0])   //dragon的初始化
	{
		int weaponNum = num % 3;
		HP = beforewarHP = dHP;
		atk = dA;
		if (color == quartername[0]) //其值为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量
			morale = red.element / double(dHP);
		if (color == quartername[1])
			morale = blue.element / double(dHP);
		switch (weaponNum)
		{
		case 0:
			if (atk > 4)
			s = new sword(atk);
			break;
		case 1: b = new bomb(); break;
		case 2: a = new arrow(); break;
		default:break;
		}
	}
	if (_type == cinorder[1])   //ninja的初始化
	{
		HP = beforewarHP = nHP;
		atk = nA;
		int weapon1 = num % 3;
		int weapon2 = (num + 1) % 3;
		switch (weapon1)
		{
		case 0:
			if (atk > 4)
				s = new sword(atk);
			 break;
		case 1: b = new bomb(); break;
		case 2: a = new arrow(); break;
		default:break;
		}
		switch (weapon2)
		{
		case 0:
			if (atk > 4)
				s = new sword(atk);
			 break;
		case 1: b = new bomb(); break;
		case 2: a = new arrow(); break;
		default:break;
		}
	}
	if (_type == cinorder[2])   //iceman的初始化
	{
		int weaponNum = num % 3;  //所得武器编号
		HP = beforewarHP = iHP;
		atk = iA;
		switch (weaponNum)
		{
		case 0: 
			if (atk > 4)
				s = new sword(atk);
			break;
		case 1: b = new bomb(); break;
		case 2: a = new arrow(); break;
		default:break;
		}
	}
	if (_type == "lion") //lion
	{
		HP = beforewarHP = lHP;
		atk = lA;
		if (_color == quartername[0])  //其初始值等于它降生之后其司令部剩余生命元的数目
			loyal = red.element;
		if (_color == quartername[1])
			loyal = blue.element;
	}
	if (_type == "wolf") //wolf降生时没有武器
	{
		HP = beforewarHP = wHP;
		atk = wA;
	}
}

void warrior::lionrunaway(city &c)   //该逃跑的lion就在这一时刻逃跑了
{
	if (loyal <= 0)
	{
		Output.runaway(this);
		if (color == "red")  c.redwar = NULL; //忠诚度降至0或0以下，则该lion逃离战场, 永远消失
		if (color == "blue") c.bluewar = NULL;
	}
}

void warrior::goForward()  //向前走
{
	/*for (int i = N; i >= 0; i--)  //先移动红的
	{
		if (City[i].redwar != NULL)
		{
			if (i == N) //红色武士差一步到蓝基地
			{
				if (blue.enemy == NULL)  //第一个到达蓝基地的红武士
				{
					blue.enemycnt++;
				blue.enemy = City[i].redwar;
				City[i + 1].redwar = City[i].redwar;
				City[i].redwar = NULL;
				}
				else
				{
					blue.enemycnt++;
					blue.secenemy = City[i].redwar;
					City[i].redwar = NULL;
				}
			}
			else
			{
				City[i + 1].redwar = City[i].redwar;  //往东走一步
				City[i].redwar = NULL;
			}
		}
	}
	for (int i = 1; i <= N + 1; i++)  //再移动蓝的
	{
		if (City[i].redwar != NULL)
		{
			if (i == 1) //蓝色武士差一步到蓝基地
			{
				red.enemycnt++;
				City[i].bluewar = NULL;  //到达司令部的消除掉
			}
			else
			{
				City[i - 1].bluewar = City[i].bluewar;  //往西走一步
				City[i].bluewar = NULL;
			}
		}
	}*/
	if (color == "red")  //向东走
	{
		if (pos == N) //再走一步到蓝基地
		{
			if (blue.enemy == NULL)  //第一个到达蓝基地的红武士
			{
				blue.enemycnt++;
				blue.enemy = City[pos].redwar;
				blue.newcomer = true;
				City[pos + 1].redwar = City[pos].redwar;
				City[pos].redwar = NULL;
			}
			else   //第二个
			{
				blue.enemycnt++;
				blue.secenemy = City[pos].redwar;
				City[pos].redwar = NULL;
			}
		}
		else
		{
			City[pos + 1].redwar = City[pos].redwar;
			City[pos].redwar = NULL;
		}
		pos++;
	}
	if (color == "blue")  //向西走
	{
		if (pos == 1) //再走一步到红基地
		{
			if (red.enemy == NULL)  //第一个到达红基地的蓝武士
			{
				red.enemycnt++;
				red.enemy = City[pos].bluewar;
				red.newcomer = true;
				City[pos - 1].bluewar = City[pos].bluewar;
				City[pos].bluewar = NULL;
			}
			else   //第二个
			{
				red.enemycnt++;
				red.secenemy = City[pos].bluewar;
				City[pos].bluewar = NULL;
			}
		}
		else
		{
			City[pos - 1].bluewar = City[pos].bluewar;
			City[pos].bluewar = NULL;
		}
		pos--;
	}
	movestep++;
	if (movestep % 2 == 0 && type == "iceman")  //iceman 每前进两步，在第2步完成的时候，生命值会减少9，攻击力会增加20
	{
		HP -= 9;
		atk += 20;
		if (HP <= 0) HP = 1; //但是若生命值减9后会小于等于0，则生命值不减9,而是变为1
	}
	if (red.enemycnt == 2 || blue.enemycnt == 2) warFin = true;  //任何一方的司令部里若是出现了2个敌人，则认为该司令部已被敌人占领

	//任何一方的司令部被敌人占领，则战争结束
}

void warrior::shoot(warrior* w)  //武士射箭
{
	a->usetime--;
	w->HP -= a->atk; //敌人受到伤害
	if (a->usetime == 0) a = NULL;  //arrow使用3次后即被耗尽，武士失去arrow
	Output.shoot(this, w);
}

void warrior::attack(warrior* w) //主动发起攻击
{
	w->HP -= atk;   //被攻击者生命值会减去进攻者的攻击力值和进攻者手中sword的攻击力值
	if (s != NULL)
	{
		w->HP -= s->atk;
		s->atk *= 0.8; //攻击力变为本次战斗前的80% (去尾取整)
		if (s->atk == 0)  s = NULL; //sword攻击力变为0时，视为武士失去了sword
	}
	if (w->HP <= 0)  w->dead = true;
}

void warrior::fightback(warrior* w) //被进攻者若没死，就会发起反击
{
 	w->HP -= atk / 2;
	if (s != NULL)
	{
		w->HP -= s->atk;
		s->atk *= 0.8; //攻击力变为本次战斗前的80% (去尾取整)
		if (s->atk == 0)  s = NULL; //sword攻击力变为0时，视为武士失去了sword
	}
	if (w->HP <= 0) w->dead = true;
}

int warrior::get_atk()  //算主动攻击造成的伤害
{
	if (s != NULL) return atk + s->atk;
	else return atk;
}

int warrior::get_back_atk() //算还击造成的伤害
{
	if (s != NULL) return atk /2 + s->atk;
	else return atk/2;
}

bool warrior::will_be_killed(warrior* w)  //是否将被杀
{
	if (w->color == "red" && (City[pos].flag == "red" || (City[pos].flag == "none" && pos % 2 == 1)) || (w -> color == "blue" && (City[pos].flag == "blue" || (City[pos].flag == "none" && pos % 2 == 0))))  //如果敌人主动攻击
		return w->get_atk() >= HP;
	if (get_atk() >= w->HP) //如果攻击力大于敌人血量
		return false;
	if (w->type == "ninja") return false; //ninja从不还手
	return (w->get_back_atk() >= HP);
}

void warrior::usebomb(warrior* w)  //使用炸弹
{
	Output.bomb(this, w);
	dead = true;
	w->dead = true;
}

void warrior::win(warrior* w)  //大概是武士获取生命元部分和旗帜升起
{
	if (w->type == "lion")	{  HP += w->beforewarHP; } //lion 若是战死，则其战斗前的生命值就会转移到对手身上

	if (type == "dragon")
	{
		morale += 0.2; //dragon每取得一次战斗的胜利(敌人被杀死)，士气就会增加0.2
		if ((color == "red" && (City[pos].flag == "red" || (City[pos].flag == "none" && pos % 2 == 1))) || (color == "blue" && (City[pos].flag == "blue" || (City[pos].flag == "none" && pos % 2 == 0)))) //dragon 在一次在它主动进攻的战斗结束后，如果还没有战死
		{
			if (morale > 0.8)  //并且士气大于0.8
				Output.yell(this);
		}
	}
	if (type == "wolf")  //俘获武器
	{
		if (a == NULL) //被缴获的武器当然不能算新的，已经被用到什么样了，就是什么样的
		{
			a = w->a;
			w->a = NULL;
		}
		if (b == NULL)
		{
			b = w->b;
			w->b = NULL;
		}
		if (s == NULL)
		{
			s = w->s;
			w->s = NULL;
		}
	}
	Output.getelements(this, City[pos].cityelem);
	if (City[pos].flag != "red" && color == "red" && City[pos].lastwinner == "red")//当某个城市有连续两场战斗都是同一方的武士杀死敌人,那么该城市就会插上胜方的旗帜
	{
		City[pos].flag = "red";
		Output.raiseflag(this);
	}
	if (City[pos].flag != "blue" && color == "blue" && City[pos].lastwinner == "blue")//当某个城市有连续两场战斗都是同一方的武士杀死敌人,那么该城市就会插上胜方的旗帜
	{
		City[pos].flag = "blue";
		Output.raiseflag(this);
	}
	if (color == "red")  City[pos].lastwinner = "red"; //更新本回合赢家
	if (color == "blue")  City[pos].lastwinner = "blue";
}

void warrior::draw(warrior* w)  //平局 只有龙和狮子的事
{
	if (type == "dragon")
	{
		morale -= 0.2; //dragon每取得一次战斗的胜利(敌人被杀死)，士气就会增加0.2
		if ((color == "red" && (City[pos].flag == "red" || (City[pos].flag == "none" && pos % 2 == 1))) || (color == "blue" && (City[pos].flag == "blue" || (City[pos].flag == "none" && pos % 2 == 0)))) //dragon 在一次在它主动进攻的战斗结束后，如果还没有战死
		{
			if (morale > 0.8)  //并且士气大于0.8
				Output.yell(this);
		}
	}
	if (type == "lion") //每经过一场未能杀死敌人的战斗，忠诚度就降低K
		loyal -= K;
	City[pos].lastwinner = "none"; //但如果中间有平局的战斗，就不算连续了
}

void city::fight()
{
	if (redwar->dead && bluewar->dead) return; //如果都死了（bomb情况）
	// 如果敌人在5分钟前已经被飞来的arrow射死，那么仍然视为发生了一场战斗，而且存活者视为获得了战斗的胜利
	//此情况下不会有“武士主动攻击”，“武士反击”，“武士战死”的事件发生，但战斗胜利后应该发生的事情都会发生
	//如Wolf一样能缴获武器，旗帜也可能更换
	if (!redwar->dead && bluewar->dead)  redwar->win(bluewar);  //蓝死红没死 红胜利
	else if (redwar->dead && !bluewar->dead)  bluewar->win(redwar);  //红死蓝没死 蓝胜利
	else  //两个都没死
	{
		if (flag == "red" || (flag == "none" && citynum % 2 == 1)) //在插红旗的城市，以及编号为奇数的无旗城市，由红武士主动发起进攻
		{
			Output.attack(redwar, bluewar);   
			redwar->attack(bluewar);
			if (bluewar->type == "ninja" && !bluewar->dead) //如果被打的是ninja 并且没有死 平局
			{
				redwar->draw(bluewar);
				bluewar->draw(redwar);
			}
			else if (bluewar -> type != "ninja" && !bluewar->dead)      //如果不是ninja红进攻后蓝没死蓝反击
			{
				Output.fightback(bluewar, redwar);
				bluewar -> fightback(redwar);    
				if (!redwar->dead)    //如果反击后红没死就平局
				{
					redwar->draw(bluewar);
					bluewar->draw(redwar);
				}
				else    //如果反击后红死了蓝胜利
				{
					Output.killed(redwar);
					bluewar->win(redwar);
				}
			}
			else   //如果红进攻后蓝死了红胜利
			{
				Output.killed(bluewar);
				redwar->win(bluewar);
			}
		}
		else//在插蓝旗的城市，以及编号为偶数的无旗城市，由蓝武士主动发起进攻
		{
			Output.attack(bluewar, redwar);
			bluewar->attack(redwar);
			if (redwar->type == "ninja" && !redwar->dead) //如果被打的是ninja 并且没有死 平局
			{
				redwar->draw(bluewar);
				bluewar->draw(redwar);
			}
			else if (redwar -> type != "ninja" && !redwar->dead)      //如果蓝进攻后红没死红反击
			{
				Output.fightback(redwar, bluewar);
				redwar->fightback(bluewar);
				if (!bluewar->dead)    //如果反击后蓝没死就平局
				{
					redwar->draw(bluewar);
					bluewar->draw(redwar);
				}
				else    //如果反击后蓝死了红胜利
				{
					Output.killed(bluewar);
					redwar->win(bluewar);
				}
			}
			else   //如果蓝进攻后红死了蓝胜利
			{
				Output.killed(redwar);
				bluewar->win(redwar);
			}
		}
		//redwar->beforewarHP = redwar->HP;   //更新战后生命值
		//bluewar->beforewarHP = bluewar->HP;
	}
}

void city::clearbody()  //清理尸体
{
	if (redwar != NULL && redwar->dead)  redwar = NULL;
	if (bluewar != NULL && bluewar->dead) bluewar = NULL;
}

void output::headelements(Headquarter &h)  //放在前面不认识
{
	warclock.printtime();
	cout << h.element << " elements in " << h.name << " headquarter" << endl;
}

int main()
{
	int t; //测试组数
	cin >> t;
	for (int i = 1; i <= t; i++)
	{
		warFin = false;
		cin >> M >> N >> R >> K >> T;
		cin >> dHP >> nHP >> iHP >> lHP >> wHP;
		cin >> dA >> nA >> iA >> lA >> wA;
		cout << "Case " << i << ":" << endl;
		red.reset();  //重置生命元
		blue.reset();
		warclock.reset(); //时钟重置
		if (City != NULL) delete[]City;  //清空指针
			City= new city[N + 2];
			for (int i = 0; i <= N + 1 ; i++)  //把城市编号初始化
				City[i].citynum = i;
		while (!warFin)
		{
			//1.武士降生
			red.givebirth(); 
			blue.givebirth();
			//2.lion逃跑
			warclock.minute += 5;  
			if (warclock.isFin()) break;   //如果到时间就结束
			for (int i = 1; i < N; i++)
			{
				if (City[i].redwar != NULL && City[i].redwar->type == "lion")  //如果在城市中找到红狮子 看它会不会逃走
					City[i].redwar->lionrunaway(City[i]);
				if (City[i].bluewar != NULL && City[i].bluewar->type == "lion")  //蓝
					City[i].bluewar->lionrunaway(City[i]);
			}
			City[0].redwar = red.newwar;  //0号城的红色武士即为红司令部的新武士
			City[N + 1].bluewar = blue.newwar; //蓝
			if (red.newwar != NULL && red.newwar->type == cinorder[3]) //如果在红司令部找到
				red.newwar->lionrunaway(City[0]);
			if (blue.newwar != NULL && blue.newwar->type == cinorder[3]) //蓝
				blue.newwar->lionrunaway(City[N + 1]);
			//3.武士前进到某一城市&& 12.武士抵达敌军司令部 && 13.司令部被占领 （同时）
			warclock.minute += 5;  //10分
			if (warclock.isFin()) break;
			for (int i = N; i >= 0; i--)  //先移动红的
				if (City[i].redwar != NULL)  City[i].redwar->goForward();

			for (int i = 1; i <= N + 1; i++)  //再移动蓝的
			if (City[i].bluewar != NULL)  City[i].bluewar->goForward();

			if (red.enemycnt == 1 && red.newcomer) //如果有新敌人走进红司令部
			{
				Output.hforward(red.enemy, "red");
				red.newcomer = false;
			}
			if (red.enemycnt == 2)  //如果第二个敌人到了红司令部
			{
				Output.hforward(red.secenemy, "red");
				Output.headtaken("red");
			}
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL) Output.cforward(City[i].redwar, i);  //城市部分的输出 先输出红的再蓝的
				if (City[i].bluewar != NULL) Output.cforward(City[i].bluewar, i);
			}
			if (blue.enemycnt == 1 && blue.newcomer) //如果有新敌人走进蓝司令部
			{
				Output.hforward(blue.enemy, "blue");
				blue.newcomer = false;
			}
			if (blue.enemycnt == 2)  //如果第二个敌人到了蓝司令部
			{
				Output.hforward(blue.secenemy, "blue");
				Output.headtaken("blue");
				break;
			}
			if (warFin) break;
			//城市产生10点生命元  20分
			warclock.minute += 10;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)
				City[i].cityelem += 10;
			//10.武士取走城市生命元 30分
			warclock.minute += 10;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)//如果某个城市中只有一个武士，那么该武士取走该城市中的所有生命元，并立即将这些生命元传送到其所属的司令部
			{
				if (City[i].redwar != NULL && City[i].bluewar == NULL) //只有红
				{
					red.element += City[i].cityelem;
					Output.getelements(City[i].redwar, City[i].cityelem);
					City[i].cityelem = 0;
				}
				if (City[i].redwar == NULL && City[i].bluewar != NULL)
				{
					blue.element += City[i].cityelem;
					Output.getelements(City[i].bluewar, City[i].cityelem);
					City[i].cityelem = 0;
				}
			}
			//4.武士放箭  35分
			warclock.minute += 5;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL && City[i].redwar->a != NULL && i != N) //红武士有箭且不是最后
				{
					if (City[i + 1].bluewar != NULL)
						City[i].redwar->shoot(City[i + 1].bluewar);
				}
				if (City[i].bluewar != NULL && City[i].bluewar->a != NULL && i != 1)//蓝有箭
				{
					if (City[i - 1].redwar != NULL)
						City[i].bluewar->shoot(City[i - 1].redwar);
				}
			}
			//5.武士使用BOMB  38分
			warclock.minute += 3;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL && City[i].redwar->b != NULL && City[i].bluewar != NULL && !City[i].redwar -> dead && !City[i].bluewar -> dead) //如果一个城市有活着的红蓝武士 并且红武士有炸弹
				{
					if (City[i].redwar->will_be_killed(City[i].bluewar))  //如果红武士将要被杀就使用炸弹
						City[i].redwar->usebomb(City[i].bluewar);
				}
				if (City[i].bluewar != NULL && City[i].bluewar->b != NULL && City[i].redwar != NULL && !City[i].redwar->dead && !City[i].bluewar->dead) //如果一个城市有活着的红蓝武士 并且蓝武士有炸弹
				{
					if (City[i].bluewar->will_be_killed(City[i].redwar))  //如果蓝武士将要被杀就使用炸弹
						City[i].bluewar->usebomb(City[i].redwar);
				}
			}
			//6.武士进攻 && 7.武士反击 && 8.武士战死 &&  9. 武士欢呼 && 10.武士获取生命元 && 11.旗帜升起  40分
			warclock.minute += 2;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)  //战前更新血量
			{
				if (City[i].redwar != NULL && !City[i].redwar->dead)
					City[i].redwar->beforewarHP = City[i].redwar->HP;
				if (City[i].redwar != NULL && City[i].redwar->dead)
					City[i].redwar->beforewarHP = 0;
				if (City[i].bluewar != NULL && !City[i].bluewar->dead)
					City[i].bluewar->beforewarHP = City[i].bluewar->HP;
				if (City[i].bluewar != NULL && City[i].bluewar->dead)
					City[i].bluewar->beforewarHP = 0;
			}
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL && City[i].bluewar != NULL)
					City[i].fight();
			}
			//战后奖励清理工作

			//如果武士在战斗中杀死敌人则其司令部会立即向其发送8个生命元作为奖励，使其生命值增加8
			//当然前提是司令部得有8个生命元。如果司令部的生命元不足以奖励所有的武士，则优先奖励距离敌方司令部近的武士
			for (int i = 1; i <= N; i++)  //红
			{
				if (red.element < 8) break;
				if (City[i].redwar != NULL && !City[i].redwar->dead && City[i].bluewar != NULL && City[i].bluewar->dead)
				{
						red.element -= 8;
						City[i].redwar->HP += 8;
				}
			}
			for (int i = N; i > 0; i--)//蓝
			{
				if (blue.element < 8) break;
				if (City[i].redwar != NULL && City[i].redwar->dead && City[i].bluewar != NULL && !City[i].bluewar->dead)
				{
					blue.element -= 8;
					City[i].bluewar->HP += 8;
				}
			}
			//如果某武士在某城市的战斗中杀死了敌人，则该武士的司令部立即取得该城市中所有的生命元
			//司令部总是先完成全部奖励工作，然后才开始从各个打了胜仗的城市回收生命元
			for (int i = 1; i <= N; i++)  
			{
				if (City[i].redwar != NULL && !City[i].redwar->dead && City[i].bluewar != NULL && City[i].bluewar->dead) //红
				{
					red.element += City[i].cityelem;
					City[i].cityelem = 0;
				}
				if (City[i].redwar != NULL && City[i].redwar->dead && City[i].bluewar != NULL && !City[i].bluewar->dead)
				{
					blue.element += City[i].cityelem;
					City[i].cityelem = 0;
				}
			}
			//清理尸体
			for (int i = 1; i <= N; i++) 
				City[i].clearbody(); 
			//14.司令部报告生命元数量  50分
			warclock.minute += 10;
			if (warclock.isFin()) break;
			Output.headelements(red);
			Output.headelements(blue);
			//15.武士报告武器情况  55分
			warclock.minute += 5;
			if (warclock.isFin()) break;
			//先按从西向东的顺序所有的红武士报告，然后再从西向东所有的蓝武士报告
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL)
					Output.weapon(City[i].redwar);
			}
			if (blue.enemy != NULL) Output.weapon(blue.enemy); //蓝司令部的红武士
			if (red.enemy != NULL) Output.weapon(red.enemy);  //红司令部的兰武士
			for (int i = 1; i <= N; i++)
			{
				if (City[i].bluewar != NULL)
					Output.weapon(City[i].bluewar);
			}
			//最后再续5分钟
			warclock.minute += 5;
			if (warclock.isFin()) break;
			warclock.addhour();
			}
		}
	return 0;
}
