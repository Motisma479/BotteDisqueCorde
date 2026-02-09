#pragma once
#include "Commands/ICommand.hpp"

struct CPUInfo
{
	std::string name = "";
	uint64_t sockets = 0;
	uint64_t physicalCores = 0;
	uint64_t logicalCores = 0;
	float baseFrequency = 0.0f;//Mhz
};

struct RAMInfo
{
	uint64_t totalRam = 0;//B
	uint64_t availableRam = 0;//B
};

struct DiskInfo
{
	uint64_t totalDisk = 0;
	uint64_t availableDisk = 0;
};

struct GPUInfo
{
	std::string name = "";
	uint64_t dedicatedVRAM = 0; // bytes
	uint64_t sharedVRAM = 0;    // bytes
};

namespace Commands
{
	class Status : public ICommand
	{
	public:
		Status(const char* _name, dpp::cluster& bot, Data& data);

		void Init(CommandIds _commandIds) override;
		void Execute(const dpp::slashcommand_t& event) override;
	private:
		CPUInfo GetCPUInfo();
		RAMInfo GetRAMInfo();
		DiskInfo GetDiskInfo();
		std::vector<GPUInfo> GetGPUsInfo();
		void ReductByteValue(uint64_t _value, float& _valueAsFloat, std::string& _unit);
	};
}