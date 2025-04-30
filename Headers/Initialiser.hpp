#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>

const std::string GetToken();
class Data
{
public:
	Data();
	~Data();
	void Reload();
	const std::vector<std::filesystem::path>& GetSusImages();
	const std::vector<std::filesystem::path>& GetMemeImages();

	//member of setting.ini
	const bool& GetStopMachine();

	const std::string& GetPressenceMessage();
	void SetPressenceMessage(std::string);

	const std::vector<uint64_t>& GetIpBanListner();
	bool AddIpBanListner(uint64_t _newChannel);

private:
	void Save();
	std::vector<std::filesystem::path> AMOGUSFiles;
	std::vector<std::filesystem::path> memeFiles;
	bool stopMachine = false;
	std::string pressenceMessage;
	std::vector<uint64_t> ipBanListner;
};