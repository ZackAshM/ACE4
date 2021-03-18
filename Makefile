.PHONY: clean build all install deepclean

all: build
	+make -C build

install: install
	+make -C build install

build:
	mkdir -p build
	cmake3 -B`pwd`/build -H`pwd`

clean:
	+make -C build clean

deepclean:
	+make -C build clean
	rm -rf build/*

