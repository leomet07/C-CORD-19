#include <iostream>
#include <string.h>

using namespace std;
int main()
{
    string str1 = "Apples are red";
    string str2 = str1.substr(11, 3);
    string str3 = str1.substr(0, 6);

    cout << "Substring starting at position 11 and length 3 is: " << str2 << endl;
    cout << "Substring starting at position 0 and length 6 is: " << str3;
    return 0;
}