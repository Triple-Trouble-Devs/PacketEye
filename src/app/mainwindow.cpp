#include "mainwindow.h"

#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

#include "ui_mainwindow.h"

constexpr char kNoDevicesAvailable[] = "No devices available.";
constexpr char kNoDeviceSelected[] = "No device selected.";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      packetCapture(new PacketCapture(this)) {
  ui->setupUi(this);

  // 버튼 클릭 이벤트 연결
  connect(ui->captureButton, &QPushButton::clicked, this,
          &MainWindow::handleCapture);
  connect(ui->startCaptureButton, &QPushButton::clicked, this,
          &MainWindow::startCapture);
  connect(ui->stopCaptureButton, &QPushButton::clicked, this,
          &MainWindow::stopCapture);

  // PacketCapture의 신호 연결
  connect(packetCapture, &PacketCapture::devicesListed, this,
          &MainWindow::updateDeviceList);
  connect(packetCapture, &PacketCapture::errorOccurred, this,
          &MainWindow::showError);

  // PacketCapture의 분석 신호 연결
  connect(packetCapture, &PacketCapture::tcpHeaderAnalyzed, this,
          &MainWindow::updateTcpHeader);
  connect(packetCapture, &PacketCapture::ipHeaderAnalyzed, this,
          &MainWindow::updateIpHeader);
  connect(packetCapture, &PacketCapture::etherHeaderAnalyzed, this,
          &MainWindow::updateEtherHeader);

  // PacketCapture의 슬롯 연결
  connect(this, &MainWindow::packetAnalysisRequested, packetCapture,
          &PacketCapture::startCapture);
  updateCaptureButtons(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handleCapture() {
  // 네트워크 디바이스 리스트 가져오기
  packetCapture->listDevices();
}

void MainWindow::updatePacketList(const QStringList &packets) {
  // UI 위젯을 업데이트하여 패킷 리스트 표시
  ui->packetListWidget->clear();
  ui->packetListWidget->addItems(packets);
}

void MainWindow::showError(const QString &error) {
  // 에러 메시지 표시
  QMessageBox::critical(this, "Error", error);
}

void MainWindow::updateTcpHeader(const QString &header) {
  ui->packetListWidget->addItem("TCP Header: " + header);
}

void MainWindow::updateIpHeader(const QString &header) {
  ui->packetListWidget->addItem("IP Header: " + header);
}

void MainWindow::updateEtherHeader(const QString &header) {
  ui->packetListWidget->addItem("Ethernet Header: " + header);
}

void MainWindow::analyzePacket(const int deviceIndex) {
  // 패킷 분석 시작
  emit packetAnalysisRequested(deviceIndex);
}

void MainWindow::startCapture() {
  // 선택된 디바이스 인덱스 가져오기
  if (ui->deviceListWidget->count() == 0) {
    showError(kNoDevicesAvailable);
    return;
  }

  int deviceIndex = ui->deviceListWidget->currentRow();
  if (deviceIndex < 0) {
    showError(kNoDeviceSelected);
    return;
  }

  // 비동기로 패킷 캡처 시작
  QFuture<void> future =
      QtConcurrent::run([=]() { packetCapture->startCapture(deviceIndex); });
  // 버튼 상태 업데이트
  updateCaptureButtons(true);
}

void MainWindow::stopCapture() {
  // 비동기로 패킷 캡처 중지
  QFuture<void> future =
      QtConcurrent::run([this]() { packetCapture->stopCapture(); });
  // 버튼 상태 업데이트
  updateCaptureButtons(false);
}

void MainWindow::updateDeviceList(const QStringList &devices) {
  // UI 위젯을 업데이트하여 디바이스 리스트 표시
  ui->deviceListWidget->clear();
  ui->deviceListWidget->addItems(devices);
}

// 캡처 버튼 상태 업데이트
void MainWindow::updateCaptureButtons(bool isCapturing) {
  ui->startCaptureButton->setEnabled(!isCapturing);
  ui->stopCaptureButton->setEnabled(isCapturing);
}