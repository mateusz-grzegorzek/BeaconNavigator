#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QThread>
#include "beacons.h"
#include "calculator.h"
#include "point.h"

QT_FORWARD_DECLARE_CLASS (Beacons)
QT_FORWARD_DECLARE_CLASS (Calculator)
QT_FORWARD_DECLARE_CLASS (Point)

class Navigator: public QThread
{
    Q_OBJECT
public:
    void run();
    Navigator(Beacons* beacons, Calculator* calculator);
    Point getPosition();
    void turnOff();
private:
    Beacons* m_beacons;
    Calculator* m_calculator;
    Point m_position;
    bool m_navigate;
};

#endif // NAVIGATOR_H
