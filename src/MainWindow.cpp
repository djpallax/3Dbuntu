#include "MainWindow.hpp"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    portSelector = new QComboBox(this);
    baudrateSelector = new QComboBox(this);
    connectButton = new QPushButton("Conectar", this);
    console = new QTextEdit(this);
    console->setReadOnly(true);

    // Valores de baudrate típicos
    QList<int> baudrates = { 250000, 115200, 57600, 38400, 19200, 9600 };
    for (int rate : baudrates)
        baudrateSelector->addItem(QString::number(rate), rate);
    baudrateSelector->setCurrentIndex(0); // por defecto 250000

    // Layout superior
    auto *topRow = new QHBoxLayout();
    topRow->addWidget(portSelector);
    topRow->addWidget(baudrateSelector);
    topRow->addWidget(connectButton);

    mainLayout->addLayout(topRow);
    mainLayout->addWidget(console);

    // Línea para ingresar comandos G-code
    auto *bottomRow = new QHBoxLayout();
    inputLine = new QLineEdit(this);
    auto *sendButton = new QPushButton("Enviar", this);
    bottomRow->addWidget(inputLine);
    bottomRow->addWidget(sendButton);

    mainLayout->addLayout(bottomRow);

    // Enviar al presionar botón
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendGcodeCommand);

    // Enviar con Enter
    connect(inputLine, &QLineEdit::returnPressed, this, &MainWindow::sendGcodeCommand);

    setCentralWidget(central);
    setWindowTitle("3Dbuntu");
    resize(800, 500);

    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToPort);
    connect(&serial, &QSerialPort::readyRead, [this]() {
    receiveBuffer += QString::fromUtf8(serial.readAll());

    int newlineIndex;
    while ((newlineIndex = receiveBuffer.indexOf('\n')) != -1) {
        QString line = receiveBuffer.left(newlineIndex).trimmed();
        receiveBuffer.remove(0, newlineIndex + 1);
        if (!line.isEmpty()) {
            // Si empieza con "echo:", saco
            if (line.startsWith("echo:"))
                line.remove(0, 5); // Borra los primeros 5 caracteres
            console->append("    " + line.trimmed());
        }
    }
});


    refreshSerialPorts();
    QTimer::singleShot(1000, this, &MainWindow::refreshSerialPorts);
}

MainWindow::~MainWindow() = default;

void MainWindow::refreshSerialPorts() {
    portSelector->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        portSelector->addItem(info.portName());
    }

    if (portSelector->count() == 0) {
        console->append("No se detectaron puertos serie.");
    }
}

void MainWindow::connectToPort() {
    if (serial.isOpen()) {
        serial.close();
        connectButton->setText("Conectar");
        console->append("Desconectado.");
        return;
    }

    QString portName = portSelector->currentText();
    if (portName.isEmpty()) {
        console->append("No hay puerto seleccionado.");
        return;
    }

    int baudrate = baudrateSelector->currentData().toInt();

    serial.setPortName(portName);
    serial.setBaudRate(static_cast<QSerialPort::BaudRate>(baudrate));

    if (!serial.open(QIODevice::ReadWrite)) {
        console->append("Error al abrir el puerto.");
        return;
    }

    connectButton->setText("Desconectar");
    console->append("Conectado a " + portName + " @ " + QString::number(baudrate) + " bps");
}

void MainWindow::sendGcodeCommand() {
    if (!serial.isOpen()) {
        console->append("No conectado a ningún puerto.");
        return;
    }

    QString command = inputLine->text().trimmed();
    if (command.isEmpty()) return;

    QByteArray data = command.toUtf8() + '\n';  // G-code suele terminar en \n
    serial.write(data);

    console->append(">> " + command);
    inputLine->clear();
}
