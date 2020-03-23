#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include "libs.h"
using namespace std;

struct LRItem // ��Ŀ
{
	Production prod;
	int dot; // ��һ��Ҫ�ƽ��ķ��ŵ��±�
	set<string> token;
	bool operator == (const LRItem &itm) const {
		if (prod != itm.prod || dot != itm.dot || token != itm.token) return false;
		return true;
	}
	bool operator != (const LRItem& itm) const {
		return !(*this == itm);
	}
};

struct LRItems // ��Ŀ��
{
	vector<LRItem> items;
	set<int> indexofReducedItems; // ��Լ��Ŀ
	bool operator == (const LRItems& itms) const {
		if (items != itms.items)return false;
		return true;
	}
	bool operator != (const LRItems& itms) const {
		return !(*this == itms);
	}
};

class LR_Parser
{

public:
	string extendStartNonterminal; // ��չ����ʼ���ս��
	vector<Production> prods; // ����ʽ
	map<string, set<string>> firstofNonterminal; // ���ս���� first ��
	string blank; // �հ׷���
	string end; // ��������
	vector<string> terminal; // �ս����
	map<string, bool>canBeBlank;
	vector<LRItems> itemsList; // ��Ŀ��
	// map<string, map<int, int>> adjList; // ��Ŀ��ת��
	map<int, map<string, int>> gotoList;
	map<int, map<string, pair<int, int>>> actionList; // ��ǰ ���� 
	string spliter;

	stack<string> symbolStack; // ����ջ
	stack<int> stateStack; // ״̬ջ

	void getFirstSetofNonterminal(); // ��ȡ���ս���� first ��
	bool isTerminal(string& s);
	set<string> getFirstSetofStrings(vector<string>& strings);
	void parse(const char* tokenListPath);
	void creatTable(); // �����﷨�����γ�Action��Goto��
	LRItems getNextItems(LRItems& preItems, string token);
	void getClousureofItems(LRItems& items); // ����Ŀ���ıհ�
	void inputBNF(const char* BNFPath); // �����ķ��Ķ����ķ�
	void outputTable(const char* gotoListPath, const char* actionListPath);
	void inputTable(const char* gotoListPath, const char* actionListPath);
	vector<string> split(string str, string pattern);
};

