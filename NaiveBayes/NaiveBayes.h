#ifndef __NAIVEBAYES_H_
#define __NAIVEBAYES_H_

#include<vector>
using namespace std;

class NaiveBayes{
	vector<vector<string>>attrName;//离散属性名称集
	vector<vector<double>>data;//数据集
 private:
	vector<double> postProp;//存储后验概率
public:
	NaiveBayes();
	void loadData(string file);//读取csv文件
	int getAttrSize();//获取数据中属性个数
	void printData();//打印文件数据
	int testData(vector<vector<double>>datas, vector<vector<string>>attrname,vector<double> line);//给定数据集和属性集测试一列数据检测分类结果
	double calError();//计算以所有数据为训练集对所有数据进行分类的分类错误率
	double crossTest(int times);//进行times交叉验证
	void testRawSingle(string m);//对给定原始数据进行测试（即可能包含离散属性名称或连续属性值的原始数据）
private:
	vector<vector<string>> getSubAttrname(vector<vector<double>>datas);//从给定的数据样本中获取其属性名称集
};

#endif