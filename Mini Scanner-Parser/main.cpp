#include <iostream>
#include <vector>
using namespace std;

extern vector<pair<int, int>> tokens;
extern int scanner();
extern void grammar();
extern int lookAhead;
extern int k;

int main()
{
	scanner();
	lookAhead = tokens[k].first;
	grammar();
}