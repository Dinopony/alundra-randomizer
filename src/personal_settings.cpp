#include "personal_settings.hpp"
#include "tools/argument_dictionary.hpp"
#include "tools/exception.hpp"
#include <iostream>

PersonalSettings::PersonalSettings(const ArgumentDictionary& args)
{
    // Read settings from personal_settings.json file is it's present
    std::string settings_path = args.get_string("personalsettings");
    if(settings_path.empty())
        settings_path = "./personal_settings.json";

    std::ifstream personal_settings_file(settings_path);
    if(personal_settings_file)
    {
        std::cout << "Personal settings: '" << settings_path << "'\n";

        std::stringstream buffer;
        buffer << personal_settings_file.rdbuf();
        Json personal_settings_json = Json::parse(buffer.str(), nullptr, true, true);
        this->parse_json(personal_settings_json);
    }
    else
    {
        std::cout << "Could not open '"<< settings_path << "', will use default values.\n";
    }
}

void PersonalSettings::parse_json(const Json& json)
{
    for (auto& [key, value] : json.items())
    {
        if (key == "removeMusic")
            _remove_music = value;
        else
            throw RandomizerException("Unknown key '" + key + "' in personal settings JSON");
    }
}
