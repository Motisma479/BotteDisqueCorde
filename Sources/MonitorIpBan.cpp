#include "MonitorIpBan.hpp"
#include <unordered_map>

const std::string IPINFO_TOKEN = GetTokenIpInfo();

MonitorIpBan::MonitorIpBan(dpp::cluster& _bot, Data& _data) : cp_bot(_bot), cp_data(_data)
{
#if(WIN32)
#else
    log.open("/var/log/fail2ban.log", std::ios::in);
    if (!log.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }
    // Set the position to the end of the file initially
    log.seekg(0, std::ios::end);
    last_pos = log.tellg();
    log.close();
#endif
    Load();
}

std::string extract_value(const std::string& json, const std::string& key) {
    std::string search_key = "\"" + key + "\"";
    size_t key_pos = json.find(search_key);
    if (key_pos == std::string::npos) return "";

    size_t colon_pos = json.find(':', key_pos);
    if (colon_pos == std::string::npos) return "";

    size_t start_quote = json.find('"', colon_pos + 1);
    if (start_quote == std::string::npos) return "";

    size_t end_quote = json.find('"', start_quote + 1);
    if (end_quote == std::string::npos) return "";

    return json.substr(start_quote + 1, end_quote - start_quote - 1);
}

static const std::unordered_map<std::string, std::string> countryCodeMap = {
    {"af", "Afghanistan"},
    {"al", "Albania"},
    {"dz", "Algeria"},
    {"as", "American Samoa"},
    {"ad", "Andorra"},
    {"ao", "Angola"},
    {"ai", "Anguilla"},
    {"aq", "Antarctica"},
    {"ag", "Antigua and Barbuda"},
    {"ar", "Argentina"},
    {"am", "Armenia"},
    {"aw", "Aruba"},
    {"au", "Australia"},
    {"at", "Austria"},
    {"az", "Azerbaijan"},
    {"bs", "Bahamas"},
    {"bh", "Bahrain"},
    {"bd", "Bangladesh"},
    {"bb", "Barbados"},
    {"by", "Belarus"},
    {"be", "Belgium"},
    {"bz", "Belize"},
    {"bj", "Benin"},
    {"bm", "Bermuda"},
    {"bt", "Bhutan"},
    {"bo", "Bolivia"},
    {"ba", "Bosnia and Herzegovina"},
    {"bw", "Botswana"},
    {"br", "Brazil"},
    {"io", "British Indian Ocean Territory"},
    {"bn", "Brunei Darussalam"},
    {"bg", "Bulgaria"},
    {"bf", "Burkina Faso"},
    {"bi", "Burundi"},
    {"kh", "Cambodia"},
    {"cm", "Cameroon"},
    {"ca", "Canada"},
    {"cv", "Cabo Verde"},
    {"ky", "Cayman Islands"},
    {"cf", "Central African Republic"},
    {"td", "Chad"},
    {"cl", "Chile"},
    {"cn", "China"},
    {"cx", "Christmas Island"},
    {"cc", "Cocos (Keeling) Islands"},
    {"co", "Colombia"},
    {"km", "Comoros"},
    {"cg", "Congo"},
    {"cd", "Congo, Democratic Republic of the"},
    {"ck", "Cook Islands"},
    {"cr", "Costa Rica"},
    {"ci", "Côte d'Ivoire"},
    {"hr", "Croatia"},
    {"cu", "Cuba"},
    {"cy", "Cyprus"},
    {"cz", "Czechia"},
    {"dk", "Denmark"},
    {"dj", "Djibouti"},
    {"dm", "Dominica"},
    {"do", "Dominican Republic"},
    {"ec", "Ecuador"},
    {"eg", "Egypt"},
    {"sv", "El Salvador"},
    {"gq", "Equatorial Guinea"},
    {"er", "Eritrea"},
    {"ee", "Estonia"},
    {"sz", "Eswatini"},
    {"et", "Ethiopia"},
    {"fj", "Fiji"},
    {"fi", "Finland"},
    {"fr", "France"},
    {"gf", "French Guiana"},
    {"pf", "French Polynesia"},
    {"ga", "Gabon"},
    {"gm", "Gambia"},
    {"ge", "Georgia"},
    {"de", "Germany"},
    {"gh", "Ghana"},
    {"gi", "Gibraltar"},
    {"gr", "Greece"},
    {"gl", "Greenland"},
    {"gd", "Grenada"},
    {"gp", "Guadeloupe"},
    {"gu", "Guam"},
    {"gt", "Guatemala"},
    {"gn", "Guinea"},
    {"gw", "Guinea-Bissau"},
    {"gy", "Guyana"},
    {"ht", "Haiti"},
    {"hn", "Honduras"},
    {"hk", "Hong Kong"},
    {"hu", "Hungary"},
    {"is", "Iceland"},
    {"in", "India"},
    {"id", "Indonesia"},
    {"ir", "Iran"},
    {"iq", "Iraq"},
    {"ie", "Ireland"},
    {"il", "Israel"},
    {"it", "Italy"},
    {"jm", "Jamaica"},
    {"jp", "Japan"},
    {"jo", "Jordan"},
    {"kz", "Kazakhstan"},
    {"ke", "Kenya"},
    {"ki", "Kiribati"},
    {"kr", "South Korea"},
    {"kp", "North Korea"},
    {"kw", "Kuwait"},
    {"kg", "Kyrgyzstan"},
    {"la", "Lao People's Democratic Republic"},
    {"lv", "Latvia"},
    {"lb", "Lebanon"},
    {"ls", "Lesotho"},
    {"lr", "Liberia"},
    {"ly", "Libya"},
    {"li", "Liechtenstein"},
    {"lt", "Lithuania"},
    {"lu", "Luxembourg"},
    {"mo", "Macao"},
    {"mg", "Madagascar"},
    {"mw", "Malawi"},
    {"my", "Malaysia"},
    {"mv", "Maldives"},
    {"ml", "Mali"},
    {"mt", "Malta"},
    {"mh", "Marshall Islands"},
    {"mq", "Martinique"},
    {"mr", "Mauritania"},
    {"mu", "Mauritius"},
    {"yt", "Mayotte"},
    {"mx", "Mexico"},
    {"fm", "Micronesia"},
    {"md", "Moldova"},
    {"mc", "Monaco"},
    {"mn", "Mongolia"},
    {"me", "Montenegro"},
    {"ms", "Montserrat"},
    {"ma", "Morocco"},
    {"mz", "Mozambique"},
    {"mm", "Myanmar"},
    {"na", "Namibia"},
    {"nr", "Nauru"},
    {"np", "Nepal"},
    {"nl", "Netherlands"},
    {"nc", "New Caledonia"},
    {"nz", "New Zealand"},
    {"ni", "Nicaragua"},
    {"ne", "Niger"},
    {"ng", "Nigeria"},
    {"nu", "Niue"},
    {"nf", "Norfolk Island"},
    {"mp", "Northern Mariana Islands"},
    {"no", "Norway"},
    {"om", "Oman"},
    {"pk", "Pakistan"},
    {"pw", "Palau"},
    {"ps", "Palestine"},
    {"pa", "Panama"},
    {"pg", "Papua New Guinea"},
    {"py", "Paraguay"},
    {"pe", "Peru"},
    {"ph", "Philippines"},
    {"pl", "Poland"},
    {"pt", "Portugal"},
    {"pr", "Puerto Rico"},
    {"qa", "Qatar"},
    {"ro", "Romania"},
    {"ru", "Russia"},
    {"rw", "Rwanda"},
    {"re", "Réunion"},
    {"bl", "Saint Barthélemy"},
    {"sh", "Saint Helena, Ascension and Tristan da Cunha"},
    {"kn", "Saint Kitts and Nevis"},
    {"lc", "Saint Lucia"},
    {"mf", "Saint Martin (French part)"},
    {"pm", "Saint Pierre and Miquelon"},
    {"vc", "Saint Vincent and the Grenadines"},
    {"ws", "Samoa"},
    {"sm", "San Marino"},
    {"st", "Sao Tome and Principe"},
    {"sa", "Saudi Arabia"},
    {"sn", "Senegal"},
    {"rs", "Serbia"},
    {"sc", "Seychelles"},
    {"sl", "Sierra Leone"},
    {"sg", "Singapore"},
    {"sx", "Sint Maarten (Dutch part)"},
    {"sk", "Slovakia"},
    {"si", "Slovenia"},
    {"sb", "Solomon Islands"},
    {"so", "Somalia"},
    {"za", "South Africa"},
    {"gs", "South Georgia and the South Sandwich Islands"},
    {"ss", "South Sudan"},
    {"es", "Spain"},
    {"lk", "Sri Lanka"},
    {"sd", "Sudan"},
    {"sr", "Suriname"},
    {"sj", "Svalbard and Jan Mayen"},
    {"se", "Sweden"},
    {"ch", "Switzerland"},
    {"sy", "Syrian Arab Republic"},
    {"tw", "Taiwan"},
    {"tj", "Tajikistan"},
    {"tz", "Tanzania"},
    {"th", "Thailand"},
    {"tl", "Timor-Leste"},
    {"tg", "Togo"},
    {"tk", "Tokelau"},
    {"to", "Tonga"},
    {"tt", "Trinidad and Tobago"},
    {"tn", "Tunisia"},
    {"tr", "Turkey"},
    {"tm", "Turkmenistan"},
    {"tc", "Turks and Caicos Islands"},
    {"tv", "Tuvalu"},
    {"ug", "Uganda"},
    {"ua", "Ukraine"},
    {"ae", "United Arab Emirates"},
    {"gb", "United Kingdom"},
    {"us", "United States"},
    {"uy", "Uruguay"},
    {"uz", "Uzbekistan"},
    {"vu", "Vanuatu"},
    {"ve", "Venezuela"},
    {"vn", "Viet Nam"},
    {"vg", "Virgin Islands (British)"},
    {"vi", "Virgin Islands (U.S.)"},
    {"wf", "Wallis and Futuna"},
    {"eh", "Western Sahara"},
    {"ye", "Yemen"},
    {"zm", "Zambia"},
    {"zw", "Zimbabwe"}
};


