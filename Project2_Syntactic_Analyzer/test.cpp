#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "LALR_Parser.h"
using namespace std;

int main() {
	LALR_Parser lalr_p;


	lalr_p.inputBNF("LALR(1)\\Chomsky_II.txt");
	lalr_p.getFirstSetofNonterminal();
	//lalr_p.creatTable();
	//lalr_p.outputTable("gotolist.txt", "actionlist.txt");
	lalr_p.inputTable("LALR(1)\\gotolist.txt", "LALR(1)\\actionlist.txt");
	lalr_p.parse("LALR(1)\\testresult.txt");
	return 0;
}