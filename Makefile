all: all_binaries

all_binaries:
	mkdir -p bin
	cd libs/util/build && make && make static
	cd cat && make
	cd cp && make
	cd echo && make
	cd kill && make
	cd ls && make
	cd mkdir && make
	cd mv && make
	cd rm && make
	cd touch && make
	cd shell/build && make

clean:
	cd libs/util/build && make clean
	cd cat && make clean
	cd cp && make clean
	cd echo && make clean
	cd kill && make clean
	cd ls && make clean
	cd mkdir && make clean
	cd mv && make clean
	cd rm && make clean
	cd touch && make clean
	cd shell/build && make clean
	rm -rfv bin