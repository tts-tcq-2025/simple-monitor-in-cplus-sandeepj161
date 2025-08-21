#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

enum class Condition { TOO_LOW, APPROACH_LOW, NORMAL, APPROACH_HIGH, TOO_HIGH };

Condition getCondition(float value, float low, float high) {
  float tol = 0.05f * high;
  vector<float> boundaries {low, low + tol, high - tol, high};
  auto it = upper_bound(boundaries.begin(), boundaries.end(), value);
  int index = distance(boundaries.begin(), it);
  return static_cast<Condition>(index);
}

string getTemperatureMessage(Condition cond) {
  if (cond == Condition::NORMAL) return "";
  bool isLow = (cond == Condition::TOO_LOW || cond == Condition::APPROACH_LOW);
  if (cond == Condition::TOO_LOW || cond == Condition::TOO_HIGH) {
    return "Temperature too " + string(isLow ? "low" : "high") + "!\n";
  } else {
    return "Warning: Approaching " + string(isLow ? "under-temperature" : "over-temperature") + "!\n";
  }
}

string getSocMessage(Condition cond) {
  if (cond == Condition::NORMAL) return "";
  bool isLow = (cond == Condition::TOO_LOW || cond == Condition::APPROACH_LOW);
  if (cond == Condition::TOO_LOW || cond == Condition::TOO_HIGH) {
    return "State of Charge too " + string(isLow ? "low" : "high") + "!\n";
  } else {
    return "Warning: Approaching " + string(isLow ? "discharge" : "charge-peak") + "!\n";
  }
}

string getChargeRateMessage(Condition cond) {
  if (cond == Condition::NORMAL) return "";
  bool isLow = (cond == Condition::TOO_LOW || cond == Condition::APPROACH_LOW);
  if (cond == Condition::TOO_LOW || cond == Condition::TOO_HIGH) {
    return "Charge Rate too " + string(isLow ? "low" : "high") + "!\n";
  } else {
    return "Warning: Approaching " + string(isLow ? "low charge-rate" : "peak charge-rate") + "!\n";
  }
}

bool batteryIsOk(float temperature, float soc, float chargeRate) {
  auto tempCond = getCondition(temperature, 0, 45);
  auto socCond = getCondition(soc, 20, 80);
  auto chargeRateCond = getCondition(chargeRate, 0, 0.8);

  cout << getTemperatureMessage(tempCond);
  cout << getSocMessage(socCond);
  cout << getChargeRateMessage(chargeRateCond);

  bool tempOk = (tempCond != Condition::TOO_LOW && tempCond != Condition::TOO_HIGH);
  bool socOk = (socCond != Condition::TOO_LOW && socCond != Condition::TOO_HIGH);
  bool chargeRateOk = (chargeRateCond != Condition::TOO_LOW && chargeRateCond != Condition::TOO_HIGH);

  return tempOk && socOk && chargeRateOk;
}

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true);
  assert(batteryIsOk(50, 85, 0) == false);
}
