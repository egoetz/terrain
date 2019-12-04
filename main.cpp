#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include "fractalTerrain.cpp"
using namespace std;

FractalTerrain *terrain;
// Vectors containing coordinates and colors
vector<vector<Triple> > map;
vector<vector<RGB> > colors;


void display(){


}

void init(int lod, double roughness){
  FractalTerrain tempTerrain = FractalTerrain(lod, roughness);
  terrain = &tempTerrain;
  double x, z, altitude;

  int steps = 1 << lod;

  // Resize outer vectors
  map.resize(steps + 1);
  colors.resize(steps + 1);

  // Resize inner vectors
  for(int k = 0; k <= steps; k++){
    map[k].resize(steps+1);
    colors[k].resize(steps+1);
  }

  // Fill map and color vectors
  for(int i = 0; i <= steps; i++){
    for(int j = 0; j <= steps; j++){
      x = 1.0 * i / steps;
      z = 1.0 * j / steps;
      altitude = terrain->getAltitude(x, z);
      map[i][j] = Triple(x, altitude * roughness, z);
      colors[i][j] = terrain->getColor(x,z);
    }
  }
}

/* Allow the user to quit with 'esc'
 * Preconditions: glutKeyboardFunc(keyboard) must be called
 * Postconditions: The user can exit the program by pressing the 'esc' key
*/
void keyboard(unsigned char key, int x, int y){
  switch(key){
    case 27:
      exit(0);
      break;
  }
}

int main(int argc, char **argv){
  glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Terrain");

  int lod;
  double roughness;

  cout << "Enter an integer level of detail:\t";
  cin >> lod;
  cout << "Enter a number between 0 and 1 for roughness:\t";
  cin >> roughness;

  init(lod, roughness);


  glutDisplayFunc(display);


  glutMainLoop();
  return 0;
}
