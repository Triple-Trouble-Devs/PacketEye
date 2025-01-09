#include "packet_capture.h"

#include <pcap.h>

#include <QDebug>
#include <QString>
#include <QStringList>
#include <iostream>

#include "ether_header.h"
#include "ip_header.h"
#include "tcp_header.h"

PacketCapture::PacketCapture(QObject *parent) : QObject(parent) {}

PacketCapture::~PacketCapture() noexcept {
  if (handle) {
    pcap_close(handle);
    handle = nullptr;
    pcap_freealldevs(alldevs);
  }
}

// 네트워크 디바이스 목록을 가져오는 함수
void PacketCapture::listDevices() {
  char errbuf[PCAP_ERRBUF_SIZE];

  // 네트워크 디바이스 목록 검색
  if (pcap_findalldevs(&alldevs, errbuf) == -1) {
    emit errorOccurred(QString("Error finding devices: %1").arg(errbuf));
    return;
  }

  if (!alldevs) {
    emit errorOccurred("No devices found.");
    return;
  }

  // 디바이스 리스트 작성
  devicesList.clear();
  for (pcap_if_t *dev = alldevs; dev != nullptr; dev = dev->next) {
    QString deviceInfo = QString(dev->name);
    devicesList.append(deviceInfo);
  }

  // 디바이스 리스트 전달
  emit devicesListed(devicesList);

  // 리소스 해제
  pcap_freealldevs(alldevs);
}

//
// Converts an IP address from a byte array to a string representation
QString ipToString(const uint8_t *ip) {
  return QString::asprintf("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

// Converts a MAC address from a byte array to a string representation

QString macToString(const uint8_t *mac) {
  return QString::asprintf("%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1],
                           mac[2], mac[3], mac[4], mac[5]);
}

// 패킷 처리 핸들러
void PacketCapture::analyzePacket(u_char *userData,
                                  const struct pcap_pkthdr *pkthdr,
                                  const u_char *packet) {
  // `userData`를 `PacketCapture` 객체로 변환
  PacketCapture *packetCapture = reinterpret_cast<PacketCapture *>(userData);
  if (packetCapture) {
    packetCapture->processPacket(pkthdr, packet);
  }
}

// 패킷 처리 함수
void PacketCapture::processPacket(const struct pcap_pkthdr *pkthdr,
                                  const u_char *packet) {
  // TCP 헤더 분석
  const TcpHeader *tcpHeader = reinterpret_cast<const TcpHeader *>(
      packet + sizeof(EtherHeader) + sizeof(IpHeader));
  QString tcpHeaderStr =
      QString(
          "src port: %1, dst port: %2, seq: %3, ack num: %4, data offset: "
          "%5, "
          "flags: %6, window size: %7, checksum: %8")
          .arg(tcpHeader->src_port)
          .arg(tcpHeader->dst_port)
          .arg(tcpHeader->sequence_num)
          .arg(tcpHeader->ack_num)
          .arg((static_cast<int>(tcpHeader->data_offset) >> 4) * 4)
          .arg(std::bitset<8>(tcpHeader->flags).to_string().c_str())
          .arg(tcpHeader->window_size)
          .arg(tcpHeader->checksum);
  emit tcpHeaderAnalyzed(tcpHeaderStr);

  // IP 헤더 분석
  const IpHeader *ipHeader =
      reinterpret_cast<const IpHeader *>(packet + sizeof(EtherHeader));
  QString ipHeaderStr =
      QString(
          "src IP: %1, dst IP: %2, total length: %3, identification: %4, "
          "flags: %5, fragment offset: %6, TTL: %7, protocol: %8, checksum: "
          "%9")
          .arg(ipToString(ipHeader->src_ip))
          .arg(ipToString(ipHeader->dst_ip))
          .arg(ipHeader->total_length)
          .arg(ipHeader->identification)
          .arg(ipHeader->flags)
          .arg(ipHeader->fragment_offset)
          .arg(ipHeader->ttl)
          .arg(ipHeader->protocol)
          .arg(ipHeader->checksum);
  emit ipHeaderAnalyzed(ipHeaderStr);

  // Ethernet 헤더 분석
  const EtherHeader *etherHeader =
      reinterpret_cast<const EtherHeader *>(packet);
  QString etherHeaderStr = QString("src MAC: %1, dst MAC: %2, type: %3")
                               .arg(macToString(etherHeader->src_mac))
                               .arg(macToString(etherHeader->dst_mac))
                               .arg(etherHeader->type);
  emit etherHeaderAnalyzed(etherHeaderStr);
}

// 패킷 캡처를 시작하는 함수
void PacketCapture::startCapture(const int deviceIndex) {
  if (deviceIndex < 0 || deviceIndex >= devicesList.size()) {
    emit errorOccurred("Invalid device index.");
    return;
  }

  char errbuf[PCAP_ERRBUF_SIZE];
  const int snaplen = 65535;

  handle = pcap_open_live(devicesList[deviceIndex].toLocal8Bit().constData(),
                          snaplen, 1, 1000, errbuf);

  if (handle == nullptr) {
    emit errorOccurred(QString("Error opening device: %1").arg(errbuf));
    return;
  }

  pcap_loop(handle, 0, PacketCapture::analyzePacket,
            reinterpret_cast<u_char *>(this));  // 패킷 캡처 루프 시작
}

void PacketCapture::stopCapture() {
  if (handle) {
    pcap_breakloop(handle);
    pcap_close(handle);
    handle = nullptr;
  }
}