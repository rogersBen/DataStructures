
#include <iostream>

using namespace std;




int main() {
	
	int a = 339;

    char b = a;

    char c = (char)a;

    int d = (int)b;

    int e = a & 0xff;

    cout << a << " " << b << " " << c << " " << d << " " << e << endl;

	return 0;
}