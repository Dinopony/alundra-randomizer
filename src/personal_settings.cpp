#include "personal_settings.hpp"
#include "tools/argument_dictionary.hpp"

PersonalSettings::PersonalSettings(const ArgumentDictionary& args)
{
    // Read settings from personal_settings.json file is it's present
//    std::string settings_path = args.get_string("personalsettings");
//    if(settings_path.empty())
//        settings_path = "./personal_settings.json";
}

void PersonalSettings::parse_json(const Json& json)
{
    // TODO
}
