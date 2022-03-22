#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace TableButtons {
class Buttons;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ButtonId {
        ButtonInsert,
        ButtonRemove,
        ButtonAppend,
        ButtonPrepend,
        ButtonToggle,
        ButtonHorizontal
    };
    enum ExampleType {
        ExampleInsertRemove,
        ExampleAppendPrepend,
        ExampleToggleFixed,
        ExampleHorizontal,
        ExamplesSize
    };
    Q_ENUM(ExampleType)

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void loadCpp();
    Ui::MainWindow *ui;
    TableButtons::Buttons* mButtons;
    QMap<int,QString> mSample;
    QString mSampleHead;
protected slots:
    void onButtonsClicked(int, int);
    void onExampleCurrentIndexChanged(int example);
};

#endif // MAINWINDOW_H
