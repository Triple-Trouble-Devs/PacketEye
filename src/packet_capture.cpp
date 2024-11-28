#include <arpa/inet.h>
#include <netinet/in.h>
#include <pcap.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iomanip>
#include <iostream>

#include "ether_header.h"
#include "ip_header.h"
#include "tcp_header.h"

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr,
                    const u_char *packet) {
  std::cout << "Packet capture length: " << pkthdr->caplen << std::endl;
  // Ether header
  EtherHeader *p_ether = (EtherHeader *)packet;
  PrintMacAddress(p_ether);

  // Ip header
  if (p_ether->type != ntohs(0x0800)) return;
  IpHeader *p_ip_header = (IpHeader *)(packet + sizeof(EtherHeader));
  PrintIpHeader(p_ip_header);

  // total length
  uint16_t total_length = ntohs(p_ip_header->length);
  std::cout << "IP Total Length: " << total_length << " bytes\n";

  // TCP header
  if (p_ether->type != 0x0008) return;
  if (p_ip_header->protocol != 6) return;
  int ip_header_length = (p_ip_header->ver_ihl & 0x0F) * 4;
  TcpHeader *p_tcp_header =
      (TcpHeader *)(packet + sizeof(EtherHeader) + ip_header_length);
  PrintTcpHeader(p_tcp_header);

  // payload
  int tcp_header_length = ((p_tcp_header->data_offset) >> 4 & 0x0F * 4);
  uint8_t *p_payload = (uint8_t *)(packet + sizeof(EtherHeader) +
                                   ip_header_length + tcp_header_length);
  int payload_length = total_length - tcp_header_length;
  if (tcp_header_length != total_length) {
    std::cout << "Payload Length: " << payload_length << " bytes\n";
    std::cout << "Payload Data:\n";
    for (int i = 0; i < payload_length; ++i) {
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(p_payload[i]) << " ";
      if ((i + 1) % 16 == 0) std::cout << "\n";  // 16바이트 단위 줄바꿈
    }
    std::cout << std::dec << std::endl;  // 10진수로 복원
  } else {
    std::cout << "No Payload Data\n";
  }
}

int packet_capture_setup() {
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *alldevs;
  pcap_if_t *d;
  pcap_t *handle;
  int index = 0;
  int select_number = 0;

  // 모든 네트워크 디바이스 찾기
  if (pcap_findalldevs(&alldevs, errbuf) == -1) {
    std::cerr << "Error finding devices: " << errbuf << std::endl;
    return 1;
  }

  // 사용 가능한 디바이스 출력
  for (d = alldevs; d != NULL; d = d->next) {
    std::cout << ++index << ". " << d->name;
    if (d->description)
      std::cout << " (" << d->description << ")" << std::endl;
    else
      std::cout << " (No description available)" << std::endl;
  }

  if (alldevs == NULL) {
    std::cerr << "No devices found" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Enter the interface number (1-" << index << "):";
  std::cin >> select_number;

  if (select_number < 1 || select_number > index) {
    std::cout << "\nInterface number out of range\n";
    pcap_freealldevs(alldevs);
    exit(EXIT_FAILURE);
  }

  /* Jump to the selected adapter */
  for (d = alldevs, index = 0; index < select_number - 1; d = d->next, index++);

  // 디바이스 열기
  const int snaplen = 65535;  // BUFSIZ 대신 명시적인 값 사용
  handle = pcap_open_live(d->name, snaplen, 1, 1000, errbuf);
  if (handle == NULL) {
    std::cerr << "Could not open device " << d->name << ": " << errbuf
              << std::endl;
    pcap_freealldevs(alldevs);
    exit(EXIT_FAILURE);
  }

  // 패킷 캡처 시작
  std::cout << "Starting packet capture on " << d->name << "..." << std::endl;
  pcap_loop(handle, -1, packet_handler, NULL);

  pcap_freealldevs(alldevs);
  pcap_close(handle);
  exit(EXIT_SUCCESS);
}