#include <iostream>
#include <bitset>
#include "terrian.cpp"
#include "transcendentalTerrain.cpp"
using namespace std;

void RGBtest(){
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

void transcendentalTest(){
  TranscendentalTerrain testT(0.5, 5);
  cout << "Expected: " << "0.567997" << endl;
  cout << "Actual: " << testT.getAltitude(1, 1) << endl;

  cout << "Expected: " << endl;
  cout << "Red: 0.739713\nGreen: 0.358169\nBlue: 0" << endl;
  cout << "Actual: " << endl;
  cout << testT.getColor(1, 1);
}

int main(){
  //RGBtest();
  transcendentalTest();
}
