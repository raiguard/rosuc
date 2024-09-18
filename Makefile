all:
	meson compile -C build

setup:
	meson setup --reconfigure build

run: all
	build/rosuc

test: all
	build/rosuc-test --color=always

.PHONY: all setup run
