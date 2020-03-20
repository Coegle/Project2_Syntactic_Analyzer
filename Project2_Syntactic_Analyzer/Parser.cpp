#include "Parser.h"
#include <set>
#include <algorithm>
#include <fstream>
#include <queue>
#include "Parser.h"
#include <iostream>
using namespace std;
//字符串分割函数
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			i = pos + pattern.size() - 1;
			if (s.size() != 0) {
				result.push_back(s);
			}
		}
	}
	return result;
}

// 求非终结符的first集
void Parser::getFirstSetofNonterminal() {
	int change = 0;
	
	for (auto it = prods.begin(); it != prods.end(); it++) {
		if (it->right.size() == 1 && it->right[0] == blank) {
			canBeBlank[it->left] = true;
			firstofNonterminal[it->left].insert(blank);
			continue;
		}
		for (auto it2 = it->right.begin(); it2 != it->right.end(); it2++) {
			if (isTerminal(*it2)) {
				canBeBlank[it->left] = false;
				break;
			}
		}
	}
	do {
		change = 0;
		for (auto it = prods.begin(); it != prods.end(); it++) {
			if (canBeBlank.find(it->left) != canBeBlank.end())continue;
			for (auto it2 = it->right.begin(); it2 != it->right.end(); it2++) { // 对右侧非终结符逐个识别
				if (canBeBlank.find(*it2) != canBeBlank.end() && canBeBlank[*it2] == false) { // 在右侧找到任意一个确定的非空终结符
					canBeBlank[it->left] = false;
					change = 1;
					break;
				}
			}
		}
	} while (change); // 直到稳定，则剩下的都是可以推出空的非终结符

	for (auto it = prods.begin(); it != prods.end(); it++) {
		canBeBlank.insert(make_pair(it->left, true));
		if(canBeBlank[it->left]) firstofNonterminal[it->left].insert(blank);
	}
	

	do {
		change = 0;
		for (auto it_prod = prods.begin(); it_prod != prods.end(); it_prod++) {
			auto it1 = it_prod->right.begin(); // 产生式右部
			if (*it1 == blank) continue;
			while (it1 != it_prod->right.end()) {
				if (isTerminal(*it1)) { // 是终结符
					if (firstofNonterminal[it_prod->left].count(*it1) == 0) {
						firstofNonterminal[it_prod->left].insert(*it1);
						change = 1;
					}
					break;
				}
				else { // 是非终结符
					for (auto it2 = firstofNonterminal[*it1].begin(); it2 != firstofNonterminal[*it1].end(); it2++) {
						if (firstofNonterminal[it_prod->left].count(*it2) == 0) {
							firstofNonterminal[it_prod->left].insert(*it2);
							change = 1;
						}
					}
					if (canBeBlank[it_prod->left] == false) {
						firstofNonterminal[it_prod->left].erase(blank);
						break;
					}
				}
				it1++;
			}
		}
	} while (change);
}

// 判断是否是终结符
bool Parser::isTerminal(string& s)
{
	if (find(terminal.begin(), terminal.end(), s) == terminal.end()) {
		return false;
	}
	return true;
}

// 求string的first集
set<string> Parser::getFirstSetofStrings(vector<string>& strings)
{
	set<string> firstSet;
	auto it = strings.begin(); // 右侧第一项
	while (it != strings.end()) {
		if (isTerminal(*it) || *it == end) {
			firstSet.insert(*it);
			break;
		}
		for (auto it2 = firstofNonterminal[*it].begin(); it2 != firstofNonterminal[*it].end(); it2++) {
			firstSet.insert(*it2);
		}
		if (canBeBlank[*it] == false) break;
		it++;
	}
	return firstSet;
}

