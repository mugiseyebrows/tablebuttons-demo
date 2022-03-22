#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tablebuttons/tablebuttons_buttons.h"
#include <QStandardItemModel>

#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include <QTextCodec>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadCpp();

    // sample head
    mButtons = new TableButtons::Buttons();
    mButtons->setView(ui->tableView);
    connect(mButtons,SIGNAL(clicked(int,int)),this,SLOT(onButtonsClicked(int,int)));
    // sample head

    QStandardItemModel* m = new QStandardItemModel(5,5,ui->tableView);
    ui->tableView->setModel(m);

    QMap<int,QString> examples;
    examples[ExampleInsertRemove] = "Insert and remove buttons";
    examples[ExampleAppendPrepend] = "Prepend and append buttons";
    examples[ExampleToggleFixed] = "Toggle fixed and variable index";
    examples[ExampleHorizontal] = "Horizontal buttons";

    for(int i=0;i<ExamplesSize;i++) {
        ui->example->addItem(examples.value(i));
    }

    connect(ui->example,SIGNAL(currentIndexChanged(int)),this,SLOT(onExampleCurrentIndexChanged(int)));

    onExampleCurrentIndexChanged(ui->example->currentIndex());

}

#include <QMetaEnum>


QStringList trimmed(const QStringList& vs) {
    QStringList res;
    std::transform(vs.begin(),vs.end(),std::back_inserter(res),[](const QString& line){return line.trimmed();});
    return res;
}


void MainWindow::loadCpp() {
    QDir d(qApp->applicationDirPath());
    QString name = "mainwindow.cpp";
    QString path = d.filePath(name);
    QStringList paths;
    if (QFile(path).exists()) {
        paths << path;
    }
    if (d.dirName() == "debug") {
        d.cdUp();
        path = d.filePath(name);
        if (QFile(path).exists()) {
            paths << path;
        }
    }
    if (!paths.isEmpty()) {
        path = paths[0];
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this,"Error",QString("Cannot open %1").arg(path));
            return;
        }
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));

        QStringList lines = stream.readAll().split("\n");

        QRegularExpression rxExample("if.*==\\s*(example[a-z0-9]*)",QRegularExpression::CaseInsensitiveOption);
        QRegularExpression rxHead("//\\s+sample\\s+head");

        QMetaEnum meta = QMetaEnum::fromType<ExampleType>();

        QList<int> head;

        QList<QPair<int,int> > pos;
        for(int i=0;i<lines.size();i++) {
            QRegularExpressionMatch mExample = rxExample.match(lines[i]);
            if (mExample.hasMatch()) {
                //qDebug() << lines[i];

                //qDebug() << m.captured(1);

                QString key = mExample.captured(1);
                int value = meta.keyToValue(key.toStdString().c_str());
                //qDebug() << key << value;
                pos << QPair<int,int>(i,value);
            }
            QRegularExpressionMatch mHead = rxHead.match(lines[i]);
            if (mHead.hasMatch()) {
                head << i;
            }
        }

        for(int i=0;i<pos.size()-1;i++) {
            int line0 = pos[i].first + 1;
            int line1 = pos[i+1].first - 1;
            int value = pos[i].second;
            mSample[value] = trimmed(lines.mid(line0,line1 - line0 + 1)).join("\n");
        }

        if (head.size() == 2) {
            int line0 = head[0] + 1;
            int line1 = head[1] - 1;
            mSampleHead = trimmed(lines.mid(line0,line1 - line0 + 1)).join("\n");
        }

    }
}

void MainWindow::onExampleCurrentIndexChanged(int example) {

    QAbstractItemModel* m = ui->tableView->model();
    while (m->rowCount() > 5) {
        m->removeRow(0);
    }
    while (m->rowCount() < 5) {
        m->insertRow(0);
    }
    for(int row=0;row<m->rowCount();row++) {
        m->setData(m->index(row,0),QString("%1").arg(row));
    }

    ui->plainTextEdit->setPlainText(mSampleHead + "\n" + mSample.value(example));

    mButtons->clear();
    if (example == ExampleInsertRemove) {
        QSize size(40,30);
        mButtons->button(ButtonRemove).remove().text("del").size(size);
        mButtons->button(ButtonInsert).insert().text("ins").size(size).offset(size.width(),0);
    } else if (example == ExampleAppendPrepend) {
        QSize size(40,30);
        mButtons->button(ButtonPrepend).prepend().text("pre").size(size);
        mButtons->button(ButtonAppend).append().text("app").size(size);
    } else if (example == ExampleToggleFixed) {
        QSize size(40,30);
        mButtons->button(ButtonToggle).text("var").size(size);
    } else if (example == ExampleHorizontal) {
        QSize size(40,30);
        mButtons->button(ButtonHorizontal).horizontal().text("hor").size(size);
    } else if (example == ExamplesSize) {
        // for regexps
    }


}

void MainWindow::onButtonsClicked(int id, int index) {

    //qDebug() << id << index;
    if (id == ButtonInsert || id == ButtonAppend || id == ButtonPrepend) {
        QAbstractItemModel* m = ui->tableView->model();
        m->insertRow(index);
    } else if (id == ButtonRemove) {
        QAbstractItemModel* m = ui->tableView->model();
        m->removeRow(index);
    } else if (id == ButtonToggle) {
        auto button = mButtons->button(ButtonToggle);
        if (button.fixed()) {
            button.variable();
            button.text("var");
        } else {
            button.fixed(index);
            button.text("fix");
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui; // todo: double free (wat)
}
