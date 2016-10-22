#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QList>
#include <QPair>
#include "beacons.h"
#include "point.h"
#include "distancetobeacon.h"

QT_FORWARD_DECLARE_CLASS (Beacons)

typedef QList<double> matrix;

class Calculator
{
public:
    Calculator();
    double calcDistance(qint16 rssi);
    Point calcMultilateration(QList<DistanceToBeacon> distances);
private:
    void calcCMatrix(); // (A^T*A)^(-1)
    void calcCATMatrix(); // C*A^T
    Point calcPosition();

    QList<DistanceToBeacon> m_distances;
    DistanceToBeacon m_last_distance;
    const double f = 2400;
    const int N = 30;
    matrix c_matrix;
    matrix cat_matrix;
};

#endif // CALCULATOR_H
