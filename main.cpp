#define DEBUG 1
#include<iostream>
#include<cstdlib>
#include<string>
#include "Map.hpp"
#include "player.hpp"
#include "monster.hpp"
#include "item.hpp"
#include "bag.hpp"
#include "equip.hpp"
#define up 1
#define down 7
#define left 3
#define right 5
#define here 4
#define LeftUp 0
#define LeftDown 6
#define RightUp 2
#define RightDown 8
#define pick 666
#define ATTACK 12580
using std::cout;
using std::endl;
using std::cin;
using std::string;

void attack(player &pl, Monster* mo, int way, int dir);
void MonMoveInmain(Monster *Mon,int MMS,player &p1);
void playerdie(player p1);
void wieldEquip(player p1,Equip* tempE);
int main(){
	Map map1;
	string p;
	cout << "Please enter your name\n";
	cin >> p;
	player p1(p);
	Bag bag1;
	map1.playerMove(p1.getPoi(),p1.getPoj(),here);
	map1.printMap();//初始化
	char act = '.';
	int act2 = 0;
	int dir;
	int statusPlayerMove;
	int statusMonMove1;
	int statusMonMove2;
	int statusMonMove3;
	int statusMon1 = 0;
	int statusMon2 = 0;
	int statusMon3 = 0;
	int addMonster;
	Monster *Mon1 = NULL;
	Monster *Mon2 = NULL;
	Monster *Mon3 = NULL;
	int itemNum = 0;
	item* headI = new item(p1.getPoi(),p1.getPoj());
	itemNum++;
	item* tempI = headI;
	map1.ItemAdd(headI);
	int EquipNum = 0;
	Equip* wear = NULL;
	Equip* headE = new Equip(p1.getPoi(),p1.getPoj());
	EquipNum++;
	Equip* tempE = headE;
	map1.EquipAdd(headE);
	while(act != '0'){
		if(act == 'h')
			dir = left;
		else if(act == 'j')
			dir = down;
		else if(act == 'k')
			dir = up;
		else if(act == 'l')
			dir = right;
		else if(act == '.')
			dir = here;
		else if(act == 'y')
			dir = LeftUp;
		else if(act == 'u'){
			dir = RightUp;
		}
		else if(act == 'b'){
			dir = LeftDown;
		}
		else if(act == 'n'){
			dir = RightDown;
		}
		else if(act == ','){
			dir = here;
			tempI = headI;
			while((tempI->getNext() != NULL)&&(tempI != NULL)){
				if(map1.PickItem(p1.getPoi(),p1.getPoj(),tempI) == 1){
					//加到背包里
					bag1 += *tempI;
					//tempI->BePicked();
					cout<<"you pick a item"<<tempI->getName()<<" and add it to the bag"<<endl;
					break;
				}
				tempI = tempI->getNext();
			}
			tempE = headE;
			while(tempE->getNext() != NULL){
				if(map1.PickEquip(p1.getPoi(),p1.getPoj(),tempE) == 1){
					bag1 += *tempE;
					cout <<"you pick an equpment: "<<tempE->getName()<< "and add it to"<<endl;
					break;
				}
				tempE = tempE->getNext();
			}
		}
		else if(act == 'w'){
			cout<<bag1.showEquips()<<"What do you want to equip? Please enter the corresponding number"<<endl;
			cin >> act2;
			if(bag1.getEquip(act2) == NULL){
				cout<< "Never mind"<<endl;
			}
			else{
				tempE = headE;
				while(*tempE != *(bag1.getEquip(act2))){
					tempE = tempE->getNext();
				}
				wieldEquip(p1,tempE,wear);
				wear = tempE;
			}
		}
		else{
			cout << "Invalid move"<< endl;
		}
		statusPlayerMove = map1.playerMove(p1.getPoi(),p1.getPoj(),dir);
		if(statusPlayerMove == 1){
			p1.move(dir);
		}
		else if(statusPlayerMove == ATTACK){
			attack(p1,Mon1,1,dir);
			attack(p1,Mon2,1,dir);
			attack(p1,Mon3,1,dir);
		}
		if(Mon1 != NULL){
			statusMon1 = !map1.MonsterDie(Mon1->getPoi(),Mon1->getPoj(),Mon1->getHp());
			if(statusMon1 == 0){
				cout<<"You kill the "<<Mon1->getName()<<"!\n";
				delete Mon1;
				Mon1 = NULL;
			}
		}
		if(Mon2 != NULL){
			statusMon2 = !map1.MonsterDie(Mon2->getPoi(),Mon2->getPoj(),Mon2->getHp());
			if(statusMon2 == 0){
				cout<<"You kill the "<<Mon2->getName()<<"!\n";
				delete Mon2;
				Mon2 = NULL;
			}
		}
		if(Mon3 != NULL){
			statusMon3 = !map1.MonsterDie(Mon3->getPoi(),Mon3->getPoj(),Mon3->getHp());
			if(statusMon3 == 0){
				cout<<"You kill the "<<Mon3->getName()<<"!\n";
				delete Mon3;
				Mon3 = NULL;
			}
		}

		//怪物走一步
		addMonster = rand()%5;
		if(addMonster < 3){
			if(statusMon1 == 0){
				Mon1 = new Monster(p1.getPoi(),p1.getPoj());
				statusMon1 = 1;
				cout<<"Monster1 created:"<<Mon1->getName()<<"("<<Mon1->getPoi()<<","<<Mon1->getPoj()<<")"<<endl;
			}
			else if(statusMon2 == 0){
				Mon2 = new Monster(p1.getPoi(),p1.getPoj());
				statusMon2 = 1;
				cout <<"Monster2 created:"<<Mon2->getName()<<"("<<Mon2->getPoi()<<","<<Mon2->getPoj()<<")"<<endl;
			}
			else if(statusMon3 == 0){
				Mon3 = new Monster(p1.getPoi(),p1.getPoj());
				statusMon3 = 1;
				cout<<"Monster3 created:"<<Mon3->getName()<<"("<<Mon3->getPoi()<<","<<Mon3->getPoj()<<")"<<endl;
			}
		}

		if(Mon1 != NULL)
		MonMoveInmain(Mon1,map1.MonsterMove(Mon1->getPoi(),Mon1->getPoj(),p1.getPoi(),p1.getPoj()),p1);
		if(Mon2 != NULL)
		MonMoveInmain(Mon2,map1.MonsterMove(Mon2->getPoi(),Mon2->getPoj(),p1.getPoi(),p1.getPoj()),p1);
		if(Mon3 != NULL)
		MonMoveInmain(Mon3,map1.MonsterMove(Mon3->getPoi(),Mon3->getPoj(),p1.getPoi(),p1.getPoj()),p1);
		//参数传递是否有问题？？？
		/*if(Mon1 != NULL){
			statusMonMove1 = map1.MonsterMove(Mon1->getPoi(),Mon1->getPoj(),p1.getPoi(),p1.getPoj());
			if(statusMonMove1 == ATTACK){
				attack(p1,Mon1,2,-1);
			}
			else if((statusMonMove1 > -1)&&(statusMonMove1 < 9)){
				Mon1->Move(statusMonMove1);
			}
		}
	 if(Mon2 != NULL){
		 statusMonMove2 = map1.MonsterMove(Mon2->getPoi(),Mon2->getPoj(),p1.getPoi(),p1.getPoj());
		 if(statusMonMove2 == ATTACK){
			 attack(p1,Mon2,2,-1);
		 }
		 else if((statusMonMove2 > -1)&&(statusMonMove2 < 9)){
			 Mon2->Move(statusMonMove2);
		 }
	 }
	 if(Mon3 != NULL){
		 statusMonMove3 = map1.MonsterMove(Mon3->getPoi(),Mon3->getPoj(),p1.getPoi(),p1.getPoj());
		 if(statusMonMove3 == ATTACK){
			 attack(p1,Mon3,2,-1);
		 }
		 else if((statusMonMove3 > -1)&&(statusMonMove3 < 9)){
			 Mon3->Move(statusMonMove3);
		 }
	 }*/
		/*cout << string(100,'\n');
		map1.printMap();*/
		//生成道具,最多十五个
		int ItemRate = rand()%4+1;
		if((ItemRate < 6)&&(itemNum <= 15)){
			itemNum++;
			tempI = headI;
			while(tempI->getNext() != NULL){
				tempI = tempI->getNext();
			}
			//tempI->getNext() = new item(p1.getPoi(),p1.getPoj());
			tempI->setNext(new item(p1.getPoi(),p1.getPoj()));
			tempI = tempI->getNext();
			map1.ItemAdd(tempI);
			cout<<"new item created: "<<tempI->getPoi()<<","<<tempI->getPoj()<<endl;
		}
		int EquipRate = rand()%4+1;
		if((EquipRate < 6)&&(EquipNum <= 15)){
			EquipNum++;
			tempE = headE;
			while(tempE->getNext()!=NULL){
				tempE = tempE->getNext();
			}
			tempE->setNext(new Equip(p1.getPoi(),p1.getPoj()));
			tempE = tempE->getNext();
			map1.EquipAdd(tempE);
			cout<<"new eq created  :"<<tempE->getName()<<tempE->getPoi()<<","<<tempE->getPoj()<<endl;
		}
		//检查人物是否还生存

		if(p1.getCurrentHp() <= 0){
			playerdie(p1);
			break;
		}
		map1.printMap();
		//map1.printMapforDebug();
		cout<<bag1.openBag();
		cout <<"Player Name: "<<p1.getName()<<" CurrentHp: "<<p1.getCurrentHp()<<"  MaxHp: "<<p1.getMaxHp()<<"  Power: "<<p1.getPower()<<endl;
		cout <<"player position:("<<p1.getPoi()<<","<<p1.getPoj()<<")"<<endl;
		if(DEBUG){
			if(Mon1 != NULL)
			cout<<"MON1"<<Mon1->getName()<<"("<<Mon1->getPoi()<<","<<Mon1->getPoj()<<")"<<endl;
			if(Mon2 != NULL)
			cout<<"MON2"<<Mon2->getName()<<"("<<Mon2->getPoi()<<","<<Mon2->getPoj()<<")"<<endl;
			if(Mon3 != NULL)
			cout<<"MON3"<<Mon3->getName()<<"("<<Mon3->getPoi()<<","<<Mon3->getPoj()<<")"<<endl;
		}
		cin >> act;
		cout << string(50,'\n');
	}
	//delete item list
	tempI = headI;
	while(tempI->getNext()!=NULL){
		headI = tempI->getNext();
		delete tempI;
		tempI = headI;
	}
	delete tempI;
	tempI = NULL;

	tempE = headE;
	while(tempE->getNext()!= NULL){
		headE = tempE->getNext();
		delete tempE;
		tempE = headE;
	}
	delete tempE;
	tempE = NULL;

	if(DEBUG){
	 map1.printRoad();
 }
	return 0;
}

