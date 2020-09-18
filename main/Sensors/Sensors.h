#ifndef LEDWALL_SENSORS_H
#define LEDWALL_SENSORS_H

#include <sdkconfig.h>

namespace LedWall {
namespace Sensors {

class SensorsP;

class Sensors
{
public:
    Sensors();

private:
    SensorsP *m_p;
};

}
}

#endif //LEDWALL_SENSORS_H
