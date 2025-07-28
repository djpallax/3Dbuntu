#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QSerialPort>
#include <QSerialPortInfo>

class QComboBox;
class QTextEdit;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshSerialPorts();
    void connectToPort();
    void sendGcodeCommand();

private:
    QComboBox* portSelector;
    QComboBox* baudrateSelector;
    QTextEdit* console;
    QLineEdit* inputLine;
    QPushButton* connectButton;
    QString receiveBuffer;

    QSerialPort serial;
};
