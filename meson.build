project('rosuc', 'c', 'cpp',
        default_options: ['default_library=static', 'cpp_std=c++20'])

fmt = dependency('fmt')
minizip = dependency('minizip')
sdl = dependency('sdl3')

# criterion_proj = subproject('criterion')
# criterion_dep = criterion_proj.get_variable('criterion')

c = run_command('scripts/find_sources.sh', check: true)
sources = c.stdout().strip().split('\n')
executable('rosuc', sources,
           dependencies: [fmt, sdl, minizip],
           include_directories: 'src',
           cpp_args: '-Wall')
