#include "LR_Parser.h"
#include "LALR_Parser.h"
void LRtest() {
	LR_Parser lr_p;
	lr_p.inputBNF("LR(1)\\Chomsky_II.txt");
	lr_p.getFirstSetofNonterminal();
	//lr_p.creatTable();
	//lr_p.outputTable("LR(1)\\gotolist.txt", "LR(1)\\actionlist.txt");
	lr_p.inputTable("LR(1)\\gotolist.txt", "LR(1)\\actionlist.txt");
	lr_p.parse("LR(1)\\testresult.txt");
}

using namespace std;
void LALRtest() {
	LALR_Parser lalr_p;
	lalr_p.inputBNF("LALR(1)\\Chomsky_II.txt");
	lalr_p.getFirstSetofNonterminal();
	//lalr_p.creatTable();
	//lalr_p.outputTable("LALR(1)gotolist.txt", "LALR(1)actionlist.txt");
	lalr_p.inputTable("LALR(1)\\gotolist.txt", "LALR(1)\\actionlist.txt");
	lalr_p.parse("LALR(1)\\testresult.txt");
}

int main() {
	//LALRtest();
	LRtest();
	return 0;
}