void MonitorIpBan::Monitor()
{
#if(WIN32)
    return;
#endif
    log.open("/var/log/fail2ban.log", std::ios::in);
    if (!log.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }
    log.seekg(0, std::ios::end);
    std::streampos temp_pos = log.tellg();
    if (last_pos > temp_pos) last_pos = temp_pos;

    log.seekg(last_pos);
    std::string line;

    while (std::getline(log, line)) {
        // Process the log line if it contains the "Ban" keyword
        if (line.find("Ban ") != std::string::npos) {
            size_t ip_pos = line.find("Ban ") + 4;  // Move past "Ban "
            std::string ip = line.substr(ip_pos, line.find(" ", ip_pos) - ip_pos);
            Date today = advanced::GetActualDate();

            auto it = std::find_if(info.begin(), info.end(), [&](const auto& entry) { return std::get<0>(entry) == ip; });
            bool wasFound = it != info.end();
            
            if (wasFound)
            {
                IPInfo& found = std::get<1>(*it);

                if(found.refreshDate.dayOfWeek != today.dayOfWeek)
                {
                    std::string countryName = "ERROR";
                    auto deez = countryCodeMap.find(found.country);
                    if (deez != countryCodeMap.end())
                    {
                        countryName = deez->second;
                    }

                    messages.push_back(">>> **IP: [" + ip + "](<https://ipinfo.io/" + ip + ">) :flag_" + found.country + ":**\n-# **Country: __*" + countryName + "*__**\n-# **Region: __*" + found.region + "*__**\n-# **City: __*" + found.city + "*__**-# \n-# **Banned for 24h.**\n**" + std::to_string(found.banCount) + " previous bans recorded for this IP.**");
                    found.banCount++;
                    shouldSave = true;
                }
                else
                {
                    MessageViaRequest(ip, &found);
                }
            }
            else
            {
                MessageViaRequest(ip);
            }
        }
    }
    log.clear();
    last_pos = log.tellg();
    log.close();

    if (messages.size() == 0) return;


    for (const uint64_t& id : cp_data.GetIpBanListner())
    {
        std::shared_ptr<size_t> index = std::make_shared<size_t>(0);
        auto sendRecursive = std::make_shared<std::function<void(const dpp::confirmation_callback_t&)>>();
        *sendRecursive = [sendRecursive, this, id, index](const dpp::confirmation_callback_t& callback) mutable
            {
                (*index)++;
                if (*index >= messages.size()) return;

                cp_bot.message_create(dpp::message(id, messages[*index]), *sendRecursive);
            };
        cp_bot.message_create(dpp::message(id, messages[*index]), *sendRecursive);
    }

    messages.clear();

    if (shouldSave)
        Save();
}

