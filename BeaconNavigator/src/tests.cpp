#if(G_TEST == 1)
#include "gtest/gtest.h"
#include "beacons.h"
#include "calculator.h"
#include "device.h"
#include "navigator.h"

TEST(BeaconsTest, CheckMacAddress)
{
    Beacons beacons;

    ASSERT_TRUE(beacons.checkMacAddress("F6:C2:B1:B4:11:EC"));
    ASSERT_TRUE(beacons.checkMacAddress("E3:6B:7D:9B:A2:82"));
    ASSERT_FALSE(beacons.checkMacAddress("E3:6B:7D:9B:A3:85"));
    ASSERT_FALSE(beacons.checkMacAddress("E6:6C:7D:9B:A3:85"));
}

TEST(BeaconsTest, UpdateDistance)
{
    qint16 rssi = -20;
    QList<QString> macAddresses =
    {
        "F6:C2:B1:B4:11:EC",
        "E3:6B:7D:9B:A2:82"
    };

    Calculator calculator;
    double distance = calculator.calcDistance(rssi);

    Beacons beacons;
    beacons.setCalculator(&calculator);
    for(QString mac_address: macAddresses)
    {
        beacons.updateDistance(mac_address, rssi);
        ASSERT_DOUBLE_EQ(beacons.getDistance(mac_address), distance);
    }
}

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

TEST(BeaconsTest, ScanUpdateDistance)
{
    QString macAddress = "E3:6B:7D:9B:A2:83";

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

TEST(CalculatorTest, SimpleDistCalc)
{
    Calculator calculator;

    ASSERT_DOUBLE_EQ(calculator.calcDistance(0), 0.0047847491305590016);
    ASSERT_DOUBLE_EQ(calculator.calcDistance(-10), 0.010308429509976199);
    ASSERT_DOUBLE_EQ(calculator.calcDistance(-100), 10.3084295099762);
}

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

