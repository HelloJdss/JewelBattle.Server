#include <iostream>
#include "MyMath.cpp"

using std::cout;
using std::cin;

int main()
{
    cout << "Hello world\n";
    cout << "Input your number: ";
    int a, b;
    cin >> a >> b;
    cout << max(a, b) << "\n";
    return 0;
}

