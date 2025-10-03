#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

// Enum for breach type
enum class BreachType { NORMAL, TOO_LOW, TOO_HIGH };

// Interface for alerting mechanism
class IAlert {
public:
    virtual void sendAlert(const std::string& param, BreachType breach) = 0;
    virtual ~IAlert() = default;
};

// Console-based alert implementation
class ConsoleAlert : public IAlert {
public:
    void sendAlert(const std::string& param, BreachType breach) override {
        if (breach == BreachType::TOO_LOW) {
            std::cout << param << " too low!\n";
        } else if (breach == BreachType::TOO_HIGH) {
            std::cout << param << " too high!\n";
        }
    }
};

// Core function to detect breach
BreachType getBreachType(float value, float low, float high) {
    if (value < low) return BreachType::TOO_LOW;
    if (value > high) return BreachType::TOO_HIGH;
    return BreachType::NORMAL;
}

// Encapsulates parameter validation
class Parameter {
    std::string name;
    float lowerLimit;
    float upperLimit;

public:
    Parameter(std::string paramName, float low, float high)
        : name(std::move(paramName)), lowerLimit(low), upperLimit(high) {}

    BreachType evaluate(float value, IAlert& alert) const {
        BreachType breach = getBreachType(value, lowerLimit, upperLimit);
        if (breach != BreachType::NORMAL) {
            alert.sendAlert(name, breach);
        }
        return breach;
    }
};

// Battery checker class
class BatteryChecker {
    std::vector<Parameter> parameters;
    IAlert& alert;

public:
    BatteryChecker(IAlert& alertMechanism)
        : alert(alertMechanism) {}

    void addParameter(const Parameter& param) {
        parameters.push_back(param);
    }

    bool check(const std::vector<float>& values) {
        bool status = true;
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (parameters[i].evaluate(values[i], alert) != BreachType::NORMAL) {
                status = false;
            }
        }
        return status;
    }
};

// ================== Usage ==================
int main() {
    ConsoleAlert consoleAlert;

    BatteryChecker checker(consoleAlert);
    checker.addParameter(Parameter("Temperature", 0, 45));
    checker.addParameter(Parameter("State of Charge", 20, 80));
    checker.addParameter(Parameter("Charge Rate", 0, 0.8));

    assert(checker.check({25, 70, 0.7}) == true);
    assert(checker.check({50, 85, 0.0}) == false);

    return 0;
}
