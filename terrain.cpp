/*
 * Terrian class and RGB class
 * Erika Goetz and Aedan Pettit
*/

#ifndef TERRAIN
#define TERRAIN

#include <ostream>
#include <iostream>
using namespace std;

class RGB {
  private:
    double r, g, b;
    int toInt (double value) {
        if(value < 0.0){
            return 0;
        }
        else if(value > 1.0){
            return 255;
        }
        else{
          return (int) (value * 255.0);
        }
    }

  public:

      RGB(){
       this->r = 0;
       this->g = 0;
       this->b = 0;
     }

      RGB (double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
      }

      ~RGB(){};

      double getRed () {
        return this->r;
      }

      double getGreen () {
        return this->g;
      }

      double getBlue () {
        return this->b;
      }

      RGB(const RGB &rgb){
        this->r = rgb.r;
        this->g = rgb.g;
        this->b = rgb.b;
      }

      RGB add (RGB rgb) {
        return RGB (r + rgb.r, g + rgb.g, b + rgb.b);
      }

      RGB subtract (RGB rgb) {
        return RGB (r - rgb.r, g - rgb.g, b - rgb.b);
      }

      RGB scale (double scale) {
        return RGB (r * scale, g * scale, b * scale);
      }

      int toRGB () {
        return (255 << 24) | (toInt(this->r) << 16) | (toInt(this->g) << 8) | toInt(this->b);
      }

      double getR(){
        return this->r;
      }

      double getG(){
        return this->g;
      }

      double getB(){
        return this->b;
      }

      friend ostream &operator<<( ostream &output, const RGB &color ) {
         output << "Red: " << color.r << "\n";
         output << "Green: " << color.g << "\n";
         output << "Blue: " << color.b << "\n";
         return output;
      }

      RGB& operator=(const RGB& rgb){
        this->r = rgb.r;
        this->g = rgb.g;
        this->b = rgb.b;
        return *this;
      }
};

class Terrain {
   public:
      // pure virtual function
      double getAltitude (double i, double j);
      RGB getColor (double i, double j);
};

#endif
