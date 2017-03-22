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

#pragma once

#include <QVector>
#include <QString>

#include "editdistance.h"
#include "dictionary.h"

#define MAX_FUZZY_RESULT_ITEM 256
#define MIN_FUZZY_PERCENTAGE 85
#define MAX_SEARCH_RESULT 55

struct FuzzyResult
{
	float percentage;
	PairWord word;
};

typedef struct FuzzyResult FuzzyResult;

class Library
{
public:
	Library();
	~Library();

    bool load(const QString& filename);

    PairWordVector& search(const QString& keyword);

    void removeDict(const QString& filename);
    void removeDict(int index);

	PairWordVector& getSearchResult();
	PairWord getWord(int index, int dict);
    qint64 getWordCount() const;
    int getDictCount() const;
    Dictionary* getDict(const QString& filename) const;

private:

    QVector<Dictionary*> mDicts;
    PairWordVector mSearchResult;
    QString mLastKeyword;

};
