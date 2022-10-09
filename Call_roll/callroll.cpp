#include<iostream>
#include<fstream>
#include"callroll.h"
using namespace std;
class Greater {
public:
	bool operator()( Student &s1,  Student &s2) {
		return s1.getNum() < s2.getNum();
	}
};
Roll::Roll() {

}
//初始化班级，即把输入的数据分配给对应的班级
void Roll::Init() {
	ifstream ifs;
	ifs.open("Name1.txt", ios::in);
	if (!ifs.is_open()) {
		//文件打开失败给出提示
		cout << "文件打开失败" << endl;
		return;
	}
	int sum = 450;
	while (sum) {
		char classNumber[1024] = { 0 };
		char Num[1024] = { 0 };
		char Name[1024] = { 0 };
		ifs >> classNumber >> Num >> Name;

		string clN;
		clN = classNumber;
		int clN1 = atoi(clN.c_str());
		string Nm = (string)Num;
		string Nme = (string)Name;
		//读取状态
		int state1[20] = { 0 };
		for (int i = 0; i < 20; i++) {
			char st[1024] = { 0 };
			ifs >> st;
			string sta = (string)st;
			int sta1 = atoi(sta.c_str());
			state1[i] = sta1;
		}
		Student sd(clN1, Nm, Nme, state1);
		if (clN1 == 1) {
			this->veClass1.push_back(sd);
		}
		else if (clN1 == 2) {
			this->veClass2.push_back(sd);
		}
		else if (clN1 == 3) {
			this->veClass3.push_back(sd);
		}
		else if (clN1 == 4) {
			this->veClass4.push_back(sd);
		}
		else if (clN1 == 5) {
			this->veClass5.push_back(sd);
		}
		sum--;

	}
	ifs.close();
	//对每个班进行按照学号升序排序
	sort(veClass1.begin(), veClass1.end(), Greater());
	sort(veClass2.begin(), veClass2.end(), Greater());
	sort(veClass3.begin(), veClass3.end(), Greater());
	sort(veClass4.begin(), veClass4.end(), Greater());
	sort(veClass5.begin(), veClass5.end(), Greater());
	//*********************测试
	/*for (Student s : this->veClass1) {
		cout << s.getClassNumber() << " " << s.getNum() << " "
			<< s.getName()<<" ";
		for (int i = 1; i <= 20; i++) {
			cout << s.getState(i) << " ";
		}
		cout << endl;
	}
	for (Student s : this->veClass2) {
		cout << s.getClassNumber() << " " << s.getNum() << " "
			<< s.getName() << " ";
		for (int i = 1; i <= 20; i++) {
			cout << s.getState(i) << " ";
		}
		cout << endl;
	}
	for (Student s : this->veClass3) {
		cout << s.getClassNumber() << " " << s.getNum() << " "
			<< s.getName() << " ";
		for (int i = 1; i <= 20; i++) {
			cout << s.getState(i) << " ";
		}
		cout << endl;
	}
	for (Student s : this->veClass4) {
		cout << s.getClassNumber() << " " << s.getNum() << " "
			<< s.getName() << " ";
		for (int i = 1; i <= 20; i++) {
			cout << s.getState(i) << " ";
		}
		cout << endl;
	}
	for (Student s : this->veClass5) {
		cout << s.getClassNumber() << " " << s.getNum() << " "
			<< s.getName() << " ";
		for (int i = 1; i <= 20; i++) {
			cout << s.getState(i) << " ";
		}
		cout << endl;
	}*/
}
//单次点名并且把点到的名单输出到文件
void Roll::Callcl1(int claAount, ofstream& ofs, int clnb) {
	int sum = 0;//记录有效点名次数
	int callingCnt = 0;
	if (claAount <= 5) {
		//前五次课抽点45个人	
		callingCnt = 45;
	}
	else {
		//后课抽点30个人	
		callingCnt = 25;
	}
	if (this->notComeStu[clnb-1].empty()) {
		//黑名单为空，就从全班抽
		int rolled[90] = { 0 };//用来标记已经在这一次抽点中点到的人
		for (int i = 0; i < callingCnt;) {
			int index = rand() % 90;
			if (rolled[index] == 0) {
				//修改被点到次数
				this->veClass1[index].setCalledCnt();
				int state = this->veClass1[index].getState(claAount);
				//被点到，但是没来的同学进行加入黑名单，同时修改没来次数
				if (state == 0) {
					this->veClass1[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass1[index]));
					
					sum++;
				}
				rolled[index] = 1;
				i++;
			}
		}
	}
	else {
		//黑名单不为空，一部分从黑名单抽，一部分从黑名单之外的人抽
		//抽点黑名单
		for (int i = 0; i < this->notComeStu[clnb-1].size();i++) {
			//修改被点次数
			(this->notComeStu[clnb-1][i])->setCalledCnt();
			int state = (this->notComeStu[clnb-1][i])->getState(claAount);
			//如果没来就修改没来次数
			if (state == 0) {
				(this->notComeStu[clnb-1][i])->setNotComeCnt();
				sum++;
			}
		}
		for (int i = 0; i < callingCnt - (this->notComeStu[clnb-1].size());) {
			int index = rand() % 90;
			int flag = 0;//标记该人是否在黑名单中
			for (Student* s : this->notComeStu[clnb-1]) {
				if (s->getNum().compare( this->veClass1[index].getNum())==0) {
					flag = 1;//说明index这个人在黑名单中
					break;
				}
			}
			if (flag == 0) {
				//不在黑名单中
				//修改被点次数
				this->veClass1[index].setCalledCnt();
				int state = this->veClass1[index].getState(claAount);
				//如果没来修改没来次数,并且加入黑名单
				if (state == 0) {
					this->veClass1[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass1[index]));
					sum++;
				}
				i++;
			}
		}
	}
	if (claAount%5 ==0&&claAount<20 ) {
		if (!(this->notComeStu[clnb-1].empty())) {
			for (auto it = this->notComeStu[clnb-1].begin(); it != this->notComeStu[clnb-1].end();it++) {
				if ((*it)->getNotComeCnt()<3) {
					//没到次数小于三次的先移出黑名单
					this->notComeStu[clnb-1].erase(it);
					it = this->notComeStu[clnb-1].begin();
				}
			}
		}	
	}
	//写入文件保存结果
	ofs <<clnb<< "班第" << claAount << "次抽点 抽点" << callingCnt << "人 未到" << sum << "人" << endl;

}
void Roll::Callcl2(int claAount, ofstream& ofs, int clnb) {
	int sum = 0;//记录有效点名次数
	int callingCnt = 0;
	if (claAount <= 5) {
		//前五次课抽点45个人	
		callingCnt = 45;
	}
	else {
		//后课抽点30个人	
		callingCnt = 25;
	}
	if (this->notComeStu[clnb-1].empty()) {
		//黑名单为空，就从全班抽
		int rolled[90] = { 0 };//用来标记已经在这一次抽点中点到的人
		for (int i = 0; i < callingCnt;) {
			int index = rand() % 90;
			if (rolled[index] == 0) {
				//修改被点到次数
				this->veClass2[index].setCalledCnt();
				int state = this->veClass2[index].getState(claAount);
				//被点到，但是没来的同学进行加入黑名单，同时修改没来次数
				if (state == 0) {
					this->veClass2[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass2[index]));

					sum++;
				}
				rolled[index] = 1;
				i++;
			}
		}
	}
	else {
		//黑名单不为空，一部分从黑名单抽，一部分从黑名单之外的人抽
		//抽点黑名单
		for (int i = 0; i < this->notComeStu[clnb-1].size(); i++) {
			//修改被点次数
			(this->notComeStu[clnb-1][i])->setCalledCnt();
			int state = (this->notComeStu[clnb-1][i])->getState(claAount);
			//如果没来就修改没来次数
			if (state == 0) {
				(this->notComeStu[clnb-1][i])->setNotComeCnt();
				sum++;
			}
		}
		for (int i = 0; i < callingCnt - (this->notComeStu[clnb-1].size());) {
			int index = rand() % 90;
			int flag = 0;//标记该人是否在黑名单中
			for (Student* s : this->notComeStu[clnb-1]) {
				if (s->getNum().compare(this->veClass2[index].getNum()) == 0) {
					flag = 1;//说明index这个人在黑名单中
					break;
				}
			}
			if (flag == 0) {
				//不在黑名单中
				//修改被点次数
				this->veClass2[index].setCalledCnt();
				int state = this->veClass2[index].getState(claAount);
				//如果没来修改没来次数,并且加入黑名单
				if (state == 0) {
					this->veClass2[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass2[index]));
					sum++;
				}
				i++;
			}
		}
	}
	if (claAount % 5 == 0 && claAount < 20) {
		if (!(this->notComeStu[clnb-1].empty())) {
			for (auto it = this->notComeStu[clnb-1].begin(); it != this->notComeStu[clnb-1].end(); it++) {
				if ((*it)->getNotComeCnt() < 3) {
					//没到次数小于三次的先移出黑名单
					this->notComeStu[clnb-1].erase(it);
					it = this->notComeStu[clnb-1].begin();
				}
			}
		}
	}
	//写入文件保存结果
	ofs << clnb << "班第" << claAount << "次抽点 抽点" << callingCnt << "人 未到" << sum << "人" << endl;
}
void Roll::Callcl3(int claAount, ofstream& ofs, int clnb) {
	int sum = 0;//记录有效点名次数
	int callingCnt = 0;
	if (claAount <= 5) {
		//前五次课抽点45个人	
		callingCnt = 45;
	}
	else {
		//后课抽点30个人	
		callingCnt = 25;
	}
	if (this->notComeStu[clnb-1].empty()) {
		//黑名单为空，就从全班抽
		int rolled[90] = { 0 };//用来标记已经在这一次抽点中点到的人
		for (int i = 0; i < callingCnt;) {
			int index = rand() % 90;
			if (rolled[index] == 0) {
				//修改被点到次数
				this->veClass3[index].setCalledCnt();
				int state = this->veClass3[index].getState(claAount);
				//被点到，但是没来的同学进行加入黑名单，同时修改没来次数
				if (state == 0) {
					this->veClass3[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass3[index]));

					sum++;
				}
				rolled[index] = 1;
				i++;
			}
		}
	}
	else {
		//黑名单不为空，一部分从黑名单抽，一部分从黑名单之外的人抽
		//抽点黑名单
		for (int i = 0; i < this->notComeStu[clnb-1].size(); i++) {
			//修改被点次数
			(this->notComeStu[clnb-1][i])->setCalledCnt();
			int state = (this->notComeStu[clnb-1][i])->getState(claAount);
			//如果没来就修改没来次数
			if (state == 0) {
				(this->notComeStu[clnb-1][i])->setNotComeCnt();
				sum++;
			}
		}
		for (int i = 0; i < callingCnt - (this->notComeStu[clnb-1].size());) {
			int index = rand() % 90;
			int flag = 0;//标记该人是否在黑名单中
			for (Student* s : this->notComeStu[clnb-1]) {
				if (s->getNum().compare(this->veClass3[index].getNum()) == 0) {
					flag = 1;//说明index这个人在黑名单中
					break;
				}
			}
			if (flag == 0) {
				//不在黑名单中
				//修改被点次数
				this->veClass3[index].setCalledCnt();
				int state = this->veClass3[index].getState(claAount);
				//如果没来修改没来次数,并且加入黑名单
				if (state == 0) {
					this->veClass3[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass3[index]));
					sum++;
				}
				i++;
			}
		}
	}
	if (claAount % 5 == 0 && claAount < 20) {
		if (!(this->notComeStu[clnb-1].empty())) {
			for (auto it = this->notComeStu[clnb-1].begin(); it != this->notComeStu[clnb-1].end(); it++) {
				if ((*it)->getNotComeCnt() < 3) {
					//没到次数小于三次的先移出黑名单
					this->notComeStu[clnb-1].erase(it);
					it = this->notComeStu[clnb-1].begin();
				}
			}
		}
	}
	//写入文件保存结果
	ofs << clnb << "班第" << claAount << "次抽点 抽点" << callingCnt << "人 未到" << sum << "人" << endl;
}
void Roll::Callcl4(int claAount, ofstream& ofs, int clnb) {
	int sum = 0;//记录有效点名次数
	int callingCnt = 0;
	if (claAount <= 5) {
		//前五次课抽点45个人	
		callingCnt = 45;
	}
	else {
		//后课抽点30个人	
		callingCnt = 25;
	}
	if (this->notComeStu[clnb-1].empty()) {
		//黑名单为空，就从全班抽
		int rolled[90] = { 0 };//用来标记已经在这一次抽点中点到的人
		for (int i = 0; i < callingCnt;) {
			int index = rand() % 90;
			if (rolled[index] == 0) {
				//修改被点到次数
				this->veClass4[index].setCalledCnt();
				int state = this->veClass4[index].getState(claAount);
				//被点到，但是没来的同学进行加入黑名单，同时修改没来次数
				if (state == 0) {
					this->veClass4[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass4[index]));

					sum++;
				}
				rolled[index] = 1;
				i++;
			}
		}
	}
	else {
		//黑名单不为空，一部分从黑名单抽，一部分从黑名单之外的人抽
		//抽点黑名单
		for (int i = 0; i < this->notComeStu[clnb-1].size(); i++) {
			//修改被点次数
			(this->notComeStu[clnb-1][i])->setCalledCnt();
			int state = (this->notComeStu[clnb-1][i])->getState(claAount);
			//如果没来就修改没来次数
			if (state == 0) {
				(this->notComeStu[clnb-1][i])->setNotComeCnt();
				sum++;
			}
		}
		for (int i = 0; i < callingCnt - (this->notComeStu[clnb-1].size());) {
			int index = rand() % 90;
			int flag = 0;//标记该人是否在黑名单中
			for (Student* s : this->notComeStu[clnb-1]) {
				if (s->getNum().compare(this->veClass4[index].getNum()) == 0) {
					flag = 1;//说明index这个人在黑名单中
					break;
				}
			}
			if (flag == 0) {
				//不在黑名单中
				//修改被点次数
				this->veClass4[index].setCalledCnt();
				int state = this->veClass4[index].getState(claAount);
				//如果没来修改没来次数,并且加入黑名单
				if (state == 0) {
					this->veClass4[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass4[index]));
					sum++;
				}
				i++;
			}
		}
	}
	if (claAount % 5 == 0 && claAount < 20) {
		if (!(this->notComeStu[clnb-1].empty())) {
			for (auto it = this->notComeStu[clnb-1].begin(); it != this->notComeStu[clnb-1].end(); it++) {
				if ((*it)->getNotComeCnt() < 3) {
					//没到次数小于三次的先移出黑名单
					this->notComeStu[clnb-1].erase(it);
					it = this->notComeStu[clnb-1].begin();
				}
			}
		}
	}
	//写入文件保存结果
	ofs << clnb << "班第" << claAount << "次抽点 抽点" << callingCnt << "人 未到" << sum << "人" << endl;
}
void Roll::Callcl5(int claAount, ofstream& ofs, int clnb) {
	int sum = 0;//记录有效点名次数
	int callingCnt = 0;
	if (claAount <= 5) {
		//前五次课抽点45个人	
		callingCnt = 45;
	}
	else {
		//后课抽点30个人	
		callingCnt = 25;
	}
	if (this->notComeStu[clnb-1].empty()) {
		//黑名单为空，就从全班抽
		int rolled[90] = { 0 };//用来标记已经在这一次抽点中点到的人
		for (int i = 0; i < callingCnt;) {
			int index = rand() % 90;
			if (rolled[index] == 0) {
				//修改被点到次数
				this->veClass5[index].setCalledCnt();
				int state = this->veClass5[index].getState(claAount);
				//被点到，但是没来的同学进行加入黑名单，同时修改没来次数
				if (state == 0) {
					this->veClass5[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass5[index]));

					sum++;
				}
				rolled[index] = 1;
				i++;
			}
		}
	}
	else {
		//黑名单不为空，一部分从黑名单抽，一部分从黑名单之外的人抽
		//抽点黑名单
		for (int i = 0; i < this->notComeStu[clnb-1].size(); i++) {
			//修改被点次数
			(this->notComeStu[clnb-1][i])->setCalledCnt();
			int state = (this->notComeStu[clnb-1][i])->getState(claAount);
			//如果没来就修改没来次数
			if (state == 0) {
				(this->notComeStu[clnb-1][i])->setNotComeCnt();
				sum++;
			}
		}
		for (int i = 0; i < callingCnt - (this->notComeStu[clnb-1].size());) {
			int index = rand() % 90;
			int flag = 0;//标记该人是否在黑名单中
			for (Student* s : this->notComeStu[clnb-1]) {
				if (s->getNum().compare(this->veClass5[index].getNum()) == 0) {
					flag = 1;//说明index这个人在黑名单中
					break;
				}
			}
			if (flag == 0) {
				//不在黑名单中
				//修改被点次数
				this->veClass5[index].setCalledCnt();
				int state = this->veClass5[index].getState(claAount);
				//如果没来修改没来次数,并且加入黑名单
				if (state == 0) {
					this->veClass5[index].setNotComeCnt();
					this->notComeStu[clnb-1].push_back(&(this->veClass5[index]));
					sum++;
				}
				i++;
			}
		}
	}
	if (claAount % 5 == 0 && claAount < 20) {
		if (!(this->notComeStu[clnb-1].empty())) {
			for (auto it = this->notComeStu[clnb-1].begin(); it != this->notComeStu[clnb-1].end(); it++) {
				if ((*it)->getNotComeCnt() < 3) {
					//没到次数小于三次的先移出黑名单
					this->notComeStu[clnb-1].erase(it);
					it = this->notComeStu[clnb-1].begin();
				}
			}
		}
	}
	//写入文件保存结果
	ofs << clnb << "班第" << claAount << "次抽点 抽点" << callingCnt << "人 未到" << sum << "人" << endl;
}