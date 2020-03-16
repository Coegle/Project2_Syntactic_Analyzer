#include "Parser.h"
#include <set>
#include <algorithm>
#include "Parser.h"
#include <iostream>
// ����ս����first��
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
			for (auto it2 = it->right.begin(); it2 != it->right.end(); it2++) { // ���Ҳ���ս�����ʶ��
				if (canBeBlank.find(*it2) != canBeBlank.end() && canBeBlank[*it2] == false) { // ���Ҳ��ҵ�����һ��ȷ���ķǿ��ս��
					canBeBlank[it->left] = false;
					change = 1;
					break;
				}
			}
		}
	} while (change); // ֱ���ȶ�����ʣ�µĶ��ǿ����Ƴ��յķ��ս��

	for (auto it = prods.begin(); it != prods.end(); it++) {
		canBeBlank.insert(make_pair(it->left, true));
		if(canBeBlank[it->left]) firstofNonterminal[it->left].insert(blank);
	}
	

	do {
		change = 0;
		for (auto it_prod = prods.begin(); it_prod != prods.end(); it_prod++) {
			auto it1 = it_prod->right.begin(); // ����ʽ�Ҳ�
			if (*it1 == blank) continue;
			while (it1 != it_prod->right.end()) {
				if (isTerminal(*it1)) { // ���ս��
					if (firstofNonterminal[it_prod->left].count(*it1) == 0) {
						firstofNonterminal[it_prod->left].insert(*it1);
						change = 1;
					}
					break;
				}
				else { // �Ƿ��ս��
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

// �ж��Ƿ����ս��
bool Parser::isTerminal(string& s)
{
	if (find(terminal.begin(), terminal.end(), s) == terminal.end()) {
		return false;
	}
	return true;
}

// ��string��first��
set<string> Parser::getFirstSetofStrings(vector<string>& strings)
{
	set<string> firstSet;
	auto it = strings.begin(); // �Ҳ��һ��
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

// �����﷨�����γ�Action��Goto��
void Parser::creatTable()
{
	
	Items itms_tmp;
	Item itm_tmp;
	// ��ӳ�ʼ���
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

	for (int i = 0; i<itemsList.size(); i++) { // ������һ��Ŀ��
		for (int j = 0; j < terminal.size(); j++) { // ���������ս��
			Items nextItms = getNextItems(itemsList[i], terminal[j]);
			if (nextItms.items.size() == 0) continue;
			auto hasExisted = find(itemsList.begin(), itemsList.end(), nextItms); // �Ƿ��Ѿ�����
			if (hasExisted == itemsList.end()) {
				itemsList.push_back(nextItms);
				// shift
				actionList[i][terminal[j]] = make_pair(SHIFT, itemsList.size() - 1);
			}
			
		}
		for (auto it_nontml = firstofNonterminal.begin(); it_nontml != firstofNonterminal.end(); it_nontml++) { // �����з��ս��
			Items nextItms = getNextItems(itemsList[i], it_nontml->first);
			if (nextItms.items.size() == 0) continue;
			auto hasExisted = find(itemsList.begin(), itemsList.end(), nextItms); // �Ƿ��Ѿ�����
			if (hasExisted == itemsList.end()) {
				itemsList.push_back(nextItms);
				// goto
				gotoList[i][it_nontml->first] = itemsList.size() - 1;
			}
			
		}
		// acc �� reduce
		for (auto index = itemsList[i].indexofReducedItems.begin(); index != itemsList[i].indexofReducedItems.end(); index++) { // ���й�Լ��Ŀ
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

// ����һ����Ŀ��
Items Parser::getNextItems(Items& preItems, string token)
{
	Items nextItms;
	for (auto it_itm = preItems.items.begin(); it_itm != preItems.items.end(); it_itm++) {
		if (it_itm->prod.right.size() > it_itm->dot && it_itm->prod.right[it_itm->dot] == token) { // ���ǹ�Լ�����������һ���ƽ����ŷ���Ҫ��
			
			Item tmp; // ���������Ŀ
			tmp.prod = it_itm->prod;
			tmp.dot = it_itm->dot + 1;
			tmp.token = it_itm->token;
			if (tmp.dot >= tmp.prod.right.size()) { // ��Լ��Ŀ���
				nextItms.indexofReducedItems.insert(nextItms.items.size());
			}
			
			nextItms.items.push_back(tmp);
		}
	}
	// ��հ�ֱ��״̬�ȶ�
	nextItms = getClousureofItems(nextItms);
	return nextItms;
}

// ����Ŀ���ıհ�
Items Parser::getClousureofItems(Items items)
{
	int change;
	do {
		change = 0;
		for (int i = 0; i < items.items.size(); i++) { // ����ÿһ��
			if (items.items[i].prod.right.size() > items.items[i].dot&& firstofNonterminal.find(items.items[i].prod.right[items.items[i].dot]) != firstofNonterminal.end()) { // ����Ƿ��ս��
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
					auto hasExisted = find(items.items.begin(), items.items.end(), tmp); // ��Ŀ�Ƿ����
					if (hasExisted == items.items.end()) { // ������
						items.items.push_back(tmp);
						change = 1;
					}
					/*��������Ӧ�ñ�ע�ͣ��������LALR��1��ͬʱӦ�ø���Item���ж���ȵ�����*/
					//else { // ���ڵ�first�������Ӽ�
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
