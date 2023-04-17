# YaneuraOuGougiiOSSPM

やねうら王（NNUE評価関数）とふかうら王（やねうら王の深層学習版）を単一プロセスで両方動作させられるiOS向けのSwift Package Managerパッケージとしてビルドするスクリプト。

モデルは含まれていない。このパッケージを読み込むSwiftアプリ側に組み込んで、そのアドレスを渡す必要がある。アプリ側の実装: https://github.com/select766/YaneuraOuGougiiOS

パッケージ名は`YaneuraOuGougiiOSSPM`となっている。

# ビルド

Xcode 16.0, cmake 3.24.1

```
git submodule init
git submodule update
cd build
./build.sh
```

# インターフェース変更

インターフェースとなる関数は `build/src/ios_main.cpp` に記述する。

関数シグネチャは `build/include/yaneuraou.h`及び`Sources/YaneuraOuGougiiOSSPM/include/yaneuraou.h`に同じ内容を記述する。

ビルドするソースコードリスト、コンパイラオプションは `build/ios_{arm64,x86_64}/CMakeLists.txt` に記述する。(本当は１ファイルにまとめたいのだがcmakeの知識が足りないだけ)

# git　タグ

パッケージのバージョンはgitのタグで識別される。`1.2.3`のように3つの数字で表す。

# ライセンス

GPLv3 (やねうら王本体に従います)

ただし、 `ios.toolchain.cmake`([取得元](https://raw.githubusercontent.com/leetal/ios-cmake/master/ios.toolchain.cmake)) は修正BSDライセンス。
