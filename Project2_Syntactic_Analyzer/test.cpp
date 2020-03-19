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
	p.parse("testresult.txt");
	return 0;
}
//p.terminal.push_back("a");
//p.terminal.push_back("b");
//p.terminal.push_back("c");
//p.terminal.push_back("d");
//p.terminal.push_back("e");

//Production prod1;
//prod1.left = "S'";
//prod1.right.push_back("S");
//p.prods.push_back(prod1);

//Production prod2;
//prod2.left = "S";
//prod2.right.push_back("a");
//prod2.right.push_back("A");
//prod2.right.push_back("c");
//prod2.right.push_back("B");
//prod2.right.push_back("e");
//p.prods.push_back(prod2);

//Production prod3;
//prod3.left = "A";
//prod3.right.push_back("b");
//p.prods.push_back(prod3);

//Production prod4;
//prod4.left = "A";
//prod4.right.push_back("A");
//prod4.right.push_back("b");
//p.prods.push_back(prod4);

//Production prod5;
//prod5.left = "B";
//prod5.right.push_back("d");
//p.prods.push_back(prod5);


//vector<string> s;
//s.push_back("a");
//s.push_back("b");
//s.push_back("b");
//s.push_back("c");
//s.push_back("d");
//s.push_back("e");
//s.push_back("#");