// ether_header.h

#ifndef ETHER_HEADER_H_
#define ETHER_HEADER_H_

#include <cstdint>

/**
 * @brief Ethernet Header 구조체 정의
 *
 * 이 구조체는 Ethernet 프레임의 헤더를 나타냅니다.
 * 각 필드는 Ethernet 프로토콜의 주요 요소를 구성합니다.
 *
 * 필드 설명:
 * - src_mac: 출발지 MAC 주소 (6바이트)
 * - dst_mac: 도착지 MAC 주소 (6바이트)
 * - type:    EtherType (상위 프로토콜 식별, 예: IPv4, ARP)
 */
#pragma pack(push, 1)
typedef struct EtherHeader {
  uint8_t src_mac[6];
  uint8_t dst_mac[6];
  uint16_t type;
} EtherHeader;
#pragma pack(pop)

void PrintMacAddress(const EtherHeader* p_ether);

#endif