// ip_header.cpp

#include "ip_header.h"

#include <bitset>
#include <iostream>

void PrintIpHeader(const IpHeader* p_ip_header) {
  std::cout << "IP Version: " << ((p_ip_header->ver_ihl & 0xF0) >> 4) << ", ";
  std::cout << "IHL: " << ((p_ip_header->ver_ihl & 0x0F) * 4) << " bytes, ";
  std::cout << "Total Length: " << ntohs(p_ip_header->length) << "\n";
  std::cout << "Identification: " << ntohs(p_ip_header->id) << "\n";
  std::cout << "Flags: "
            << std::bitset<3>((p_ip_header->frag_offset & 0xE000) >> 13)
            << "\n";
  std::cout << "Fragment Offset: "
            << std::bitset<13>(p_ip_header->frag_offset & 0x1FFF) << "\n";
  // TTL과 Protocol 출력
  std::cout << "TTL: " << static_cast<int>(p_ip_header->ttl) << ", ";
  std::cout << "Protocol: " << static_cast<int>(p_ip_header->protocol) << "\n";
  // Src addr 출력
  std::cout << "Src addr: ";
  for (int i = 0; i < 4; i++) {
    std::cout << std::dec << static_cast<int>(p_ip_header->src_ip[i]);
    if (i < 3) {
      std::cout << ".";
    }
  }
  std::cout << " -> ";
  // Dst addr 출력
  std::cout << "Dst addr: ";
  for (int i = 0; i < 4; i++) {
    std::cout << std::dec << static_cast<int>(p_ip_header->dst_ip[i]);
    if (i < 3) {
      std::cout << ".";
    }
  }
  std::cout << "\n";
}