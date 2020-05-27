#include "pch.h"
#include "Inv_index.h"


/***************************************************public*********************************************************/


Inv_index::Inv_index()
{
}


Inv_index::~Inv_index()
{
}

//@name <Inv_index::searchFileInDirectory>
//@brief <检索当前目录下所有的文档文件，将.txt文件名写入文件名目录>
//@param <vector<string> &fileNameDirectory:文件名目录>
//@return <true/false>
bool Inv_index::searchFileInDirectory(vector<string> &fileNameDirectory)
{
	long handle;	//用于查找的句柄
	const char* to_search = "*.txt";	//指定文件类型 
	struct _finddata_t fileinfo;    //文件信息的结构体
	

	handle = _findfirst(to_search, &fileinfo);    //第一次查找
	if (-1 == handle) {
		cout << "Error in searchFileInDirectory()" << endl;
		return false;
	}

	//cout << fileinfo.name << endl;    //打印出找到的文件的文件名（用于试错）
	fileNameDirectory.push_back(convertCharToStr(fileinfo.name));
	
	while (!_findnext(handle, &fileinfo)) {    //循环查找其它符合的文件，直到找不到其它文件为止
		//cout << fileinfo.name << endl;    //打印出找到的文件的文件名（用于试错）
		fileNameDirectory.push_back(convertCharToStr(fileinfo.name));
	}
	
	_findclose(handle);
	return true;
}

//@name <Inv_index::wordExtractor>
//@brief <提取检索到的.txt文件的内容，将其写入文档内容集合>
//@param <vector<string> &fileNameDirectory:文件名目录>
//		 <vector<string>& documentCollection:文档内容集合>
//@return <true/false>
bool Inv_index::wordExtractor(vector<string> &fileNameDirectory,vector<string>& documentCollection)
{
	fstream infile;

	for (unsigned int i = 0;i < fileNameDirectory.size();i++)
	{
		infile.open(fileNameDirectory[i]);   //将文件流对象与文件连接起来 
		
		if (!infile.is_open())   //若失败,则输出错误消息,并终止程序运行 
		{
			cout << "Error in opening the"<< fileNameDirectory[i] << endl;
			return false;
		}

		istreambuf_iterator<char> begin(infile), end; //输入流缓冲区迭代器读取全部内容
		string strdata(begin, end);

		//cout << strdata << endl; //打印读取到的内容（用于试错）
		documentCollection.push_back(strdata);

		infile.close(); //关闭文件输入流
	}
	           
	return true;
}

//@name <Inv_index::wordAnalysis>
//@brief <分析文档内容集合中的内容，提取其中的词项、对应的词频以及文档id索引>
//@param <vector<string>& documentCollection:文档内容集合>
//		 <map<string, int> &wordOfFrequency:词项-词频映射>
//		 <map<string, vector<int>> &wordOfDocId:词项-文档id索引映射>
//@return <true/false>
bool Inv_index::wordAnalysis(
	vector<string>& documentCollection, 
	map<string, int> &wordOfFrequency,
	map<string, vector<int>> &wordOfDocId
	)
{
	string word;

	const char SPACE = ' ', //空格
		COMMA = ',', //逗号
		PERIOD = '.', //句号
		QUOTATION = '"'; //引号

	for (unsigned int i = 0;i < documentCollection.size();i++)
	{
		transform(
			documentCollection[i].begin(),
			documentCollection[i].end(),
			documentCollection[i].begin(),
			::tolower
		);   //把字符串全部转换成小写

		for (auto it : documentCollection[i])
		{
			
			if (it == SPACE || it == COMMA || it == PERIOD || it == QUOTATION )   //标志着一个单词的结尾
			{
				if (word != "")
				{
					wordOfFrequency[word]++; //词项与词频关联
					determineDocIdInDictionary(word, i, wordOfDocId); //词项与文档id关联
				}
				word = "";    //将word置为空,开始记录下一个单词
			}
			else
			{
				word += it;
			}
		}
	}
	
	return true;
}

//@name <Inv_index::printInvertedIndex>
//@brief <打印倒排记录表>
//@param <map<string, int> &wordOfFrequency:词项-词频映射>
//		 <map<string, vector<int>> &wordOfDocId:词项-文档id索引映射>
//@return <true/false>
bool Inv_index::printInvertedIndex(
	map<string, int>& wordOfFrequency,
	map<string, vector<int>>& wordOfDocId)
{
	for (auto it = wordOfDocId.begin(); it != wordOfDocId.end(); it++)
	{
		cout << it->first << ":(" << wordOfFrequency[it->first] << ")"; //打印词项以及词频
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			cout << "->" << *it2; //打印posting
		}
		cout << endl;

	}

	return true;
}

