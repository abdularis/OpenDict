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

#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>

#include "dictlistmanagerdialog.h"
#include "ui_dictlistmanagerdialog.h"
#include "dictinfo.h"
#include "odict/dictlistmodel.h"
#include "odict/library.h"
#include "dictinfo.h"

#include "common.h"

DictListManagerDialog::DictListManagerDialog(DictListModel* m, Library* lib, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DictListManagerDialog),
    mModel(m),
    mLib(lib)
{
    ui->setupUi(this);
    mCurrSelected = QModelIndex();

    ui->listView->setModel(mModel);

    connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(onListSelectionChanged(QItemSelection,QItemSelection)));
}

DictListManagerDialog::~DictListManagerDialog()
{
    delete ui;
}

void DictListManagerDialog::onUpBtnClicked()
{
    if (mCurrSelected.isValid()) {
        bool ok = mModel->moveUp(mModel->getRecord(mCurrSelected.row()).id);
        if (!ok) {
            showErrMsg();
        }
        else {
            mCurrSelected = mModel->index(mCurrSelected.row() - 1);
            ui->listView->selectionModel()->select(mCurrSelected,
                                                   QItemSelectionModel::Clear | QItemSelectionModel::Select);
        }
    }
}

void DictListManagerDialog::onDownBtnClicked()
{
    if (mCurrSelected.isValid()) {
        bool ok = mModel->moveDown(mModel->getRecord(mCurrSelected.row()).id);
        if (!ok) {
            showErrMsg();
        }
        else {
            mCurrSelected = mModel->index(mCurrSelected.row() + 1);
            ui->listView->selectionModel()->select(mCurrSelected,
                                                   QItemSelectionModel::Clear | QItemSelectionModel::Select);
        }
    }
}

void DictListManagerDialog::onAddBtnClicked()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Select a dict file"), QDir::homePath());
    if (!fname.isEmpty()) {
        if (!mLib->load(fname)) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to load dict file: \n") + fname);
        }
        else {
            if (!mModel->add(fname)) {
                showErrMsg();
            }
        }
    }
}

void DictListManagerDialog::onRemoveBtnClicked()
{
    if (mCurrSelected.isValid()) {
        int idx = mCurrSelected.row();
        bool ok = mModel->remove(mModel->getRecord(mCurrSelected.row()).id);
        if (!ok) {
            showErrMsg();
        }
        else {
            mLib->removeDict(idx);
        }
    }
}

void DictListManagerDialog::onInfoBtnClicked()
{
    if (mCurrSelected.isValid() && mLib) {
        QString fname = mModel->data(mCurrSelected).toString();
        Dictionary* dict = mLib->getDict(fname);
        if (dict) {
            DictInfoDialog d(dict);
            d.exec();
        }
    }
}

void DictListManagerDialog::onListSelectionChanged(const QItemSelection &selected,
                                                   const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList lst = selected.indexes();
    if (lst.size() > 0) {
        QModelIndex currIndex = lst.at(0);
        if (currIndex.isValid()) {
            ui->downBtn->setEnabled(true);
            ui->removeBtn->setEnabled(true);
            ui->infoBtn->setEnabled(true);

            mCurrSelected = currIndex;

            if (mCurrSelected == mModel->index(0, 0))
                ui->upBtn->setEnabled(false);
            else
                ui->upBtn->setEnabled(true);

            int lastRow = mModel->rowCount() - 1;
            if (mCurrSelected == mModel->index(lastRow, 0))
                ui->downBtn->setEnabled(false);
            else
                ui->downBtn->setEnabled(true);
        }
    }
    else {
        ui->upBtn->setEnabled(false);
        ui->downBtn->setEnabled(false);
        ui->removeBtn->setEnabled(false);
        ui->infoBtn->setEnabled(false);

        mCurrSelected = QModelIndex();
    }
}

void DictListManagerDialog::showErrMsg()
{
    // haha
    QMessageBox::critical(this, tr("Error"), tr("Something is wrong, i can feel it."));
}
