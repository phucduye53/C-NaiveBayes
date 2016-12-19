#pragma once
#include<vector>
#include<math.h>
#include<map>
#include<string>
#include<algorithm>
#include<iostream>
#include<fstream>
#include <sstream>
using namespace std;

struct Mushroom
{
	char att[9];
};




class Bayes
{
private:
	vector<Mushroom> listData; // dữ liệu từ input
	vector<Mushroom> testData; // dữ liệu từ test
	vector<vector<char>> allAttributes; // lữu trữ giá trị của riêng biệt của thuộc tính , shape , habitat, suface, color ...
	map<char, vector<Mushroom>> classSubsets; // lưu trữ dữ liệu nấm có thuộc tính
	map<char, double> classProbabilities; // xác suất
	int indexOfClassLabel; // thuộc tính có độc hay không độc , mặc định là 0
	int numberOfAttributes; // số lượng thuộc tính

public:
	void setData(vector<Mushroom> data, int index)
	{
		listData = data;
		indexOfClassLabel = index;
		numberOfAttributes = 9;
		cout << numberOfAttributes;
		generateUniqueAttributes();
		findClassSubsets();

	}
	void setTestData(vector<Mushroom> data)
	{
		testData = data;
	}
	void generateUniqueAttributes()
	{
		int n = listData.size();
		for (int i = 0; i < numberOfAttributes; i++)
		{
			vector<char> attributeSet;
			addAtt(attributeSet,i);
			allAttributes.push_back(attributeSet);
		}

		//for (int j = 0; j < allAttributes.size(); j++)
		//{
			//for (int k = 0; k < allAttributes.at(j).size(); k++)
			//{
				//cout << allAttributes.at(j).at(k) << " ";
			//}
			//cout << endl;
		//}

		//for (int i = 0; i < numberOfAttributes; i++)
		//{
			//for (int j = 0; j < n; j++)
			//{
				//char a = listData.at(j).att[i];
				//if (findAtt(allAttributes, a, i) == true)
				//{
					//allAttributes.at(i).push_back(a);
					//cout << a << " ";
				//}
			//}
			//cout << endl;
		//}	
	}

	//bool findAtt(vector<vector<char>> temp, char att, int indexAttributes)
	//{
		//int n = temp.size();
		//for (int i = 0; i < n; i++)
		//{
			//if (temp.at(indexAttributes).at(i) == att)
			//{
				//return false;
			//}
		//}
		//return true;
	//}
	
	void addAtt(vector<char> &temp,int &index) // thêm các giá trị riêng biệt vào các thuộc tính
	{
		temp.push_back(listData.at(0).att[index]);
		int n = listData.size();

		for (int i = 1; i < n; i++)
		{
			int count = 0;
			for (int j = 0; j < temp.size(); j++)
			{
				if (temp.at(j) != listData.at(i).att[index])
				{
					count++;
					if (count == temp.size())
					{
						temp.push_back(listData.at(i).att[index]);
					}
				}
			}
		}	

	}
	// tìm xác suất 
	void findClassSubsets()
	{

			vector<char> UniqueAtt = allAttributes.at(indexOfClassLabel);
			int n = UniqueAtt.size();
			for (int j = 0; j < n; j++)
			{
				vector<Mushroom> subset = getSubset(UniqueAtt.at(j), indexOfClassLabel);
				classSubsets.insert(pair<char, vector<Mushroom>>(UniqueAtt.at(j), subset)); // filter
				classProbabilities.insert(pair<char, double>(UniqueAtt.at(j), (double)subset.size() / listData.size())); // = vector đã filter / listData
			}

		
	}
	// lấy vector nấm có cùng thuộc tính
	vector<Mushroom> getSubset(char att, int index)
	{
		vector<Mushroom> subset;
		int n = listData.size();
		for (int i = 0; i < n; i++)
		{
			if (listData.at(i).att[index] == att)
			{
				subset.push_back(listData.at(i));
			}
		}
		return subset;
	}
	// In ra file model.txt
	void PrintModel(char *output)
	{
		fstream file(output);

		if (file.is_open())
		{
			vector<char> classLabels = allAttributes.at(indexOfClassLabel);
			int m = classLabels.size();
			for (int i = 0; i < m; i++)
			{
				double priorProbability = classProbabilities.at(classLabels[i]);
				file << priorProbability << " ";
			}
			file << endl;
			for (int i = 0; i < m; i++)
			{
				vector<Mushroom> subset = classSubsets.at(classLabels[i]);
				file << subset[0].att[0] << endl;
				for (int j = 1; j < numberOfAttributes; j++)
				{
					for (int k = 0; k < allAttributes.at(j).size(); k++)
					{
						int count = countAttributeInSubset(allAttributes.at(j).at(k), subset, j);
						double conditionalProbability;
						conditionalProbability = ((double)count+1) / (subset.size()+classLabels.size()) ;
						file << conditionalProbability << " ";
					}
					file << endl;
				}
			}

		}
		file.close();
	}
	// chạy ra output.txt
	void NaiveBayes(char *output)
	{
		fstream file(output);
		if (file.is_open())
		{
			vector<char> classLabels = allAttributes.at(indexOfClassLabel); // lấy class label gồm p và e
			int correct = 0, incorrect = 0;
			int n = testData.size(); 
			for (int i = 0; i < n; i++)
			{
				char trueClassification = testData.at(i).att[indexOfClassLabel]; // lấy p || e từ testData
				char testClassifiation = '\0'; // khởi tạo biến
				double oldTotalProbability = 0.0;
				int m = classLabels.size(); // size = 2
				for (int j = 0; j < m; j++)
				{
					vector<Mushroom> subset = classSubsets.at(classLabels[j]); 
					double priorProbability = classProbabilities.at(classLabels[j]);
					double conditionalProbability = 1.0;
					for (int k = 0; k < numberOfAttributes; k++)
					{
						if (k == indexOfClassLabel)
						{
							continue;
						}
						int count = countAttributeInSubset(testData.at(i).att[k], subset, k);
						conditionalProbability *= ((double)count+1) / (subset.size()+classLabels.size());
					}
					double newTotalProbability = conditionalProbability*priorProbability;
					if (newTotalProbability>oldTotalProbability)
					{
						oldTotalProbability = newTotalProbability;
						testClassifiation = classLabels.at(j);
					}
				}
				if (trueClassification == testClassifiation)
				{
					file << trueClassification << " " << testClassifiation << endl;
				}
				else{
					file << trueClassification << " " << testClassifiation << endl;
				}

			}
		}
		file.close();

	}

	// đếm số lượng giá trị của thuộc tính có trong subset 
	int countAttributeInSubset(char attribute, vector<Mushroom> subset, int indexOfAttribute)
	{
		int sum = 0;
		int n = subset.size();
		for (int i = 0; i < n; i++)
		{
			if (subset.at(i).att[indexOfAttribute] == attribute)
			{
				sum++;
			}
		}
		return sum;
	}



	Bayes();
	~Bayes();
};

