#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "point.h"
#include <QThread>

class Beacons;

class Navigator: public QThread{
    Q_OBJECT
public:
    Navigator(Beacons* beacons);
    void run();
    void turnOff();
    bool isNavigating();
Q_SIGNALS:
    void positionChanged(Point);
private:
    bool minimumThreeBeaconsFounded();
    Beacons* m_beacons;
    bool m_is_navigating = false;
};

#endif // NAVIGATOR_H
