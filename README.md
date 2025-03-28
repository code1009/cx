# cx

## 개요
`cx`는 윈도우 어플리케이션을 개발하기 위한 C++ 프레임워크입니다.  

### 특징
- Modern C++ 스타일
- RAII 스타일
- Window API 기반
- STL 기반

### 이력
- 2025년 01월 22일 : 생성



## 라이브러리
`cx/library`의 하위 디렉토리별로 라이브러리를 구성하고 있습니다.  
`component`와 `runtime`는 공통 라이브러리입니다.

### `component`
- 유틸리티 모음입니다.

### `runtime`
- 기본적인 디버깅 정보를 위한 런타임 환경을 제공합니다.

### `wui(Windows User Interface)`
- WinAPI 기반의 창(window)을 관리하는 라이브러리입니다.
- 현재 기본적인 창 관리만 구현되어 있습니다.

### `gw(Graphic Widget)`
- Direct2D 기반 그래픽 라이브러리입니다.
- 다이어그램이나 드로잉 위젯을 만들기 위한 라이브러리입니다.
- 아직 개발 중입니다.

### `network`
- 간단한 socket 라이브러리입니다.





## 도구

### `VcFileGenerator`
- `cx/tool/VcFileGenerator`는 Visual Studio 프로젝트 파일을 생성하는 도구입니다.
- `cx/template/`에 있는 템플릿을 이용하여 프로젝트를 생성합니다.

### `WidgetEditor`
- `cx/tool/WidgetEditor`는 윈도우 어플리케이션의 위젯을 디자인하는 도구입니다.
- 아직 개발 진행 중입니다.
