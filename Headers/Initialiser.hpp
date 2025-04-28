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
	const bool& GetStopMachine();
	const std::string& GetPressenceMessage();
	void SetPressenceMessage(std::string);
private:
	void Save();
	std::vector<std::filesystem::path> AMOGUSFiles;
	std::vector<std::filesystem::path> memeFiles;
	bool stopMachine;
	std::string pressenceMessage;
};