void MonitorIpBan::MessageViaRequest(std::string ip, IPInfo* data)
{
    cp_bot.request("https://ipinfo.io/" + ip + "?token=" + IPINFO_TOKEN, dpp::m_get,
    [this, ip, data](const dpp::http_request_completion_t& result) {
        if (result.status == 200)
        {
            Date today = advanced::GetActualDate();

            IPInfo ipData;
            ipData.country = extract_value(result.body, "country");
            std::transform(ipData.country.begin(), ipData.country.end(), ipData.country.begin(), ::tolower);
            ipData.region = extract_value(result.body, "region");
            ipData.city = extract_value(result.body, "city");
            ipData.refreshDate = today;
            if (data)
                ipData.banCount = data->banCount;
                
            std::string countryName = "ERROR";
            auto it = countryCodeMap.find(ipData.country);
            if (it != countryCodeMap.end())
                countryName = it->second;

            messages.push_back(">>> **IP: [" + ip + "](<https://ipinfo.io/" + ip + ">) :flag_" + ipData.country + ":**\n-# **Country: __*" + countryName + "*__**\n-# **Region: __*" + ipData.region + "*__**\n-# **City: __*" + ipData.city + "*__**\n-# **Banned for 24h.**-# \n**" + std::to_string(ipData.banCount) + " previous bans recorded for this IP.**");
        
            if (data)
            {
                data->country = ipData.country;
                data->region = ipData.region;
                data->city = ipData.city;

                data->banCount++;
            }
            else
            {
                info.push_back(std::make_tuple(ip, ipData));
            }
            shouldSave = true;
        }
        else
        {
            messages.push_back(">>> **IP: " + ip + "**\n-# **Banned for 24h.**");
        }
    });
}

