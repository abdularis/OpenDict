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

#include <QSqlQuery>
#include <QVariant>
#include <QtDebug>

#include "dictlistmodel.h"

DictListModel::DictListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    mDb = QSqlDatabase::addDatabase("QSQLITE", "dict_list");
    mDb.setDatabaseName("dictlist.db");
    if (!mDb.open()) {
        qDebug() << "[!] Failed to open DB: dictlist.db";
        return;
    }
}

DictListModel::~DictListModel()
{
}

int DictListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mLists.size();
}

QVariant DictListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        return mLists.at(index.row()).file_name;
    }
    return QVariant();
}

QVector<QString> DictListModel::getFilenames()
{
    QVector<QString> v;
    for (auto l : mLists)
        v.push_back(l.file_name);
    return v;
}

QVector<Record> DictListModel::getRecords()
{
    return mLists;
}

Record DictListModel::getRecord(int index)
{
    if (index < 0 || index >= mLists.size())
        return Record();
    return mLists[index];
}

bool DictListModel::refresh()
{
    if (!mDb.isOpen())
        return false;

    beginResetModel();
    mLists.clear();
    QSqlQuery query("SELECT * FROM dict_list ORDER BY priority;", mDb);
    while (query.next()) {
        Record r;
        r.id = query.value(0).toInt();
        r.priority = query.value(1).toInt();
        r.file_name = query.value(2).toString();

        mLists.push_back(r);
    }

    query = QSqlQuery("SELECT seq FROM sqlite_sequence;", mDb);
    if (query.first()) {
        mLastId = query.value(0).toInt();
    }
    endResetModel();
    return true;
}

bool DictListModel::add(const QString& filename)
{
    if (!mDb.isOpen())
        return false;

    QSqlQuery q(mDb);
    q.prepare("INSERT INTO dict_list(priority, file_name) VALUES(?, ?);");
    q.addBindValue(mLastId + 1);
    q.addBindValue(filename);
    if (q.exec()) {
        mLastId++;

        Record r;
        r.id = q.lastInsertId().toInt();
        r.priority = mLastId;
        r.file_name = filename;

        beginInsertRows(QModelIndex(), mLists.size(), mLists.size());
        mLists.push_back(r);
        endInsertRows();

        return true;
    }
    return false;
}

bool DictListModel::remove(int id)
{
    if (!mDb.isOpen())
        return false;

    QSqlQuery q(mDb);
    q.prepare("DELETE FROM dict_list WHERE id=?;");
    q.addBindValue(id);
    bool ok = q.exec();
    if (!ok)
        return false;
    else {
        beginRemoveRows(QModelIndex(), mLists.size() - 1, mLists.size() - 1);
        mLists.remove(getIndexFromId(id));
        endRemoveRows();
    }
    return true;
}

bool DictListModel::moveUp(int id)
{
    int idx1 = getIndexFromId(id);
    int idx2 = idx1 - 1;
    return swap(idx1, idx2);
}

bool DictListModel::moveDown(int id)
{
    int idx1 = getIndexFromId(id);
    int idx2 = idx1 + 1;
    return swap(idx1, idx2);
}

int DictListModel::getIndexFromId(int id)
{
    for (int i = 0; i < mLists.size(); i++) {
        if (mLists.at(i).id == id)
            return i;
    }
    return -1;
}

bool DictListModel::swap(int idx1, int idx2)
{
    if (idx1 < 0 || idx1 >= mLists.size() || idx2 < 0 || idx2 >= mLists.size())
        return false;

    Record r1 = mLists.at(idx1);
    Record r2 = mLists.at(idx2);

    int p = r1.priority;
    r1.priority = r2.priority;
    r2.priority = p;

    mLists[idx1] = r2;
    mLists[idx2] = r1;

    QSqlQuery q(mDb);
    q.prepare("UPDATE dict_list SET priority=? WHERE id=?;");
    q.addBindValue(r1.priority);
    q.addBindValue(r1.id);
    bool ok = q.exec();

    q.addBindValue(r2.priority);
    q.addBindValue(r2.id);

    ok = ok && q.exec();
    if (ok) {
        emit dataChanged(index(idx1), index(idx2));
    }
    return ok;
}
