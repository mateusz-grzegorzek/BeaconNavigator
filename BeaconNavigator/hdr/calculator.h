#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "point.h"
#include "distancetobeacon.h"
#include <QObject>
#include <QList>
#include <QMap>
#include <QString>

class Beacons;

typedef QList<double> matrix;

class Calculator{
public:
    static double calcDistance(const qint16& rssi);
    static bool calcMultilateration(Point& position, QList<DistanceToBeacon> distances);
    static bool calcWeightedArithMean(Point& position, QList<DistanceToBeacon> distances);
    static void applyFilter(Point& point);
    static void calcMedian(const QString& mac_address, qint16& rssi);
private:
    static void calcCMatrix(); // (A^T*A)^(-1)
    static void calcCATMatrix(); // C*A^T
    static void calcBMatrix(); // b
    static bool calcPosition(Point &position);

    static QList<DistanceToBeacon> m_distances;
    static DistanceToBeacon m_last_distance;

    static matrix c_matrix;
    static matrix cat_matrix;
    static matrix b_matrix;

    static QList<Point> m_last_points;
    static QMap<QString, QList<qint16>> m_median_cache;
};

#endif // CALCULATOR_H