void MonitorIpBan::Save()
{
    std::ofstream file("banData.ini");
    if (file.is_open())
    {
        for (const auto& [ip, data] : info)
        {
            file << "[" << ip << "]\n";
            file << "country=" << data.country << '\n';
            file << "region=" << data.region << '\n';
            file << "city=" << data.city << '\n';
            file << "banCount=" << std::to_string(data.banCount) << '\n';
            Date d = data.refreshDate;
            file << "pollDate=" << (d.day < 10 ? ("0" + std::to_string(d.day)) : std::to_string(d.day)) << '-' << (d.month < 10 ? ("0" + std::to_string(d.month)) : std::to_string(d.month)) << '-' << d.year << ' ' << (d.hour < 10 ? ("0" + std::to_string(d.hour)) : std::to_string(d.hour)) << ':' << (d.minute < 10 ? ("0" + std::to_string(d.minute)) : std::to_string(d.minute)) << '\n';
            
            file << '\n';
        }
        file.close();
    }
}
void MonitorIpBan::Load()
{
    std::ifstream file("banData.ini");
    dpp::snowflake chanelId;
    IPInfo data;
    std::string ip;
    bool first = true;

    int i = 0;

    for (std::string line; std::getline(file, line); )
    {
        std::istringstream stream(line);
        std::string param;
        stream >> param;

        std::string p = param;

        if (param.size() <= 1) continue;
        else if (param.starts_with("["))
        {
            i++;
            if (first)
            {
                first = false;
            }
            else
            {
                info.push_back(std::make_tuple(ip, data));
            }
            ip = param.substr(1);
            ip.pop_back();
        }
        else if (param.starts_with("country="))
        {
            std::string temp = param.substr(8);

            data.country = temp;
        }
        else if (param.starts_with("region="))
        {
            std::string temp = param.substr(7);

            data.region = temp;
        }
        else if (param.starts_with("city="))
        {
            std::string temp = param.substr(5);

            data.city = temp;
        }
        else if (param.starts_with("banCount="))
        {
            std::string temp = param.substr(9);
            data.banCount = std::stoul(temp);
        }
        else if (param.starts_with("refreshDate="))
        {
            std::string l; std::getline(stream, l);
            std::string temp = param.substr(12) + l;

            data.refreshDate = advanced::ParseDateTime(temp);
        }
    }
    info.push_back(std::make_tuple(ip, data));
}