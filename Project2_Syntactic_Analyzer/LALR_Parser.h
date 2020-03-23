#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include "libs.h"
using namespace std;

struct LALRItem // ��Ŀ
{
	Production prod;
	int dot; // ��һ��Ҫ�ƽ��ķ��ŵ��±�
	set<string> token;
	bool operator == (const LALRItem &itm) const {
		if (prod != itm.prod || dot != itm.dot) return false;
		return true;
	}
	bool operator != (const LALRItem& itm) const {
		return !(*this == itm);
	}
};

struct LALRItems // ��Ŀ��
{
	vector<LALRItem> items;
	set<int> indexofReducedItems; // ��Լ��Ŀ
	bool operator == (const LALRItems& itms) const {
		if (items != itms.items)return false;
		return true;
	}
	bool operator != (const LALRItems& itms) const {
		return !(*this == itms);
	}
};
class LALR_Parser
{

public:
	string extendStartNonterminal; // ��չ����ʼ���ս��
	vector<Production> prods; // ����ʽ
	map<string, set<string>> firstofNonterminal; // ���ս���� first ��
	string blank; // �հ׷���
	string end; // ��������
	vector<string> terminal; // �ս����
	map<string, bool>canBeBlank;
	vector<LALRItems> itemsList; // ��Ŀ��
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
	LALRItems getNextItems(LALRItems& preItems, string token);
	void getClousureofItems(LALRItems& items); // ����Ŀ���ıհ�
	void inputBNF(const char* BNFPath); // �����ķ��Ķ����ķ�
										// ���ļ�����action��goto��
	void inputTable(const char* gotoListPath, const char* actionListPath);
	// ���action��goto��
	void outputTable(const char* gotoListPath, const char* actionListPath);
	vector<string> split(string str, string pattern);
};

