/* transcendentalTerrain.cpp
 * transcendentalTerrian class
 * Erika Goetz and Aedan Pettit
 */

#ifndef T_TERRAIN
#define T_TERRAIN

#include "terrain.cpp"
#include "math.h"

// Implements a transcendental terrain
class TranscendentalTerrain: public Terrain {

    private: 
        double alpha,
        beta;

    public: 
        // Constructor - sets values of alpha and beta
        TranscendentalTerrain(double alpha, double beta) {
            this->alpha = alpha;
            this->beta = beta;
        }

        // Returns altitude (y value) at a given (x,z) point
        double getAltitude(double i, double j) {
            return 0.5 + 0.5 * sin(i * alpha) * cos(j * beta);
        }

        // Returns the color at a given (x,z) point
        RGB getColor(double i, double j) {
            return RGB(.5 + .5 * sin(i * alpha), .5 - .5 * cos(j * beta), 0.0);
        }

};

#endif
