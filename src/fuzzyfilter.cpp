/*
* @Author: WJG
* @Date:   2016-04-22 00:54:04
* @Last Modified by:   WJG
* @Last Modified time: 2016-06-19 10:30:41
*/

#include "fuzzyfilter.h"
#include <algorithm>
 
/**
 * [FuzzyFilter::edit_distance use dynamic process to mesure the distance between two strings.]
 * @param  s1 [str1]
 * @param  s2 [str2]
 * @return    [distance]
 */
int FuzzyFilter::edit_distance(const String& s1, const String& s2) const
{
	const std::size_t len1 = s1.size(), len2 = s2.size();

	std::vector<std::vector<unsigned int> > d(len1 + 1, std::vector<unsigned int>(len2 + 1));

	d[0][0] = 0;

	for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;

	for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

	for(unsigned int i = 1; i <= len1; ++i)
	{
		for(unsigned int j = 1; j <= len2; ++j)
		{
			d[i][j] = std::min( std::min(d[i - 1][j] + 1, d[i][j - 1] + 1),
								d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) );
        }
    }

	return d[len1][len2];
}

/**
 * [FuzzyFilter::find_first_of find the first char ch in the string ]
 * @param  str   [str]
 * @param  start [start index for search]
 * @param  ch    [char to find]
 * @return       [index of ch in str]
 */
int FuzzyFilter::find_first_of(const String &str, int start, char ch) const
{
    for(int i=start; i<str.length(); i++)
    {
        if(str[i] == ch)
            return i;
    }

    return -1;
}

/**
 * [FuzzyFilter::match check if the target string contains the chars in source string sequensally]
 * @param  source [souce string]
 * @param  target [target string]
 * @return        [if the target string match]
 */
bool FuzzyFilter::match(const String &source, const String &target) const
{
    if(source.empty()) return false;

	int lendiff = target.length() - source.length();

	if(lendiff < 0) return false;

	if(lendiff == 0 && source == target) return true;

    int idx = 0;
    for(int i=0; i<source.length(); i++)
	{
        idx = find_first_of(target, idx, source[i]);

        if(idx == -1) return false;

        idx++;
	}

	return true;
}

/**
 * [FuzzyFilter::rank calc a score how the target string matches the source string]
 * @param  source [str1]
 * @param  target [str2]
 * @return        [score that 2 strings match]
 */
int FuzzyFilter::rank(const String &source, const String &target) const
{
    if(source.empty()) return 999999;

	int lendiff = target.length() - source.length();

	if(lendiff < 0) return -1;

	if(lendiff == 0 && source == target) return 0;

	int chardiff = 0;

    int idx = 0;
    for(int i=0; i<source.length(); i++)
	{
        int last = idx;

        idx = find_first_of(target, idx, source[i]);

        if(idx == -1) return -1;

        chardiff += idx - last;

        idx++;
	}

    //rest of the target
	chardiff += target.length() - idx;

	return chardiff;
}

/**
 * [FuzzyFilter::find find the strings that match the source string in the target]
 * @param  source  [str1]
 * @param  targets [target strings]
 * @param  f       [function that filter the strings, such as to_lower_case()]
 * @return         [vector of matched strings]
 */
FuzzyFilter::VecString FuzzyFilter::find( const String &source,
                                          const VecString &targets,
                                          String (*f)(const String &)
                                        ) const
{
	VecString matches;

	for(int i=0; i<targets.size(); i++)
	{
		if(match(f(source), f(targets[i])))
			matches.push_back(targets[i]);
	}

	return matches;
}

/**
 * [FuzzyFilter::rank_find rank the result of the FuzzyFilter::find using edit distance]
 * @param  source  [str1]
 * @param  targets [target strings]
 * @param  f       [function that filter the strings, such as to_lower_case()]
 * @return         [vector of matched strings sorted by edit distance]
 */
FuzzyFilter::VecRankResult FuzzyFilter::rank_find( const String &source,
                                                   const VecString &targets,
                                                   String (*f)(const String &)
                                                 ) const
{
	VecRankResult results;

	VecString matches = find(source, targets, f);

	for(int i=0; i<matches.size(); i++)
	{
		int disance = edit_distance(f(source), f(matches[i]));

		results.push_back(RankResult(matches[i], disance));
	}

	std::sort(results.begin(), results.end());

	return results;
}
