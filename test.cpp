#include <iostream>
#include <bitset>
#include "terrian.cpp"
using namespace std;

int main(){
    RGB test1(1, 1, 1);
    RGB test2(2, 2, 2);
    
    cout << "Expected" << endl;
    cout << "Red: 3\nGreen: 3\nBlue 3\n";
    cout << "Actual" << endl;
    cout << test1.add(test2) << endl;
    
    
    cout << "Expected" << endl;
    cout << "Red: -1\nGreen: -1\nBlue -1\n";
    cout << "Actual" << endl;
    cout << test1.subtract(test2) << endl;
    
    
    cout << "Expected" << endl;
    cout << "Red: 4\nGreen: 4\nBlue 4\n";
    cout << "Actual" << endl;
    cout << test1.scale(4) << endl;
    
    
    cout << "Expected" << endl;
    cout << "Red: 3\nGreen: 3\nBlue 3\n";
    cout << "Actual" << endl;
    bitset<32> bits = test1.toRGB();
    cout << bits << endl;
}
