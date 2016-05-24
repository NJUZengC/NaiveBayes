#include<iostream>
#include<string>
#include"NaiveBayes.h"
using namespace std;

int main(){
	

	while (1){
		
		cout << endl << "-----------------------------------------------" << endl;
		cout << "  1.西瓜数据3.0" << endl;
		cout << "  2.glass.csv" << endl;
		cout << "  3.breast-cancer.csv" << endl;
		cout << "  4.退出" << endl;
		int choose;
		cin >> choose;
		while (choose < 1 || choose>4)
		{
			cout << "  请输入正确编号!" << endl;
			cin >> choose;
		}
		NaiveBayes nb;
		switch (choose)
		{
		case 1:
		{
			nb.loadData("hw5_data\\西瓜数据3.0.csv");
			break;
		}
		case 2:nb.loadData("hw5_data\\glass.csv"); break;
		case 3:nb.loadData("hw5_data\\breast-cancer.csv"); break;
		case 4:return 0;
		}

		cout << "  1.输入测试样例" << endl;
		cout << "  2.全验证" << endl;
		cout << "  3.交叉验证" << endl;
		int twchoose;
		cin >> twchoose;

		while (choose < 1 || choose>3)
		{
			cout << "  请输入正确编号!" << endl;
			cin >> choose;
		}
		cout << endl << "-----------------------------------------------" << endl;
		switch (twchoose)
		{
		case 1:
		{
			string s;
			cout << endl << "    ";
			cin >> s;
			
			nb.testRawSingle(s);
			break;
		}
		case 2:cout << "    分类错误率为：" << nb.calError(); break;
		case 3:
		{
			int num = 2;
			cout <<endl<< "    请输入交叉验证次数：" << endl;
			cin >> num;
			while (num <= 1)
			{
				cout << "    请输入正确交叉验证次数！" << endl;
				cin >> num;
			}
			nb.crossTest(num);	
		}
		}
	}
	return 0;
};


