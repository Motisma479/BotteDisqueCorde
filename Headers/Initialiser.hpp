#include <string>
#include <vector>
#include <filesystem>

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
private:
	std::vector<std::filesystem::path> AMOGUSFiles;
	std::vector<std::filesystem::path> memeFiles;
	bool stopMachine;
};