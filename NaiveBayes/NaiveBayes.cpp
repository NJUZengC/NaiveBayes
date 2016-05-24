#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>
#include<math.h>
#include"NaiveBayes.h"

NaiveBayes::NaiveBayes(){};

//UTF-8��GB2312��ת��
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
};

//GB2312��UTF-8��ת��
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

double Prop(vector<double> list,double x)//�����������ݼ���������ݼ�����������ڸ������ϵĸ���
{
	double mean = 0;
	double fangcha = 0;

	double sum = 0;
	for (unsigned int i = 0; i < list.size(); i++)
	{
		sum += list[i];
	}
	mean = sum / double(list.size());

	sum = 0;
	for (unsigned int i = 0; i < list.size(); i++)
	{
		sum += pow(abs(list[i]-mean),2);
	}
	fangcha = abs(sum / double(list.size()));
	
	double result = 0;
	double a = (1.0 / (sqrt(2 * 3.1415926)*sqrt(fangcha+0.000001)));//��ϸ΢С������������ֹ����δ������
	double b = exp(-1.0*pow(abs(x - mean), 2) / (2.0 * fangcha + 0.000001));
	result =a*b;
	return result;
};

void NaiveBayes::loadData(string file){//�������ݶ����ļ�
	fstream input(file, ios::in);
	if (!input)
	{
		cout << "�����ļ������ڣ���������ȷ����·��" << endl;
		return;
	}
	bool firstline = true;//�ж��Ƿ����ļ��ĵ�һ������
	while (!input.eof()){
		string line;
		getline(input, line);
		unsigned int index = 0;
		int attindex = 0;//�����±�
		vector<double> dataline;
		while (index < line.length())
		{
			
			string att = "";
			bool numflag = 1;//1��ʾ����������ֵ����Ϊ��ɢ��
			while (index < line.length() && line[index] != ',')//��ȡ�ӿ�ʼ�����Ž�ֹ֮�������ֵ
			{
				if ((numflag == 1) && (line[index]<'0' || line[index]>'9') && line[index] != '.')//һ�������쳣�ַ���Ϊ��ɢ������
					numflag = 0;
				att = att + line[index];
				index++;
			}
			att = U2G(att.c_str());//����תΪGB2312����
			if (firstline)//����ǵ�һ�����ʼ���������Ʊ�
			{
				vector<string> s;
				if (numflag == 0){//��ɢ�ͣ������ƴ�����Ӧ�������Ʊ�
					s.push_back(att);
					attrName.push_back(s);
					dataline.push_back(0);//���������0��ʾΪ��Ӧ���Կ���ֵ�ĵ�0λ����ֵ
				}
				else{
					double n = atof(att.c_str());
					attrName.push_back(s);
					dataline.push_back(n);
				}
				
			}
			else{
				
				if (numflag == 0){

					bool find = false;
					for (unsigned int i = 0; i < attrName[attindex].size(); i++)
					{
						if (attrName[attindex][i] == att)
						{
							dataline.push_back(i);
							find = true;
							break;
						}
					}
					if (find == false){
						attrName[attindex].push_back(att);
						dataline.push_back(attrName[attindex].size()-1);
					}
				}
				else{
					double n = atof(att.c_str());
					dataline.push_back(n);
				}
			}
			index++;
			attindex++;
		}
		if (firstline)
			firstline = false;
		data.push_back(dataline);
	}
	input.close();
};


void NaiveBayes::printData(){//��ӡ����
	for (unsigned int i = 0; i < data.size(); i++)
	{
		for (unsigned int j = 0; j < data[i].size(); j++){
			if (attrName[j].size()>0)
				cout << (attrName[j][(unsigned int)data[i][j]].c_str()) << "--";
			else
			cout << data[i][j] << "--";
		}
		cout << endl;
	}
	
};

int NaiveBayes::testData(vector<vector<double>>datas, vector<vector<string>>attrname, vector<double> line){
	postProp.clear();
	//�������������������Ը�������򷵻�ģ�ͽ����Ӧ���±꣬�������Ը�����1�������мලѧϰ�������ǣ��򷵻�0��ʾԤ�����1��ʾԤ����ȷ
	 if ((line.size() == attrname.size() - 1) ||( line.size() == attrname.size()))
	{
		int bestClass = 0;//��ʾ�����
		double bestQuality = -100;//�����������
		for (unsigned int i = 0; i < attrname[attrname.size() - 1].size(); i++)//��ÿ���������Ӧ����
		{
			double quality = 1;

			int num = 0;
			for (unsigned int j = 0; j < datas.size(); j++)
				if (abs(datas[j][attrname.size() - 1] - i) <= 0.0000001)
					num++;

			//cout << double(num + 1) / double(datas.size() + attrname[attrname.size() - 1].size()) << endl;
			quality *= double(num + 1) / double(datas.size() + attrname[attrname.size() - 1].size());


			if (line.size() == attrname.size()){
				for (unsigned int j = 0; j < line.size()-1; j++)
				{
					if (attrname[j].size() > 0)
					{
						int snum = 0;
						for (unsigned int k = 0; k < datas.size(); k++)
							if (abs(datas[k][attrname.size() - 1] - i) <= 0.0000001 && (abs(datas[k][j] - line[j]) <= 0.0000001))
								snum++;
						//cout << double(snum + 1) / double(num + attrname[j].size()) << endl;
						quality *= double(snum + 1) / double(num + attrname[j].size());
					}
					else{
						vector<double> list;
						for (unsigned int k = 0; k < datas.size(); k++)
							if (abs(datas[k][attrname.size() - 1] - i) <= 0.0000001)
								list.push_back(datas[k][j]);
						double prop = Prop(list, line[j]);
						//cout << prop << endl;
						quality *= prop;
					}
				}
			}
			else{
				for (unsigned int j = 0; j < line.size(); j++)
				{
					if (attrname[j].size() > 0)
					{
						int snum = 0;
						for (unsigned int k = 0; k < datas.size(); k++)
							if (abs(datas[k][attrname.size() - 1] - i) <= 0.0000001 && (abs(datas[k][j] - line[j]) <= 0.0000001))
								snum++;
						//cout << double(snum + 1) / double(num + attrname[j].size()) << endl;
						quality *= double(snum + 1) / double(num + attrname[j].size());
					}
					else{
						vector<double> list;
						for (unsigned int k = 0; k < datas.size(); k++)
							if (abs(datas[k][attrname.size() - 1] - i) <= 0.0000001)
								list.push_back(datas[k][j]);
						double prop = Prop(list, line[j]);
						//cout << prop << endl;
						quality *= prop;
					}
				}
			}
			postProp.push_back(quality);
			if (quality > bestQuality)//�������ֵ
			{
				bestClass = i;
				bestQuality = quality;
			}
		}
		if (line.size() == attrname.size()-1)
		return bestClass;
		else{
			
			if (abs(bestClass - line[line.size() - 1]) <= 0.0000001)
				return 1;
			else
				return 0;
		}
	}
	else
		return 0;
};


