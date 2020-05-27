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
//@brief <������ǰĿ¼�����е��ĵ��ļ�����.txt�ļ���д���ļ���Ŀ¼>
//@param <vector<string> &fileNameDirectory:�ļ���Ŀ¼>
//@return <true/false>
bool Inv_index::searchFileInDirectory(vector<string> &fileNameDirectory)
{
	long handle;	//���ڲ��ҵľ��
	const char* to_search = "*.txt";	//ָ���ļ����� 
	struct _finddata_t fileinfo;    //�ļ���Ϣ�Ľṹ��
	

	handle = _findfirst(to_search, &fileinfo);    //��һ�β���
	if (-1 == handle) {
		cout << "Error in searchFileInDirectory()" << endl;
		return false;
	}

	//cout << fileinfo.name << endl;    //��ӡ���ҵ����ļ����ļ����������Դ�
	fileNameDirectory.push_back(convertCharToStr(fileinfo.name));
	
	while (!_findnext(handle, &fileinfo)) {    //ѭ�������������ϵ��ļ���ֱ���Ҳ��������ļ�Ϊֹ
		//cout << fileinfo.name << endl;    //��ӡ���ҵ����ļ����ļ����������Դ�
		fileNameDirectory.push_back(convertCharToStr(fileinfo.name));
	}
	
	_findclose(handle);
	return true;
}

//@name <Inv_index::wordExtractor>
//@brief <��ȡ��������.txt�ļ������ݣ�����д���ĵ����ݼ���>
//@param <vector<string> &fileNameDirectory:�ļ���Ŀ¼>
//		 <vector<string>& documentCollection:�ĵ����ݼ���>
//@return <true/false>
bool Inv_index::wordExtractor(vector<string> &fileNameDirectory,vector<string>& documentCollection)
{
	fstream infile;

	for (unsigned int i = 0;i < fileNameDirectory.size();i++)
	{
		infile.open(fileNameDirectory[i]);   //���ļ����������ļ��������� 
		
		if (!infile.is_open())   //��ʧ��,�����������Ϣ,����ֹ�������� 
		{
			cout << "Error in opening the"<< fileNameDirectory[i] << endl;
			return false;
		}

		istreambuf_iterator<char> begin(infile), end; //��������������������ȡȫ������
		string strdata(begin, end);

		//cout << strdata << endl; //��ӡ��ȡ�������ݣ������Դ�
		documentCollection.push_back(strdata);

		infile.close(); //�ر��ļ�������
	}
	           
	return true;
}

