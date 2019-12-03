#include "terrian.cpp"
#include "math.h"

class TranscendentalTerrain: public Terrain{

private:
  double alpha, beta;

public:
  TranscendentalTerrain(double alpha, double beta){
    this->alpha = alpha;
    this->beta = beta;
  }

  double getAltitude(double i, double j){
    return 0.5 + 0.5 * sin(i * alpha) * cos(j * beta);
  }

  RGB getColor (double i, double j) {
    return  RGB(.5 + .5 * sin (i * alpha),.5 - .5 * cos (j * beta), 0.0);
  }

};
