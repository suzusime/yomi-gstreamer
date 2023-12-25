# chap3
第3章「付属CLIでGStreamerと戯れる」に登場するコマンド（コピペ用）です。

## 3.1節
```sh
$ gst-play-1.0
```

## 3.2節
```sh
$ wget https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm
$ wget https://gstreamer.freedesktop.org/data/media/medium/sugar.ogg

$ gst-play-1.0 sintel_trailer-480p.webm
$ gst-play-1.0 sugar.ogg
```

## 3.3節
```sh
$ gst-launch-1.0 playbin3 uri=file:///$(pwd)/sintel_trailer-480p.webm

# MSYS2用
$ gst-launch-1.0 playbin uri=file:///$(cygpath -m $(pwd))/sintel_trailer-480p.webm


$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! decodebin3 ! videoconvert ! autovideosink

$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! decodebin3 ! videobalance saturation=0.0 ! videoconvert ! autovideosink

$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! decodebin3 ! videoconvert ! videoscale ! autovideosink

$ gst-launch-1.0 filesrc location=sugar.ogg ! decodebin3 ! audioconvert ! audioresample ! autoaudiosink

$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! decodebin3 ! audioconvert ! audioresample ! autoaudiosink
```

## 3.4節
```sh
$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! decodebin3 ! videoconvert ! autovideosink
```

## 3.5節
```sh
$ gst-discoverer-1.0 sintel_trailer-480p.webm

$ gst-discoverer-1.0 sugar.ogg


$ gst-inspect-1.0
$ gst-inspect-1.0 | grep ogg
$ gst-inspect-1.0 | grep vorbis
$ gst-inspect-1.0 oggdemux

$ gst-launch-1.0 filesrc location=sugar.ogg ! oggdemux ! vorbisdec ! audioconvert ! audioresample ! autoaudiosink


$ gst-inspect-1.0 | grep webm
$ gst-inspect-1.0 | grep matroska
$ gst-inspect-1.0 | grep vp8

$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! matroskademux ! vp8dec ! videoconvert ! autovideosink
$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! matroskademux ! vorbisdec ! audioconvert ! audioresample ! autoaudiosink
```

## 3.6節
```sh
$ gst-inspect-1.0 matroskademux
```

## 3.7節
```sh
$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! matroskademux name=d d.video_0 ! queue ! vp8dec ! videoconvert ! autovideosink d.audio_0 ! queue ! vorbisdec ! audioconvert ! audioresample ! autoaudiosink

$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! matroskademux name=d ! queue ! vp8dec ! videoconvert ! autovideosink d. ! queue ! vorbisdec ! audioconvert ! audioresample ! autoaudiosink

$ gst-launch-1.0 filesrc location=sintel_trailer-480p.webm ! decodebin3 name=d d.video_0 ! queue ! videoconvert ! autovideosink d.audio_0 ! queue ! audioconvert ! audioresample ! autoaudiosink
```

