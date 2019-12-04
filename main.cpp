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
vector<vector<Triple> > map, normals;
vector<vector<RGB> > colors;
vector<Triangle> triangles;

int steps, numTriangles;


void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(2, 2, 2,
            0, 0, 0,
            0, 1, 0);

  glBegin(GL_TRIANGLES);
    for(int i = 0; i < numTriangles; i ++){
      RGB color1 = colors[triangles[i].geti()[0]][triangles[i].getj()[0]];
      Triple vertex1 = map[triangles[i].geti()[0]][triangles[i].getj()[0]];

      RGB color2 = colors[triangles[i].geti()[1]][triangles[i].getj()[1]];
      Triple vertex2 = map[triangles[i].geti()[1]][triangles[i].getj()[1]];

      RGB color3 = colors[triangles[i].geti()[2]][triangles[i].getj()[2]];
      Triple vertex3 = map[triangles[i].geti()[2]][triangles[i].getj()[2]];
      glColor3f(color1.getR(), color1.getG(), color1.getB());
      glVertex3f(vertex1.getX(), vertex1.getY(), vertex1.getZ());
      glColor3f(color2.getR(), color2.getG(), color2.getB());
      glVertex3f(vertex2.getX(), vertex2.getY(), vertex2.getZ());
      glColor3f(color3.getR(), color2.getG(), color3.getB());
      glVertex3f(vertex3.getX(), vertex3.getY(), vertex3.getZ());
    }

  glEnd();

  glutSwapBuffers();
}

// Causes segfault - haven't investigated why that happens
void computeNormalsAndLighting(){
  double ambient = 0.3;
  double diffuse = 4.0;

  normals.resize(steps+1);
  for(int i = 0; i < normals.size(); i++){
    normals[i].resize(steps+1);
  }

  for(int i = 0; i < numTriangles; i++){
    for(int j = 0; j < 3; j++){
      normals[i][j] = Triple();
    }
  }

  Triple sun = Triple(3.6, 3.9, 0.6);

  // Compute triangle normals and vertex averaged normals
  for(int i = 0; i < numTriangles; i++){
    Triple v0 = map[triangles[i].geti()[0]][triangles[i].getj()[0]];
    Triple v1 = map[triangles[i].geti()[1]][triangles[i].getj()[1]];
    Triple v2 = map[triangles[i].geti()[2]][triangles[i].getj()[2]];
    Triple normal = v0.subtract(v1).cross(v2.subtract(v1)).normalize();
    triangles[i].setNorm(normal);

    for(int j = 0; j < 3; j++){
      normals[triangles[i].geti()[j]][triangles[i].getj()[j]] =
        normals[triangles[i].geti()[j]][triangles[i].getj()[j]].add(normal);
    }
  }

  // Compute vertex colors and triangle average colors
  for(int i = 0; i < numTriangles; i++){
    RGB avg = RGB();
    for(int j = 0; j < 3; j++){
      int k = triangles[i].geti()[j];
      int l = triangles[i].getj()[j];
      Triple vertex = map[k][l];
      RGB color = colors[k][l];
      Triple normal = normals[k][l].normalize();
      Triple light = vertex.subtract(sun);
      double distance2 = light.length2();
      double dot = light.normalize().dot(normal);
      double lighting;
      if(dot < 0.0){
        lighting = -dot / distance2;
      }
      else{
        lighting = 0.0;
      }

      color = color.scale(lighting);
      // Left out triangles[i].color[j] = color;
      // I didn't understand what it was doing - not sure if it makes sense in C++
      // Java has a Color class so maybe it could use this but it does not appear to be
      // Compatible with our RGB class
      avg = avg.add(color);
    }

    triangles[i].setColor(avg.scale(1.0/3.0));
  }

}

void reshape(int w, int h){
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, GLfloat(w) / GLfloat(h),
    1.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void init(int lod, double roughness){
  FractalTerrain tempTerrain = FractalTerrain(lod, roughness);
  terrain = &tempTerrain;
  double x, z, altitude;


  steps = 1 << lod;
  numTriangles = steps * steps * 2;

  // Resize outer vectors
  map.resize(steps + 1);
  colors.resize(steps + 1);
  triangles.resize(numTriangles);

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
  int triangle = 0;
  for(int m = 0; m < steps; m++){
    for(int n = 0; n < steps; n++){
      triangles[triangle] = Triangle(m, n, m + 1, n, m, n + 1);
      triangle++;
      triangles[triangle] = Triangle(m +1, n, m + 1, n + 1, m, n + 1);
      triangle++;
    }
  }

  //computeNormalsAndLighting();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  reshape(400, 300);
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
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);


  glutMainLoop();
  return 0;
}
