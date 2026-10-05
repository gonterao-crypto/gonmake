# gonmake
日本製のC++makeツール
## 必要なもの
・Clang++
・ccache
・mingw32
・windows環境
・パソコン
## 使い方
**必須** -dir [ディレクトリのパス] デフォルト：``<br>
-o [出力するファイルの名前] デフォルト：program.exe`<br>
-cm [コマンドの前] デフォルト：ccache clang++ -c `<br>
-cu [コマンドの後ろ] デフォルト：--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths`<br>
-lcm [リンカーコマンドの前] デフォルト：`clang++ `<br>
-lcu [リンカーコマンドの後ろ] デフォルト：`--target=x86_64-w64-mingw32 -fuse-ld=lld -std=c++26 -fdiagnostics-absolute-paths `<br>
