setup:
	mkdir -p bin

build_z80: setup
	zcc +rc2014 -subtype=basic threed.c -o bin/threed_z80 -create-app -lm

build_local: setup
	gcc threed.c -o bin/threed_local -lm
