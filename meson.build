project('rosuc', 'c', 'cpp',
        default_options: ['default_library=static', 'cpp_std=c++23'])

glad_includes = include_directories('lib/glad/include')
glad = library('glad', 'lib/glad/src/glad.c',
               include_directories: glad_includes)

imgui = dependency('imgui')
minizip = dependency('minizip')
sdl = dependency('sdl2')
sdl_image = dependency('sdl2_image')
sdl_mixer = dependency('sdl2_mixer')
glm = dependency('glm')

criterion_proj = subproject('criterion')
criterion_dep = criterion_proj.get_variable('criterion')

c = run_command('scripts/find_sources.sh', check: true)
sources = c.stdout().strip().split('\n')
executable('rosuc', sources,
           link_with: glad,
           include_directories: ['src', glad_includes],
           dependencies: [glm, imgui, minizip, sdl, sdl_image, sdl_mixer],
           cpp_args: ['-Wall', '-Werror'])

c2 = run_command('scripts/find_sources.sh', 'tests', check: true)
sources2 = c2.stdout().strip().split('\n')
executable('rosuc-test', sources2,
           link_with: glad,
           include_directories: ['src', 'tests', glad_includes],
           dependencies: [imgui, minizip, sdl, sdl_mixer, sdl_image, glm, criterion_dep],
           cpp_args: ['-Wall', '-Werror'])
