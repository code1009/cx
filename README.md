# cx
- 윈도우 어플리케이션을 개발하기 위한 C++ 프레임 워크입니다.
- 아직 코드를 작성 중입니다.





## 소개

### 라이브러리
- `cx/library`의 하위 디렉토리별로 라이브러리를 구성하고 있습니다.
- `component`와 `runtime`는 공통 라이브러리 입니다.

#### `component`  
- Utility 모음 입니다.

#### `runtime`  
- 기본적인 디버깅 정보를 위한 런타임 환경을 제공합니다.

#### `wui(Windows User Interface)`  
- winapi기반의 창(window)을 관리하는 라이브러리 입니다.
- 아직 기본적인 창 관리만 구현되어 있습니다.
- 앞으로 진행 계획은 다음과 같습니다.
  - GDIObject
    - Pen
    - Brush
    - Font
    - Bitmap
  - Common Control
    - Button
	- ListBox
	- ListView
	- TreeView
	- ...
  - Common Dialog
    - OpenFileDialog(일부완료)
    - SaveFileDialog(일부완료)
    - FolderDialog(일부완료)
    - FontDialog
    - ColorDialog
  - Menu
  - Toolbar


#### `gw(Graphic Widget)`  
- direct2d기반 그래픽 라이브러리 입니다.
- diagram이나 drawing widget을 만들기 위한 라이브러리 입니다.
- 아직 개발중입니다.





### 도구

#### `VcFileGenerator`
- `cx/tool/VcFileGenerator`는 Visual Studio 프로젝트 파일을 생성하는 도구 입니다.  
- `cx/template/`의 있는 템플릿을 이용하여 프로젝트를 생성합니다.

#### `WidgetEditor`
- `cx/tool/WidgetEditor`는 윈도우 어플리케이션의 widget을 디자인하는 도구 입니다.
- 아직 개발진행 중입니다.





### 특징
- Modern C++ 스타일  
- RAII 스타일  
- Window API 기반  
- STL 기반  





## 변경이력

### _2025년 01월 22일_
- 생성




