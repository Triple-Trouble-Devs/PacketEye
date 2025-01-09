#ifndef PACKET_CAPTURE_H
#define PACKET_CAPTURE_H

#include <pcap.h>

#include <QObject>
#include <QString>
#include <QStringList>

#include "ether_header.h"
#include "ip_header.h"
#include "tcp_header.h"

class PacketCapture : public QObject {
  Q_OBJECT

 public:
  explicit PacketCapture(QObject *parent = nullptr);
  ~PacketCapture() noexcept;

  void listDevices();
  static void analyzePacket(u_char *userData, const struct pcap_pkthdr *pkthdr,
                            const u_char *packet);

 signals:
  void devicesListed(const QStringList &packets);  // 패킷 캡처 신호
  void tcpHeaderAnalyzed(const QString &tcpHeader);
  void ipHeaderAnalyzed(const QString &ipHeader);
  void etherHeaderAnalyzed(const QString &etherHeader);
  void errorOccurred(const QString &error);  // 에러 발생 신호

 public slots:
  void startCapture(const int deviceIndex);
  void stopCapture();

 private:
  void processPacket(const struct pcap_pkthdr *pkthdr, const u_char *packet);
  QStringList devicesList;
  pcap_t *handle;
  pcap_if_t *alldevs;
};

#endif  // PACKET_CAPTURE_H