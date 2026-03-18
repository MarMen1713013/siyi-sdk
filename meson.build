project(
  'siyi-sdk',
  ['c', 'cpp'],
  version: '0.1.0',
  default_options: ['cpp_std=c++17'],
  )

libcrc = subproject('crc')
gencrc_dep = libcrc.get_variable('gencrc_dep')

siyi_sdk_lib = library(
  'siyi-sdk',
  sources: files(
    'src/message.cpp',
    'src/sdk.cpp',
    'src/ai.cpp',
    'src/gimbal.cpp',
    'src/remote.cpp',
    ),
  include_directories: include_directories('include'),
  dependencies: [gencrc_dep],
  install: false,
  )

siyi_sdk_dep = declare_dependency(
  link_with: siyi_sdk_lib,
  include_directories: include_directories('include'),
  dependencies: [gencrc_dep],
  )