//@name <Inv_index::wordAnalysis>
//@brief <�����ĵ����ݼ����е����ݣ���ȡ���еĴ����Ӧ�Ĵ�Ƶ�Լ��ĵ�id����>
//@param <vector<string>& documentCollection:�ĵ����ݼ���>
//		 <map<string, int> &wordOfFrequency:����-��Ƶӳ��>
//		 <map<string, vector<int>> &wordOfDocId:����-�ĵ�id����ӳ��>
//@return <true/false>
bool Inv_index::wordAnalysis(
	vector<string>& documentCollection, 
	map<string, int> &wordOfFrequency,
	map<string, vector<int>> &wordOfDocId
	)
{
	string word;

	const char SPACE = ' ', //�ո�
		COMMA = ',', //����
		PERIOD = '.', //���
		QUOTATION = '"'; //����

	for (unsigned int i = 0;i < documentCollection.size();i++)
	{
		transform(
			documentCollection[i].begin(),
			documentCollection[i].end(),
			documentCollection[i].begin(),
			::tolower
		);   //���ַ���ȫ��ת����Сд

		for (auto it : documentCollection[i])
		{
			
			if (it == SPACE || it == COMMA || it == PERIOD || it == QUOTATION )   //��־��һ�����ʵĽ�β
			{
				if (word != "")
				{
					wordOfFrequency[word]++; //�������Ƶ����
					determineDocIdInDictionary(word, i, wordOfDocId); //�������ĵ�id����
				}
				word = "";    //��word��Ϊ��,��ʼ��¼��һ������
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
//@brief <��ӡ���ż�¼��>
//@param <map<string, int> &wordOfFrequency:����-��Ƶӳ��>
//		 <map<string, vector<int>> &wordOfDocId:����-�ĵ�id����ӳ��>
//@return <true/false>
bool Inv_index::printInvertedIndex(
	map<string, int>& wordOfFrequency,
	map<string, vector<int>>& wordOfDocId)
{
	for (auto it = wordOfDocId.begin(); it != wordOfDocId.end(); it++)
	{
		cout << it->first << ":(" << wordOfFrequency[it->first] << ")"; //��ӡ�����Լ���Ƶ
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			cout << "->" << *it2; //��ӡposting
		}
		cout << endl;

	}

	return true;
}

//@name <Inv_index::SearchWithAND>
//@brief <��AND��ʽ����˫�����ѯ>
//@param <map<string, vector<int>> &wordOfDocId:����-�ĵ�id����ӳ��>
//		 <vector<string> &fileNameDirectory:�ļ���Ŀ¼>
//@return <true/false>
bool Inv_index::SearchWithAND(map<string, vector<int>>& wordOfDocId, vector<string> &fileNameDirectory)
{
	string firstWord, secondWord; //����������ѯ����
	vector<int> answerList; //�����������

	cout << "Please enter two words you want to search:" << endl;
	cin >> firstWord >> secondWord;

	if (firstWord.empty() || secondWord.empty())
	{
		cout<<"You must enter two words if you want to search, please enter again:"<<endl;
		cin >> firstWord >> secondWord; //�ж������Ƿ�Ϊ��
	}

	if (wordOfDocId.find(firstWord) != wordOfDocId.end() && wordOfDocId.find(secondWord) != wordOfDocId.end())
	{
		auto firstPoint = wordOfDocId[firstWord].begin();
		auto secondPoint = wordOfDocId[secondWord].begin(); //��ʼ��������

		while (firstPoint != wordOfDocId[firstWord].end() && secondPoint != wordOfDocId[secondWord].end())
		{
			if (*firstPoint == *secondPoint) //���������ͬ��docId����docIdд���������
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
		cout << "There are no result after search in directory." << endl; //δ�ҵ���ѯ���
		return false;
	}


	cout << "firstWord AND secondWord are in ";
	for (auto it : answerList)
	{
		cout << "->" << fileNameDirectory[it-1]<<"("<<it<<")"; //��ӡ��ѯ���
	}
	cout << endl;

	return true;
}

//@name <Inv_index::inPutMenu>
//@brief <��ӡ����ѡ��˵�����������>
//@param <vector<string> &fileNameDirectory:�ļ���Ŀ¼>
//		 <map<string, int> &wordOfFrequency:����-��Ƶӳ��>
//		 <map<string, vector<int>> &wordOfDocId:����-�ĵ�id����ӳ��>
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
		cin >> selection; //����ѡ��

		switch (convertStrToInt(selection))
		{
		case 1:
			printInvertedIndex(wordOfFrequency, wordOfDocId); //��ӡ����������
			break;
		case 2:
			SearchWithAND(wordOfDocId, fileNameDirectory); //AND��ѯ
			break;
		case 3:
			return 0; //����ѭ������������
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
//@brief <��char���͵��ַ���ת��Ϊstring����>
//@param <char *s_char:�ļ���Ŀ¼>
//@return <ת����string�ַ���>
string Inv_index::convertCharToStr(char *s_char)
{
	stringstream convertCharToStr; //stringstream��ʵ��ת��
	string s_string;

	convertCharToStr << s_char; 

	s_string = convertCharToStr.str(); //char����ת��Ϊstring����

	return s_string; //����ת�����
}

//@name <Inv_index::convertStrToInt>
//@brief <��string���͵��ַ���ת��Ϊ������>
//@param <char *s_char:�ļ���Ŀ¼>
//@return <ת����������>
int Inv_index::convertStrToInt(string n_str)
{
	stringstream convertStrToInt; //stringstream��ʵ��ת��
	int n_int;

	convertStrToInt << n_str;
	convertStrToInt >> n_int; //��string����ת��Ϊint����

	return n_int; //����ת�����
}


//@name <Inv_index::convertStrToInt>
//@brief <����ĵ�id������-�ĵ�id����ӳ��>
//@param <string word:����>
//       <int docId:�ĵ�id>
//       <map<string, vector<int>> &wordOfDocId:����-�ĵ�id����ӳ��>
//@return <true/false>
bool Inv_index::determineDocIdInDictionary(string word, int docId, map<string, vector<int>> &wordOfDocId)
{
	bool criterion = false;	//��ʼ���о�
	docId += 1; //�����docId����i����ʵ�ʵ�docId���1
	vector<int> docId_temp; //��wordOfDocIdΪ��ʱ��ʱ�洢����ֵ

	if (wordOfDocId.empty()) 
	{ //��һ�������wordOfDocIdû�м�ֵ��д�룬����ʼ״̬
		docId_temp.push_back(docId);
		wordOfDocId.insert(pair<string, vector<int>>(word, docId_temp));
		criterion = true;
	}
	else if (!wordOfDocId.empty() && wordOfDocId[word].empty()) 
	{ //�ڶ�����������м�ֵ��д�룬docIdû��д��vector
		wordOfDocId[word].push_back(docId);
		criterion = true;
	}
	else if (!wordOfDocId.empty() && !wordOfDocId[word].empty())
	{ //��������������м�ֵ��д�룬�ж�vector�е�docId�Ƿ��ظ�
		if (wordOfDocId[word].back() != docId)
		{
			wordOfDocId[word].push_back(docId);
			criterion = true;
		}
		criterion = false;
	}

	docId_temp.clear(); //�����ʱ����ֵ

	return criterion; //�����о�
}
