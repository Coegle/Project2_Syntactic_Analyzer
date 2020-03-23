#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "LR_Parser.h"
using namespace std;

int main() {
	LR_Parser lr_p;
	lr_p.inputBNF("LR(1)\\Chomsky_II.txt");
	lr_p.getFirstSetofNonterminal();
	//lr_p.creatTable();
	//lr_p.outputTable("LR(1)\\gotolist.txt", "LR(1)\\actionlist.txt");
	lr_p.inputTable("LR(1)\\gotolist.txt", "LR(1)\\actionlist.txt");
	lr_p.parse("LR(1)\\testresult.txt");

	return 0;
}