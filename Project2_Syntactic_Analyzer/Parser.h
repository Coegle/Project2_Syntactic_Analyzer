#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
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
		if (prod != itm.prod || dot != itm.dot || token != itm.token) return false;
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


class Parser
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
	const int ACC = 1;
	const int SHIFT = 2;
	const int REDUCE = 3;

	void getFirstSetofNonterminal(); // 获取非终结符的 first 集
	bool isTerminal(string& s);
	set<string> getFirstSetofStrings(vector<string>& strings);
	void parse();
	Items getNextItems(Items& preItems, string token);
	Items getClousureofItems(Items items); // 求项目集的闭包
};

