/*
* @Author: WJG
* @Date:   2016-05-1 23:00:05
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 23:33:51
*/

#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "filesupport.h"

class History
{
public:

    typedef std::string String;
    typedef std::vector<String> VecString;

    void process_history(const String &filename, const String &path)
    {
        FileSupport fs;
        VecString hists = fs.read_from_file(filename);

        if(!is_unique_in_history(hists, path)) return;

        if(hists.size() < 500)
        {
            fs.append_to_file(filename, path);
        }
        else
        {
            hists.push_back(path);

            fs.write_to_file(filename, hists, 1, hists.size());
        }
    }

    bool is_unique_in_history(const VecString &hists, const String &path)
    {
        for(int i=0; i<hists.size(); i++)
        {
            if(path == hists[i])
                return false;
        }

        return true;
    }
};

#endif // __HISTORY_H__
