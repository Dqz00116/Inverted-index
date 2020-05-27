// Inverted index.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include"Inv_index.h"

int main()
{
	Inv_index test01;
	

	vector<string> fileNameDirectory;
	vector<string> documentCollection;
	map<string, int> wordOfFrequency;
	map<string, vector<int>> wordOfDocId;
	
	test01.searchFileInDirectory(fileNameDirectory);
	test01.wordExtractor(fileNameDirectory, documentCollection);
	test01.wordAnalysis(documentCollection, wordOfFrequency, wordOfDocId);

	test01.inPutMenu(fileNameDirectory, wordOfFrequency, wordOfDocId);

	

	return 0;
}
