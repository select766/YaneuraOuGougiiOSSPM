# YaneuraOuiOSSPM

やねうら王をiOS向けのSwift Package Managerパッケージとしてビルドするスクリプト。

試作版。駒得評価のみ。

# ビルド

Xcode 16.0, cmake 3.24.1

```
cd build
./build.bash
```

# インターフェース変更

インターフェースとなる関数は `build/src/ios_main.cpp` に記述する。

関数シグネチャは `build/include/yaneuraou.h`及び`Sources/YaneuraOuiOSSPM/include/yaneuraou.h`に同じ内容を記述する。

ビルドするソースコードリスト、コンパイラオプションは `build/ios_{arm64,x86_64}/CMakeLists.txt` に記述する。(本当は１ファイルにまとめたいのだがcmakeの知識が足りないだけ)

# git　タグ

パッケージのバージョンはgitのタグで識別される。`1.2.3`のように3つの数字で表す。

# ライセンス

GPLv3 (やねうら王本体に従います)

ただし、 `ios.toolchain.cmake`([取得元](https://raw.githubusercontent.com/leetal/ios-cmake/master/ios.toolchain.cmake)) は修正BSDライセンス。
