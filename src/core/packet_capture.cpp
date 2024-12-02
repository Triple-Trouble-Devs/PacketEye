#include "packet_capture.h"

#include <pcap.h>

#include <QString>
#include <QStringList>
#include <iostream>

PacketCapture::PacketCapture(QObject *parent) : QObject(parent) {}

PacketCapture::~PacketCapture() noexcept {}

// 네트워크 디바이스 목록을 가져오는 함수
void PacketCapture::listDevices() {
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *alldevs = nullptr;

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
  QStringList deviceList;
  for (pcap_if_t *dev = alldevs; dev != nullptr; dev = dev->next) {
    QString deviceInfo = QString("Name: %1").arg(dev->name);
    if (dev->description) {
      deviceInfo += QString(", Description: %1").arg(dev->description);
    } else {
      deviceInfo += ", Description: (No description available)";
    }
    deviceList.append(deviceInfo);
  }

  // 디바이스 리스트 전달
  emit devicesListed(deviceList);

  // 리소스 해제
  pcap_freealldevs(alldevs);
}
