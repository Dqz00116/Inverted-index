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
	bool searchFileInDirectory(vector<string> &fileNameDirectory); //检索文档文件
	bool wordExtractor(vector<string> &fileNameDirectory,
		vector<string> &documentCollection
	); //提取文档内容
	bool wordAnalysis(
		vector<string> &documentCollection,
		map<string, int> &wordOfFrequency,
		map<string, vector<int>> &wordOfDocId
	); //词项分析
	bool printInvertedIndex(
		map<string, int> &wordOfFrequency,
		map<string, vector<int>> &wordOfDocId
	);//打印倒排记录表
	bool SearchWithAND(
		map<string, vector<int>> &wordOfDocId,
		vector<string> &fileNameDirectory
	); //AND查询
	bool inPutMenu(
		vector<string> &fileNameDirectory,
		map<string, int> &wordOfFrequency,
		map<string, vector<int>> &wordOfDocId
	);

private:
	string convertCharToStr(char *s_char); //将char类型转换为string类型
	int convertStrToInt(string n_str); //将string类型转换为int类型
	bool determineDocIdInDictionary(
		string word, 
		int docId, 
		map<string, vector<int>> &wordOfDocId
	); //添加文档id到词项
};

