#pragma once
#include <string>
#include <vector>
using namespace std;
struct Production// ²úÉúÊ½
{
	string left;
	vector<string> right;
	bool operator == (const Production& prod) const {
		if (left != prod.left || right != prod.right) return false;
		return true;
	}
	bool operator != (const Production& prod) const {
		return !(*this == prod);
	}
};
const int ACC = 1;
const int SHIFT = 2;
const int REDUCE = 3;