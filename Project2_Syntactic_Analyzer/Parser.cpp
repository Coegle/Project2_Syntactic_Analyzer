#include "Parser.h"
#include <set>
#include <algorithm>
#include "Parser.h"
#include <iostream>
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
	itms_tmp = getClousureofItems(itms_tmp);
	itemsList.push_back(itms_tmp);

	for (int i = 0; i<itemsList.size(); i++) { // 对于任一项目集
		for (int j = 0; j < terminal.size(); j++) { // 对于所有终结符
			Items nextItms = getNextItems(itemsList[i], terminal[j]);
			if (nextItms.items.size() == 0) continue;
			auto hasExisted = find(itemsList.begin(), itemsList.end(), nextItms); // 是否已经存在
			if (hasExisted == itemsList.end()) {
				itemsList.push_back(nextItms);
				// shift
				actionList[i][terminal[j]] = make_pair(SHIFT, itemsList.size() - 1);
			}
			
		}
		for (auto it_nontml = firstofNonterminal.begin(); it_nontml != firstofNonterminal.end(); it_nontml++) { // 对所有非终结符
			Items nextItms = getNextItems(itemsList[i], it_nontml->first);
			if (nextItms.items.size() == 0) continue;
			auto hasExisted = find(itemsList.begin(), itemsList.end(), nextItms); // 是否已经存在
			if (hasExisted == itemsList.end()) {
				itemsList.push_back(nextItms);
				// goto
				gotoList[i][it_nontml->first] = itemsList.size() - 1;
			}
			
		}
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

void Parser::parse(vector<string>& input) 
{
	stateStack.push(0);
	symbolStack.push(end);
	int index = 0;
	int errorflag = 0;
	while (errorflag == 0) {
		pair<int, int> action = actionList[stateStack.top()][input[index]];
		Production reducedProd;
		switch (action.first)
		{
		case ACC:
			cout << "parse succeed" << endl;
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
			symbolStack.push(input[index]);
			stateStack.push(action.second);
			index++;
			break;
		default:
			cout << "error" << endl;
			errorflag = 1;
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
	nextItms = getClousureofItems(nextItms);
	return nextItms;
}

// 求项目集的闭包
Items Parser::getClousureofItems(Items items)
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
					//else { // 存在但first集不是子集
					//	set<string> tokenUnion = hasExisted->token;
					//	tokenUnion.insert(tmp.token.begin(), tmp.token.end());
					//	
					//	if (tokenUnion != hasExisted->token) {
					//		hasExisted->token.insert(tmp.token.begin(), tmp.token.end());
					//		change = 1;
					//	}
					//	
					//}
				}
			}
		}
	} while (change);

	return items;
}
