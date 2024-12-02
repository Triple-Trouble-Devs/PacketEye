#ifndef PACKET_CAPTURE_H
#define PACKET_CAPTURE_H

#include <QObject>
#include <QString>
#include <QStringList>

class PacketCapture : public QObject {
  Q_OBJECT

 public:
  explicit PacketCapture(QObject *parent = nullptr);
  ~PacketCapture() noexcept;

  void listDevices();
  void startCapture(const QString &deviceName);

 signals:
  void devicesListed(const QStringList &packets);  // 패킷 캡처 신호
  void errorOccurred(const QString &error);        // 에러 발생 신호

 private:
  QStringList capturedPackets;
};

#endif  // PACKET_CAPTURE_H