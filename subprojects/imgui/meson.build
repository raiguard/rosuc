project(
  'imgui',
  'cpp',
  license: 'MIT',
  version: '1.89.9',
  meson_version: '>=0.50.0',
)

if host_machine.system() == 'darwin'
  add_languages('objcpp')
endif

include_dirs = include_directories('.', 'backends')
sources = files(
  'imgui.cpp',
  'imgui_demo.cpp',
  'imgui_draw.cpp',
  'imgui_tables.cpp',
  'imgui_widgets.cpp',
  'imgui_stdlib.cpp',
  'backends/imgui_impl_sdl3.cpp',
  'backends/imgui_impl_opengl3.cpp'
)

sdl3 = dependency('sdl3')

imgui = library(
  'imgui',
  sources,
  gnu_symbol_visibility: 'hidden',
  dependencies: sdl3,
  include_directories: include_dirs,
)

imgui_dep = declare_dependency(
  include_directories: include_dirs,
  link_with: imgui,
)
