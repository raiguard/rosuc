all:
	meson compile -C build

setup:
	meson setup --reconfigure build

run: all
	build/rosuc

.PHONY: all setup run