// 进行语法分析形成Action和Goto表
void Parser::creatTable()
{
	
	Items itms_tmp;
	Item itm_tmp;
	// 添加初始结点
	for (auto it_prod = prods.begin(); it_prod != prods.end(); it_prod++) {
		if (it_prod->left == extendStartNonterminal) {
			itm_tmp.dot = 0;
			itm_tmp.token.insert(end);
			itm_tmp.prod = *it_prod;
			break;
		}
	}
	itms_tmp.items.push_back(itm_tmp);
	getClousureofItems(itms_tmp);
	itemsList.push_back(itms_tmp);

	queue<int> q;
	q.push(0);
	while (q.size())
	{
		int i = q.front();
		q.pop();

		for (int j = 0; j < terminal.size(); j++) { // 对于所有终结符
			Items nextItms = getNextItems(itemsList[i], terminal[j]);
			if (nextItms.items.size() == 0) continue;
			auto hasExisted = find(itemsList.begin(), itemsList.end(), nextItms); // 是否已经存在
			if (hasExisted == itemsList.end()) {
				itemsList.push_back(nextItms);
				q.push(itemsList.size() - 1);
				// shift
				actionList[i][terminal[j]] = make_pair(SHIFT, itemsList.size() - 1);
			}
			else {
				int change = 0;
				for (auto it_in = nextItms.items.begin(); it_in != nextItms.items.end(); it_in++) {

					auto it_it = find(hasExisted->items.begin(), hasExisted->items.end(), *it_in);
					int tmp_size = it_it->token.size();
					it_it->token.insert(it_in->token.begin(), it_in->token.end());

					if (it_it->token.size() != tmp_size) {
						change = 1;
						
					}
				}
				if (change) {
					q.push(hasExisted - itemsList.begin());
				}
				actionList[i][terminal[j]] = make_pair(SHIFT, hasExisted - itemsList.begin());
			}
		}
		for (auto it_nontml = firstofNonterminal.begin(); it_nontml != firstofNonterminal.end(); it_nontml++) { // 对所有非终结符
			Items nextItms = getNextItems(itemsList[i], it_nontml->first);
			if (nextItms.items.size() == 0) continue;
			auto hasExisted = find(itemsList.begin(), itemsList.end(), nextItms); // 是否已经存在
			if (hasExisted == itemsList.end()) {
				itemsList.push_back(nextItms);
				q.push(itemsList.size() - 1);
				// goto
				gotoList[i][it_nontml->first] = itemsList.size() - 1;
			}
			else {
				int change = 0;
				for (auto it_in = nextItms.items.begin(); it_in != nextItms.items.end(); it_in++) {

					auto it_it = find(hasExisted->items.begin(), hasExisted->items.end(), *it_in);
					int tmp_size = it_it->token.size();
					it_it->token.insert(it_in->token.begin(), it_in->token.end());

					if (it_it->token.size() != tmp_size) {
						change = 1;
					}
				}
				if (change) {
					q.push(hasExisted - itemsList.begin());
				}
				gotoList[i][it_nontml->first] = hasExisted - itemsList.begin();
			}
		}
	}
	for (int i = 0; i < itemsList.size(); i++) { // 对于任一项目集
		
	}
	for (int i = 0; i < itemsList.size(); i++) {
		// acc 和 reduce
		for (auto index = itemsList[i].indexofReducedItems.begin(); index != itemsList[i].indexofReducedItems.end(); index++) { // 所有规约项目
			for (auto it_token = itemsList[i].items[*index].token.begin(); it_token != itemsList[i].items[*index].token.end(); it_token++) {
				if (itemsList[i].items[*index].prod.left == extendStartNonterminal) {
					actionList[i][*it_token] = make_pair(ACC, find(prods.begin(), prods.end(), itemsList[i].items[*index].prod) - prods.begin());
				} else {
					actionList[i][*it_token] = make_pair(REDUCE, find(prods.begin(), prods.end(), itemsList[i].items[*index].prod) - prods.begin());
				}
				
			}
		}

	}
}

void Parser::parse(const char* tokenListPath) 
{
	ifstream fin;
	fin.open(tokenListPath);
	if (fin.fail()) {
		cout << "Could not open source file!" << endl;
		exit(0);
	}
	string line;
	

	stateStack.push(0);
	symbolStack.push(end);
	int index = 0;
	int errorflag = 0;

	vector<string> substr;
	getline(fin, line);
	substr = split(line, " ");

	while (errorflag == 0) {
		pair<int, int> action = actionList[stateStack.top()][substr[1]];
		Production reducedProd;
		switch (action.first)
		{
		case ACC:
			cout << "parse succeed" << endl;
			fin.close();
			return;
		case REDUCE:
			reducedProd = prods[action.second];
			for (int i = 0; i < reducedProd.right.size(); i++) {
				stateStack.pop();
				symbolStack.pop();
			}
			symbolStack.push(reducedProd.left);
			stateStack.push(gotoList[stateStack.top()][symbolStack.top()]);
			break;
		case SHIFT:
			symbolStack.push(substr[1]);
			stateStack.push(action.second);
			
			getline(fin, line);
			substr = split(line, " ");
			
			break;
		default:
			cout << "error in line" << substr[0] << endl;
			errorflag = 1;
			fin.close();
			return;
		}	
	}
}

// 求下一个项目集
Items Parser::getNextItems(Items& preItems, string token)
{
	Items nextItms;
	for (auto it_itm = preItems.items.begin(); it_itm != preItems.items.end(); it_itm++) {
		if (it_itm->prod.right.size() > it_itm->dot && it_itm->prod.right[it_itm->dot] == token) { // 不是规约项或接受项，且下一个移进符号符合要求
			
			Item tmp; // 待移入的项目
			tmp.prod = it_itm->prod;
			tmp.dot = it_itm->dot + 1;
			tmp.token = it_itm->token;
			if (tmp.dot >= tmp.prod.right.size()) { // 规约项目标记
				nextItms.indexofReducedItems.insert(nextItms.items.size());
			}
			
			nextItms.items.push_back(tmp);
		}
	}
	// 求闭包直到状态稳定
	getClousureofItems(nextItms);
	return nextItms;
}

