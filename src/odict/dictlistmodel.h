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


#ifndef DICTLISTMODEL_H
#define DICTLISTMODEL_H

#include <QSqlDatabase>
#include <QAbstractListModel>
#include <QVector>

struct Record
{
    Record() : id(0), priority(0), file_name("") {}
    int id;
    int priority;
    QString file_name;
};

class DictListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DictListModel(QObject *parent = 0);
    ~DictListModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVector<QString> getFilenames();
    QVector<Record> getRecords();
    Record getRecord(int index);

    bool add(const QString& filename);
    bool remove(int id);

    bool moveUp(int id);
    bool moveDown(int id);
    bool refresh();

private:
    int getIndexFromId(int id);
    bool swap(int idx1, int idx2);

    QVector<Record> mLists;
    QSqlDatabase mDb;
    int mLastId;
};

#endif // DICTLISTMODEL_H
