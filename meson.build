project('rosuc', 'c', 'cpp',
        default_options: ['default_library=static', 'cpp_std=c++23'])

minizip = dependency('minizip')
sdl = dependency('sdl3')

imgui = subproject('imgui')

# criterion_proj = subproject('criterion')
# criterion_dep = criterion_proj.get_variable('criterion')

c = run_command('scripts/find_sources.sh', check: true)
sources = c.stdout().strip().split('\n')
executable('rosuc', sources,
           dependencies: [sdl, minizip, imgui.get_variable('imgui_dep')],
           include_directories: 'src',
           cpp_args: ['-Wall', '-Werror'])