//@name <Inv_index::SearchWithAND>
//@brief <以AND方式接受双词项查询>
//@param <map<string, vector<int>> &wordOfDocId:词项-文档id索引映射>
//		 <vector<string> &fileNameDirectory:文件名目录>
//@return <true/false>
bool Inv_index::SearchWithAND(map<string, vector<int>>& wordOfDocId, vector<string> &fileNameDirectory)
{
	string firstWord, secondWord; //储存两个查询词项
	vector<int> answerList; //储存索引结果

	cout << "Please enter two words you want to search:" << endl;
	cin >> firstWord >> secondWord;

	if (firstWord.empty() || secondWord.empty())
	{
		cout<<"You must enter two words if you want to search, please enter again:"<<endl;
		cin >> firstWord >> secondWord; //判断输入是否为空
	}

	if (wordOfDocId.find(firstWord) != wordOfDocId.end() && wordOfDocId.find(secondWord) != wordOfDocId.end())
	{
		auto firstPoint = wordOfDocId[firstWord].begin();
		auto secondPoint = wordOfDocId[secondWord].begin(); //初始化迭代器

		while (firstPoint != wordOfDocId[firstWord].end() && secondPoint != wordOfDocId[secondWord].end())
		{
			if (*firstPoint == *secondPoint) //如果存在相同的docId，将docId写入索引结果
			{
				answerList.push_back(*firstPoint);
				firstPoint++;
				secondPoint++;
			}
			else if(*firstPoint < *secondPoint)
			{
				firstPoint++;
			}
			else
			{
				secondPoint++;
			}
		}
	}
	else
	{
		cout << "There are no result after search in directory." << endl; //未找到查询结果
		return false;
	}


	cout << "firstWord AND secondWord are in ";
	for (auto it : answerList)
	{
		cout << "->" << fileNameDirectory[it-1]<<"("<<it<<")"; //打印查询结果
	}
	cout << endl;

	return true;
}

//@name <Inv_index::inPutMenu>
//@brief <打印功能选择菜单，进行输入>
//@param <vector<string> &fileNameDirectory:文件名目录>
//		 <map<string, int> &wordOfFrequency:词项-词频映射>
//		 <map<string, vector<int>> &wordOfDocId:词项-文档id索引映射>
//@return <true/false>
bool Inv_index::inPutMenu(
	vector<string> &fileNameDirectory,
	map<string, int> &wordOfFrequency,
	map<string, vector<int>> &wordOfDocId)
{
	while (1) 
	{
		cout << "Select function you want to use:" << endl
			<< "1.Print InvertedIndex" << endl
			<< "2.Search words in InvertedIndex" << endl
			<< "3.Exit" << endl;

		string selection;
		cin >> selection; //输入选项

		switch (convertStrToInt(selection))
		{
		case 1:
			printInvertedIndex(wordOfFrequency, wordOfDocId); //打印倒排索引表
			break;
		case 2:
			SearchWithAND(wordOfDocId, fileNameDirectory); //AND查询
			break;
		case 3:
			return 0; //跳出循环，结束程序
		default:
			cout << "Input Error! Please try again!" << endl;
			break;
		}

		system("PAUSE");
		system("CLS");
		
	}
	
	return true;
}

/***************************************************private********************************************************/

//@name <Inv_index::convertCharToStr>
//@brief <将char类型的字符串转换为string类型>
//@param <char *s_char:文件名目录>
//@return <转换后string字符串>
string Inv_index::convertCharToStr(char *s_char)
{
	stringstream convertCharToStr; //stringstream类实现转换
	string s_string;

	convertCharToStr << s_char; 

	s_string = convertCharToStr.str(); //char类型转换为string类型

	return s_string; //返回转换结果
}

//@name <Inv_index::convertStrToInt>
//@brief <将string类型的字符串转换为整型数>
//@param <char *s_char:文件名目录>
//@return <转换后整型数>
int Inv_index::convertStrToInt(string n_str)
{
	stringstream convertStrToInt; //stringstream类实现转换
	int n_int;

	convertStrToInt << n_str;
	convertStrToInt >> n_int; //将string类型转换为int类型

	return n_int; //返回转换结果
}


//@name <Inv_index::convertStrToInt>
//@brief <添加文档id到词项-文档id索引映射>
//@param <string word:词项>
//       <int docId:文档id>
//       <map<string, vector<int>> &wordOfDocId:词项-文档id索引映射>
//@return <true/false>
bool Inv_index::determineDocIdInDictionary(string word, int docId, map<string, vector<int>> &wordOfDocId)
{
	bool criterion = false;	//初始化判据
	docId += 1; //输入的docId（即i）与实际的docId相差1
	vector<int> docId_temp; //当wordOfDocId为空时临时存储插入值

	if (wordOfDocId.empty()) 
	{ //第一种情况：wordOfDocId没有键值对写入，即初始状态
		docId_temp.push_back(docId);
		wordOfDocId.insert(pair<string, vector<int>>(word, docId_temp));
		criterion = true;
	}
	else if (!wordOfDocId.empty() && wordOfDocId[word].empty()) 
	{ //第二种情况：已有键值对写入，docId没有写入vector
		wordOfDocId[word].push_back(docId);
		criterion = true;
	}
	else if (!wordOfDocId.empty() && !wordOfDocId[word].empty())
	{ //第三种情况：已有键值对写入，判断vector中的docId是否重复
		if (wordOfDocId[word].back() != docId)
		{
			wordOfDocId[word].push_back(docId);
			criterion = true;
		}
		criterion = false;
	}

	docId_temp.clear(); //清空临时插入值

	return criterion; //返回判据
}
