#include <iostream>
#include <vector>

using namespace std;

vector <string> vec;
string in;

int main()
{
	cout << "This program will read your input until you say STOP." << endl;
	while (true)
	{
		cout << "Enter a string." << endl;
		cin >> in;
		if (in == "STOP") break;
		vec.push_back(in);
	}
	cout << "Your inputs will now be displayed in reverse." << endl;
	for (int i = vec.size() - 1; i >= 0; i--) cout << vec[i] << endl;
	vec.clear();
	return 0;
}
