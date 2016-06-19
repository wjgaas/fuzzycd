/*
* @Author: WJG
* @Date:   2016-04-21 00:54:04
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 23:32:35
*/

#ifndef __FILE_SUPPORT_H__
#define __FILE_SUPPORT_H__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <dirent.h>
#include <sys/stat.h>


/**
 * class for getting the subfolders under the path.
 */
class FileSupport
{
public:

    typedef std::string String;

    typedef std::vector<String> VecString;

public:

    /**
     * [list_folders wrapper of recursive version of list_folders]
     * @param  path [path]
     * @return      [folders]
     */
    VecString list_folders(String path) const
    {
        while(path[path.length()-1] == '/')
        {
            path.erase(path.end()-1);
        }

        path += "/";

        VecString folders;

        list_folders(path, folders);

        return folders;
    }

    /**
     * [read_from_file read lines from a file]
     * @param  filename [filename]
     * @return          [vector of lines]
     */
    VecString read_from_file(const String &filename)
    {
        VecString contents;

        std::ifstream r_file(filename.c_str(), std::ios::in);

        std::string temp;

        if(r_file.good())
        {
            while(std::getline(r_file, temp))
            {
                contents.push_back(temp);
            }

            r_file.close();
        }

        return contents;
    }

    /**
     * [write_to_file write muti lines to a file]
     * @param  filename [filename]
     * @param  contents [lines]
     */
    void write_to_file(const String &filename, const VecString &contents)
    {
        write_to_file(filename, contents, 0, contents.size());
    }

    /**
     * [write_to_file write muti lines to a file]
     * @param filename [filename]
     * @param contents [lines]
     * @param s        [start line index]
     * @param e        [after end line index]
     */
    void write_to_file(const String &filename, const VecString &contents, int s, int e)
    {
        if(s < 0) return;
        if(e > contents.size()) return;

        std::ofstream w_file(filename.c_str(), std::ios::out);

        if(w_file.good())
        {
            for(int i=s; i<e; i++)
            {
                w_file << contents[i] << std::endl;
            }

            w_file.close();
        }
    }

    /**
     * [write_to_file write a line to file]
     * @param filename [filename]
     * @param line     [line]
     */
    void write_to_file(const String &filename, const String &line)
    {
        std::ofstream w_file(filename.c_str(), std::ios::out);

        if(w_file.good())
        {
            w_file << line << std::endl;

            w_file.close();
        }
    }

    /**
     * [append_to_file apend muti lines to a file]
     * @param filename [filename]
     * @param contents [lines]
     */
    void append_to_file(const String &filename, const VecString &contents)
    {
        std::ofstream w_file(filename.c_str(), std::ios::app);

        if(w_file.good())
        {
            for(int i=0; i<contents.size(); i++)
            {
                w_file << contents[i] << std::endl;
            }

            w_file.close();
        }
    }

    /**
     * [append_to_file apend a line to a file]
     * @param filename [filename]
     * @param line     [line]
     */
    void append_to_file(const String &filename, const String &line)
    {
        std::ofstream w_file(filename.c_str(), std::ios::app);

        if(w_file.good())
        {
            w_file << line << std::endl;

            w_file.close();
        }
    }

    /**
     * [total_lines get how many lines in a file]
     * @param  filename [filename]
     * @return          [number of lines]
     */
    int total_lines(const String &filename)
    {
        int lines = 0;

        std::ifstream r_file(filename.c_str(), std::ios::in);

        std::string temp;

        if(!r_file.good())
        {
            return -1;
        }
        else
        {
            while(std::getline(r_file, temp))
            {
                lines++;
            }

            r_file.close();
        }

        return lines;
    }


private:

    /**
     * [list_folders recursive version fo finding subfolders under path
     *  without extra copy]
     * @param path    [path]
     * @param folders [folders]
     */
    void list_folders(const String &path, VecString &folders) const
    {
        DIR *dp;
        struct dirent *dirp;

        if((dp=opendir(path.c_str())) == NULL)
        {
            //std::cout << "can't open dir "
            //          << path << std::endl;
            return;
        }

        while((dirp=readdir(dp)) != NULL)
        {
            if(dirp->d_type & DT_DIR)
            {
                std::string name(dirp->d_name);

                if(name == "." || name == "..") continue;

                folders.push_back(path + name);

                list_folders(path + name + "/", folders);
            }
        }

        closedir(dp);
    }
};

#endif // __FILE_SUPPORT_H__
