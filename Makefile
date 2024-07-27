# Cloud Land's building script
#
# created on 2024.7.25 at Haidian, Beijing

.DEFAULT_GOAL := all

.PHONY: help
help:
	@echo "cloudland makefile"
	@echo "---------------"
	@echo "commands available:"
	@echo "- make run"
	@echo "    build and launch"
	@echo "- make debug"
	@echo "    build (with debug) and copy the binary to \"target\" folder"
	@echo "- make release"
	@echo "    build (without debug) and copy the binary to \"target\" folder"
	@echo "- make"
	@echo "    alias for \"make all\""
	@echo "- (sudo) make install"
	@echo "    build and copy binary to /usr/sbin"
	@echo "- (sudo) make uninstall"
	@echo "    uninstall from system"

.PHONY: prepare-debug
prepare-debug:
	@mkdir -p build && cd build \
	&& cmake -DCMAKE_BUILD_TYPE=Debug -G"Ninja" ../src


.PHONY: prepare-release
prepare-release:
	@mkdir -p build && cd build \
	&& cmake -DCMAKE_BUILD_TYPE=Release -G"Ninja" ../src


# private target: --build
.PHONY: --build
--build:
	@cd build && cmake --build . -- -j 8
	@mkdir -p target && cp build/cloudland target/
	@cd target && mkdir -p asm-dump \
	&& objdump -d ./cloudland > asm-dump/cloudland.text.asm
	@echo -e "\033[32mbuild success (cloudland).\033[0m"


.PHONY: build-debug
build-debug: prepare-debug --build


.PHONY: debug
debug: build-debug


.PHONY: build-release
build-release: prepare-release --build


.PHONY: release
release: build-release


.PHONY: clean
clean:
	rm -rf ./build
	rm -rf ./target
	rm -f ./src/config.cpp


.PHONY: --remove-debug-dirs
--remove-debug-dirs:
	-fusermount3 -q -u ./target/mount-point
	-rm -r ./target/mount-point
	-rm -r ./target/data-dir


.PHONY: --run
--run: --remove-debug-dirs
	cd target \
	&& ./cloudland \
		--mount-point ./mount-point \
		--data-dir ./data-dir \
		--fs alipan



.PHONY: run
run: debug --run


.PHONY: run-release
run-release: release --run



.PHONY: install-debug
install-debug: debug
	cp target/cloudland /usr/sbin/cloudland



.PHONY: install-release
install-release: release
	cp target/cloudland /usr/sbin/cloudland


.PHONY: install
install: install-release


.PHONY: uninstall
uninstall:
	rm -f /usr/sbin/cloudland


.PHONY: all
all: debug
