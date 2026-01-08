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
- 2026년 01월 08일 : 라이브러리 및 샘플 목록 현행화



## 라이브러리
`cx/library`의 하위 폴더 단위로 라이브러리를 구성하고 있습니다.  
하위 폴더 `runtime`과 `common`은 공통으로 사용하는 모듈입니다.  
`runtime`과 `common`을 제외하고, 그 이외의 폴더 들은 각각 
독립적으로 사용이 가능합니다.

### `common`
- 기본적인 공통 모듈입니다.

### `runtime`
- log와 assert문을 제공합니다.

### `component`
- 유틸리티 모음입니다.

### `wui(Windows User Interface)`
- WinAPI 기반의 창(window)을 관리하는 라이브러리입니다.
- 윈도우 생성, 메시지 맵 처리, 드래그 앤 드롭 핸들러 등을 제공합니다.

### `d2d(Direct2D)`
- Direct2D 1.1 기반의 그래픽 라이브러리 래퍼입니다.
- Canvas, Brush, Geometry 등의 리소스를 Modern C++ 스타일로 관리합니다.
- *개발 중입니다.*

### `network`
- 간단한 socket 라이브러리입니다.

### `gw(Graphic Widget)` (Deprecated)
- *삭제 예정*



## 도구

### `VcFileGenerator`
- `cx/tool/VcFileGenerator`는 Visual Studio 프로젝트 파일을 생성하는 도구입니다.
- `cx/template/`에 있는 템플릿을 이용하여 프로젝트를 생성합니다.

### `WidgetEditor` (Deprecated)
- *삭제 예정*