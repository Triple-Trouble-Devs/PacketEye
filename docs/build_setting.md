# [개발환경 세팅]VS Code에서 C++ 패킷 프로그래밍 설정 가이드

## 1. VS Code 설치

vscode는 범용성 있는 MS에서 개발한 오픈 소스 코드 편집기로, 다양한 프로그래밍 언어를 지원하며 뛰어난 확장 기능을 제공하는 편집기입니다.
[VS Code 공식 웹사이트](https://code.visualstudio.com/)에서 설치 파일을 다운로드하고 설치합니다.

## 2. C++ 확장 설치

- VS Code를 실행하고, 왼쪽의 **Extensions**(확장) 탭을 클릭하거나 `Ctrl + Shift + X`를 눌러 확장 탭을 엽니다.
- "C/C++"를 검색하고, Microsoft에서 제공하는 **C/C++ Extension**을 설치합니다.

## 3. 컴파일러 설치

C++ 코드를 컴파일하기 위해 컴파일러가 필요합니다. Windows, Mac, Linux에 따라 각기 다른 컴파일러를 설치해야 합니다.

### Windows

- **MinGW** 설치:
  - [MinGW 설치 페이지](http://www.mingw.org/)로 이동하여 다운로드합니다.
  - 설치 후 환경 변수에 `bin` 경로를 추가합니다 (예: `C:\MinGW\bin`).

### Mac

- **Xcode Command Line Tools** 설치:
  - 터미널에서 `xcode-select --install` 명령어를 입력합니다.

### Linux

- **GCC** 설치:
  - 터미널에서 다음 명령어를 입력하여 GCC를 설치합니다: `sudo apt-get install build-essential`

## 4. Build 설정

Visual Studio를 사용하는 것과 다르게 VSCode에서는 task.json을 통해 빌드 컴파일 하는 방법을 정의하는 방식으로 작동시킬수 있습니다. 이 프로젝트에서는 Packet 트레이싱을 위한 라이브러리를 연결시켜주는 것이 필요합니다.
필자는 현재 맥(Apple Slicon)을 기준으로 설정하였습니다. 다른 운영체제에 대한 내용도 작성하였지만 정확하지 않을 수 있습니다.

### Mac(Apple Slicon)

맥에는 xcode를 통해 기본으로 내장된 libpcap이라는 라이브러리가 존재합니다. 그러나 이 라이브러리는 기본적으로 x64와 호환되는 라이브러리로 ARM이 내장된 실리콘 맥에는 적용되지 않습니다. 따라서 실리콘 맥에서는 homebrew를 통한 설치 후에 라이브러리를 연동하여 실행시킬 수 있습니다.

- homebrew로 `libpcap` 설치
  - iterm나 터미널에 `brew install libpcap`를 통해 설치합니다.
- `libpcap` 설치후 task설정 - task는 프로젝트 루트폴더 내에 `.vscode/tasts.json`형식으로 생성해야 합니다.
  tasks.json

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build project",
      "type": "shell",
      "command": "/opt/homebrew/bin/g++-14",
      "args": [
        "main.cpp",
        "-o",
        "main",
        "-I/opt/homebrew/opt/libpcap/include",
        "-L/opt/homebrew/opt/libpcap/lib",
        "-lpcap"
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

다음은 각각의 테스크에 대한 설명입니다.

- **`"label"`**: Task 이름입니다.
- **`"type"`**: Task가 실행될때 사용되는 타입입니다. 여기서는 `shell`을 통해 셸 명령어를 실행하는 작업을 나타냅니다.
- **`"commend"`**: task를 실행할 명령어입니다. 필자는 `g++-14`를 homebrew를 통해 설치한 버전을 활용했습니다. <span style="background:#fff88f">사용자에 맞는 버전</span>으로 적용해야 합니다.
- **`"args"`**: `g++14`에 전달되는 인수를 설정하는 부분입니다.
  - **`"main.cpp"`**: 컴파일할 C++ 소스 파일입니다.
  - **`"-o"`**: 생성할 실행 파일의 이름을 지정하는 옵션입니다.
  - **`"main"`**: 빌드된 실행 파일의 이름입니다.
  - **`"-I/opt/homebrew/opt/libpcap/include"`**: `libpcap` 라이브러리의 헤더 파일들이 있는 디렉터리를 포함하기 위한 옵션입니다. (`-I`는 include 디렉터리를 지정하는 옵션입니다.)
  - **`"-L/opt/homebrew/opt/libpcap/lib"`**: `libpcap` 라이브러리의 라이브러리 파일들이 있는 디렉터리를 지정합니다. (`-L`은 라이브러리 디렉터리를 지정하는 옵션입니다.)
  - **`"-lpcap"`**: `libpcap` 라이브러리를 링크하기 위한 옵션입니다. (`-l` 옵션은 특정 라이브러리를 링크할 때 사용합니다.)
- **`"option"`**: 이 task가 실행될 때의 추가 옵션을 정의합니다.
  - **`"cwd": "${workspaceFolder}"`**
    - **설명**: 현재 작업 디렉터리(current working directory)를 설정합니다. `${workspaceFolder}`는 VS Code의 작업 공간 디렉터리를 나타냅니다.
    - **용도**: 빌드를 실행할 때, 현재 디렉터리를 프로젝트의 루트 디렉터리로 설정합니다.
- `"group": {...}`
  - **설명**: 이 Task가 속하는 그룹을 정의합니다.
  - **`"kind": "build"`**: 이 Task는 빌드 작업임을 나타냅니다.
  - **`"isDefault": true`**: 이 Task를 기본 빌드 작업으로 설정합니다. 빌드 명령(Ctrl + Shift + B)을 실행할 때 이 작업이 실행됩니다.

---

### Linux & Mac(Intel)

리눅스와 인텔맥에는 기본적으로 `libpcap`이 존재합니다. 따라서 따로 설치할 필요없이 설정이 가능합니다. 만약 옵션 추가를 통해 실행이 제대로 되지 않는다면 실리콘 맥과 같이 경로를 포함해주는 방식으로 설정해야 합니다.

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build",
      "type": "shell",
      "command": "g++",
      "args": [
        "-g",
        "${file}",
        "-o",
        "${fileDirname}/${fileBasenameNoExtension}",
        "-lpcap" // libpcap 라이브러리 참조
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": ["$gcc"],
      "detail": "컴파일 명령 설정 (Linux)"
    }
  ]
}
```

- `-lpcap`을 통해서 빌드가 되지 않는다면 `homebrew`를 통해서 설치하는 것을 권장합니다.
- 기본 내장된 라이브러리 파일의 위치는 아래의 명령어를 통해서 확인할 수 있습니다.
  - `ls /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/pcap.h`

---

### Windows

윈도우의 경우에는 Visual Studio를 통해서 프로그래밍 개발을 하는 것이 권장되지만, 좀 더 가볍게 개발하기 위해서는 Visual Studio Code도 좋은 선택지라고 생각합니다.
윈도우는 기본으로 내장된 패킷 라이브러리가 존재하지 않습니다. 이에 두가지 라이브러리를 활용하는 경우가 많습니다.
**Npcap**:

- **Npcap**은 현재 Windows에서 가장 널리 사용되는 패킷 캡처 라이브러리입니다.
- **WinPcap**의 향상된 버전으로, 최신 Windows 시스템에서 더 안정적으로 작동합니다.
- [Npcap 공식 웹사이트](https://npcap.com/#download)에서 다운로드할 수 있습니다.
  **WinPcap**
- **WinPcap**은 예전에 널리 사용되던 Windows용 패킷 캡처 라이브러리입니다.
- 더 이상 업데이트되지 않으며, 최신 Windows 버전에서는 권장되지 않습니다.
  선택지가 2개 이긴 하지만 Npcap을 설치해서 설정하는 것을 추천합니다.

---

**설치방법**

- `Npcap`설치하기
  - [Npcap 공식 웹사이트](https://npcap.com/#download)를 통해서 설치합니다.
  - 설치 경로를 잘 기억해놓아야 합니다. 보통 `C:/Npcap`에 압축을 푸는 것을 권장합니다.
- tasks 설정하기

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build project",
      "type": "shell",
      "command": "g++", // Windows에서는 MinGW-w64로 설치된 g++ 경로 사용
      "args": [
        "main.cpp",
        "-o",
        "main.exe", // Windows에서는 실행 파일 확장자로 .exe 사용
        "-I\"C:/Npcap/Include\"", // Npcap 헤더 파일 경로
        "-L\"C:/Npcap/Lib\"", // Npcap 라이브러리 파일 경로
        "-lwpcap" // Npcap 라이브러리 링크 (WinPcap API 호환)
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

- 윈도우의 경우에는 command와 헤더 파일과 라이브러리 경로를 컴파일러에 인수로 추가해주는 방식으로 빌드를 할 수 있습니다.

## 4. 예제 코드 빌드 및 테스트

빌드 설정이 완료된 후에는 간단한 프로젝트를 빌드할 수 있는 예제 코드를 실행해본다.
아래는 간단한 패킷 프로그래밍 예제입니다.

### Mac & Linux

```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pcap.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
  std::cout << "Packet capture length: " << pkthdr->caplen << std::endl;
  std::cout << "Packet total length: " << pkthdr->len << std::endl;
}

