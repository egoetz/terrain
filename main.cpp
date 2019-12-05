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
int lod;
double roughness;


void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(2.0, 2.0, 2.0,
            0, 0, 0,
            0, 1, 0);

  glBegin(GL_TRIANGLES);
    for(int i = 0; i < numTriangles; i ++){
      RGB color = triangles[i].getColor();
      Triple vertex1 = map[triangles[i].geti()[0]][triangles[i].getj()[0]];
      Triple vertex2 = map[triangles[i].geti()[1]][triangles[i].getj()[1]];
      Triple vertex3 = map[triangles[i].geti()[2]][triangles[i].getj()[2]];
      glColor3f(color.getR(), color.getG(), color.getB());
      glVertex3f(vertex1.getX(), vertex1.getY(), vertex1.getZ());
      glVertex3f(vertex2.getX(), vertex2.getY(), vertex2.getZ());
      glVertex3f(vertex3.getX(), vertex3.getY(), vertex3.getZ());
    }

  glEnd();

  glutSwapBuffers();
}

// Causes segfault - haven't investigated why that happens
void computeNormalsAndLighting(){
  double ambient = 0.5;
  double diffuse = 4.0;

  normals.resize(steps + 1);
  for(int i = 0; i <= steps; i++){
    normals[i].resize(steps + 1);
  }

  for(int i = 0; i <= steps; i++){
    for(int j = 0; j <= steps; j++){
      normals[i][j] = Triple();
    }
  }

  //Triple sun = Triple(3.6, 3.9, 0.6);
  Triple sun = Triple(-3.0, 4.0, 2.0);

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

  vector<vector<double> > shade;
  shade.resize(steps + 1);
  for(int i = 0; i <= steps; i++){
    shade[i].resize(steps + 1);
    for(int j = 0; j <= steps; j++){
      shade[i][j] = 1.0;
      Triple vertex = map[i][j];
      Triple ray = sun.subtract(vertex);
      double distance = steps * sqrt(ray.getX() * ray.getX() + ray.getZ() * ray.getZ());
      for(double place = 1.0; place < distance; place += 1.0){
        Triple sample = vertex.add(ray.scale(place/distance));
        double sx = sample.getX();
        double sy = sample.getY();
        double sz = sample.getZ();
        if((sx < 0.0) || (sx > 1.0) || (sz < 0.0) || (sz > 1.0)){
          break;
        }

        double ground = roughness * terrain->getAltitude(sx, sz);
        if(ground > sy){
          shade[i][j] = (2 * sy)/ground;
          break;
        }

      }
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
      double shadow = shade[k][l];
      double lighting;
      if(dot < 0.0){
        lighting = ambient + diffuse * (dot / (distance2 * shadow));
      }
      else{
        lighting = ambient;
      }
      color = color.scale(lighting);

      avg = avg.add(color);
    }
    //cout << avg.scale(3.0/4.0) << endl;
    triangles[i].setColor(avg.scale(3.0/4.0));
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
  int triangle = -1;
  for(int m = 0; m < steps; m++){
    for(int n = 0; n < steps; n++){
      triangle++;
      triangles[triangle] = Triangle(m, n, m + 1, n, m, n + 1);
      triangle++;
      triangles[triangle] = Triangle(m +1, n, m + 1, n + 1, m, n + 1);
    }
  }


  computeNormalsAndLighting();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

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

  cout << "Enter an integer level of detail: ";
  cin >> lod;
  cout << "Enter a number between 0 and 1 for roughness: ";
  cin >> roughness;

  init(lod, roughness);


  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);


  glutMainLoop();
  return 0;
}
