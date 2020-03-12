#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Parser.h"
using namespace std;

int main() {
	Parser p;
	p.blank = "$";
	p.terminal.push_back("n");
	p.terminal.push_back("(");
	p.terminal.push_back("+");
	p.terminal.push_back("*");

	production prod1;
	prod1.left = "E";
	prod1.right.push_back("T");
	prod1.right.push_back("R");
	p.prods.push_back(prod1);

	production prod2;
	prod2.left = "R";
	prod2.right.push_back("$");
	p.prods.push_back(prod2);

	production prod3;
	prod3.left = "R";
	prod3.right.push_back("+");
	prod3.right.push_back("E");
	p.prods.push_back(prod3);

	production prod4;
	prod4.left = "T";
	prod4.right.push_back("F");
	prod4.right.push_back("S");
	p.prods.push_back(prod4);

	production prod5;
	prod5.left = "S";
	prod5.right.push_back("$");
	p.prods.push_back(prod5);

	production prod6;
	prod6.left = "S";
	prod6.right.push_back("*");
	prod6.right.push_back("T");
	p.prods.push_back(prod6);

	production prod7;
	prod7.left = "F";
	prod7.right.push_back("n");
	p.prods.push_back(prod7);

	production prod8;
	prod8.left = "F";
	prod8.right.push_back("(");
	prod8.right.push_back("E");
	prod8.right.push_back(")");
	p.prods.push_back(prod8);


	p.getFirstSetofNonterminal();
	return 0;
}