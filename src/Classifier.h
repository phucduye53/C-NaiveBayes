#pragma once
#include"Bayes.h"



class Classifier
{
private:
	Bayes bayes;
	vector<Mushroom> listData;
	vector<Mushroom> testData;
	int indexOfClasslabel = 0;


public:


	void run(char *data, char *test,char *outputModel,char *output)
	{
		listData = readData(data);
		testData = readData(test);
		bayes.setData(listData, indexOfClasslabel);
		bayes.setTestData(testData);
		bayes.PrintModel(outputModel);
		bayes.NaiveBayes(output);
	}
	// cắt chuỗi 
	vector<Mushroom> readData(char* filename)
	{
		ifstream file;
		file.open(filename);
		vector<Mushroom> data;
		if (file.is_open())
		{
			while (!file.eof())
			{
				string line="";
				getline(file, line);
				Mushroom tuple;
				int i = 0;
				istringstream iss(line);
				string token;
				while (getline(iss, token, ' '))
				{
					tuple.att[i] = token[0];
					i++;
				}
				//for (int j = 0; j < 9; j++)
				//{
					//cout << tuple.att[j] << " ";
				//}
				//cout << endl;
				data.push_back(tuple);
			}
			
		}
		else
		{
			cout << "khong tim thay file";
		}
		return data;
		file.close();
	}
	Classifier();
	~Classifier();
};