// way 1:player attack monster ,2:monster attack player
void attack(player &pl, Monster* mo, int way, int dir){
	int hurt;
	if(mo == NULL)
		return;
	int gi,gj;
	if(dir == up){
		gi = pl.getPoi() -1;
		gj = pl.getPoj();
	}
	else if(dir == down){
		gi = pl.getPoi() + 1;
		gj = pl.getPoj();
	}
	else if(dir == left){
		gi = pl.getPoi();
		gj = pl.getPoj() - 1;
	}
	else if(dir == right){
		gi = pl.getPoi();
		gj = pl.getPoj() + 1;
	}
	else if(dir == here){
		gj = pl.getPoj();
		gi = pl.getPoi();
	}
	else if(dir == LeftUp){
		gi = pl.getPoi() - 1;
		gj = pl.getPoj() -1;
	}
	else if(dir == LeftDown){
		gi = pl.getPoi() + 1;
		gj = pl.getPoj() - 1;
	}
	else if(dir == RightUp){
		gi = pl.getPoi() - 1;
		gj = pl.getPoj() + 1;
	}
	else if(dir == RightDown){
		gi = pl.getPoi() + 1;
		gj = pl.getPoj() + 1;
	}
	if(way == 1){
		if((mo->getPoi() != gi)||(mo->getPoj() != gj)){
			return;
		}
		hurt = pl.getPower();
		mo->subHp(hurt);
	}
	else if(way == 2){
		hurt = mo->getPower() - pl.getDefence();
		pl.subHp(hurt);
		cout<<mo->getName()<<" hits you!!!"<<endl;
	}
}

void MonMoveInmain(Monster *Mon, int MMS, player &p1){
	if(Mon == NULL)
		return;
	if(MMS == ATTACK){
		attack(p1, Mon, 2,-1);
	}
	else if((MMS > -1)&&(MMS < 9)){
		Mon->Move(MMS);
	}
}//determine whether to attack or to change monster's position

void playerdie(player p1){
	cout << string(50,'\n');
	cout << "Fare thee well " <<p1.getName()<< endl;
}
void wieldEquip(player p1,Equip* tempE,Equip* wear){
	if(wear == NULL){
		p1.addDefence(tempE->getDefence(),1);
		p1.addPower(tempE->getharm());
	}
	else{
		p1.subDefence(wear->getDefence(),1);
		p1.subPower(wear->getharm());
		p1.addDefence(tempE->getDefence(),1);
		p1.addPower(tempE->getharm());
	}
	if(wear != NULL)
		cout<<"you take down "<<wear->getName()<<" and ";
	cout<<"you wear up  "<<tempE->getName()<<endl;
}