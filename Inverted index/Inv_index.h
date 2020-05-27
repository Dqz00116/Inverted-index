#pragma once

#include<iostream>
#include<stdlib.h>
#include<string>
#include<io.h>
#include<sstream>
#include<vector>
#include<fstream>
#include<map>
#include<algorithm>

using namespace std;


class Inv_index
{
public:
	Inv_index();
	~Inv_index();
	bool searchFileInDirectory(vector<string> &fileNameDirectory); //�����ĵ��ļ�
	bool wordExtractor(vector<string> &fileNameDirectory,
		vector<string> &documentCollection
	); //��ȡ�ĵ�����
	bool wordAnalysis(
		vector<string> &documentCollection,
		map<string, int> &wordOfFrequency,
		map<string, vector<int>> &wordOfDocId
	); //�������
	bool printInvertedIndex(
		map<string, int> &wordOfFrequency,
		map<string, vector<int>> &wordOfDocId
	);//��ӡ���ż�¼��
	bool SearchWithAND(
		map<string, vector<int>> &wordOfDocId,
		vector<string> &fileNameDirectory
	); //AND��ѯ
	bool inPutMenu(
		vector<string> &fileNameDirectory,
		map<string, int> &wordOfFrequency,
		map<string, vector<int>> &wordOfDocId
	);

private:
	string convertCharToStr(char *s_char); //��char����ת��Ϊstring����
	int convertStrToInt(string n_str); //��string����ת��Ϊint����
	bool determineDocIdInDictionary(
		string word, 
		int docId, 
		map<string, vector<int>> &wordOfDocId
	); //����ĵ�id������
};

