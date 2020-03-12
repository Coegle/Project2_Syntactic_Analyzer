#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;
struct production// 产生式
{
	string left;
	vector<string> right;
};

struct item // 项目
{
	production prod;
	int dot;
	vector<string> token;
};

class Parser
{

public:
	vector<production> prods; // 产生式
	map<string, set<string>> firstofNonterminal; // 非终结符的 first 集
	string blank; // 空白符号
	vector<string> terminal; // 终结符集
	map<string, bool>canBeBlank;

	void getFirstSetofNonterminal(); // 获取非终结符的 first 集
	bool isTerminal(string& s);
	set<string> getFirstSetofStrings(vector<string>& strings);
};

