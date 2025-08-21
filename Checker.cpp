#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

enum class BreachType { NORMAL, TOO_LOW, TOO_HIGH };

struct BatteryStatus {
  BreachType tempBreach = BreachType::NORMAL;
  BreachType socBreach = BreachType::NORMAL;
  BreachType chargeRateBreach = BreachType::NORMAL;
};

BreachType getBreachType(float value, float low, float high) {
  if (value < low) {
    return BreachType::TOO_LOW;
  }
  if (value > high) {
    return BreachType::TOO_HIGH;
  }
  return BreachType::NORMAL;
}

BatteryStatus getBatteryStatus(float temperature, float soc, float chargeRate) {
  BatteryStatus status;
  status.tempBreach = getBreachType(temperature, 0, 45);
  status.socBreach = getBreachType(soc, 20, 80);
  status.chargeRateBreach = getBreachType(chargeRate, 0, 0.8);
  return status;
}

bool isNormal(BreachType breach) {
  return breach == BreachType::NORMAL;
}

bool batteryIsOk(float temperature, float soc, float chargeRate) {
  auto status = getBatteryStatus(temperature, soc, chargeRate);

  if (!isNormal(status.tempBreach)) {
    cout << "Temperature " << (status.tempBreach == BreachType::TOO_LOW ? "too low" : "too high") << "!\n";
  }
  if (!isNormal(status.socBreach)) {
    cout << "State of Charge " << (status.socBreach == BreachType::TOO_LOW ? "too low" : "too high") << "!\n";
  }
  if (!isNormal(status.chargeRateBreach)) {
    cout << "Charge Rate " << (status.chargeRateBreach == BreachType::TOO_LOW ? "too low" : "too high") << "!\n";
  }

  return isNormal(status.tempBreach) &&
         isNormal(status.socBreach) &&
         isNormal(status.chargeRateBreach);
}

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true);
  assert(batteryIsOk(50, 85, 0) == false);
}
