#include <QDebug>
#include "calculator.h"
#include <math.h>

double Calculator::calcDistance(qint16 rssi) const
{
    double power = (-rssi - 30 + 28 - 20 * log10(f)) / N;
    double path = pow(10, power);
    return path;
}

Point Calculator::calcMultilateration(QList<DistanceToBeacon> distances)
{
    logMessage("Calculator::calcMultilateration");
    if(distances.length() < 3)
    {
        logMessage("ERROR: Less than three beacon's registered!");
        return {0,0};
    }
    int num_of_dist_above_zero = 0;
    for(DistanceToBeacon dtb : distances)
    {
        if(dtb.distance > 0)
        {
            num_of_dist_above_zero++;
            if(num_of_dist_above_zero > 2)
            {
                break;
            }
        }
    }
    if(num_of_dist_above_zero < 3)
    {
        logMessage("ERROR Less than three beacon's founded!");
        return {0,0};
    }
    m_distances = distances;
    m_last_distance = m_distances.last();
    calcCMatrix();
    calcCATMatrix();
    calcBMatrix();
    return calcPosition();
}

Point Calculator::calcWeightedArithMean(QList<DistanceToBeacon> distances)
{
    logMessage("Calculator::calcWeightedArithMean");
    m_distances = distances;
    Point point{0,0};
    double sum_of_distances = 0;
    for(DistanceToBeacon distance: m_distances)
    {
        point.x += distance.point.x*distance.distance;
        point.y += distance.point.y*distance.distance;
        sum_of_distances += distance.distance;
    }
    point.x /= sum_of_distances;
    point.y /= sum_of_distances;
    return point;
}

void Calculator::calcCMatrix()
{
    double a = 0, b = 0, d = 0;
    Point last_point = m_last_distance.point;
    for(DistanceToBeacon distance: m_distances)
    {
        Point point = distance.point;
        a += (point.x - last_point.x)*(point.x - last_point.x);
        b += (point.x - last_point.x)*(point.y - last_point.y);
        d += (point.y - last_point.y)*(point.y - last_point.y);
    }
    a *= 4;
    b *= 4;
    d *= 4;
    double diff = a*d - b*b;
    c_matrix.clear();
    c_matrix.append(d/diff);
    c_matrix.append(-b/diff);
    c_matrix.append(a/diff);
}

void Calculator::calcCATMatrix()
{
    cat_matrix.clear();
    Point last_point = m_last_distance.point;
    for(DistanceToBeacon distance: m_distances)
    {
        Point point = distance.point;
        cat_matrix.append(c_matrix[0] * 2 * (point.x - last_point.x) + c_matrix[1] * 2 * (point.y - last_point.y));
    }
    for(DistanceToBeacon distance: m_distances)
    {
        Point point = distance.point;
        cat_matrix.append(c_matrix[1] * 2 * (point.x - last_point.x) + c_matrix[2] * 2 * (point.y - last_point.y));
    }
}

void Calculator::calcBMatrix()
{
    b_matrix.clear();
    Point last_point = m_last_distance.point;
    for(DistanceToBeacon distance: m_distances)
    {
        Point point = distance.point;
        b_matrix.append(point.x*point.x - last_point.x*last_point.x
                        + point.y*point.y - last_point.y*last_point.y
                        + m_last_distance.distance*m_last_distance.distance
                        - distance.distance*distance.distance);
    }
}

Point Calculator::calcPosition()
{
    double x = 0, y = 0;
    int size = b_matrix.size();
    if(cat_matrix.size() != 2*size)
    {
        logMessage("Calculator::calcPosition::ERROR!!!");
        return {0,0};
    }
    for(int i = 0; i < size; ++i)
    {
        x += cat_matrix[i]*b_matrix[i];
        y += cat_matrix[size+i]*b_matrix[i];
    }
    return {x,y};
}
