#include <iostream>
#include "DeviceStatus.h"

std::string getDeviceStatus(DeviceStatus status)
{
	switch (status)
	{
	case DeviceStatus::Connected:
		return "Connected";

	case DeviceStatus::Disconnected:
		return "Disconnected";

	case DeviceStatus::Standby:
		return "Standby";
	}

	return "Unknown";
}
