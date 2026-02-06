#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <array>

const std::string GetToken();
const std::string GetTokenIpInfo();
const std::string GetTokenWeather();

struct WeatherData
{
	double temperature;
	double temperatureFeel;
	int humidity;
	int cloud;
	double uv;
	double windSpeed;
	int windDir;
	int weatherId;
	std::string weatherName;
	std::string weatherDesc;
};

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
	bool GetStopMachine() const;
	void SetStopMachine(bool _value);
	double GetLatitude() const;
	void SetLatitude(double _value);
	double GetLongitude() const;
	void SetLongitude(double _value);
	std::string GetMetrics() const;
	void SetMetrics(std::string _value);

	const std::string& GetPressenceMessage();
	void SetPressenceMessage(std::string);

	const std::vector<uint64_t>& GetIpBanListner();
	bool AddIpBanListner(uint64_t _newChannel);

	WeatherData GetWeatherData() const;
	void SetWeatherData(const WeatherData& _data);

	void Save();
private:
	void InsertWord(const std::string& word);

	std::vector<std::filesystem::path> AMOGUSFiles;
	std::vector<std::filesystem::path> memeFiles;
	std::vector<std::array<char, 6>> words;
	bool stopMachine = false;
	std::string pressenceMessage;
	std::vector<uint64_t> ipBanListner;

	double latitude;
	double longitude;
	std::string metrics;
	WeatherData weatherData;
};