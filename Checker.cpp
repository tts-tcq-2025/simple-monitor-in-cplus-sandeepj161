#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

enum class BreachType { NORMAL, TOO_LOW, TOO_HIGH };

BreachType getBreachType(float value, float low, float high) {
  if (value < low) {
    return BreachType::TOO_LOW;
  }
  if (value > high) {
    return BreachType::TOO_HIGH;
  }
  return BreachType::NORMAL;
}

BreachType getChargeRateBreach(float chargeRate) {
  // To complete coverage, treat <0 as TOO_LOW (abnormal for charging phase)
  return getBreachType(chargeRate, 0, 0.8);
}

bool batteryIsOk(float temperature, float soc, float chargeRate) {
  auto tempBreach = getBreachType(temperature, 0, 45);
  auto socBreach = getBreachType(soc, 20, 80);
  auto chargeRateBreach = getChargeRateBreach(chargeRate);

  // Print breaches (I/O)
  auto printBreach = [](const string& param, BreachType breach) {
    if (breach == BreachType::TOO_LOW) {
      cout << param << " too low!\n";
    } else if (breach == BreachType::TOO_HIGH) {
      cout << param << " too high!\n";
    }
  };

  printBreach("Temperature", tempBreach);
  printBreach("State of Charge", socBreach);
  printBreach("Charge Rate", chargeRateBreach);

  return tempBreach == BreachType::NORMAL &&
         socBreach == BreachType::NORMAL &&
         chargeRateBreach == BreachType::NORMAL;
}

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true
