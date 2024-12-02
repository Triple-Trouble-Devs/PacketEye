// tcp_header.h

#ifndef TCP_HEADER_H_
#define TCP_HEADER_H_

#include <cstdint>

/**
 * @brief TCP Header 구조체
 *
 * TCP(Transmission Control Protocol) 헤더를 나타내는 구조체입니다.
 * 각 필드는 TCP 프로토콜의 핵심 요소를 나타냅니다.
 *
 * 필드 설명:
 * - src_port: 출발지 포트 번호 (16비트)
 * - dst_port: 목적지 포트 번호 (16비트)
 * - sequence_num: 시퀀스 번호 (32비트)
 * - ack_num: 응답 번호 (32비트)
 * - data_offset: 데이터 오프셋 (헤더 길이를 나타냄, 4비트 상위)
 * - flags: TCP 플래그 (URG, ACK, PSH, RST, SYN, FIN 등 6비트 사용)
 * - window_size: 수신 윈도 크기 (흐름 제어, 16비트)
 * - checksum: TCP 체크섬 (16비트)
 * - urgent_pointer: 긴급 포인터 (16비트)
 */
#pragma pack(push, 1)
typedef struct TcpHeader {
  uint16_t src_port;
  uint16_t dst_port;
  uint32_t sequence_num;
  uint32_t ack_num;
  uint8_t data_offset;
  uint16_t flags;
  uint16_t window_size;
  uint16_t checksum;
  uint16_t urgent_pointer;
} TcpHeader;
#pragma pack(pop)

void PrintTcpHeader(const TcpHeader* p_tcp_header);

#endif