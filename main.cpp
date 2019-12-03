#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "fractalTerrain.cpp"

FractalTerrain *terrain;


void display(){

}

void init(int lod, double roughness){
  FractalTerrain tempTerrain = FractalTerrain(lod, roughness);
  terrain = &tempTerrain;
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
