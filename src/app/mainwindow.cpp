#include "mainwindow.h"

#include <QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      packetCapture(new PacketCapture(this)) {
  ui->setupUi(this);

  // 버튼 클릭 이벤트 연결
  connect(ui->captureButton, &QPushButton::clicked, this,
          &MainWindow::handleCapture);

  // PacketCapture의 신호 연결
  connect(packetCapture, &PacketCapture::devicesListed, this,
          &MainWindow::updatePacketList);
  connect(packetCapture, &PacketCapture::errorOccurred, this,
          &MainWindow::showError);
}

MainWindow::~MainWindow() {
  delete ui;  // Ui 해제
}

void MainWindow::handleCapture() {
  // 네트워크 디바이스 리스트 가져오기
  packetCapture->listDevices();
}

void MainWindow::updatePacketList(const QStringList &packets) {
  // UI 위젯을 업데이트하여 패킷 리스트 표시
  ui->packetListWidget->clear();
  for (const QString &packet : packets) {
    ui->packetListWidget->addItem(packet);
  }
}

void MainWindow::showError(const QString &error) {
  // 에러 메시지 표시
  QMessageBox::critical(this, "Error", error);
}
