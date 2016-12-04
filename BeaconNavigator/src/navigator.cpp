#include "navigator.h"
#include "device.h"
#include "logger.h"
#include "calculator.h"
#include "beacons.h"

Navigator::Navigator(Beacons *beacons): m_beacons(beacons){
    connect(this, SIGNAL(positionChanged(Point)), m_beacons, SLOT(updatePosition(Point)));
    qRegisterMetaType<Point>("Point");
}

void Navigator::run(){
    m_is_navigating = true;
    while(m_is_navigating){
        QThread::sleep(1);
        if(!m_beacons->getDevice()->getScanState()){
            Logger::logMessage("Navigator::Device Scan State Off");
            m_is_navigating = false;
            break;
        }
        estimation_type type = m_beacons->getEstimationType();
        Point position;
        bool succes = false;
        if(type == multilateration){
            succes = Calculator::calcMultilateration(position, m_beacons->getDistances());
        } else if(type == weighted_arith_mean){
            succes = Calculator::calcWeightedArithMean(position, m_beacons->getDistances());
        } else {
            Logger::logMessage("ERROR: Unknown estimation chosen.");
        }
        if(succes){
            Q_EMIT positionChanged(position);
        }
    }
}

void Navigator::turnOff(){
    Logger::logMessage("Navigator::turnOff");
    m_is_navigating = false;
}

bool Navigator::isNavigating(){
    return m_is_navigating;
}
