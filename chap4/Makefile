default: main playbin-proto1 playbin-proto2 playbin-proto3
.PHONY: default

main: main.c
	gcc -Wall -o main main.c $$(pkg-config --cflags --libs gstreamer-1.0)

playbin-proto1: playbin-proto1.c
	gcc -Wall -o playbin-proto1 playbin-proto1.c $$(pkg-config --cflags --libs gstreamer-1.0)

playbin-proto2: playbin-proto2.c
	gcc -Wall -o playbin-proto2 playbin-proto2.c $$(pkg-config --cflags --libs gstreamer-1.0)

playbin-proto3: playbin-proto3.c
	gcc -Wall -o playbin-proto3 playbin-proto3.c $$(pkg-config --cflags --libs gstreamer-1.0)