int main()
{
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *alldevs;
  pcap_if_t *d;
  pcap_t *handle;

  // 모든 네트워크 디바이스 찾기
  if (pcap_findalldevs(&alldevs, errbuf) == -1)
  {
    std::cerr << "Error finding devices: " << errbuf << std::endl;
    return 1;
  }

  // 사용 가능한 디바이스 출력
  for (d = alldevs; d != NULL; d = d->next)
  {
    std::cout << d->name;
    if (d->description)
      std::cout << " (" << d->description << ")" << std::endl;
    else
      std::cout << " (No description available)" << std::endl;
  }

  // 첫 번째 디바이스 사용
  if (alldevs == NULL)
  {
    std::cerr << "No devices found" << std::endl;
    return 1;
  }

  // 디바이스 열기
  const int snaplen = 65535;
  handle = pcap_open_live(alldevs->name, snaplen, 1, 1000, errbuf);
  if (handle == NULL)
  {
    std::cerr << "Could not open device " << alldevs->name << ": " << errbuf << std::endl;
    pcap_freealldevs(alldevs);
    return 1;
  }

  // 패킷 캡처 시작
  std::cout << "Starting packet capture on " << alldevs->name << "..." << std::endl;
  pcap_loop(handle, -1, packet_handler, NULL);

  pcap_freealldevs(alldevs);
  pcap_close(handle);
  return 0;
}

