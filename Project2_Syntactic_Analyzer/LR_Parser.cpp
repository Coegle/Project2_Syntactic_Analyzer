#include "LR_Parser.h"
#include <set>
#include <algorithm>
#include <fstream>
#include <iostream>
//�ַ����ָ��
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
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

// ����ս����first��
void LR_Parser::getFirstSetofNonterminal() {
	int change = 0;
	firstofNonterminal[blank].insert(blank);
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
			if (*it1 == blank) {
				continue;
			}
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
bool LR_Parser::isTerminal(string& s)
{
	if (find(terminal.begin(), terminal.end(), s) == terminal.end()) {
		return false;
	}
	return true;
}

// ��string��first��
set<string> LR_Parser::getFirstSetofStrings(vector<string>& strings)
{
	set<string> firstSet;
	auto it = strings.begin(); // �Ҳ��һ��
	while (it != strings.end()) {
		if (isTerminal(*it) || *it == end) {
			firstSet.insert(*it);
			break;
		}
		firstSet.insert(firstofNonterminal[*it].begin(), firstofNonterminal[*it].end());
		if (firstSet.count(blank) != 0) {
			firstSet.erase(blank);
		}
		if (canBeBlank[*it] == false) { 
			break;
		}
		if (it - strings.begin() + 1 == strings.size() && canBeBlank[*it] == true) {// �����һ���ҿ����Ƴ���
			firstSet.insert(blank);
		}
		it++;
	}
	return firstSet;
}

// �����﷨�����γ�Action��Goto��
void LR_Parser::creatTable()
{
	
	Items itms_tmp;
	Item itm_tmp;
	// ���ӳ�ʼ���
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
			else {
				actionList[i][terminal[j]] = make_pair(SHIFT, hasExisted - itemsList.begin());
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
			} else {
				gotoList[i][it_nontml->first] = hasExisted - itemsList.begin();
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

void LR_Parser::parse(const char* tokenListPath) 
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
			if (reducedProd.right.size() == 1 && reducedProd.right[0] == blank) {
			} else {
				for (int i = 0; i < reducedProd.right.size(); i++) {
					stateStack.pop();
					symbolStack.pop();
				}
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

// ����һ����Ŀ��
Items LR_Parser::getNextItems(Items& preItems, string token)
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
	getClousureofItems(nextItms);
	return nextItms;
}

// ����Ŀ���ıհ�
void LR_Parser::getClousureofItems(Items& items)
{
	int change;
	do {
		change = 0;
		for (int i = 0; i < items.items.size(); i++) { // ����ÿһ��
			if (items.items[i].prod.right.size() > items.items[i].dot && firstofNonterminal.find(items.items[i].prod.right[items.items[i].dot]) != firstofNonterminal.end()) { // ����Ƿ��ս��
				for (int prod = 0; prod < prods.size(); prod++) {
					if (prods[prod].left != items.items[i].prod.right[items.items[i].dot]) continue;
					Item tmp;
					int flag = 0; // ��¼�Ƿ��пղ���ʽ�����Ĺ�Լ��Ŀ
					if (prods[prod].right.size() == 1 && prods[prod].right[0] == blank) { // �ղ���ʽֱ�ӱ�Ϊ��Լ��Ŀ
						tmp.dot = 1;
						flag = 1;
					}
					else {
						tmp.dot = 0;
					}
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
						if (flag == 1) { // �ղ���ʽ�Ĺ�Լ��Ŀ
							items.indexofReducedItems.insert(items.items.size() - 1);
						}
						change = 1;
					}
				}
			}
		}
	} while (change);

}


void LR_Parser::inputBNF(const char* BNFPath)
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
// ���ļ�����action��goto��
void LR_Parser::inputTable(const char* gotoListPath, const char* actionListPath)
{
	ifstream fingoto(gotoListPath, std::ios::in);
	if (fingoto.fail()) {
		cout << "Could not open gotolist file!" << endl;
		exit(0);
	}
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
	if (finaction.fail()) {
		cout << "Could not open actionlist file!" << endl;
		exit(0);
	}
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
	finaction.close();
}

// ���action��goto��
void LR_Parser::outputTable(const char* gotoListPath, const char* actionListPath)
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