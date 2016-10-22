#include <QDebug>
#include "navigator.h"

void Navigator::run()
{
    m_navigate = true;
    while(m_navigate)
    {
        QThread::sleep(2);
        m_position = m_calculator->calcMultilateration(m_beacons->getDistances());
        m_beacons->updatePosition();
    }
}

Navigator::Navigator(Beacons* beacons, Calculator* calculator)
    :m_beacons(beacons), m_calculator(calculator), m_navigate(false)
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
