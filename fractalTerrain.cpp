#ifndef F_TERRAIN
#define F_TERRAIN

#include "terrain.cpp"
#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include <iostream>
using namespace std;

class FractalTerrain: public Terrain{
  private:
    vector<vector<double> > terrain;
    double roughness, min, max;
    int divisions;
    const unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
    minstd_rand rand;


    void diamond (int x, int y, int side, double scale) {
        if (side > 1) {
          int half = side / 2;
          double avg = (terrain[x][y] + terrain[x + side][y] +
                    terrain[x + side][y + side] + terrain[x][y + side]) * 0.25;
          terrain[x + half][y + half] = avg + rnd () * scale;
        }
    }

    void square (int x, int y, int side, double scale) {
        int half = side / 2;
        double avg = 0.0, sum = 0.0;
        if (x >= 0)
            { avg += terrain[x][y + half]; sum += 1.0; }
        if (y >= 0)
            { avg += terrain[x + half][y]; sum += 1.0; }
        if (x + side <= divisions)
            { avg += terrain[x + side][y + half]; sum += 1.0; }
        if (y + side <= divisions)
            { avg += terrain[x + half][y + side]; sum += 1.0; }
        terrain[x + half][y + half] = avg / sum + rnd () * scale;
    }

    double rnd () {
        return 2. * rand() - 1.0;
    }

    RGB blue = RGB (0.0, 0.0, 1.0);
    RGB green = RGB (0.0, 1.0, 0.0);
    RGB white = RGB (1.0, 1.0, 1.0);


  public:
    FractalTerrain (int lod, double roughness) {
      rand.seed(seed);
      this->roughness = roughness;
      this->divisions = 1 << lod;
      terrain.resize(divisions + 1);
      for (int i = 0; i < divisions + 1; ++i) terrain[i].resize(divisions + 1);

      terrain[0][0] = rnd ();
      terrain[0][divisions] = rnd ();
      terrain[divisions][divisions] = rnd ();
      terrain[divisions][0] = rnd ();

      double rough = roughness;
      for (int i = 0; i < lod; ++ i) {
        int r = 1 << (lod - i), s = r >> 1;
        for (int j = 0; j < divisions; j += r)
          for (int k = 0; k < divisions; k += r)
            diamond (j, k, r, rough);
        if (s > 0)
          for (int j = 0; j <= divisions; j += s)
            for (int k = (j + s) % r; k <= divisions; k += r)
              square (j - s, k - s, r, rough);
        rough *= roughness;
      }

      min = max = terrain[0][0];
      for (int i = 0; i <= divisions; ++ i)
        for (int j = 0; j <= divisions; ++ j)
          if (terrain[i][j] < min) min = terrain[i][j];
          else if (terrain[i][j] > max) max = terrain[i][j];
      }

      double getAltitude (double i, double j) {
        double alt = terrain[(int) (i * divisions)][(int) (j * divisions)];
        return (alt - min) / (max - min);
      }

      RGB getColor (double i, double j) {
        double a = getAltitude (i, j);
        if (a < .5)
          return blue.add (green.subtract (blue).scale ((a - 0.0) / 0.5));
        else
          return green.add (white.subtract (green).scale ((a - 0.5) / 0.5));
      }
};

class Triple{
  private:
    double x, y, z;
  public:
    Triple(double x, double y, double z){
      this->x = x;
      this->y = y;
      this->z = z;
    }

    Triple(){
      this->x = 0;
      this->y = 0;
      this->z = 0;
    }

    Triple add(Triple t){
      return Triple(x + t.x, y + t.y, z + t.z);
    }

    Triple subtract(Triple t){
      return Triple(x - t.x, y - t.y, z - t.z);
    }

    Triple cross(Triple t){
      double newX = (y * t.z) - (z * t.y);
      double newY = (z * t.x) - (x * t.z);
      double newZ = (x * t.y) - (y * t.x);
      return Triple(newX, newY, newZ);
    }

    double dot(Triple t){
      return (x * t.x) + (y * t.y) + (z * t.z);
    }

    double length2(){
      return dot(*this);
    }

    Triple scale(double scale){
      return Triple(x * scale, y * scale, z * scale);
    }

    Triple normalize(){
      return scale(1.0 / sqrt(length2()));
    }

    Triple& operator=(const Triple& t){
      x = t.x;
      y = t.y;
      z = t.z;
      return *this;
    }
};


class Triangle{
  private:
    int i[3];
    int j[3];
    //Triple n;
    //RGB rgb[3];
    //Color color;

  public:
    Triangle(int i0, int j0, int i1, int j1, int i2, int j2){
      i[0] = i0;
      i[1] = i1;
      i[2] = i2;
      j[0] = j0;
      j[1] = j1;
      j[2] = j2;
    }
};

#endif
