#include <arpa/inet.h>
#include <netinet/in.h>
#include <pcap.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr,
                    const u_char *packet) {
  std::cout << "Packet capture length: " << pkthdr->caplen << std::endl;
  std::cout << "Packet total length: " << pkthdr->len << std::endl;
}

int main() {
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

  // 첫 번째 디바이스 사용
  if (alldevs == NULL) {
    std::cerr << "No devices found" << std::endl;
    return 1;
  }

  std::cout << "Enter the interface number (1-" << index << "):";
  std::cin >> select_number;

  if (select_number < 1 || select_number > index) {
    std::cout << "\nInterface number out of range\n";
    pcap_freealldevs(alldevs);
    return -1;
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
    return 1;
  }

  // 패킷 캡처 시작
  std::cout << "Starting packet capture on " << d->name << "..." << std::endl;
  pcap_loop(handle, -1, packet_handler, NULL);

  pcap_freealldevs(alldevs);
  pcap_close(handle);
  return 0;
}