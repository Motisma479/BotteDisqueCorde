#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <array>

const std::string GetToken();
const std::string GetTokenIpInfo();


class Data
{
public:
	Data();
	~Data();
	void Reload();
	const std::vector<std::filesystem::path>& GetSusImages();
	const std::vector<std::filesystem::path>& GetMemeImages();

	bool IsValid(std::string word) const;
	const char* GetRandomWord() const;

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
	std::vector<std::array<char, 6>> words;
	bool stopMachine = false;
	std::string pressenceMessage;
	std::vector<uint64_t> ipBanListner;
};