double NaiveBayes::calError(){//����ȫ����ʴ���������
	int errorNum = 0;
	for (unsigned int i = 0; i < data.size(); i++){
		int res = testData(data,attrName,data[i]);
		if (res == 0)
			errorNum++;
	}
	return double(errorNum)/double(data.size());
};


double NaiveBayes::crossTest(int times){//������֤
	vector<double> errRate;
	vector<vector<vector<double>>> classfySet;//�������з��ౣ��

	for (unsigned int i = 0; i < attrName[attrName.size() - 1].size(); i++)
	{
		vector<vector<double>> line;
		for (unsigned int j = 0; j < data.size(); j++)
		{
			if (abs(data[j][attrName.size() - 1] - i) <= 0.0000001)
				line.push_back(data[j]);
		}
		classfySet.push_back(line);
	}

	for ( int i = 0; i < times; i++)
	{
		vector<vector<double>> trainSet;
		vector<vector<double>> testSet;
	
		//�������л���ѵ��������Լ�
		for (unsigned k = 0; k < classfySet.size(); k++){
				for (unsigned m = 0; m < classfySet[k].size(); m++){
					if (m%times == i)
						testSet.push_back(classfySet[k][m]);
					else
						trainSet.push_back(classfySet[k][m]);
				}
			}
		

		vector<vector<string>> attrname =  getSubAttrname(trainSet);//�Եõ���ѵ�����õ���Ӧ���������Ƽ�
		int errorNum = 0;
		for (unsigned int i = 0; i < testSet.size(); i++){
			int res = testData(trainSet,attrname, testSet[i]);
			if (res == 0)//����0��ʾԤ�����
				errorNum++;
		}
		errRate.push_back(double(errorNum) / double(testSet.size()));//���������
		
	}
	double sum = 0;
	cout <<endl<< "    �����������������£�" << endl<<endl;
	for (unsigned int i = 0; i < errRate.size(); i++)
	{
		cout << "    " << errRate[i] << endl;
		sum += errRate[i];
	}
	cout << endl << "    ƽ��������Ϊ��" <<sum*1.0/double(times)<< endl << endl;
	return 0;
};


vector<vector<string>> NaiveBayes::getSubAttrname(vector<vector<double>>datas){
	vector<vector<string>> attrname;
	for (unsigned int i = 0; i < attrName.size(); i++)
	{
		vector<string> s;
		for (unsigned int j = 0; j < attrName[i].size(); j++)
		{
			bool find = false;
			for (unsigned int k = 0; k < datas.size(); k++)
			{
				if (abs(datas[k][i] - j) <= 0.0000001)
				{
					find = true;
					break;
				}
			}
			if (find == true)
				s.push_back(attrName[i][j]);
		}
		attrname.push_back(s);
	}
	return attrname;
};

int NaiveBayes::getAttrSize(){
	return max(attrName.size() - 1,0);
};


void NaiveBayes::testRawSingle(string m){//����ԭʼ������������
	
	vector<string> s;
	unsigned int index=0;
	while (index < m.size())
	{
		string n = "";
		while (index<m.size()&&m[index] != ',')
		{
			n = n + m[index];
			index++;
		}
		s.push_back(n);
		index++;
	}

	if (s.size() != attrName.size() - 1)
	{
		cout << "    ����������������̫���̫�٣�" << endl;
		exit(0);
	};




	vector<double> dataline;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		string UTF = (s[i].c_str());
		if (attrName[i].size() > 0)
		{
			bool find = false;
			for (unsigned int j = 0; j < attrName[i].size(); j++)
			{
				
				if (UTF == attrName[i][j])
				{
					dataline.push_back(j);
					find = true;
					break;
				}
			}
			if (find == false)
				dataline.push_back(attrName[i].size());
		}
		else{
			dataline.push_back(atof(UTF.c_str()));
		}
	}
	/*for (unsigned int i = 0; i < dataline.size(); i++)
		cout << " " << dataline[i];
	cout << endl;*/
	int res = testData(data, attrName, dataline);
	cout <<endl<< "    �жϸ�������������£�" << endl<<endl;
	for (unsigned int i = 0; i < attrName[attrName.size() - 1].size(); i++)
	{
		cout << "    " << (attrName[attrName.size() - 1][i].c_str()) << " : " << postProp[i] << endl;
	}
	cout << endl << "    ���ر�Ҷ˹ģ�ͽ���ó�������Ϊ��" << attrName[attrName.size() - 1][res] << endl;
};