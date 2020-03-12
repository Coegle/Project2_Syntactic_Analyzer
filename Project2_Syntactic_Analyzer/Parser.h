#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;
struct production// ����ʽ
{
	string left;
	vector<string> right;
};

struct item // ��Ŀ
{
	production prod;
	int dot;
	vector<string> token;
};

class Parser
{

public:
	vector<production> prods; // ����ʽ
	map<string, set<string>> firstofNonterminal; // ���ս���� first ��
	string blank; // �հ׷���
	vector<string> terminal; // �ս����
	map<string, bool>canBeBlank;

	void getFirstSetofNonterminal(); // ��ȡ���ս���� first ��
	bool isTerminal(string& s);
	set<string> getFirstSetofStrings(vector<string>& strings);
};

