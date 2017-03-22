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

#ifndef DICTINFO_H
#define DICTINFO_H

#include <QDialog>

class Dictionary;

namespace Ui {
class DictInfoDialog;
}

class DictInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DictInfoDialog(Dictionary* d, QWidget *parent = 0);
    ~DictInfoDialog();

private:
    Ui::DictInfoDialog *ui;
};

#endif // DICTINFO_H
