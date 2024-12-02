// tcp_header.cpp

#include "tcp_header.h"

#include <bitset>
#include <iostream>

void PrintTcpHeader(const TcpHeader* p_tcp_header) {
  std::cout << "src port : " << p_tcp_header->src_port;
  std::cout << " -> ";
  std::cout << "dst port : " << p_tcp_header->dst_port;
  std::cout << "\n";
  std::cout << "seq : " << p_tcp_header->sequence_num;
  std::cout << "\n";
  std::cout << "ack num : " << p_tcp_header->ack_num;
  std::cout << "\n";
  std::cout << "data(bytes) : "
            << (static_cast<int>(p_tcp_header->data_offset) >> 4) * 4;
  std::cout << "\n";
  std::cout << "flags : " << std::bitset<8>(p_tcp_header->flags);
  std::cout << "\n";
  std::cout << "window size : " << p_tcp_header->window_size;
  std::cout << "\n";
  std::cout << "checksum : " << p_tcp_header->checksum;
  std::cout << "\n";
  std::cout << "urgent pointer : " << p_tcp_header->urgent_pointer;
  std::cout << "\n";
}
