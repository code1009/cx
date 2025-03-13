# vcpkg 설치하기  
[https://vcpkg.io/en/index.html](https://vcpkg.io/en/index.html)  
[https://vcpkg.io/en/packages](https://vcpkg.io/en/packages)  


## {vs2022/메뉴/도구/명령줄/개발자 명령 프롬프트} 실행  
```

**********************************************************************
** Visual Studio 2022 Developer Command Prompt v17.9.6
** Copyright (c) 2022 Microsoft Corporation
**********************************************************************
E:\>

```


## github에서 git으로 vcpkg clone  
```

E:\>c:
C:\Program Files (x86)\Microsoft Visual Studio\Installer>cd \
C:\>mkdir vcpkg
C:\>cd vcpkg
C:\vcpkg>git clone https://github.com/Microsoft/vcpkg.git
         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
C:\vcpkg>cd vcpkg
C:\vcpkg\vcpkg>

```


## vcpkg 빌드  
### bootstrap-vcpkg.bat  
### vcpkg.exe 만들기  

```

C:\vcpkg\vcpkg>dir
 C 드라이브의 볼륨에는 이름이 없습니다.
 볼륨 일련 번호: 907C-CE44

 C:\vcpkg\vcpkg 디렉터리

2024-07-04  오후 10:36    <DIR>          .
2024-07-04  오후 10:36    <DIR>          ..
2024-07-04  오후 10:36               141 .gitattributes
2024-07-04  오후 10:36    <DIR>          .github
2024-07-04  오후 10:36             5,590 .gitignore
2024-07-04  오후 10:36                 0 .vcpkg-root
2024-07-04  오후 10:36               102 bootstrap-vcpkg.bat
2024-07-04  오후 10:36               109 bootstrap-vcpkg.sh
2024-07-04  오후 10:36             2,432 CONTRIBUTING.md
2024-07-04  오후 10:36             2,785 CONTRIBUTING_pt.md
2024-07-04  오후 10:36             2,186 CONTRIBUTING_zh.md
2024-07-04  오후 10:36    <DIR>          docs
2024-07-04  오후 10:36             1,073 LICENSE.txt
2024-07-04  오후 10:36             2,334 NOTICE.txt
2024-07-04  오후 10:36             2,538 NOTICE_pt.txt
2024-07-04  오후 10:36    <DIR>          ports
2024-07-04  오후 10:36             6,922 README.md
2024-07-04  오후 10:36    <DIR>          scripts
2024-07-04  오후 10:36             2,757 SECURITY.md
2024-07-04  오후 10:36               923 shell.nix
2024-07-04  오후 10:36    <DIR>          toolsrc
2024-07-04  오후 10:36    <DIR>          triplets
2024-07-04  오후 10:36    <DIR>          versions
              14개 파일              29,892 바이트
               9개 디렉터리  691,301,515,264 바이트 남음

```

```

C:\vcpkg\vcpkg>bootstrap-vcpkg.bat
               ~~~~~~~~~~~~~~~~~~~
Downloading https://github.com/microsoft/vcpkg-tool/releases/download/2024-06-10/vcpkg.exe -> C:\vcpkg\vcpkg\vcpkg.exe... done.
Validating signature... done.

vcpkg 패키지 관리 프로그램 버전 2024-06-10-02590c430e4ed9215d27870138c2e579cc338772

라이선스 정보는 LICENSE.txt를 참조하세요.
Telemetry
---------
vcpkg collects usage data in order to help us improve your experience.
The data collected by Microsoft is anonymous.
You can opt-out of telemetry by re-running the bootstrap-vcpkg script with -disableMetrics,
passing --disable-metrics to vcpkg on the command line,
or by setting the VCPKG_DISABLE_METRICS environment variable.

Read more about vcpkg telemetry at docs/about/privacy.md

C:\vcpkg\vcpkg>

```


```

C:\vcpkg\vcpkg>dir
 C 드라이브의 볼륨에는 이름이 없습니다.
 볼륨 일련 번호: 907C-CE44

 C:\vcpkg\vcpkg 디렉터리

2024-07-04  오후 10:39    <DIR>          .
2024-07-04  오후 10:36    <DIR>          ..
2024-07-04  오후 10:36               141 .gitattributes
2024-07-04  오후 10:36    <DIR>          .github
2024-07-04  오후 10:36             5,590 .gitignore
2024-07-04  오후 10:36                 0 .vcpkg-root
2024-07-04  오후 10:36               102 bootstrap-vcpkg.bat
2024-07-04  오후 10:36               109 bootstrap-vcpkg.sh
2024-07-04  오후 10:36             2,432 CONTRIBUTING.md
2024-07-04  오후 10:36             2,785 CONTRIBUTING_pt.md
2024-07-04  오후 10:36             2,186 CONTRIBUTING_zh.md
2024-07-04  오후 10:36    <DIR>          docs
2024-07-04  오후 10:36             1,073 LICENSE.txt
2024-07-04  오후 10:36             2,334 NOTICE.txt
2024-07-04  오후 10:36             2,538 NOTICE_pt.txt
2024-07-04  오후 10:36    <DIR>          ports
2024-07-04  오후 10:36             6,922 README.md
2024-07-04  오후 10:36    <DIR>          scripts
2024-07-04  오후 10:36             2,757 SECURITY.md
2024-07-04  오후 10:36               923 shell.nix
2024-07-04  오후 10:36    <DIR>          toolsrc
2024-07-04  오후 10:36    <DIR>          triplets
2024-07-04  오후 10:39         8,066,496 vcpkg.exe
                                        ~~~~~~~~~
2024-07-04  오후 10:36    <DIR>          versions
              15개 파일           8,096,388 바이트
               9개 디렉터리  691,294,969,856 바이트 남음

```


## vcpkg와 Visual Studio 연동  
### vcpkg integrate install  
```

C:\vcpkg\vcpkg>vcpkg integrate install
               ~~~~~~~~~~~~~~~~~~~~~~~
warning: vcpkg C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg\vcpkg.exe이(가) 검색된 vcpkg 루트 C:\vcpkg\vcpkg을(를) 사용하고 있으며 일치하지 않는 VCPKG_ROOT 환경 값 C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg을(를) 무시합니다. 이 메시지를 표시하지 않도록 하려면 환경 변수를 설정 해제하거나 --vcpkg-root 명령줄 스위치를  사용하세요.
이 vcpkg 루트에 대한 사용자 차원의 통합을 적용했습니다.
CMake 프로젝트는 "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake"를 사용해야 합니다.

이제 모든 MSBuild C++ 프로젝트에 설치된 라이브러리를 #include할 수 있습니다. 연결은 자동으로 처리됩니다. 새 라이브러리를 설치하면 즉시 사용할 수 있습니다.

C:\vcpkg\vcpkg>

```


## vcpkg와 .vcxproj연동  
### .vcxproj 파일과 같은 위치에 vcpkg.json 파일 작성  
### vcpkg.json 내용  
```
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg/master/scripts/vcpkg.schema.json",
  "name": "my-app",
  "version": "0.0.1",
  "dependencies": [
    "fmt",
    "pugixml",
    "nlohmann-json",
    "gtest"
  ]
}
```


### 프로젝트 속성 페이지/구성 속성/vcpkg  
```
+-------------------------------------------------------------------+
|v General                                                          |
| +-----------------------------------------------------------------+
| | Use Vcpkg                           : 예                        |
| | Use Vcpkg Manifest                  : 예                        |
| |                                       ~~                        |
| | Install Vcpkg Dependencies          : 예                        |
| | Use AutoLink                        : 예                        |
| | App-locally depoy DLLs              : 예                        |
| | Use Built-in app-local depolyment   : 아니오                    |
| | Install Directory                   :                           |
| | Addtional Options                   :                           |
+-+-----------------------------------------------------------------+
|v Target and Configuration Specific                                |
| +-----------------------------------------------------------------+
| | Use Static Libraries                : 예                        |
| |                                       ~~                        |
| | Use Dynamic CRT                     :                           |
| | Triplet                             : x64-windows               |
| |                                       ~~~~~~~~~~~               |
| | Host Triplet                        :                           |
| | Vcpkg Configuration Debug           : Debug                     |
+-+-----------------------------------------------------------------+
```


### 위와 같이 설정시 아래 내용이 .vcxproj 파일에 추가 됨  
```
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Label="Vcpkg">
    <VcpkgEnableManifest>true</VcpkgEnableManifest>
  </PropertyGroup>
  <PropertyGroup Label="Vcpkg" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <VcpkgUseStatic>true</VcpkgUseStatic>
    <VcpkgTriplet>x64-windows</VcpkgTriplet>
  </PropertyGroup>
  <PropertyGroup Label="Vcpkg" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <VcpkgUseStatic>true</VcpkgUseStatic>
    <VcpkgTriplet>x64-windows</VcpkgTriplet>
  </PropertyGroup>
```



