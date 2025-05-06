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
    {"AF", "Afghanistan"},
    {"AL", "Albania"},
    {"DZ", "Algeria"},
    {"AS", "American Samoa"},
    {"AD", "Andorra"},
    {"AO", "Angola"},
    {"AI", "Anguilla"},
    {"AQ", "Antarctica"},
    {"AG", "Antigua and Barbuda"},
    {"AR", "Argentina"},
    {"AM", "Armenia"},
    {"AW", "Aruba"},
    {"AU", "Australia"},
    {"AT", "Austria"},
    {"AZ", "Azerbaijan"},
    {"BS", "Bahamas"},
    {"BH", "Bahrain"},
    {"BD", "Bangladesh"},
    {"BB", "Barbados"},
    {"BY", "Belarus"},
    {"BE", "Belgium"},
    {"BZ", "Belize"},
    {"BJ", "Benin"},
    {"BM", "Bermuda"},
    {"BT", "Bhutan"},
    {"BO", "Bolivia"},
    {"BA", "Bosnia and Herzegovina"},
    {"BW", "Botswana"},
    {"BR", "Brazil"},
    {"IO", "British Indian Ocean Territory"},
    {"BN", "Brunei Darussalam"},
    {"BG", "Bulgaria"},
    {"BF", "Burkina Faso"},
    {"BI", "Burundi"},
    {"KH", "Cambodia"},
    {"CM", "Cameroon"},
    {"CA", "Canada"},
    {"CV", "Cabo Verde"},
    {"KY", "Cayman Islands"},
    {"CF", "Central African Republic"},
    {"TD", "Chad"},
    {"CL", "Chile"},
    {"CN", "China"},
    {"CX", "Christmas Island"},
    {"CC", "Cocos (Keeling) Islands"},
    {"CO", "Colombia"},
    {"KM", "Comoros"},
    {"CG", "Congo"},
    {"CD", "Congo, Democratic Republic of the"},
    {"CK", "Cook Islands"},
    {"CR", "Costa Rica"},
    {"CI", "Côte d'Ivoire"},
    {"HR", "Croatia"},
    {"CU", "Cuba"},
    {"CY", "Cyprus"},
    {"CZ", "Czechia"},
    {"DK", "Denmark"},
    {"DJ", "Djibouti"},
    {"DM", "Dominica"},
    {"DO", "Dominican Republic"},
    {"EC", "Ecuador"},
    {"EG", "Egypt"},
    {"SV", "El Salvador"},
    {"GQ", "Equatorial Guinea"},
    {"ER", "Eritrea"},
    {"EE", "Estonia"},
    {"SZ", "Eswatini"},
    {"ET", "Ethiopia"},
    {"FJ", "Fiji"},
    {"FI", "Finland"},
    {"FR", "France"},
    {"GF", "French Guiana"},
    {"PF", "French Polynesia"},
    {"GA", "Gabon"},
    {"GM", "Gambia"},
    {"GE", "Georgia"},
    {"DE", "Germany"},
    {"GH", "Ghana"},
    {"GI", "Gibraltar"},
    {"GR", "Greece"},
    {"GL", "Greenland"},
    {"GD", "Grenada"},
    {"GP", "Guadeloupe"},
    {"GU", "Guam"},
    {"GT", "Guatemala"},
    {"GN", "Guinea"},
    {"GW", "Guinea-Bissau"},
    {"GY", "Guyana"},
    {"HT", "Haiti"},
    {"HN", "Honduras"},
    {"HK", "Hong Kong"},
    {"HU", "Hungary"},
    {"IS", "Iceland"},
    {"IN", "India"},
    {"ID", "Indonesia"},
    {"IR", "Iran"},
    {"IQ", "Iraq"},
    {"IE", "Ireland"},
    {"IL", "Israel"},
    {"IT", "Italy"},
    {"JM", "Jamaica"},
    {"JP", "Japan"},
    {"JO", "Jordan"},
    {"KZ", "Kazakhstan"},
    {"KE", "Kenya"},
    {"KI", "Kiribati"},
    {"KR", "South Korea"},
    {"KP", "North Korea"},
    {"KW", "Kuwait"},
    {"KG", "Kyrgyzstan"},
    {"LA", "Lao People's Democratic Republic"},
    {"LV", "Latvia"},
    {"LB", "Lebanon"},
    {"LS", "Lesotho"},
    {"LR", "Liberia"},
    {"LY", "Libya"},
    {"LI", "Liechtenstein"},
    {"LT", "Lithuania"},
    {"LU", "Luxembourg"},
    {"MO", "Macao"},
    {"MG", "Madagascar"},
    {"MW", "Malawi"},
    {"MY", "Malaysia"},
    {"MV", "Maldives"},
    {"ML", "Mali"},
    {"MT", "Malta"},
    {"MH", "Marshall Islands"},
    {"MQ", "Martinique"},
    {"MR", "Mauritania"},
    {"MU", "Mauritius"},
    {"YT", "Mayotte"},
    {"MX", "Mexico"},
    {"FM", "Micronesia"},
    {"MD", "Moldova"},
    {"MC", "Monaco"},
    {"MN", "Mongolia"},
    {"ME", "Montenegro"},
    {"MS", "Montserrat"},
    {"MA", "Morocco"},
    {"MZ", "Mozambique"},
    {"MM", "Myanmar"},
    {"NA", "Namibia"},
    {"NR", "Nauru"},
    {"NP", "Nepal"},
    {"NL", "Netherlands"},
    {"NC", "New Caledonia"},
    {"NZ", "New Zealand"},
    {"NI", "Nicaragua"},
    {"NE", "Niger"},
    {"NG", "Nigeria"},
    {"NU", "Niue"},
    {"NF", "Norfolk Island"},
    {"MP", "Northern Mariana Islands"},
    {"NO", "Norway"},
    {"OM", "Oman"},
    {"PK", "Pakistan"},
    {"PW", "Palau"},
    {"PS", "Palestine"},
    {"PA", "Panama"},
    {"PG", "Papua New Guinea"},
    {"PY", "Paraguay"},
    {"PE", "Peru"},
    {"PH", "Philippines"},
    {"PL", "Poland"},
    {"PT", "Portugal"},
    {"PR", "Puerto Rico"},
    {"QA", "Qatar"},
    {"RO", "Romania"},
    {"RU", "Russia"},
    {"RW", "Rwanda"},
    {"RE", "Réunion"},
    {"BL", "Saint Barthélemy"},
    {"SH", "Saint Helena, Ascension and Tristan da Cunha"},
    {"KN", "Saint Kitts and Nevis"},
    {"LC", "Saint Lucia"},
    {"MF", "Saint Martin (French part)"},
    {"PM", "Saint Pierre and Miquelon"},
    {"VC", "Saint Vincent and the Grenadines"},
    {"WS", "Samoa"},
    {"SM", "San Marino"},
    {"ST", "Sao Tome and Principe"},
    {"SA", "Saudi Arabia"},
    {"SN", "Senegal"},
    {"RS", "Serbia"},
    {"SC", "Seychelles"},
    {"SL", "Sierra Leone"},
    {"SG", "Singapore"},
    {"SX", "Sint Maarten (Dutch part)"},
    {"SK", "Slovakia"},
    {"SI", "Slovenia"},
    {"SB", "Solomon Islands"},
    {"SO", "Somalia"},
    {"ZA", "South Africa"},
    {"GS", "South Georgia and the South Sandwich Islands"},
    {"SS", "South Sudan"},
    {"ES", "Spain"},
    {"LK", "Sri Lanka"},
    {"SD", "Sudan"},
    {"SR", "Suriname"},
    {"SJ", "Svalbard and Jan Mayen"},
    {"SE", "Sweden"},
    {"CH", "Switzerland"},
    {"SY", "Syrian Arab Republic"},
    {"TW", "Taiwan"},
    {"TJ", "Tajikistan"},
    {"TZ", "Tanzania"},
    {"TH", "Thailand"},
    {"TL", "Timor-Leste"},
    {"TG", "Togo"},
    {"TK", "Tokelau"},
    {"TO", "Tonga"},
    {"TT", "Trinidad and Tobago"},
    {"TN", "Tunisia"},
    {"TR", "Turkey"},
    {"TM", "Turkmenistan"},
    {"TC", "Turks and Caicos Islands"},
    {"TV", "Tuvalu"},
    {"UG", "Uganda"},
    {"UA", "Ukraine"},
    {"AE", "United Arab Emirates"},
    {"GB", "United Kingdom"},
    {"US", "United States"},
    {"UY", "Uruguay"},
    {"UZ", "Uzbekistan"},
    {"VU", "Vanuatu"},
    {"VE", "Venezuela"},
    {"VN", "Viet Nam"},
    {"VG", "Virgin Islands (British)"},
    {"VI", "Virgin Islands (U.S.)"},
    {"WF", "Wallis and Futuna"},
    {"EH", "Western Sahara"},
    {"YE", "Yemen"},
    {"ZM", "Zambia"},
    {"ZW", "Zimbabwe"}
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
            cp_bot.request("https://ipinfo.io/" +ip + "?token=" + IPINFO_TOKEN, dpp::m_get,
            [this, ip](const dpp::http_request_completion_t& result) {
                if (result.status == 200)
                {
                    std::string country = extract_value(result.body, "country");
                    std::string countryName = "ERROR";
                    std::string region = extract_value(result.body, "region");
                    std::string city = extract_value(result.body, "city");

                    auto it = countryCodeMap.find(country);
                    if (it != countryCodeMap.end())
                        countryName = it->second;

                    std::transform(country.begin(), country.end(), country.begin(), ::tolower);

                    messages.push_back(">>> **IP: " + ip + " :flag_" + country + ":**\n-# **Country: __"+ countryName +"__.**\n-# **Region: __" + region + "__.**\n-# **City: __" + city + "__.**\n-# **Banned for 24h.**");
                }
                else
                {
                    messages.push_back( ">>> **IP: " + ip + "**\n-# **Banned for 24h.**" );
                }
            });
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
}
