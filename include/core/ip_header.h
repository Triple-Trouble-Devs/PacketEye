// ip_header.h

#ifndef IP_HEADER_H_
#define IP_HEADER_H_

#include <cstdint>

/**
 * @brief
 * IP Header 구조체 정의
 * ** 이 구조체는 IPv4 패킷의 헤더 정보를 나타냅니다.
 * 각 필드는 IPv4 프로토콜의 주요 요소를 구성합니다.
 *
 * 필드 설명 :
 * - ver_ihl : 버전(4비트) + 헤더 길이(4비트)
 * - tos : 서비스 타입(Type of Service)
 * - total_length : 전체 패킷의 길이(헤더 + 데이터)
 * - identification : 패킷 식별자(Identification)
 * - flags : 플래그
 * - fragment_offset : 조각 오프셋(13비트)
 * - ttl : 패킷 생존 시간(Time to Live)
 * - protocol : 상위 계층 프로토콜(예: TCP, UDP)
 * - checksum : 헤더 체크섬(무결성 확인)
 * - src_ip : 출발지 IP 주소
 * - dst_ip : 도착지 IP 주소
 */
#pragma pack(push, 1)
typedef struct IpHeader {
  uint8_t ver_ihl;
  uint8_t tos;
  uint16_t total_length;
  uint16_t identification;
  uint16_t flags;
  uint16_t fragment_offset;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t checksum;
  uint8_t src_ip[4];
  uint8_t dst_ip[4];
} IpHeader;
#pragma pack(pop)

void PrintIpHeader(const IpHeader* p_ip_header);

#endif