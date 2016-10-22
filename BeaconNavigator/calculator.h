#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>

class Calculator
{
public:
    Calculator();
    double calcDistance(qint16 rssi);
    double calcMultilateration();
private:
    const double f = 2400;
    const int N = 30;
};

#endif // CALCULATOR_H
