# rosuc

**R**ai's **osu**!standard **c**lone. A small game written in C++ with SDL3.

## Compiling

Uses the `meson` build system. The project has dependencies on `SDL3` and `minizip-ng`.

```
make setup
make
```

## Beatmaps

Create a folder called `beatmaps` in the project root directory and add your beatmap files to this directory.

## Running

Either use `make run` or manually execute `build/rosuc`.
