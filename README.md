# fuzzycd

## A Fuzzy Method To Change Directory 

fuzzycd can hlep to use cd command in a fuzzy way if we do not remember the directory clearly.

## Installation

The project is built under code::blocks, it runs on the Linux.

we need two steps to install it.
[in the ./install dir]

step 1:
./install.sh

step 2:
source ~/.bashrc

## Usage

we can use 'cd' command as usual, this program only recored the history of the path.
As to fuzzycd, we use 'cdf' command, the program will give a rank list of matches directory,
we can choose one of them to cd or exit directly.

### KEY
F1		: help
Esc		: exit
CTRL-X	: exit
Tab 	: next item
Up 		: select up item
Down 	: select down item
Enter	: select and 'cd'

### CONFIG

config file is ${INSTALL_PATH}/fuzzycd.config,
we use json to store the root path that we want to find.

example:
{
	"FuzzyPath": [
		"/home",
		"/opt"
	]
}
