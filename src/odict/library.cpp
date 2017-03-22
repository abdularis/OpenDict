/*
    OpenDict
    Copyright (C) 2015-2017 Abdul Aris R. <abdularisrahmanudin10@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <QtDebug>

#include <iostream>
#include <algorithm>
#include <cstring>

#include "library.h"

Library::Library()
{
}

Library::~Library()
{
    for (Dictionary* d : mDicts)
        delete d;
}

bool Library::load(const QString& filename)
{
	bool succes;
	Dictionary* dict = new Dictionary();

	succes = dict->load(filename);

	if ( succes ) {
        mDicts.push_back(dict);
        return true;
    }

    return false;
}

PairWordVector& Library::search(const QString& keyword)
{
    if ( keyword == mLastKeyword || keyword.size() <= 0)
        return mSearchResult;

    mLastKeyword = keyword;
    QChar key_prefix = keyword[0].toLower();
    Dictionary* currDict = NULL;
    PairWord currPairWord;
    QString firstWord;
	FuzzyResult fr[MAX_FUZZY_RESULT_ITEM];
	int currFuzzResIndex = 0;
	EditDistance ed;
	int dist;
	float perc;
	int w_len;
	int k_len = keyword.length();
	long wordCount = 0;

    /*
     * Iterate all dicts
     */
    for ( int i = 0; i < mDicts.size(); i++) {

        currDict = mDicts[i];
        wordCount = currDict->getWordCount();

		for (int j = 0; j < wordCount; j++) {
            currPairWord = currDict->getWord(j);

            if ( currPairWord.first[0].toLower() != key_prefix)
				continue;

            firstWord = currPairWord.first.toLower();
            w_len = firstWord.length();

            if (w_len > k_len) {
                firstWord = firstWord.mid(0, k_len);

                /* perc = (1 - (distance/string_len)) * 100% */
                dist = ed.hamm_distance( keyword.toLower(), firstWord );
				perc = (1.0f - static_cast<float>(dist)/static_cast<float>(w_len)) * 100.0f;

                if (currPairWord.first[k_len] == ' ') {
                    perc -= ((float)(w_len-k_len)) / 100.0f;
                }
                else {
                    perc -= w_len-k_len;
                }
			}
			else {

                /* perc = (1 - (distance/string_len)) * 100% */
                dist = ed.lev_distance( keyword.toLower(), firstWord);
				perc = (1.0f - static_cast<float>(dist)/static_cast<float>(w_len)) * 100.0f;
			}


            /* check for fuzzines */
			if (perc > MIN_FUZZY_PERCENTAGE) {
				if (currFuzzResIndex < MAX_FUZZY_RESULT_ITEM) {
                    fr[currFuzzResIndex].word = currPairWord;
					fr[currFuzzResIndex].percentage = perc;
					currFuzzResIndex++;
				}
				else {
					break;
				}

			}

        } /* for each word */

		if (currFuzzResIndex >= MAX_FUZZY_RESULT_ITEM) {
			break;
		}

    } /* for each dict */

    /* sort */
	for (int i = 0; i < currFuzzResIndex; i++) {
		for (int j = 0; j < currFuzzResIndex; j++) {
			if (fr[i].percentage > fr[j].percentage) {
				FuzzyResult temp = fr[i];
				fr[i] = fr[j];
				fr[j] = temp;
			}
		}
	}

    int resCount = MAX_SEARCH_RESULT;
    if (currFuzzResIndex < resCount)
        resCount = currFuzzResIndex;
    mSearchResult.clear();
    for (int i = 0; i < resCount; i++) {
        mSearchResult.push_back(fr[i].word);
	}

    return mSearchResult;
}

void Library::removeDict(const QString &filename)
{
    for (int i = 0; i < mDicts.size(); i++) {
        if (filename == mDicts[i]->getFileName()) {
            removeDict(i);
            break;
        }
    }
}

void Library::removeDict(int index)
{
    if (index >= 0 && index < mDicts.size())
        mDicts.remove(index);
}

PairWordVector& Library::getSearchResult()
{
    return mSearchResult;
}

PairWord Library::getWord(int index, int dict)
{
    if ( mDicts[dict] != NULL )
        return mDicts[dict]->getWord(index);
    return PairWord(" ", " ");
}

qint64 Library::getWordCount() const
{
    qint64 c = 0;
    for (Dictionary* d : mDicts)
        c += d->getWordCount();
    return c;
}

int Library::getDictCount() const
{
    return mDicts.size();
}

Dictionary* Library::getDict(const QString &filename) const
{
    for (Dictionary* d : mDicts) {
        if (d->getFileName() == filename)
            return d;
    }
    return nullptr;
}
