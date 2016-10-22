#include <QDebug>
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

Point Calculator::calcMultilateration(QList<DistanceToBeacon> distances)
{
    m_distances = distances;
    m_last_distance = m_distances.last();
    calcCMatrix();
    calcCATMatrix();
    return calcPosition();
}

void Calculator::calcCMatrix()
{
    c_matrix.clear();
    double a = 0, b = 0, d = 0;
    Point last_point = m_last_distance.point;
    for(DistanceToBeacon distance: m_distances)
    {
        Point point = distance.point;
        a += pow(2*(point.x - last_point.x), 2);
        b += 4*(point.x - last_point.x)*(point.y - last_point.y);
        d += pow(2*(point.y - last_point.y), 2);
    }
    double diff = a*d - b*b;
    c_matrix.append(d/diff);
    c_matrix.append(-b/diff);
    c_matrix.append(-b/diff);
    c_matrix.append(a/diff);
    qDebug() << "c_matrix[{0,0}] = " << c_matrix[0];
    qDebug() << "c_matrix[{0,1}] = " << c_matrix[1];
    qDebug() << "c_matrix[{1,0}] = " << c_matrix[2];
    qDebug() << "c_matrix[{1,1}] = " << c_matrix[3];
}

void Calculator::calcCATMatrix()
{
    cat_matrix.clear();
    for(int i = 0; i < m_distances.size(); ++i)
    {

    }
}

Point Calculator::calcPosition()
{
    return {1,1};
}
