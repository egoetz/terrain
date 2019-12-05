/* fractalTerrain.cpp
 * fractalTerrian, Triple, and Triangle classes
 * Erika Goetz and Aedan Pettit
 */

#ifndef F_TERRAIN
#define F_TERRAIN

#include "terrain.cpp"
#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include <iostream>

using namespace std;

//A class creates a fractal-based terrian map to display in OpenGL
class FractalTerrain: public Terrain {
    private: 
        //Coordinates of terrian
        vector<vector<double>> terrain;
        //How sharp/sudden altitude change, min altitude, max altitude
        double roughness, min, max;
        //How many polygons to create the map from
        int divisions;
        //Seed for random number generator
        const unsigned int seed = chrono::system_clock::now(
                                                ).time_since_epoch().count();
        //Random number generator to simulate arbitrary change in terrain
        minstd_rand rand;

        /* Implement Diamond part of Diamond-Square Algorithm
         * Preconditions: terrain[x][y], terrain[x + side][y],
           terrain[x + side][y + side], and terrain[x][y + side] must be set.
         * Postconditions: 
         *     Return value: None
         *     Side Effects: terrain[x + half][y + half] is filled with a 
         *     variations on the average between x and y. These variations are 
         *     scaled with the scale variable.
        */
        void diamond(int x, int y, int side, double scale) {
            if (side > 1) {
                int half = side / 2;
                double avg = (terrain[x][y] + terrain[x + side][y] +
                    terrain[x + side][y + side] + terrain[x][y + side]) * 0.25;
                terrain[x + half][y + half] = avg + rnd() * scale;
            }
        }

        /* Implement Square part of Diamond-Square Algorithm
         * Preconditions: terrain[x][y + half], terrain[x + half][y], 
         * terrain[x + side][y + half], and terrain[x + half][y + side] must
         * be filled if they are in the bounds of the array terrain.
         * Postconditions: 
         *     Return value: None
         *     Side Effects: terrain[x + half][y + half] is filled with a 
         *     variations on the average between the values mentioned in the
         *     preconditions. These variations are scaled with the scale 
         *     variable.
        */
        void square(int x, int y, int side, double scale) {
            int half = side / 2;
            double avg = 0.0, sum = 0.0;
            if (x >= 0) {
                avg += terrain[x][y + half];
                sum += 1.0;
            }
            if (y >= 0) {
                avg += terrain[x + half][y];
                sum += 1.0;
            }
            if (x + side <= divisions) {
                avg += terrain[x + side][y + half];
                sum += 1.0;
            }
            if (y + side <= divisions) {
                avg += terrain[x + half][y + side];
                sum += 1.0;
            }
            terrain[x + half][y + half] = avg / sum + rnd() * scale;
        }

        /* Get random number
         * Preconditions: None
         * Postconditions: returns a random number
         *     Return value: A random number
         *     Side Effects: None
        */
        double rnd() {
            return 2. * rand() - 1.0;
        }

        RGB blue = RGB(0.0, 0.0, 1.0);
        RGB green = RGB(0.0, 1.0, 0.0);
        RGB white = RGB(1.0, 1.0, 1.0);

    public: 
        /* FractalTerrain constructor
         * Preconditions: Given a level of detail and roughness value
         * Postconditions: Creates a FractalTerrain object with a level of 
         * detail, lod, and roughness of roughness.
        */
        FractalTerrain(int lod, double roughness) {
            rand.seed(seed);
            ( * this).roughness = roughness;
            ( * this).divisions = 1 << lod;
            terrain.resize(divisions + 1);
            for (int i = 0; i < divisions + 1; ++i) terrain[i].resize(divisions + 1);

            terrain[0][0] = rnd();
            terrain[0][divisions] = rnd();
            terrain[divisions][divisions] = rnd();
            terrain[divisions][0] = rnd();

            double rough = roughness;
            for (int i = 0; i < lod; ++i) {
                int r = 1 << (lod - i), s = r >> 1;
                for (int j = 0; j < divisions; j += r)
                    for (int k = 0; k < divisions; k += r)
                        diamond(j, k, r, rough);
                if (s > 0)
                    for (int j = 0; j <= divisions; j += s)
                        for (int k = (j + s) % r; k <= divisions; k += r)
                            square(j - s, k - s, r, rough);
                rough *= roughness;
            }

            min = max = terrain[0][0];
            for (int i = 0; i <= divisions; ++i)
                for (int j = 0; j <= divisions; ++j)
                    if (terrain[i][j] < min) min = terrain[i][j];
                    else if (terrain[i][j] > max) max = terrain[i][j];
        }

        /* calculate Atltitude at a given (x,z) point
         * Preconditions: given an (x,z) point 
         * Postconditions: returns a double representing the y coordinate for
         * the point
        */
        double getAltitude(double i, double j) {
            double alt = terrain[(int)(i * divisions)][(int)(j * divisions)];
            return (alt - min) / (max - min);
        }

        /* get the color at a given (x,z) point
         * Preconditions: given an (x,z) point
         * Postconditions: returns an RGB object with the color for the given
         * point
         */
        RGB getColor(double i, double j) {
            double a = getAltitude(i, j);
            if (a < .5)
                return blue.add(green.subtract(blue).scale((a - 0.0) / 0.5));
            else
                return green.add(white.subtract(green).scale((a - 0.5) / 0.5));
        }
};

// Class representing a point or a vector
class Triple {
    private:
        double x, y, z; // coordinates
    public:
    
