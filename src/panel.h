/*
* @Author: WJG
* @Date:   2016-04-21 00:54:04
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 10:40:34
*/

#ifndef __PANEL_H__
#define __PANEL_H__

#include "fuzzyfilter.h"

#include <iostream>
#include <vector>
#include <string>

/**
 * class for paint the panel of this tool.
 */
class Panel
{

public:

    Panel();

    ~Panel();

    void init(std::vector<std::string> &config_path);

    void get_input(struct tb_event *ev);

    void paint();

    void draw_help();

    void select_item_index_up();

    void select_item_index_down();

    int get_select_item_index() { return select_item_index; }

    void set_select_item_index(int idx) { select_item_index = idx; }

    std::string get_select_item();

private:

    void draw_panel();

    void draw_info();

    void draw_input(std::string &);

    void draw_output(const std::vector<RankResult> &targets, int start, int end);

    void draw_output(const std::vector<std::string> &targets, int start, int end);

    void draw_select_item(int index);

    void print_tb(const char *str, int x, int y, unsigned short fg, unsigned short bg);

    void printf_tb(int x, int y, unsigned short fg, unsigned short bg, const char *fmt, ...);

private:

    std::vector<std::string> config_path;

    std::vector<std::string> targets;

    std::vector<RankResult> rank_results;

    std::string _input;

    int select_item_index;

};

#endif // __PANEL_H__