// 求项目集的闭包
void Parser::getClousureofItems(Items& items)
{
	int change;
	do {
		change = 0;
		for (int i = 0; i < items.items.size(); i++) { // 对于每一项
			if (items.items[i].prod.right.size() > items.items[i].dot&& firstofNonterminal.find(items.items[i].prod.right[items.items[i].dot]) != firstofNonterminal.end()) { // 如果是非终结符
				for (int prod = 0; prod < prods.size(); prod++) {
					if (prods[prod].left != items.items[i].prod.right[items.items[i].dot]) continue;
					Item tmp;
					tmp.dot = 0;
					tmp.prod = prods[prod];
					vector<string>tmp_token;
					if (items.items[i].prod.right.begin() + items.items[i].dot + 1 != items.items[i].prod.right.end()) { // beta
						tmp_token.insert(tmp_token.begin(), items.items[i].prod.right.begin() + items.items[i].dot + 1, items.items[i].prod.right.end());
					}
					for (auto it_token = items.items[i].token.begin(); it_token != items.items[i].token.end(); it_token++) { 
						tmp_token.push_back(*it_token);
						auto tmp_firstSet = getFirstSetofStrings(tmp_token);
						tmp.token.insert(tmp_firstSet.begin(), tmp_firstSet.end());
						tmp_token.pop_back();
					}
					auto hasExisted = find(items.items.begin(), items.items.end(), tmp); // 项目是否存在
					if (hasExisted == items.items.end()) { // 不存在
						items.items.push_back(tmp);
						change = 1;
					}
					/*以下内容应该被注释，否则会变成LALR（1）同时应该更改Item中判定相等的条件*/
					else { // 存在但first集不是子集
						set<string> tokenUnion = hasExisted->token;
						tokenUnion.insert(tmp.token.begin(), tmp.token.end());
						
						if (tokenUnion != hasExisted->token) {
							hasExisted->token.insert(tmp.token.begin(), tmp.token.end());
							change = 1;
						}
						
					}
				}
			}
		}
	} while (change);

}


void Parser::inputBNF(const char* BNFPath)
{
	ifstream fin;
	fin.open(BNFPath);
	if (fin.fail()) {
		cout << "Could not open BNF file!" << endl;
		exit(0);
	}
	fin >> blank >> extendStartNonterminal >> end >> spliter;
	int tokenNum;
	fin >> tokenNum;
	
	for (int i = 0; i < tokenNum; i++) {
		string tmp;
		fin >> tmp;
		terminal.push_back(tmp);
	}
	fin.ignore();
	string line;
	while (getline(fin, line)) {
		if (line.size() == 0) continue;
		vector<string> tmp = split(line, spliter);
		string tmp_l = tmp[0];
		string tmp_r = tmp[1];
		Production tmp_prod;
		tmp_prod.left = split(tmp_l, " ")[0];
		tmp_prod.right = split(tmp_r, " ");
		prods.push_back(tmp_prod);
	}
	fin.close();

}


// 从文件读入action和goto表
void Parser::inputTable(const char* gotoListPath, const char* actionListPath)
{
	ifstream fingoto(gotoListPath, std::ios::in);
	int num;
	fingoto >> num;
	for (int i = 0; i < num; i++) {
		int num1, state;
		fingoto >> state >> num1;
		for (int j = 0; j < num1; j++) {
			string input;
			int gotostate;
			fingoto >> input >> gotostate;
			gotoList[state][input] = gotostate;
		}
	}
	fingoto.close();

	ifstream finaction(actionListPath, std::ios::in);
	finaction >> num;
	for (int i = 0; i < num; i++) {
		int num1, state;
		finaction >> state >> num1;
		for (int j = 0; j < num1; j++) {
			string input;
			int s1, s2;
			finaction >> input >> s1 >> s2;
			actionList[state][input] = make_pair(s1, s2);
		}
	}
}

// 输出action和goto表
void Parser::outputTable(const char* gotoListPath, const char* actionListPath)
{
	ofstream writer(gotoListPath, std::ios::app);
	writer << gotoList.size() << endl;
	for (auto it = gotoList.begin(); it != gotoList.end(); it++) {
		writer << it->first << " " << it->second.size() << endl;
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			writer << it2->first << " " << it2->second << endl;
		}
	}
	writer.close();

	ofstream writer1(actionListPath, std::ios::app);
	writer1 << actionList.size() << endl;
	for (auto it = actionList.begin(); it != actionList.end(); it++) {
		writer1 << it->first << " " << it->second.size() << endl;
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			writer1 << it2->first << " " << it2->second.first << " " << it2->second.second << endl;
		}
	}
	writer1.close();
}
