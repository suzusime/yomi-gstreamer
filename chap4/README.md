# chapter4
第4章「C言語で書いてみる」掲載ののC言語アプリケーションです。

## 使い方

```sh
# ビルド
$ make

# サンプルファイルのダウンロード
$ wget https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm
$ wget https://gstreamer.freedesktop.org/data/media/medium/sugar.ogg

# 各プログラムの起動
$ ./make
$ ./playbin-proto1 file:///$(pwd)/sugar.ogg
$ ./playbin-proto2 file:///$(pwd)/sintel_trailer-480p.webm
$ ./playbin-proto3 sintel_trailer-480p.webm
```
