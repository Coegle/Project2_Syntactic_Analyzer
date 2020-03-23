#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Parser.h"
using namespace std;

int main() {
	Parser p;



	p.inputBNF("Chomsky_II.txt");
	p.getFirstSetofNonterminal();
	//p.creatTable();
	//p.outputTable("gotolist.txt", "actionlist.txt");
	p.inputTable("gotolist.txt", "actionlist.txt");
	p.parse("testresult.txt");
	return 0;
}