# SE4
SE4 Game Engine

# How to build
윈도우는 extern/ 디렉토리에 SDL2 라이브러리 다운받고 cmake 체크. -> 2020.10.16 현재 일단 extern에 포함시킴  
리눅스는 패키지 매니저로 SDL2 설치  
그 외 라이브러리는 submodule로 추가되어 있음. 클론 이후 받아와야 함.  
```git
git submodule update --init --recursive
```
test/main.cpp 이용해서 디버깅   

# Licnese
MIT  
todo : 외부 라이센스 명시  
SDL2, SDL2_image, glog, box2d
