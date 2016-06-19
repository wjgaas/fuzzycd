/*
* @Author: WJG
* @Date:   2016-04-22 00:54:04
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 10:30:10
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "jsoncpp/json/json.h"

/**
 * class for parse config file, using jsoncpp
 */
class Config
{
public:

    /**
     * parse the config path
     */
    static std::vector<std::string> get_config_path(const std::string &filepath)
    {
        std::vector<std::string> config_path;

        Json::Reader reader;

        Json::Value root;

        std::ifstream cfgfile(filepath.c_str());

        if(reader.parse(cfgfile, root))
        {
            for(int i=0; i<root["FuzzyPath"].size(); i++)
            {
                config_path.push_back(root["FuzzyPath"][i].asString());
            }
        }

        return config_path;
    }
};

#endif // __CONFIG_H__
