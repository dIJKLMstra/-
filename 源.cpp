#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
int M, N, R, K, T;
int dHP, nHP, iHP, lHP, wHP; //��ʼ����ֵ
int dA, nA, iA, lA, wA; //������
string redorder[5] = { "iceman","lion","wolf","ninja","dragon" }; //��ʿһ��������
string blueorder[5] = { "lion","dragon","ninja","iceman","wolf" };
string cinorder[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string quartername[2] = { "red", "blue" };
bool warFin ; //ս������

class city;
class Headquarter;
class wartime //ʱ��
{
public:
	int hour;   //��¼Сʱ
	int minute; //��¼����
	wartime(int _h, int _m) :hour(_h), minute(_m){}
	void addhour()  //��λ
	{
		if (minute >= 60)
		{
			minute -= 60;
			hour++;
		}
	}
	void printtime()       //ÿ�ζ�Ҫ�����ʱ��
	{
		cout << setw(3) << setfill('0') << hour << ":";
		cout << setw(2) << setfill('0') << minute << " ";
	}
	bool isFin()  //�ж��Ƿ�ﵽ����ʱ��
	{
		if (hour * 60 + minute > T) return true;
		else  return false;
	}
	void reset()  //����
	{
		hour = minute = 0;
	}
}; wartime warclock(0, 0);


class sword //��
{
public:
	int atk;
	sword(int _waratk):atk(_waratk/5){} //��ʼֵΪӵ��������ʿ�Ĺ�������20%
};

class arrow //��
{
public:
	int atk;
	int usetime; //�;ö�
	arrow() :atk(R),usetime(3){}  //arrow��һ��������ֵR
};

class bomb //ը��
{
public:
	bool usebomb;  //�Ƿ�ʹ��bomb
	bomb() :usebomb(false){}
};

class warrior //��ʿ
{
public:	
	sword* s;  //�Ƿ�������
	arrow* a;
	bomb* b;
	int loyal; //�ҳ϶�
	int warnum;//��ʿ���
	int atk; //������
	int HP; //����ֵ
	int pos;//���ڳ��б��
	int movestep;//Ϊѩ��׼�����ƶ�����
	int beforewarHP;//Ϊʨ��׼����սǰѪ��
	double morale; //ʿ��
	string type; //����
	bool dead; //�Ƿ�����
	string color; //�컹����

	//�ó��Ĺ��캯������
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

class city //����
{
public:
	int citynum; //���б��
	int cityelem; //��������Ԫ
	warrior* redwar; //�Ƿ��к�ɫ��ʿ
	warrior* bluewar;//��ɫ��ʿ
	string lastwinner;//��һ����Ӯ��
	string flag;//������ɫ
	city() :citynum(0), cityelem(0), redwar(NULL), bluewar(NULL),lastwinner("none"),flag("none"){}
	void fight(); // ����ս������
	void clearbody(); //���ʬ��
}; city *City;

class output  //����������
{
public:
	//��ʿ�����������
	void born(warrior* w)  
	{
		warclock.printtime();
		cout << w -> color << " " << w -> type << " " << w -> warnum << " born" << endl;
		if (w->type == "dragon")   //���dragon
			cout << "Its morale is " << setiosflags(ios::fixed)<<setprecision(2)<<w->morale<<endl;//�������뵽С�������λ
		if (w->type == cinorder[3])    //���lion
			cout << "Its loyalty is " << w->loyal << endl;
	}
	//lion���ܲ������
	void runaway(warrior* w) 
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " ran away" << endl;
	}
	//��ʿǰ���������
	void cforward(warrior* w,int citynum) //�ִ����
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " marched to city " << citynum << " with " << w->HP << " elements and force " << w->atk<<endl;
	}
	void hforward(warrior* w, string color)//�ִ�˾�
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " reached " << color << " headquarter with " << w->HP << " elements and force " << w->atk << endl;
	}
	void headtaken(string color) //˾���ռ��
	{
		warclock.printtime();
		cout << color << " headquarter was taken" << endl;
	}
	//��ʿ��ȡ����Ԫ�������
	void getelements(warrior* w,int elements)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " earned " << elements << " elements for his headquarter" << endl;
	}
	//��ʿ�ż��������
	void shoot(warrior* w1, warrior* w2)
	{
		if (w2->HP <= 0)  //���ɱ������
		{
			w2->dead = true;
			warclock.printtime();
			cout << w1->color << " " << w1->type << " " << w1->warnum << " shot and killed "<<w2 ->color<<" "<<w2 ->type<<" "<<w2 -> warnum<<endl;
		}
		else
		{
			warclock.printtime(); //δɱ��
			cout << w1->color << " " << w1->type << " " << w1->warnum << " shot" << endl;
		}
	
	}
	//��ʿʹ��bomb�������
	void bomb(warrior* w1,warrior* w2)
	{
		warclock.printtime();
		cout << w1->color << " " << w1->type << " " << w1->warnum << " used a bomb and killed " << w2->color << " " << w2->type << " " << w2->warnum << endl;
	}
	//��ʿ���������������
	void attack(warrior* w1,warrior* w2)
	{
		warclock.printtime();
		cout << w1->color << " " << w1->type << " " << w1->warnum << " attacked " << w2->color << " " << w2->type << " " << w2->warnum << " in city " << w1->pos << " with " << w1->HP << " elements and force " << w1->atk<< endl;
	}
	//��ʿ�����������
	void fightback(warrior*w1, warrior* w2)
	{
		warclock.printtime();
		cout << w1->color << " " << w1->type << " " << w1->warnum << " fought back against " << w2->color << " " << w2->type << " " << w2->warnum << " in city " << w1->pos << endl;
	}
	//��ʿս���������
	void killed(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " was killed in city " << w->pos << endl;
	}
	//��ʿ�����������
	void yell(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " yelled in city " << w->pos << endl;
	}
	//�������𲿷����
	void raiseflag(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " flag raised in city " << w->pos << endl;
	}
	//˾���������Ԫ�����������
	void headelements(Headquarter &h);
	//�������沿�����
	void weapon(warrior* w)
	{
		warclock.printtime();
		cout << w->color << " " << w->type << " " << w->warnum << " has ";
		if (w->a == NULL && w->b == NULL && w->s == NULL)  //���û����
			cout << "no weapon" << endl;
		else  //���������
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


class Headquarter   //˾�
{
public:
	int element; //����Ԫ
	string name;//����
	int prodnum;//����������ʿ����
	int enemycnt;//����˾����˵�����
	bool newcomer;//�жϵ����ǲ�����غ�����
	warrior* newwar; //˾�������ʿ
	warrior* enemy; //��һ������˾��ĵ���
	warrior* secenemy; //�ڶ�������˾��ĵ���
	Headquarter(int n, string s) :element(n), name(s),prodnum(0),newwar(NULL),newcomer(false),enemycnt(0),enemy(NULL),secenemy(NULL){}
	void reset() //����
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
		if (_HP <= element)   //����㹻������ʿ
		{
			prodnum++;     //��ʿ��������
			element -= _HP;  //����Ԫ����
			newwar = new warrior(prodnum, warname, name);
			Output.born(newwar);
		}
	}
	void givebirth()  //��ʿ����
	{
		newwar = NULL;//����
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
	if (_color == "red")		pos = 0;  //��ħ����˾��������Ϊ0�ĳ���
	if (_color == "blue")    pos = N + 1; //��ħ����˾��������ΪN+1�ĳ���

	if (_type == cinorder[0])   //dragon�ĳ�ʼ��
	{
		int weaponNum = num % 3;
		HP = beforewarHP = dHP;
		atk = dA;
		if (color == quartername[0]) //��ֵΪ����������˾�ʣ������Ԫ������������dragon���������Ԫ����
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
	if (_type == cinorder[1])   //ninja�ĳ�ʼ��
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
	if (_type == cinorder[2])   //iceman�ĳ�ʼ��
	{
		int weaponNum = num % 3;  //�����������
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
		if (_color == quartername[0])  //���ʼֵ����������֮����˾�ʣ������Ԫ����Ŀ
			loyal = red.element;
		if (_color == quartername[1])
			loyal = blue.element;
	}
	if (_type == "wolf") //wolf����ʱû������
	{
		HP = beforewarHP = wHP;
		atk = wA;
	}
}

void warrior::lionrunaway(city &c)   //�����ܵ�lion������һʱ��������
{
	if (loyal <= 0)
	{
		Output.runaway(this);
		if (color == "red")  c.redwar = NULL; //�ҳ϶Ƚ���0��0���£����lion����ս��, ��Զ��ʧ
		if (color == "blue") c.bluewar = NULL;
	}
}

void warrior::goForward()  //��ǰ��
{
	/*for (int i = N; i >= 0; i--)  //���ƶ����
	{
		if (City[i].redwar != NULL)
		{
			if (i == N) //��ɫ��ʿ��һ����������
			{
				if (blue.enemy == NULL)  //��һ�����������صĺ���ʿ
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
				City[i + 1].redwar = City[i].redwar;  //������һ��
				City[i].redwar = NULL;
			}
		}
	}
	for (int i = 1; i <= N + 1; i++)  //���ƶ�����
	{
		if (City[i].redwar != NULL)
		{
			if (i == 1) //��ɫ��ʿ��һ����������
			{
				red.enemycnt++;
				City[i].bluewar = NULL;  //����˾���������
			}
			else
			{
				City[i - 1].bluewar = City[i].bluewar;  //������һ��
				City[i].bluewar = NULL;
			}
		}
	}*/
	if (color == "red")  //����
	{
		if (pos == N) //����һ����������
		{
			if (blue.enemy == NULL)  //��һ�����������صĺ���ʿ
			{
				blue.enemycnt++;
				blue.enemy = City[pos].redwar;
				blue.newcomer = true;
				City[pos + 1].redwar = City[pos].redwar;
				City[pos].redwar = NULL;
			}
			else   //�ڶ���
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
	if (color == "blue")  //������
	{
		if (pos == 1) //����һ���������
		{
			if (red.enemy == NULL)  //��һ���������ص�����ʿ
			{
				red.enemycnt++;
				red.enemy = City[pos].bluewar;
				red.newcomer = true;
				City[pos - 1].bluewar = City[pos].bluewar;
				City[pos].bluewar = NULL;
			}
			else   //�ڶ���
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
	if (movestep % 2 == 0 && type == "iceman")  //iceman ÿǰ���������ڵ�2����ɵ�ʱ������ֵ�����9��������������20
	{
		HP -= 9;
		atk += 20;
		if (HP <= 0) HP = 1; //����������ֵ��9���С�ڵ���0��������ֵ����9,���Ǳ�Ϊ1
	}
	if (red.enemycnt == 2 || blue.enemycnt == 2) warFin = true;  //�κ�һ����˾������ǳ�����2�����ˣ�����Ϊ��˾��ѱ�����ռ��

	//�κ�һ����˾�������ռ�죬��ս������
}

void warrior::shoot(warrior* w)  //��ʿ���
{
	a->usetime--;
	w->HP -= a->atk; //�����ܵ��˺�
	if (a->usetime == 0) a = NULL;  //arrowʹ��3�κ󼴱��ľ�����ʿʧȥarrow
	Output.shoot(this, w);
}

void warrior::attack(warrior* w) //�������𹥻�
{
	w->HP -= atk;   //������������ֵ���ȥ�����ߵĹ�����ֵ�ͽ���������sword�Ĺ�����ֵ
	if (s != NULL)
	{
		w->HP -= s->atk;
		s->atk *= 0.8; //��������Ϊ����ս��ǰ��80% (ȥβȡ��)
		if (s->atk == 0)  s = NULL; //sword��������Ϊ0ʱ����Ϊ��ʿʧȥ��sword
	}
	if (w->HP <= 0)  w->dead = true;
}

void warrior::fightback(warrior* w) //����������û�����ͻᷢ�𷴻�
{
 	w->HP -= atk / 2;
	if (s != NULL)
	{
		w->HP -= s->atk;
		s->atk *= 0.8; //��������Ϊ����ս��ǰ��80% (ȥβȡ��)
		if (s->atk == 0)  s = NULL; //sword��������Ϊ0ʱ����Ϊ��ʿʧȥ��sword
	}
	if (w->HP <= 0) w->dead = true;
}

int warrior::get_atk()  //������������ɵ��˺�
{
	if (s != NULL) return atk + s->atk;
	else return atk;
}

int warrior::get_back_atk() //�㻹����ɵ��˺�
{
	if (s != NULL) return atk /2 + s->atk;
	else return atk/2;
}

bool warrior::will_be_killed(warrior* w)  //�Ƿ񽫱�ɱ
{
	if (w->color == "red" && (City[pos].flag == "red" || (City[pos].flag == "none" && pos % 2 == 1)) || (w -> color == "blue" && (City[pos].flag == "blue" || (City[pos].flag == "none" && pos % 2 == 0))))  //���������������
		return w->get_atk() >= HP;
	if (get_atk() >= w->HP) //������������ڵ���Ѫ��
		return false;
	if (w->type == "ninja") return false; //ninja�Ӳ�����
	return (w->get_back_atk() >= HP);
}

void warrior::usebomb(warrior* w)  //ʹ��ը��
{
	Output.bomb(this, w);
	dead = true;
	w->dead = true;
}

void warrior::win(warrior* w)  //�������ʿ��ȡ����Ԫ���ֺ���������
{
	if (w->type == "lion")	{  HP += w->beforewarHP; } //lion ����ս��������ս��ǰ������ֵ�ͻ�ת�Ƶ���������

	if (type == "dragon")
	{
		morale += 0.2; //dragonÿȡ��һ��ս����ʤ��(���˱�ɱ��)��ʿ���ͻ�����0.2
		if ((color == "red" && (City[pos].flag == "red" || (City[pos].flag == "none" && pos % 2 == 1))) || (color == "blue" && (City[pos].flag == "blue" || (City[pos].flag == "none" && pos % 2 == 0)))) //dragon ��һ����������������ս�������������û��ս��
		{
			if (morale > 0.8)  //����ʿ������0.8
				Output.yell(this);
		}
	}
	if (type == "wolf")  //��������
	{
		if (a == NULL) //���ɻ��������Ȼ�������µģ��Ѿ����õ�ʲô���ˣ�����ʲô����
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
	if (City[pos].flag != "red" && color == "red" && City[pos].lastwinner == "red")//��ĳ����������������ս������ͬһ������ʿɱ������,��ô�ó��оͻ����ʤ��������
	{
		City[pos].flag = "red";
		Output.raiseflag(this);
	}
	if (City[pos].flag != "blue" && color == "blue" && City[pos].lastwinner == "blue")//��ĳ����������������ս������ͬһ������ʿɱ������,��ô�ó��оͻ����ʤ��������
	{
		City[pos].flag = "blue";
		Output.raiseflag(this);
	}
	if (color == "red")  City[pos].lastwinner = "red"; //���±��غ�Ӯ��
	if (color == "blue")  City[pos].lastwinner = "blue";
}

void warrior::draw(warrior* w)  //ƽ�� ֻ������ʨ�ӵ���
{
	if (type == "dragon")
	{
		morale -= 0.2; //dragonÿȡ��һ��ս����ʤ��(���˱�ɱ��)��ʿ���ͻ�����0.2
		if ((color == "red" && (City[pos].flag == "red" || (City[pos].flag == "none" && pos % 2 == 1))) || (color == "blue" && (City[pos].flag == "blue" || (City[pos].flag == "none" && pos % 2 == 0)))) //dragon ��һ����������������ս�������������û��ս��
		{
			if (morale > 0.8)  //����ʿ������0.8
				Output.yell(this);
		}
	}
	if (type == "lion") //ÿ����һ��δ��ɱ�����˵�ս�����ҳ϶Ⱦͽ���K
		loyal -= K;
	City[pos].lastwinner = "none"; //������м���ƽ�ֵ�ս�����Ͳ���������
}

void city::fight()
{
	if (redwar->dead && bluewar->dead) return; //��������ˣ�bomb�����
	// ���������5����ǰ�Ѿ���������arrow��������ô��Ȼ��Ϊ������һ��ս�������Ҵ������Ϊ�����ս����ʤ��
	//������²����С���ʿ����������������ʿ������������ʿս�������¼���������ս��ʤ����Ӧ�÷��������鶼�ᷢ��
	//��Wolfһ���ܽɻ�����������Ҳ���ܸ���
	if (!redwar->dead && bluewar->dead)  redwar->win(bluewar);  //������û�� ��ʤ��
	else if (redwar->dead && !bluewar->dead)  bluewar->win(redwar);  //������û�� ��ʤ��
	else  //������û��
	{
		if (flag == "red" || (flag == "none" && citynum % 2 == 1)) //�ڲ����ĳ��У��Լ����Ϊ������������У��ɺ���ʿ�����������
		{
			Output.attack(redwar, bluewar);   
			redwar->attack(bluewar);
			if (bluewar->type == "ninja" && !bluewar->dead) //����������ninja ����û���� ƽ��
			{
				redwar->draw(bluewar);
				bluewar->draw(redwar);
			}
			else if (bluewar -> type != "ninja" && !bluewar->dead)      //�������ninja���������û��������
			{
				Output.fightback(bluewar, redwar);
				bluewar -> fightback(redwar);    
				if (!redwar->dead)    //����������û����ƽ��
				{
					redwar->draw(bluewar);
					bluewar->draw(redwar);
				}
				else    //����������������ʤ��
				{
					Output.killed(redwar);
					bluewar->win(redwar);
				}
			}
			else   //���������������˺�ʤ��
			{
				Output.killed(bluewar);
				redwar->win(bluewar);
			}
		}
		else//�ڲ�����ĳ��У��Լ����Ϊż����������У�������ʿ�����������
		{
			Output.attack(bluewar, redwar);
			bluewar->attack(redwar);
			if (redwar->type == "ninja" && !redwar->dead) //����������ninja ����û���� ƽ��
			{
				redwar->draw(bluewar);
				bluewar->draw(redwar);
			}
			else if (redwar -> type != "ninja" && !redwar->dead)      //������������û���췴��
			{
				Output.fightback(redwar, bluewar);
				redwar->fightback(bluewar);
				if (!bluewar->dead)    //�����������û����ƽ��
				{
					redwar->draw(bluewar);
					bluewar->draw(redwar);
				}
				else    //��������������˺�ʤ��
				{
					Output.killed(bluewar);
					redwar->win(bluewar);
				}
			}
			else   //������������������ʤ��
			{
				Output.killed(redwar);
				bluewar->win(redwar);
			}
		}
		//redwar->beforewarHP = redwar->HP;   //����ս������ֵ
		//bluewar->beforewarHP = bluewar->HP;
	}
}

void city::clearbody()  //����ʬ��
{
	if (redwar != NULL && redwar->dead)  redwar = NULL;
	if (bluewar != NULL && bluewar->dead) bluewar = NULL;
}

void output::headelements(Headquarter &h)  //����ǰ�治��ʶ
{
	warclock.printtime();
	cout << h.element << " elements in " << h.name << " headquarter" << endl;
}

int main()
{
	int t; //��������
	cin >> t;
	for (int i = 1; i <= t; i++)
	{
		warFin = false;
		cin >> M >> N >> R >> K >> T;
		cin >> dHP >> nHP >> iHP >> lHP >> wHP;
		cin >> dA >> nA >> iA >> lA >> wA;
		cout << "Case " << i << ":" << endl;
		red.reset();  //��������Ԫ
		blue.reset();
		warclock.reset(); //ʱ������
		if (City != NULL) delete[]City;  //���ָ��
			City= new city[N + 2];
			for (int i = 0; i <= N + 1 ; i++)  //�ѳ��б�ų�ʼ��
				City[i].citynum = i;
		while (!warFin)
		{
			//1.��ʿ����
			red.givebirth(); 
			blue.givebirth();
			//2.lion����
			warclock.minute += 5;  
			if (warclock.isFin()) break;   //�����ʱ��ͽ���
			for (int i = 1; i < N; i++)
			{
				if (City[i].redwar != NULL && City[i].redwar->type == "lion")  //����ڳ������ҵ���ʨ�� �����᲻������
					City[i].redwar->lionrunaway(City[i]);
				if (City[i].bluewar != NULL && City[i].bluewar->type == "lion")  //��
					City[i].bluewar->lionrunaway(City[i]);
			}
			City[0].redwar = red.newwar;  //0�ųǵĺ�ɫ��ʿ��Ϊ��˾�������ʿ
			City[N + 1].bluewar = blue.newwar; //��
			if (red.newwar != NULL && red.newwar->type == cinorder[3]) //����ں�˾��ҵ�
				red.newwar->lionrunaway(City[0]);
			if (blue.newwar != NULL && blue.newwar->type == cinorder[3]) //��
				blue.newwar->lionrunaway(City[N + 1]);
			//3.��ʿǰ����ĳһ����&& 12.��ʿ�ִ�о�˾� && 13.˾���ռ�� ��ͬʱ��
			warclock.minute += 5;  //10��
			if (warclock.isFin()) break;
			for (int i = N; i >= 0; i--)  //���ƶ����
				if (City[i].redwar != NULL)  City[i].redwar->goForward();

			for (int i = 1; i <= N + 1; i++)  //���ƶ�����
			if (City[i].bluewar != NULL)  City[i].bluewar->goForward();

			if (red.enemycnt == 1 && red.newcomer) //������µ����߽���˾�
			{
				Output.hforward(red.enemy, "red");
				red.newcomer = false;
			}
			if (red.enemycnt == 2)  //����ڶ������˵��˺�˾�
			{
				Output.hforward(red.secenemy, "red");
				Output.headtaken("red");
			}
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL) Output.cforward(City[i].redwar, i);  //���в��ֵ���� ��������������
				if (City[i].bluewar != NULL) Output.cforward(City[i].bluewar, i);
			}
			if (blue.enemycnt == 1 && blue.newcomer) //������µ����߽���˾�
			{
				Output.hforward(blue.enemy, "blue");
				blue.newcomer = false;
			}
			if (blue.enemycnt == 2)  //����ڶ������˵�����˾�
			{
				Output.hforward(blue.secenemy, "blue");
				Output.headtaken("blue");
				break;
			}
			if (warFin) break;
			//���в���10������Ԫ  20��
			warclock.minute += 10;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)
				City[i].cityelem += 10;
			//10.��ʿȡ�߳�������Ԫ 30��
			warclock.minute += 10;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)//���ĳ��������ֻ��һ����ʿ����ô����ʿȡ�߸ó����е���������Ԫ������������Щ����Ԫ���͵���������˾�
			{
				if (City[i].redwar != NULL && City[i].bluewar == NULL) //ֻ�к�
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
			//4.��ʿ�ż�  35��
			warclock.minute += 5;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL && City[i].redwar->a != NULL && i != N) //����ʿ�м��Ҳ������
				{
					if (City[i + 1].bluewar != NULL)
						City[i].redwar->shoot(City[i + 1].bluewar);
				}
				if (City[i].bluewar != NULL && City[i].bluewar->a != NULL && i != 1)//���м�
				{
					if (City[i - 1].redwar != NULL)
						City[i].bluewar->shoot(City[i - 1].redwar);
				}
			}
			//5.��ʿʹ��BOMB  38��
			warclock.minute += 3;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL && City[i].redwar->b != NULL && City[i].bluewar != NULL && !City[i].redwar -> dead && !City[i].bluewar -> dead) //���һ�������л��ŵĺ�����ʿ ���Һ���ʿ��ը��
				{
					if (City[i].redwar->will_be_killed(City[i].bluewar))  //�������ʿ��Ҫ��ɱ��ʹ��ը��
						City[i].redwar->usebomb(City[i].bluewar);
				}
				if (City[i].bluewar != NULL && City[i].bluewar->b != NULL && City[i].redwar != NULL && !City[i].redwar->dead && !City[i].bluewar->dead) //���һ�������л��ŵĺ�����ʿ ��������ʿ��ը��
				{
					if (City[i].bluewar->will_be_killed(City[i].redwar))  //�������ʿ��Ҫ��ɱ��ʹ��ը��
						City[i].bluewar->usebomb(City[i].redwar);
				}
			}
			//6.��ʿ���� && 7.��ʿ���� && 8.��ʿս�� &&  9. ��ʿ���� && 10.��ʿ��ȡ����Ԫ && 11.��������  40��
			warclock.minute += 2;
			if (warclock.isFin()) break;
			for (int i = 1; i <= N; i++)  //սǰ����Ѫ��
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
			//ս����������

			//�����ʿ��ս����ɱ����������˾����������䷢��8������Ԫ��Ϊ������ʹ������ֵ����8
			//��Ȼǰ����˾�����8������Ԫ�����˾�������Ԫ�����Խ������е���ʿ�������Ƚ�������з�˾�������ʿ
			for (int i = 1; i <= N; i++)  //��
			{
				if (red.element < 8) break;
				if (City[i].redwar != NULL && !City[i].redwar->dead && City[i].bluewar != NULL && City[i].bluewar->dead)
				{
						red.element -= 8;
						City[i].redwar->HP += 8;
				}
			}
			for (int i = N; i > 0; i--)//��
			{
				if (blue.element < 8) break;
				if (City[i].redwar != NULL && City[i].redwar->dead && City[i].bluewar != NULL && !City[i].bluewar->dead)
				{
					blue.element -= 8;
					City[i].bluewar->HP += 8;
				}
			}
			//���ĳ��ʿ��ĳ���е�ս����ɱ���˵��ˣ������ʿ��˾�����ȡ�øó��������е�����Ԫ
			//˾����������ȫ������������Ȼ��ſ�ʼ�Ӹ�������ʤ�̵ĳ��л�������Ԫ
			for (int i = 1; i <= N; i++)  
			{
				if (City[i].redwar != NULL && !City[i].redwar->dead && City[i].bluewar != NULL && City[i].bluewar->dead) //��
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
			//����ʬ��
			for (int i = 1; i <= N; i++) 
				City[i].clearbody(); 
			//14.˾���������Ԫ����  50��
			warclock.minute += 10;
			if (warclock.isFin()) break;
			Output.headelements(red);
			Output.headelements(blue);
			//15.��ʿ�����������  55��
			warclock.minute += 5;
			if (warclock.isFin()) break;
			//�Ȱ������򶫵�˳�����еĺ���ʿ���棬Ȼ���ٴ��������е�����ʿ����
			for (int i = 1; i <= N; i++)
			{
				if (City[i].redwar != NULL)
					Output.weapon(City[i].redwar);
			}
			if (blue.enemy != NULL) Output.weapon(blue.enemy); //��˾��ĺ���ʿ
			if (red.enemy != NULL) Output.weapon(red.enemy);  //��˾�������ʿ
			for (int i = 1; i <= N; i++)
			{
				if (City[i].bluewar != NULL)
					Output.weapon(City[i].bluewar);
			}
			//�������5����
			warclock.minute += 5;
			if (warclock.isFin()) break;
			warclock.addhour();
			}
		}
	return 0;
}
