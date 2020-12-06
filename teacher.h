#pragma once
#include "reader.h"
using namespace std;

class teacher : public reader {
public:
	teacher(string a, string b, int c, int d, int e, vector <int> f, vector <int> g);
	teacher(string a, string b);
};
