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

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>
#include <QPair>
#include <QVector>

typedef QPair<QString, QString> PairWord;
typedef QVector< PairWord > PairWordVector;

class Dictionary
{
public:
	Dictionary();
    Dictionary(const QString& filename);
	~Dictionary();

    bool load(const QString& filename);
	bool reload();

    qint64 getWordCount() const;
    PairWord& getWord(qint64 index);
    PairWord& getNextWord();
    PairWord& getPrevWord();
    PairWordVector& getData();
    inline QString getFileName() const { return mFileName; }
    inline QString getName() const { return mName; }
    inline QString getAuthor() const { return mAuthor; }
    inline QString getAuthorWeb() const { return mAuthorWeb; }
    inline QString getDesciption() const { return mDescription; }

private:

    QString mFileName;
    QString mName;
    QString mAuthor;
    QString mAuthorWeb;
    QString mDescription;

    PairWordVector mData;

    qint64 mCurrentIndex;
};

#endif // DICTIONARY_H
