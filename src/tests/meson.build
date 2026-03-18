test(
    'crc_test',
    executable(
        'crc_test',
        sources: 'crc_test.cpp',
        dependencies: [siyi_sdk_dep],
    )
)

# four_point_test non ha add_test() nel CMake originale,
# quindi lo compilo ma non lo registro come test automatico
executable(
    'four_point_test',
    sources: 'four_point_test.cpp',
    dependencies: [siyi_sdk_dep],
)
