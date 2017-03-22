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

#ifndef DICTMANAGER_H
#define DICTMANAGER_H

#include <QDialog>
#include <QModelIndex>
#include <QItemSelection>
#include <QFile>

class DictListModel;
class Library;

namespace Ui {
class DictListManagerDialog;
}

class DictListManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DictListManagerDialog(DictListModel* m, Library* lib, QWidget *parent = 0);
    ~DictListManagerDialog();

private slots:
    void onUpBtnClicked();
    void onDownBtnClicked();
    void onAddBtnClicked();
    void onRemoveBtnClicked();
    void onInfoBtnClicked();

    void onListSelectionChanged(const QItemSelection &selected,
                                const QItemSelection &deselected);

private:
    void showErrMsg();

    Ui::DictListManagerDialog *ui;
    QFile mFile;
    QModelIndex mCurrSelected;
    DictListModel* mModel;
    Library* mLib;
};

#endif // DICTMANAGER_H
