/*
* @Author: WJG
* @Date:   2016-04-22 00:54:04
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 10:30:37
*/

#ifndef __FUZZYFILTER_H__
#define __FUZZYFILTER_H__

#include <iostream>
#include <string>
#include <vector>

/**
 * struct that contains a string and 
 * it's crossbounding score.
 */
struct RankResult
{
public:

	RankResult(std::string t, int s)
		: target(t), score(s) {}

	/**
	 * override the < operator for std::sort
	 */
	bool operator < (RankResult &rhs)
	{
		return this->score < rhs.score;
	}

public:

	std::string target;

	int score;
};

/**
 * class for fuzzy find
 */
class FuzzyFilter
{
public:

    typedef std::string String;

    typedef std::vector<String> VecString;

    typedef std::vector<RankResult> VecRankResult;

	/**
	 * [FuzzyFilter::find find the strings that match the source string in the target]
	 * @param  source  [str1]
	 * @param  targets [target strings]
	 * @param  f       [function that filter the strings, such as to_lower_case()]
	 * @return         [vector of matched strings]
	 */
	VecString find(const String &source, const VecString &targets, String (*f)(const String &)) const;

	/**
	 * [FuzzyFilter::rank_find rank the result of the FuzzyFilter::find using edit distance]
	 * @param  source  [str1]
	 * @param  targets [target strings]
	 * @param  f       [function that filter the strings, such as to_lower_case()]
	 * @return         [vector of matched strings sorted by edit distance]
	 */
	VecRankResult rank_find(const String &source, const VecString &targets, String (*f)(const String &)) const;

private:

	/**
	 * [FuzzyFilter::find_first_of find the first char ch in the string ]
	 * @param  str   [str]
	 * @param  start [start index for search]
	 * @param  ch    [char to find]
	 * @return       [index of ch in str]
	 */
    int find_first_of(const String &str, int start, char ch) const;

	/**
	 * [FuzzyFilter::edit_distance use dynamic process to mesure the distance between two strings.]
	 * @param  s1 [str1]
	 * @param  s2 [str2]
	 * @return    [distance]
	 */
	int edit_distance(const String& s1, const String& s2) const;
	
	/**
	 * [FuzzyFilter::match check if the target string contains the chars in source string sequensally]
	 * @param  source [souce string]
	 * @param  target [target string]
	 * @return        [if the target string match]
	 */
	bool match(const String &source, const String &target) const;

	/**
	 * [FuzzyFilter::rank calc a score how the target string matches the source string]
	 * @param  source [str1]
	 * @param  target [str2]
	 * @return        [score that 2 strings match]
	 */
	int rank(const String &source, const String &target) const;
};

#endif //__FUZZYFILTER_H__
