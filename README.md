# gonmake
日本製のC++makeツール

## 必要なもの
・Clang++

・ccache

・mingw32

・windows環境

・パソコン

## 使い方


### 基本の形
```
gonmake -dir ./
```

### オプション解説
**必須** -dir [ディレクトリのパス] デフォルト：``

-o [出力するファイルの名前] デフォルト：`program.exe`

-cm [コマンドの前] デフォルト：`ccache clang++ -c `

-cu [コマンドの後ろ] デフォルト：`--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths`

-lcm [リンカーコマンドの前] デフォルト：`clang++ `

-lcu [リンカーコマンドの後ろ] デフォルト：`--target=x86_64-w64-mingw32 -fuse-ld=lld -std=c++26 -fdiagnostics-absolute-paths `

-noecho デフォルト：`-echo`

-echo デフォルト：`-echo`

### 発展コマンド例
`gonmake -dir "./" -o "program.exe" -cu "--target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths -fcolor-diagnostics -fansi-escape-codes"`

## インストール方法
https://github.com/gonterao-crypto/gonmake/releases

ここから、最新版をダウンロードして、

PATHを通すか、同じディレクトリにおいて、

コマンドプロンプトから叩いてください。
