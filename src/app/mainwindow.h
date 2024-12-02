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
  void handleCapture();
  void updatePacketList(const QStringList &packets);
  void showError(const QString &error);

 private:
  Ui::MainWindow *ui;
  PacketCapture *packetCapture;  // PacketCapture 객체를 멤버로 추가
};

#endif  // MAINWINDOW_H
