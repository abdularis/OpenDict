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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QtDebug>

#include "dictionary.h"


Dictionary::Dictionary()
{
    mCurrentIndex = 0;
}

Dictionary::Dictionary(const QString& filename)
{
    mCurrentIndex = 0;
	load(filename);
}

Dictionary::~Dictionary()
{
}

bool Dictionary::load(const QString& filename)
{
    QSqlDatabase db = QSqlDatabase::database(filename);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE", filename);
    }

    if (!db.isOpen()) {
        db.setDatabaseName(filename);
        if (!db.open()) {
            return false;
        }
    }

    QSqlQuery query("SELECT * FROM info LIMIT 1;", db);
    if (query.first()) {
        mName = query.value("name").toString();
        mAuthor = query.value("author").toString();
        mAuthorWeb = query.value("author_web").toString();
        mDescription = query.value("desc").toString();
        query.finish();
    }
    else {
        return false;
    }

    query = QSqlQuery("SELECT word, translation FROM data;", db);
    while (query.next()) {
        QString word = query.value(0).toString();
        QString translation = query.value(1).toString();

        mData.push_back(PairWord(word, translation));
    }

    mFileName = filename;
    mCurrentIndex = 0;
	return true;
}

bool Dictionary::reload()
{
    if ( mFileName.isEmpty() )
		return false;
    return load(mFileName);
}

qint64 Dictionary::getWordCount() const
{
    return mData.size();
}

PairWord& Dictionary::getWord(qint64 index)
{
    mCurrentIndex = index;
    return mData[index];
}

PairWord& Dictionary::getNextWord()
{
    return mData[ ++mCurrentIndex ];
}

PairWord& Dictionary::getPrevWord()
{
    return mData[ --mCurrentIndex ];
}

PairWordVector& Dictionary::getData()
{
    return mData;
}
