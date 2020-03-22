#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Parser.h"
using namespace std;

int main() {
	Parser p;



	p.inputBNF("testII.txt");
	p.getFirstSetofNonterminal();
	p.creatTable();
	//p.inputTable("gotolist.txt", "actionlist");
	p.outputTable("gotolist.txt", "actionlist.txt");
 	p.parse("testresult.txt");
	return 0;
}