#ifndef __NAIVEBAYES_H_
#define __NAIVEBAYES_H_

#include<vector>
using namespace std;

class NaiveBayes{
	vector<vector<string>>attrName;//��ɢ�������Ƽ�
	vector<vector<double>>data;//���ݼ�
 private:
	vector<double> postProp;//�洢�������
public:
	NaiveBayes();
	void loadData(string file);//��ȡcsv�ļ�
	int getAttrSize();//��ȡ���������Ը���
	void printData();//��ӡ�ļ�����
	int testData(vector<vector<double>>datas, vector<vector<string>>attrname,vector<double> line);//�������ݼ������Լ�����һ�����ݼ�������
	double calError();//��������������Ϊѵ�������������ݽ��з���ķ��������
	double crossTest(int times);//����times������֤
	void testRawSingle(string m);//�Ը���ԭʼ���ݽ��в��ԣ������ܰ�����ɢ�������ƻ���������ֵ��ԭʼ���ݣ�
private:
	vector<vector<string>> getSubAttrname(vector<vector<double>>datas);//�Ӹ��������������л�ȡ���������Ƽ�
};

#endif