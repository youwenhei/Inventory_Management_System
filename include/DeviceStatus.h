#pragma once
#include <string>

enum class DeviceStatus
{
	Disconnected,
	Connected,
	Standby
};

std::string getDeviceStatus(DeviceStatus status);
