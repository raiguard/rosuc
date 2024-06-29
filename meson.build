project('rosuc', 'c', 'cpp',
        default_options: ['default_library=static', 'cpp_std=c++23'])

glad_includes = include_directories('lib/glad/include')
glad = library('glad', 'lib/glad/src/glad.c',
               include_directories: glad_includes)

imgui = subproject('imgui')

minizip = dependency('minizip')
sdl = dependency('sdl3')

# criterion_proj = subproject('criterion')
# criterion_dep = criterion_proj.get_variable('criterion')

c = run_command('scripts/find_sources.sh', check: true)
sources = c.stdout().strip().split('\n')
executable('rosuc', sources,
           link_with: glad,
           include_directories: ['src', glad_includes],
           dependencies: [sdl, minizip, imgui.get_variable('imgui_dep')],
           cpp_args: ['-Wall', '-Werror'])
