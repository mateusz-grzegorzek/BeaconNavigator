#include <QDebug>
#include "navigator.h"

void Navigator::run()
{
    m_navigate = true;
    while(m_navigate)
    {
        QThread::sleep(1);
        static int x = 0;
        static int y = 0;
        x++;
        y++;
        m_position.x = x;
        m_position.y = y;
        m_beacons->updatePosition();
    }
}

Navigator::Navigator(Beacons* beacons, Calculator* calculator)
    :m_navigate(false), m_beacons(beacons), m_calculator(calculator)
{

}

Point Navigator::getPosition()
{
    return m_position;
}

void Navigator::turnOff()
{
    qDebug() << "Navigator::turnOff";
    m_navigate = false;
}
