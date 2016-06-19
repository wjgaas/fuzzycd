/*
* @Author: WJG
* @Date:   2016-04-21 00:54:04
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 12:14:28
*/

#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "termbox/termbox.h"

#include "panel.h"
#include "filesupport.h"

/**
 * [convert description]
 * @param  s [description]
 * @return   [description]
 */
std::string convert(const std::string &s)
{
    std::string r;

    for(int i=0; i<s.length(); i++)
        r.push_back( s[i] >= 'A' && s[i] <='Z' ? s[i] - 'A' + 'a' : s[i] );

    return r;
}

Panel::Panel()
{
    // init termbox
    int ret = tb_init();

    if(ret)
    {
        std::cout << "tb_init() failed with error code "
                  << ret << std::endl;
        return;
    }

    select_item_index = -1;
}

Panel::~Panel()
{
    tb_shutdown();
}

void Panel::init(std::vector<std::string> &cfg_path)
{
    config_path = cfg_path;

    // get all sub folders under config path
    for(int i=0; i<config_path.size(); i++)
    {
        FileSupport fs;

        std::vector<std::string> subtargets;

        subtargets = fs.list_folders(config_path[i]);

        for(int j=0; j<subtargets.size(); j++)
        {
            targets.push_back(subtargets[j]);
        }
    }

    // set input and output of termbox
    tb_select_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);

    tb_select_output_mode(TB_OUTPUT_256);

    // draw panel noly
    draw_panel();

    // draw default output
    draw_output(config_path, 0, config_path.size());

    tb_set_cursor(0, 0);

    draw_info();

    draw_help();

    tb_present();
}

// clear and repaint panel, input and output
void Panel::paint()
{
    tb_clear();

    draw_panel();

    if(_input.empty())
    {
        draw_output(config_path, 0, config_path.size());

        rank_results.clear();

        draw_select_item(select_item_index);
    }
    else
    {
        draw_input(_input);

        // fuzzy find and rank the results
        FuzzyFilter fuzzy;

        rank_results = fuzzy.rank_find(_input, targets, convert);

        draw_output(rank_results, 0, rank_results.size());

        draw_select_item(select_item_index);
    }

    draw_info();

    tb_present();
}

void Panel::draw_panel()
{
    int w = tb_width();

    int h = tb_height();

    // two lines on the panel
    for(int i=0; i<w; i++)
    {
        tb_change_cell(i,   1, '-', 99, 22);

        tb_change_cell(i, h-2, '-', 99, 22);
    }
}

void Panel::draw_info()
{
    printf_tb(5, tb_height()-2, 30, 22, \
              " F2 : Help ");

    printf_tb(tb_width()-15, tb_height()-2, 30, 22, \
              " Total : %d ", rank_results.size());
}

// draw input chars on the panel
void Panel::draw_input(std::string &input)
{
    printf_tb(0, 0, TB_WHITE, TB_DEFAULT, "%s", input.c_str());

    tb_set_cursor(input.length(), 0);
}

void Panel::draw_output(const std::vector<RankResult> &targets, int start, int end)
{
    if(targets.empty() || end <= start || end > targets.size()) return;

    int bottom = std::min(tb_height() - 4, end-start);

    for(int i=0; i<bottom; i++)
        printf_tb(0, i+2, TB_WHITE, TB_DEFAULT, "%s", targets[i].target.c_str());
}

void Panel::draw_output(const std::vector<std::string> &targets, int start, int end)
{
    if(targets.empty() || end <= start || end > targets.size()) return;

    int bottom = std::min(tb_height() - 4, end-start);

    for(int i=0; i<bottom; i++)
    {
        printf_tb(0, i+2, TB_WHITE, TB_DEFAULT, "%s", targets[i].c_str());
    }
}

void Panel::draw_select_item(int index)
{
    if(rank_results.empty())
    {
        set_select_item_index(-1);
        return;
    }

    printf_tb(0, index+2, TB_WHITE, TB_CYAN, "%s", rank_results[index].target.c_str());
}

void Panel::draw_help()
{
    int w = tb_width();
    int h = tb_height();

    if(w < 50) return;

    if(h < 20) return;

    printf_tb(w-25, 3, 30, 22, " | Esc    : exit    |");
    printf_tb(w-25, 4, 30, 22, " | Enter  : select  |");
    printf_tb(w-25, 5, 30, 22, " | Tab    : down    |");
    printf_tb(w-25, 6, 30, 22, " | up     :  up     |");
    printf_tb(w-25, 7, 30, 22, " | down   : down    |");
    printf_tb(w-25, 8, 30, 22, " | CTRL-X : Dir/Hist|");

    tb_present();
}

bool check_input_char(char ch)
{
    if( ('A'<=ch && ch <='Z') || ('a'<=ch && ch <='z') || ch == ' ')
        return true;

    return false;
}

void Panel::get_input(struct tb_event *ev)
{
    if(ev->key == TB_KEY_BACKSPACE || ev->key == TB_KEY_CTRL_8) //backspace
    {
        if(!_input.empty())
            _input.erase(_input.end() - 1);
    }
    else if(ev->key == TB_KEY_SPACE) //whitespace
    {
        _input.push_back(' ');
    }
    else  if(ev->key == 0x0) // char
    {
        char buf[4];
        tb_utf8_unicode_to_char(buf, ev->ch);

        if(check_input_char(buf[0]))
            _input.push_back(buf[0]);
    }

    select_item_index = 0;
}

void Panel::select_item_index_up()
{
    if(select_item_index > 0)
    {
        select_item_index--;
    }
}

void Panel::select_item_index_down()
{
    int bottom = std::min(tb_height()-5, int(rank_results.size()-1));

    if(select_item_index < bottom)
    {
        select_item_index++;
    }
}

std::string Panel::get_select_item()
{
    if(select_item_index < 0 ||
       select_item_index >= rank_results.size())
    {
        return ".";
    }

    return rank_results[select_item_index].target;
}

// draw a line of format string on the panel
void Panel::printf_tb(int x, int y, uint16_t fg, uint16_t bg, const char *fmt, ...)
{
	char buf[4096];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf(buf, sizeof(buf), fmt, vl);
	va_end(vl);
	print_tb(buf, x, y, fg, bg);
}

// draw a line of string on the panel
void Panel::print_tb(const char *str, int x, int y, uint16_t fg, uint16_t bg)
{
	while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, fg, bg);
		x++;
	}
}

