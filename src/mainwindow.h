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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "odict/library.h"

class QClipboard;
class DictListModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onAutoSearchToggled(bool checked);
    void onKeywordTextChanged(const QString& text);
    void onKeywordReturnPressed();

    //actions
    void onAboutActTriggered();
    void onAboutQtActTriggered();
    void onManageDictActTriggered();
    void onScanClipboardActToggled(bool checked);

    void onResultListSelectionChanged();

    void clipboardSearch();

protected:
    void closeEvent(QCloseEvent* event);

private:
    void loadDictionaries();
    void setTranslationTextWith(int resIndex);
    void search(const QString& text);

    void saveConfig();
    void loadConfig();

    Ui::MainWindow *ui;

    PairWordVector mCurrResult;
    Library* mLib;
    DictListModel* mDListModel;

    QClipboard* mClipboard;
};

#endif // MAINWINDOW_H
