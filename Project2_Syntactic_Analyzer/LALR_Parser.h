#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
using namespace std;

struct Production// ����ʽ
{
	string left;
	vector<string> right;
	bool operator == (const Production& prod) const {
		if (left != prod.left || right != prod.right) return false;
		return true;
	}
	bool operator != (const Production& prod) const {
		return !(*this == prod);
	}
};

struct Item // ��Ŀ
{
	Production prod;
	int dot; // ��һ��Ҫ�ƽ��ķ��ŵ��±�
	set<string> token;
	bool operator == (const Item &itm) const {
		if (prod != itm.prod || dot != itm.dot) return false;
		return true;
	}
	bool operator != (const Item& itm) const {
		return !(*this == itm);
	}
};

struct Items // ��Ŀ��
{
	vector<Item> items;
	set<int> indexofReducedItems; // ��Լ��Ŀ
	bool operator == (const Items& itms) const {
		if (items != itms.items)return false;
		return true;
	}
	bool operator != (const Items& itms) const {
		return !(*this == itms);
	}
};
std::vector<std::string> split(std::string str, std::string pattern);
const int ACC = 1;
const int SHIFT = 2;
const int REDUCE = 3;
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
	vector<Items> itemsList; // ��Ŀ��
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
	Items getNextItems(Items& preItems, string token);
	void getClousureofItems(Items& items); // ����Ŀ���ıհ�
	void inputBNF(const char* BNFPath); // �����ķ��Ķ����ķ�
										// ���ļ�����action��goto��
	void inputTable(const char* gotoListPath, const char* actionListPath);
	// ���action��goto��
	void outputTable(const char* gotoListPath, const char* actionListPath);
};
