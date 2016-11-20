#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QList>
#include <QPair>
#include "beacons.h"
#include "point.h"
#include "distancetobeacon.h"
#include "logger.h"
#include "loggerinterface.h"

QT_FORWARD_DECLARE_CLASS (Beacons)

typedef QList<double> matrix;

class Calculator: public LoggerInterface
{
public:
    double calcDistance(qint16 rssi) const;
    Point calcMultilateration(QList<DistanceToBeacon> distances);
    Point calcWeightedArithMean(QList<DistanceToBeacon> distances);
private:
    void calcCMatrix(); // (A^T*A)^(-1)
    void calcCATMatrix(); // C*A^T
    void calcBMatrix(); // b
    Point calcPosition();

    Logger* m_logger;
    QList<DistanceToBeacon> m_distances;
    DistanceToBeacon m_last_distance;
    const double f = 2400;
    const int N = 30;
    matrix c_matrix;
    matrix cat_matrix;
    matrix b_matrix;
};

#endif // CALCULATOR_H
