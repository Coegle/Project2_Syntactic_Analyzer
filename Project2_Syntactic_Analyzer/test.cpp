#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Parser.h"
using namespace std;

int main() {
	Parser p;
	p.blank = "$";
	p.extendStartNonterminal = "S'";
	p.end = "#";
	p.terminal.push_back("a");
	p.terminal.push_back("b");
	p.terminal.push_back("c");
	p.terminal.push_back("d");
	p.terminal.push_back("e");


	Production prod1;
	prod1.left = "S'";
	prod1.right.push_back("S");
	p.prods.push_back(prod1);

	Production prod2;
	prod2.left = "S";
	prod2.right.push_back("a");
	prod2.right.push_back("A");
	prod2.right.push_back("d");
	p.prods.push_back(prod2);

	Production prod3;
	prod3.left = "S";
	prod3.right.push_back("b");
	prod3.right.push_back("A");
	prod3.right.push_back("c");
	p.prods.push_back(prod3);

	Production prod4;
	prod4.left = "S";
	prod4.right.push_back("a");
	prod4.right.push_back("e");
	prod4.right.push_back("c");
	p.prods.push_back(prod4);

	Production prod5;
	prod5.left = "S";
	prod5.right.push_back("b");
	prod5.right.push_back("e");
	prod5.right.push_back("d");
	p.prods.push_back(prod5);

	Production prod6;
	prod6.left = "A";
	prod6.right.push_back("e");
	p.prods.push_back(prod6);

	p.getFirstSetofNonterminal();
	p.parse();
	return 0;
}