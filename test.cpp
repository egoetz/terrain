/* test.cpp
 * test class for checking RGB and transcendentalTerrian classes
 * Erika Goetz and Aedan Pettit
 */

#ifndef TEST
#define TEST

#include <iostream>
#include <bitset>
#include <chrono>
#include <random>
#include <typeinfo>
#include "terrain.cpp"
#include "transcendentalTerrain.cpp"
#include "fractalTerrain.cpp"

using namespace std;

#define PI 3.14159265

//A class to test various classes from our program
class Test {
    private:
        const unsigned int seed = chrono::system_clock::now().
        time_since_epoch().count();
        minstd_rand rand;

        /* Test the RGB class
         * Preconditions: None
         * Postconditions: Prints report as to whether tests for .add(),
         * .subtract(), .scaling(), and .toInt() work to stdout.
         */
        void RGBtest() {
            bool isTestSuccess = false;

            cout << "Testing RGB Class..." << endl;
            short unsigned int ints[6];
            for (int i = 0; i < 6; i++) {
                ints[i] = rand() % 255;
            }
            RGB test1(ints[0], ints[1], ints[2]);
            RGB test2(ints[3], ints[4], ints[5]);

            RGB added = test1.add(test2);
            if (added.getRed() == ints[0] + ints[3] and added.getGreen() == ints[1] + ints[4] and added.getBlue() == ints[2] + ints[5]) {
                isTestSuccess = true;
            }
            cout << "\tPassed Addition Test?\t\t" << isTestSuccess << endl;
            if (not isTestSuccess) {
                cout << "\t\tExpected: (" << ints[0] + ints[3] << ", " <<
                    ints[1] + ints[4] << ", " <<
                    ints[2] + ints[5] << ")" << endl;
                cout << "\t\tReceived: (" << added.getRed() << ", " <<
                    added.getGreen() << ", " <<
                    added.getBlue() << ")" << endl;
            }

            isTestSuccess = false;
            RGB subtracted = test1.subtract(test2);
            if (subtracted.getRed() == ints[0] - ints[3] and subtracted.getGreen() == ints[1] - ints[4] and subtracted.getBlue() == ints[2] - ints[5]) {
                isTestSuccess = true;
            }
            cout << "\tPassed Subtraction Test?\t" << isTestSuccess << endl;
            if (not isTestSuccess) {
                cout << "\t\tExpected: (" << ints[0] - ints[3] << ", " <<
                    ints[1] - ints[4] << ", " <<
                    ints[2] - ints[5] << ")" << endl;
                cout << "\t\tReceived: (" << subtracted.getRed() << ", " <<
                    subtracted.getGreen() << ", " <<
                    subtracted.getBlue() << ")" << endl;
            }

            isTestSuccess = false;
            int scaleFactor = rand() % 500;
            RGB scaled = test1.scale(scaleFactor);
            if (scaled.getRed() == ints[0] * scaleFactor and scaled.getGreen() == ints[1] * scaleFactor and scaled.getBlue() == ints[2] * scaleFactor) {
                isTestSuccess = true;
            }
            cout << "\tPassed Scaling Test?\t\t" << isTestSuccess << endl;
            if (not isTestSuccess) {
                cout << "\t\tExpected: (" << ints[0] * scaleFactor << ", " <<
                    ints[0] * scaleFactor << ", " <<
                    ints[0] * scaleFactor << ")" <<
                    endl;
                cout << "\t\tReceived: (" << scaled.getRed() << ", " <<
                    scaled.getGreen() << ", " <<
                    scaled.getBlue() << ")" << endl;
            }

            isTestSuccess = false;
            bitset < 32 > bits = test1.toRGB();
            bitset < 32 > alpha = 255 << 24;
            bitset < 32 > red = ints[0] << 16;
            bitset < 32 > green = ints[1] << 8;
            bitset < 32 > blue = ints[2];
            bitset < 32 > expected = alpha | red | green | blue;
            if (bits == expected) {
                isTestSuccess = true;
            }
            cout << "\tPassed Conversion Test?\t\t" << isTestSuccess << endl;
            if (not isTestSuccess) {
                cout << "\t\tExpected: " << expected << endl;
                cout << "\t\tReceived: " << bits << endl;
            }
        }


        /* Test the TranscendentalTerrain class
         * Preconditions: None
         * Postconditions: Prints report as to whether tests for .getAltitude()
         * work.
         */
        void transcendentalTest() {
            cout << "Testing transcendentalTerrain Class..." << endl;

            int a, b = 0;
            TranscendentalTerrain testTerrain(a, b);
            bool isTestSuccess = false;
            int denominator = rand();
            int numerator = rand() % denominator;
            double x = numerator / denominator;
            denominator = rand();
            numerator = rand() % denominator;
            double z = numerator / denominator;
            double altitude = testTerrain.getAltitude(x, z);
            if (altitude == 0.5) {
                isTestSuccess = true;
            }
            cout << "\tPassed Altitute Test 1?\t\t" << isTestSuccess << endl;
            if (not isTestSuccess) {
                cout << "\t\tExpected: " << 0.5 << endl;
                cout << "\t\tReceived: " << altitude << endl;
            }

            a, b = 90 * PI / 180;
            TranscendentalTerrain testTerrain2(a, b);
            isTestSuccess = false;
            denominator = rand();
            numerator = rand() % denominator;
            x = numerator / denominator;
            denominator = rand();
            numerator = rand() % denominator;
            z = numerator / denominator;
            float altitude2 = testTerrain.getAltitude(x, z);
            if (altitude2 == 0.5) {
                isTestSuccess = true;
            }
            cout << "\tPassed Altitute Test 2?\t\t" << isTestSuccess << endl;
            if (not isTestSuccess) {
                cout << "\t\tExpected: " << 0.5 << endl;
                cout << "\t\tReceived: " << altitude << endl;
            }

        }

    public:
        /* Runs tests written to private section of class
         * Preconditions: None
         * Postconditions: Prints test reports.
         */
        Test() {
            rand.seed(seed);
            RGBtest();
            transcendentalTest();
        }
};

/* Runs tests
 * Preconditions: None
 * Postconditions: Prints test reports.
 */
int main() {
    Test();
}

#endif
