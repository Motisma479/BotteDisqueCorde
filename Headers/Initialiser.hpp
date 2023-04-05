#include <string>
#include <vector>
#include <filesystem>

const std::string getToken();
class Data
{
public:
	Data();
	~Data();
	const std::vector<std::filesystem::path>& getSusImages();
	const std::vector<std::filesystem::path>& getMemeImages();
	void reload();
private:
	std::vector<std::filesystem::path> AMOGUSFiles;
	std::vector<std::filesystem::path> memeFiles;
};