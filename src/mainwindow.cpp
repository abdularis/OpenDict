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

#include <QFile>
#include <QMessageBox>
#include <QClipboard>
#include <QSettings>
#include <QCloseEvent>
#include <QtDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "dictlistmanagerdialog.h"
#include "common.h"
#include "odict/library.h"
#include "odict/dictlistmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadConfig();

    mClipboard = QApplication::clipboard();
    mLib = new Library();
    mDListModel = new DictListModel(this);
    if (!mDListModel->refresh()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to read Dict List DB file."));
    }
    else {
        loadDictionaries();
    }

    ui->statusBar->addPermanentWidget(new QLabel(QString::number(mLib->getWordCount()) + tr(" words")));

    connect(ui->resultList, SIGNAL(itemSelectionChanged()), this, SLOT(onResultListSelectionChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mLib;
}

void MainWindow::loadDictionaries()
{
    QVector<QString> fnames = mDListModel->getFilenames();
    for (QString fname : fnames) {
        if (!mLib->load(fname)) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to load dictionary:\n") + fname);
        }
    }
}

void MainWindow::setTranslationTextWith(int resIndex)
{
    if (mCurrResult.size() > 0) {
        PairWord p = mCurrResult.at(resIndex);

        QString str = "<font color=blue> <i><b>" +
                        p.first +
                        "</i></b> </font> <br>" +
                        p.second;
        ui->transText->setText(str);
    }
}

void MainWindow::search(const QString &text)
{
    QString keyword = text.simplified();
    if (keyword.size() <= 0) {
        mCurrResult.clear();
        ui->resultList->clear();
        ui->transText->clear();
        ui->statusBar->clearMessage();
        return;
    }

    ui->resultList->clear();
    ui->transText->clear();

    mCurrResult = mLib->search(keyword);
    if (mCurrResult.size() == 0)
        return;

    for (PairWord w : mCurrResult) {
        ui->resultList->addItem(w.first);
    }

    ui->resultList->item(0)->setSelected(true);
    ui->statusBar->showMessage(QString::number(mCurrResult.size()) + tr(" Results."), 2000);
}

void MainWindow::onResultListSelectionChanged()
{
    QList<QListWidgetItem*> selectedList = ui->resultList->selectedItems();
    if (selectedList.size() > 0 && mCurrResult.size() > 0) {
        QListWidgetItem* sel = selectedList.at(0);
        if (sel) {
            int row = ui->resultList->row(sel);
            setTranslationTextWith(row);
        }
    }
}

void MainWindow::onAutoSearchToggled(bool checked)
{
    if (checked) {
        connect(ui->keywordLinedEdit, SIGNAL(textChanged(QString)),
                this, SLOT(onKeywordTextChanged(QString)));
    }
    else {
        disconnect(ui->keywordLinedEdit, SIGNAL(textChanged(QString)),
                   this, SLOT(onKeywordTextChanged(QString)));
    }
}

void MainWindow::onKeywordTextChanged(const QString &text)
{
    search(text);
}

void MainWindow::onKeywordReturnPressed()
{
    search(ui->keywordLinedEdit->text());
}

void MainWindow::onAboutActTriggered()
{
    AboutDialog d;
    d.exec();
}

void MainWindow::onAboutQtActTriggered()
{
    qApp->aboutQt();
}

void MainWindow::onManageDictActTriggered()
{
    DictListManagerDialog d(mDListModel, mLib);
    d.exec();
}

void MainWindow::onScanClipboardActToggled(bool checked)
{
    if (checked) {
        connect(mClipboard, SIGNAL(selectionChanged()), this, SLOT(clipboardSearch()));
    }
    else {
        disconnect(mClipboard, SIGNAL(selectionChanged()), this, SLOT(clipboardSearch()));
    }
}

void MainWindow::clipboardSearch()
{
    search(mClipboard->text(QClipboard::Selection));
}

void MainWindow::saveConfig()
{
    QSettings set(PROGRAM_NAME);
    set.setValue("AutoSearch", ui->autoSearchCB->isChecked());
    set.setValue("Geometry", saveGeometry());
}

void MainWindow::loadConfig()
{
    QSettings set(PROGRAM_NAME);
    bool autoSearch = set.value("AutoSearch", false).toBool();
    QByteArray geometry = set.value("Geometry", QByteArray()).toByteArray();

    ui->autoSearchCB->setChecked(autoSearch);
    restoreGeometry(geometry);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveConfig();
    event->accept();
}
