/* terrain.cpp
 * Terrian class and RGB class
 * Erika Goetz and Aedan Pettit
 */

#ifndef TERRAIN
# define TERRAIN

#include <ostream>
#include <iostream>

using namespace std;

// Class used to store colors
class RGB {
    private:
        double r, g, b; // red green and blue
        
        // Used to convert a double value into an integer
        int toInt(double value) {
            if (value < 0.0) {
                return 0;
            } else if (value > 1.0) {
                return 255;
            } else {
                return (int)(value * 255.0);
            }
        }

    public:
        /* Default constructor
         * Preconditions: None
         * Postconditions: Creates RGB with values of 0
         */
        RGB() {
            this->r = 0;
            this->g = 0;
            this->b = 0;
        }

        /* RGB constructor
         * Preconditions: doubles representing red, green, and blue
         * Postconditions: Sets the r, g, and b values respectively
         */
        RGB(double r, double g, double b) {
                this->r = r;
                this->g = g;
                this->b = b;
            }
            
            // Destructor - trivial (no dynamic memory allocation)
            ~RGB() {};

        /* Copy constructor 
         * Preconditions: RGB to copy
         * Postconditions: copies values of rgb to this RGB
         */
        RGB(const RGB & rgb) {
            this->r = rgb.r;
            this->g = rgb.g;
            this->b = rgb.b;
        }

        /* Add RGBs
         * Preconditions: RGB to add
         * Postconditions: Adds values of rgb to this RGB
         */
        RGB add(RGB rgb) {
            return RGB(r + rgb.r, g + rgb.g, b + rgb.b);
        }

        /* Subtract RGBs
         * Preconditions: RGB to add
         * Postconditions: Subtracts values of rgb from this RGB
         */
        RGB subtract(RGB rgb) {
            return RGB(r - rgb.r, g - rgb.g, b - rgb.b);
        }

        /* Scale an RGB
         * Preconditions: scaling factor
         * Postconditions: Multiplies the values of this RGB by scale
         */
        RGB scale(double scale) {
            return RGB(r * scale, g * scale, b * scale);
        }

        /* bitwise RGB conversion
         * Preconditions: None
         * Postconditions: returns binary integer representing RGB values
         */        
        int toRGB() {
            return (255 << 24) | (toInt(this->r) << 16) | (toInt(this->g) << 8) | toInt(this->b);
        }

        /* Get the R value
         * Preconditions: None
         * Postconditions: Returns double represnting red portion of RGB
         */
        double getR() {
            return this->r;
        }

        /* Get the G value
         * Preconditions: None
         * Postconditions: Returns double represnting green portion of RGB
         */
        double getG() {
            return this->g;
        }

        /* Get the B value
         * Preconditions: None
         * Postconditions: Returns double represnting blue portion of RGB
         */
        double getB() {
            return this->b;
        }

        /* << operator overload
         * Preconditions: RGB to print
         * Postconditions: Returns ostream object to enable printing of RGB
         */
        friend ostream & operator << (ostream & output,
            const RGB & color) {
            output << "Red: " << color.r << "\n";
            output << "Green: " << color.g << "\n";
            output << "Blue: " << color.b << "\n";
            return output;
        }

        /* Assignment operator overload
         * Preconditions: RGB values to assign from
         * Postconditions: Assigned values from rgb to this RGB
         */
        RGB & operator = (const RGB & rgb) {
            this->r = rgb.r;
            this->g = rgb.g;
            this->b = rgb.b;
            return *this;
        }
};

// Abstract base class used for implementing terrains
class Terrain {
    public:
        // pure virtual function
        double getAltitude(double i, double j);
        RGB getColor(double i, double j);
};

#endif
