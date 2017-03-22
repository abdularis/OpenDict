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

#include "dictinfo.h"
#include "ui_dictinfodialog.h"

#include "odict/dictionary.h"

DictInfoDialog::DictInfoDialog(Dictionary* d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DictInfoDialog)
{
    ui->setupUi(this);

    if (d) {
        ui->tableWidget->setRowCount(4);
        ui->tableWidget->setColumnCount(2);

        ui->tableWidget->setColumnWidth(1, 250);
        ui->tableWidget->setItem(0, 0, new QTableWidgetItem(tr("Name")));
        ui->tableWidget->setItem(1, 0, new QTableWidgetItem(tr("Author")));
        ui->tableWidget->setItem(2, 0, new QTableWidgetItem(tr("Author Website")));
        ui->tableWidget->setItem(3, 0, new QTableWidgetItem(tr("Description")));

        ui->tableWidget->setItem(0, 1, new QTableWidgetItem(d->getName()));
        ui->tableWidget->setItem(1, 1, new QTableWidgetItem(d->getAuthor()));
        ui->tableWidget->setItem(2, 1, new QTableWidgetItem(d->getAuthorWeb()));
        ui->tableWidget->setItem(3, 1, new QTableWidgetItem(d->getDesciption()));
    }
}

DictInfoDialog::~DictInfoDialog()
{
    delete ui;
}
