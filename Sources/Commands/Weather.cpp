#include "Commands/Weather.hpp"
#include <format>

Commands::Weather::Weather(const char* _name, dpp::cluster& bot, Data& data) : ICommand(_name, bot, data) {}

void Commands::Weather::Init(CommandIds _commandIds)
{
    if (dpp::run_once<struct register_bot_commands>())
    {
        commands.chatCommand = new dpp::slashcommand(name, "Get the curent weather of paris", cp_bot.me.id);
        commands.chatCommand->set_dm_permission(true);
        ICommand::Init(_commandIds);
    }
}

void Commands::Weather::Execute(const dpp::slashcommand_t& _event)
{
    ICommand::Execute(_event);
    if (_event.command.get_command_name() == name) {
        WeatherData wd = cp_data.GetWeatherData();
        dpp::message message;
        message.flags |= dpp::m_using_components_v2;
        message.add_component_v2(
            dpp::component()
            .set_type(dpp::cot_container)
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("# " + GetEmojiFromWeatherId(wd.weatherId) + ' ' + wd.weatherName + "\n-# " + wd.weatherDesc)
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_separator)
                .set_divider(true)
                .set_spacing(dpp::sep_small)
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("## " + std::string(wd.temperatureFeel <= 5.0 ? "snowflake": wd.temperatureFeel >= 25.0 ? ":fire:" : ":thermometer:") +std::format("  {:.2f}℃ | {:.2f}℃", wd.temperature, wd.temperatureFeel))
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("## :droplet:  "+std::to_string(wd.humidity) + "%")
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content("## :cloud:  "+std::to_string(wd.cloud) +"%")
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content(std::format("## __UV__  {:.2f}",wd.uv))
            )
            .add_component_v2(
                dpp::component()
                .set_type(dpp::cot_text_display)
                .set_content(std::format("## :dash:  {:.2f}m/s | ",wd.windSpeed)+GetWindDirPoint(wd.windDir))
            )
        );
        _event.reply(message);
    }
}

std::string Commands::Weather::GetWindDirPoint(int _degrees)
{
    static std::string directions[16] = {
        "<:N:1468254392544395550> N", "<:NNE:1468254392544395550> NNE", "<:NE:1468254523855339530> NE", "<:ENE:1468254464648413390> ENE",
        "<:E:1468254464648413390> E", "<:ESE:1468254464648413390> ESE", "<:SE:1468254506000322572> SE", "<:SSE:1468254380602953862> SSE",
        "<:S:1468254380602953862> S", "<:SSW:1468254380602953862> SSW", "<:SW:1468254569065742570> SW", "<:WSW:1468254400911904961> WSW",
        "<:W:1468254400911904961> W", "<:WNW:1468254400911904961> WNW", "<:NW:1468254546466967634> NW", "<:NNW:1468254392544395550> NNW"
    };

        //Normalize degrees to 0 - 360
    _degrees = (_degrees % 360 + 360) % 360;

    //Each direction spans 22.5°
    int index = static_cast<int>(std::floorf((_degrees) / 22.5f));

    return directions[index];
}

std::string Commands::Weather::GetEmojiFromWeatherId(int _id)
{
    switch (_id)
    {
    case 200:
    case 201:
    case 202:
    case 230:
    case 231:
    case 232:
        return ":thunder_cloud_rain:";
    case 210:
    case 211:
    case 212:
    case 221:
        return ":cloud_lightning:";
    case 300:
    case 301:
    case 302:
    case 310:
    case 311:
    case 312:
    case 313:
    case 314:
    case 321:
        return ":cloud_rain:";
    case 500:
    case 501:
    case 502:
    case 503:
    case 504:
    case 511:
    case 520:
    case 521:
    case 522:
    case 531:
        return ":white_sun_rain_cloud:";
    case 600:
    case 601:
    case 602:
    case 611:
    case 612:
    case 613:
    case 615:
    case 616:
    case 620:
    case 621:
    case 622:
        return ":cloud_snow:";
    case 701:
    case 711:
    case 721:
    case 731:
    case 741:
    case 751:
    case 761:
    case 762:
    case 771:
        return ":fog:";
    case 781:
        return ":cloud_tornado:";
    case 800:
        return ":sunny:";
    case 801:
        return ":white_sun_small_cloud:";
    case 802:
        return ":partly_sunny:";
    case 803:
        return ":white_sun_cloud:";
    case 804:
        return ":cloud:";
    default:
        return ":question:";
    }
}