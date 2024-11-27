// ether_header.cpp

#include "ether_header.h"

#include <cstring>
#include <iomanip>
#include <iostream>

/**
 * @brief MAC 주소를 출력하는 함수
 * @param mac 6바이트 크기의 MAC 주소 배열
 */
void PrintMacAddress(const EtherHeader* p_ether) {
  for (int i = 0; i < 6; i++) {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(p_ether->src_mac[i]);
    if (i < 5) {
      std::cout << ":";
    }
  }
  std::cout << " -> ";
  for (int i = 0; i < 6; i++) {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(p_ether->dst_mac[i]);
    if (i < 5) {
      std::cout << ":";
    }
  }
  // Ethernet 타입 출력
  std::cout << " Ethernet Type: " << std::hex << std::setw(4)
            << ntohs(p_ether->type) << std::dec << std::endl;
}
