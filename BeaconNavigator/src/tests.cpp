#if(G_TEST == 1)
#include "gtest/gtest.h"
#include "beacons.h"
#include "calculator.h"
#include "device.h"
#include "navigator.h"

/**
 * @brief TEST Beacon::BeaconTest function.
 *
 * Cases:
 * 1. The beacon exists.
 * 2. The beacon address is correct, but the beacon is not present in set.
 * 3. The beacon address has wrong format.
*/
TEST(BeaconsTest, CheckMacAddress)
{
    Beacons beacons;

    ASSERT_TRUE(beacons.checkMacAddress("C3:FD:C2:D1:73:6E"));
    ASSERT_FALSE(beacons.checkMacAddress("E3:6B:7D:9B:A3:85"));
    ASSERT_FALSE(beacons.checkMacAddress("E6A:6C:7D:9B:A3:85"));
}

/**
 * @brief TEST Beacon::updateBeaconInfo function.
 * Checks whether the function works properly.
 *
*/
TEST(BeaconsTest, updateBeaconInfo)
{
    qint16 rssi = -20;
    QList<QString> macAddresses =
    {
        "C3:FD:C2:D1:73:6E",
        "C4:2F:31:C4:3C:22"
    };

    Calculator calculator;
    double distance = calculator.calcDistance(rssi);

    Beacons beacons;
    beacons.setCalculator(&calculator);
    for(QString mac_address: macAddresses)
    {
        beacons.updateBeaconInfo(mac_address, rssi);
        ASSERT_DOUBLE_EQ(beacons.getDistance(mac_address), distance);
    }
}

/**
 * @brief TEST Beacons::SimpleScan function.
 * Checks whether the function works properly.
 */
TEST(BeaconsTest, SimpleScan)
{
    Beacons beacons;
    Calculator calculator;
    Device device(&beacons);
    Navigator navigator(&beacons, &calculator);

    beacons.setNavigator(&navigator);
    beacons.setDevice(&device);

    beacons.startTracking();
    QThread::msleep(1);
    ASSERT_TRUE(device.getScanState());
    beacons.stopTracking();
    QThread::msleep(1);
    ASSERT_FALSE(device.getScanState());
}

/**
 * @brief TEST Beacon::ScanUpdateDistance function.
 * Checks whether the function works properly.
 */
TEST(BeaconsTest, ScanUpdateDistance)
{
    QString macAddress = "C4:2F:31:C4:3C:22";

    Beacons beacons;
    Calculator calculator;
    Device device(&beacons);
    Navigator navigator(&beacons, &calculator);

    beacons.setNavigator(&navigator);
    beacons.setDevice(&device);
    beacons.setCalculator(&calculator);

    beacons.startTracking();
    QThread::msleep(1);
    ASSERT_TRUE(device.getScanState());
    ASSERT_GT(beacons.getDistance(macAddress), 0);
    beacons.stopTracking();
    QThread::msleep(1);
    ASSERT_FALSE(device.getScanState());
}

/**
 * @brief TEST Calculator::SimpleDistCalc function.
 * It checks whether the function correctly calculates distances from different RSSI values.
 *
 * Cases:
 * 1. RSSI=0
 * 2. RSSI=-10
 * 3. RSSI=-100
 */
TEST(CalculatorTest, SimpleDistCalc)
{
    Calculator calculator;

    ASSERT_DOUBLE_EQ(calculator.calcDistance(0), 0.0047847491305590016);
    ASSERT_DOUBLE_EQ(calculator.calcDistance(-10), 0.010308429509976199);
    ASSERT_DOUBLE_EQ(calculator.calcDistance(-100), 10.3084295099762);
}

/**
 * @brief TEST Calculator::MultilaterationCalc function.
 * It checks whether the function correctly calculates estimated localization.
 *
 * Beacon's locatization:
 * {0, 3}, 3.5}
 * {4, -1}, 3.2}
 * {-2, -3}, 2.2}
*/
TEST(CalculatorTest, MultilaterationCalc)
{
    QList<DistanceToBeacon> distances =
    {
        {{0, 3}, 3.5},
        {{4, -1}, 3.2},
        {{-2, -3}, 2.2}
    };

    Calculator calculator;

    Point point = calculator.calcMultilateration(distances);
    ASSERT_DOUBLE_EQ(point.x, 0.2253124999999998);
    ASSERT_DOUBLE_EQ(point.y, -1.0259374999999999);
}

/**
 * @brief TEST Calculator:: WeightedArithMeanCalc function.
 * It checks whether the function correctly calculates estimated localization.
 *
 * Beacon's locatization:
 * {0, 3}, 3.5}
 * {4, -1}, 3.2}
 * {-2, -3}, 2.2}
*/
TEST(CalculatorTest, WeightedArithMeanCalc)
{
    QList<DistanceToBeacon> distances =
    {
        {{0, 3}, 3.5},
        {{4, -1}, 3.2},
        {{-2, -3}, 2.2}
    };

    Calculator calculator;

    Point point = calculator.calcWeightedArithMean(distances);
    ASSERT_DOUBLE_EQ(point.x, 0.9438202247191011);
    ASSERT_DOUBLE_EQ(point.y, 0.078651685393258342);
}

#endif

