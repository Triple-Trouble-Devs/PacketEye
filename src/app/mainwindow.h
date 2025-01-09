#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

#include "packet_capture.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void updateDeviceList(
      const QStringList &devices);  // 디바이스 리스트 업데이트
  void handleCapture();             // 캡처 시작 버튼 핸들링
  void updatePacketList(const QStringList &packets);  // 패킷 리스트 업데이트
  void showError(const QString &error);               // 에러 메시지 표시
  void analyzePacket(const int deviceIndex);          // 패킷 분석 요청
  void startCapture();                                // 캡처 시작
  void stopCapture();
  void updateCaptureButtons(
      bool isCapturing);  // 버튼 상태 업데이트 // 캡처 중지
  void updateTcpHeader(const QString &tcpHeader);  // TCP 헤더 업데이트
  void updateIpHeader(const QString &ipHeader);    // IP 헤더 업데이트
  void updateEtherHeader(const QString &etherHeader);  // Ethernet 헤더 업데이트

 signals:
  void packetAnalysisRequested(int deviceIndex);  // 패킷 분석 요청 신호

 private:
  Ui::MainWindow *ui;            // UI 객체
  PacketCapture *packetCapture;  // PacketCapture 객체
};

#endif  // MAINWINDOW_H