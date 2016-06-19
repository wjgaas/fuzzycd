/*
* @Author: WJG
* @Date:   2016-04-21 00:54:04
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 10:39:06
* @Desc: this project is a fuzzy verson fo cd,
*        if you can not remember the path you want to change clearly,
*        this useful tool will help you cd quickly.
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "termbox/termbox.h"

#include "filesupport.h"
#include "panel.h"
#include "config.h"
#include "history.h"

/** path for command file */
const std::string command_file_path = "/home/wjg/fuzzycd/command.sh";

/** path for history file */
const std::string history_file_path = "/home/wjg/fuzzycd/fuzzycd.history";

/** path for config file */
const std::string config_file_path = "/home/wjg/fuzzycd/fuzzycd.config";


/**
 * [main main loop for get and process input]
 * @return  [0]
 */
int main(int argc, char *argv[])
{
    History hist;

    if(argc == 2)
    {
        hist.process_history(history_file_path, std::string(argv[1]));
        return 0;
    }

    if(argc > 2) return -2;

    FileSupport fs;

    std::vector<std::string> config_path;

    config_path = Config::get_config_path(config_file_path);

    Panel panel;
    panel.init(config_path);

    struct tb_event ev;

    while (tb_poll_event(&ev))
    {
		switch (ev.type)
		{
		case TB_EVENT_KEY:
            /** Enter */
            if(ev.key == TB_KEY_ENTER)
            {
                fs.write_to_file(command_file_path, "cd " + panel.get_select_item());

                goto done;
            }
            /** Exit */
            else  if(ev.key == TB_KEY_CTRL_X || ev.key == TB_KEY_ESC)
            {
                fs.write_to_file(command_file_path, "cd .");

                goto done;
            }
            /** Help */
            else  if(ev.key == TB_KEY_F2)
            {
                panel.draw_help(); break;
            }
            /** down */
            else  if(ev.key == TB_KEY_TAB)
            {
                panel.select_item_index_down();
            }
            /** up */
            else  if(ev.key == TB_KEY_ARROW_UP)
            {
                panel.select_item_index_up();
            }
            /** down */
            else  if(ev.key == TB_KEY_ARROW_DOWN)
            {
                panel.select_item_index_down();
            }
            /** char */
            else
            {
                panel.get_input(&ev);
            }

            panel.paint();

			break;
		case TB_EVENT_RESIZE:

            panel.paint();

			break;
		case TB_EVENT_MOUSE:
            //TODO:

			break;
		default:
			break;
		}
	}

done:

    return 0;
}