```

### Windows(Npcap 라이브러리 활용)

```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pcap.h>
#include <winsock2.h>  // Windows 소켓 헤더

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
  std::cout << "Packet capture length: " << pkthdr->caplen << std::endl;
  std::cout << "Packet total length: " << pkthdr->len << std::endl;
}

int main()
{
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *alldevs;
  pcap_if_t *d;
  pcap_t *handle;

  // 모든 네트워크 디바이스 찾기
  if (pcap_findalldevs(&alldevs, errbuf) == -1)
  {
    std::cerr << "Error finding devices: " << errbuf << std::endl;
    return 1;
  }

  // 사용 가능한 디바이스 출력
  for (d = alldevs; d != NULL; d = d->next)
  {
    std::cout << d->name;
    if (d->description)
      std::cout << " (" << d->description << ")" << std::endl;
    else
      std::cout << " (No description available)" << std::endl;
  }

  // 첫 번째 디바이스 사용
  if (alldevs == NULL)
  {
    std::cerr << "No devices found" << std::endl;
    return 1;
  }

  // 디바이스 열기
  const int snaplen = 65535;
  handle = pcap_open_live(alldevs->name, snaplen, 1, 1000, errbuf);
  if (handle == NULL)
  {
    std::cerr << "Could not open device " << alldevs->name << ": " << errbuf << std::endl;
    pcap_freealldevs(alldevs);
    return 1;
  }

  // 패킷 캡처 시작
  std::cout << "Starting packet capture on " << alldevs->name << "..." << std::endl;
  pcap_loop(handle, -1, packet_handler, NULL);

  pcap_freealldevs(alldevs);
  pcap_close(handle);
  return 0;
}
```
