#include "calculator.h"
#include <math.h>

Calculator::Calculator()
{

}

double Calculator::calcDistance(qint16 rssi)
{
    double power = (-rssi - 30 + 28 - 20 * log10(f)) / N;
    double path = pow(10, power);
    return path;
}
