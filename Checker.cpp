#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

enum class BreachType { TOO_LOW, APPROACH_LOW, NORMAL, APPROACH_HIGH, TOO_HIGH };

BreachType getCondition(float value, float min, float max) {
  float tolerance = 0.015f * max;
  vector<float> boundaries{min, min + tolerance, max - tolerance, max};
  auto it = upper_bound(boundaries.begin(), boundaries.end(), value);
  size_t index = distance(boundaries.begin(), it);
  return static_cast<BreachType>(index);
}

void printBreach(const string& param, BreachType breach, const string& lowWarning, const string& highWarning) {
  int code = static_cast<int>(breach);
  if (code == 2) return;
  int isHighIdx = code / 3;
  bool isApproach = (code % 2 == 1);
  string directions[2] = {"low", "high"};
  string warningPhrases[2] = {lowWarning, highWarning};
  string message;
  if (isApproach) {
    message = "Warning: Approaching " + warningPhrases[isHighIdx] + "!\n";
  } else {
    message = param + " too " + directions[isHighIdx] + "!\n";
  }
  cout << message;
}

bool isNotBreach(BreachType b) {
  return b != BreachType::TOO_LOW && b != BreachType::TOO_HIGH;
}

bool batteryIsOk(float temperature, float soc, float chargeRate) {
  auto tempCond = getCondition(temperature, 0, 45);
  auto socCond = getCondition(soc, 20, 80);
  auto chargeRateCond = getCondition(chargeRate, 0, 0.8f);
  printBreach("Temperature", tempCond, "under-temperature", "over-temperature");
  printBreach("State of Charge", socCond, "discharge", "charge-peak");
  printBreach("Charge Rate", chargeRateCond, "low charge-rate", "peak charge-rate");
  return isNotBreach(tempCond) && isNotBreach(socCond) && isNotBreach(chargeRateCond);
}

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true);
  assert(batteryIsOk(50, 85, 0) == false);
}
