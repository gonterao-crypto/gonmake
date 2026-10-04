del /s /q gonmake.exe
clang++ main.cpp --target=x86_64-w64-mingw32 -fuse-ld=lld -o gonmake.exe -std=c++26
if exist "gonmake.exe" (
  gonmake -dir "./"
) else (
  echo Error!
)
