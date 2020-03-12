#include "Parser.h"


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

bool Parser::isTerminal(string& s)
{
	if (find(terminal.begin(), terminal.end(), s) == terminal.end()) {
		return false;
	}
	return true;
}

set<string> Parser::getFirstSetofStrings(vector<string>& strings)
{
	set<string> firstSet;
	auto it = strings.begin(); // 产生式左侧第一项
	while (it != strings.end()) {
		if (isTerminal(*it)) {
			firstSet.insert(*it);
			break;
		}
		for (auto it2 = firstofNonterminal[*it].begin(); it2 != firstofNonterminal[*it].end(); it2++) {
			firstSet.insert(*it2);
		}
		if (canBeBlank[*it] == false) break;
	}
	return firstSet;
}

