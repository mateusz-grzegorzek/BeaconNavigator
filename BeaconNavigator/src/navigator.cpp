#include <QDebug>
#include "navigator.h"

void Navigator::run()
{
    m_navigate = true;

    while(m_navigate)
    {
        QThread::sleep(2);
        if(!m_beacons->getDevice()->getScanState())
        {
            break;
        }
        estimation_type type = m_beacons->getEstimationType();
        if(type == multilateration)
        {
            m_position = m_calculator->calcMultilateration(m_beacons->getDistances());
        }
        else if(type == weightedArithMean)
        {
            m_position = m_calculator->calcWeightedArithMean(m_beacons->getDistances());
        }
        else
        {
            logMessage("ERROR: Unknown estimation chosen.");
            m_position = {0,0};
        }
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
    logMessage("Navigator::turnOff");
    m_navigate = false;
}
