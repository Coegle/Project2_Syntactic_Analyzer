#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
using namespace std;

struct Production// 产生式
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

struct Item // 项目
{
	Production prod;
	int dot; // 下一个要移进的符号的下标
	set<string> token;
	bool operator == (const Item &itm) const {
		if (prod != itm.prod || dot != itm.dot) return false;
		return true;
	}
	bool operator != (const Item& itm) const {
		return !(*this == itm);
	}
};

struct Items // 项目集
{
	vector<Item> items;
	set<int> indexofReducedItems; // 规约项目
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
	string extendStartNonterminal; // 扩展的起始非终结符
	vector<Production> prods; // 产生式
	map<string, set<string>> firstofNonterminal; // 非终结符的 first 集
	string blank; // 空白符号
	string end; // 结束符号
	vector<string> terminal; // 终结符集
	map<string, bool>canBeBlank;
	vector<Items> itemsList; // 项目集
	// map<string, map<int, int>> adjList; // 项目集转换
	map<int, map<string, int>> gotoList;
	map<int, map<string, pair<int, int>>> actionList; // 当前 输入 
	string spliter;

	stack<string> symbolStack; // 符号栈
	stack<int> stateStack; // 状态栈

	void getFirstSetofNonterminal(); // 获取非终结符的 first 集
	bool isTerminal(string& s);
	set<string> getFirstSetofStrings(vector<string>& strings);
	void parse(const char* tokenListPath);
	void creatTable(); // 进行语法分析形成Action和Goto表
	Items getNextItems(Items& preItems, string token);
	void getClousureofItems(Items& items); // 求项目集的闭包
	void inputBNF(const char* BNFPath); // 输入文法的二型文法
										// 从文件读入action和goto表
	void inputTable(const char* gotoListPath, const char* actionListPath);
	// 输出action和goto表
	void outputTable(const char* gotoListPath, const char* actionListPath);
};