        /* Triple constructor
         * Preconditions: Give x, y, and z values
         * Postconditions: Creates triple object w/ given values
         */
        Triple(double x, double y, double z) {
            ( * this).x = x;
            ( * this).y = y;
            ( * this).z = z;
        }

        /* Default constructor
         * Preconditions: None
         * Postconditions: Creates triple object filled with 0s
         */
        Triple() {
            ( * this).x = 0;
            ( * this).y = 0;
            ( * this).z = 0;
        }

        /* Add two triples
         * Preconditions: Given another triple to add 
         * Postconditions: Adds the values of t to the current triple
         */
        Triple add(Triple t) {
            return Triple(x + t.x, y + t.y, z + t.z);
        }

        /* Subtract two triples
         * Preconditions: Given another triple to subtract
         * Postconditions: Subtracts the values of t from the current triple
         */
        Triple subtract(Triple t) {
            return Triple(x - t.x, y - t.y, z - t.z);
        }

        /* Calculate the cross product of two triples
         * Preconditions: Another triple to compute the cross product with
         * Postconditions: Returns the Triple that is the cross product between
         * the current triple and t
         */
        Triple cross(Triple t) {
            double newX = (y * t.z) - (z * t.y);
            double newY = (z * t.x) - (x * t.z);
            double newZ = (x * t.y) - (y * t.x);
            return Triple(newX, newY, newZ);
        }

        /* Calculate the dot product of two triples
         * Preconditions: Another triple to compute the dot product with
         * Postconditions: returns the double that is the dot product of this
         * triple and t
         */
        double dot(Triple t) {
            return (x * t.x) + (y * t.y) + (z * t.z);
        }

        /* Calculate the square of the length of the triple
         * Preconditions: None
         * Postconditions: returns the double representing the square of the
         * length of the triple
         */
        double length2() {
            return dot( * this);
        }

        /* Scale the triple
         * Preconditions: scaling factor
         * Postconditions: returns the Triple with each element multiplied by
         * scale
         */
        Triple scale(double scale) {
            return Triple(x * scale, y * scale, z * scale);
        }

        /* Normalize the triple 
         * Preconditions: None
         * Postconditions: Returns normalized version of the triple
         */
        Triple normalize() {
            return scale(1.0 / sqrt(length2()));
        }

        /* Get the x value of the triple
         * Preconditions: None
         * Postconditions: Returns the x value of the triple
         */
        double getX() {
            return ( * this).x;
        }

        /* Get the y value of the triple
         * Preconditions: None
         * Postconditions: Returns the y value of the triple
         */
        double getY() {
            return ( * this).y;
        }

        /* Get the z value of the triple
         * Preconditions: None
         * Postconditions: Returns the z value of the triple
         */
        double getZ() {
            return ( * this).z;
        }

        /* Copy constructor
         * Preconditions: Triple to copy 
         * Postconditions: Sets this triple's elements to equal the elements of
         * t
         */
        Triple & operator = (const Triple & t) {
            x = t.x;
            y = t.y;
            z = t.z;
            return *this;
        }

        /* << operator overload
         * Preconditions: Triple to print
         * Postconditions: Returns ostream object with values for printing 
         * triple object to console
         */
        friend ostream & operator << (ostream & output,
            const Triple & t) {
            output << "X: " << t.x << "\tY: " << t.y << "\tZ: " << t.z << endl;
            return output;
        }
};

// A class used to store triangles of points
class Triangle {
    private:
        vector < int > i; 
        vector < int > j;
        Triple norm; // normal for the triangle
        vector < RGB > rgb;
        RGB color; // color of triangle

    public:
        /* Triangle constructor
         * Preconditions: given values to fill i and j vectors
         * Postconditions: creates a Triangle object with filled i and j vectors
         */
        Triangle(int i0, int j0, int i1, int j1, int i2, int j2) {
            i.resize(3);
            j.resize(3);
            rgb.resize(3);
            i[0] = i0;
            i[1] = i1;
            i[2] = i2;
            j[0] = j0;
            j[1] = j1;
            j[2] = j2;
        }

        /* Default constructor
         * Preconditions: None
         * Postconditions: creates a triangle filled with 0s
         */
        Triangle() {
            i.resize(3);
            j.resize(3);
            rgb.resize(3);
            i[0] = 0;
            i[1] = 0;
            i[2] = 0;
            j[0] = 0;
            j[1] = 0;
            j[2] = 0;
        }

        /* Sets the normal for the triangle
         * Preconditions: given a Triple representing a normal vector
         * Postconditions: sets the norm of the triangle to n
         */
        void setNorm(Triple n) {
            ( * this).norm = n;
        }

        /* Sets the color of the triangle
         * Preconditiosn: Given an RGB object representing the color of the
         * triangle
         * Postconditions: sets the color of the triangle to color
         */
        void setColor(RGB color) {
            ( * this).color = color;
        }

        /* get the color of the triangle
         * Preconditions: None
         * Postconditions: returns the RGB object representing the color of the
         * triangle
         */
        RGB getColor() {
            return ( * this).color;
        }

        /* get the i values of the triangle
         * Preconditions: None
         * Postconditions: returns a vector containing the i values
         */
        vector < int > geti() {
            return ( * this).i;
        }

        /* get the j values of the triangle
         * Preconditions: None
         * Postconditions: returns a vector containing the j values
         */
        vector < int > getj() {
            return ( * this).j;
        }

};

#endif
