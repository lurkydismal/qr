project(
    'qr',
    [ 'c', 'cpp' ],
    version: 'v1.0.1',
    default_options: [ 'c_std=c99', 'cpp_std=c++14', 'warning_level=3' ],
    license: 'GPL-3.0',
    license_files: [ 'LICENSE' ]
)

c = meson.get_compiler( 'c' )
cxx = meson.get_compiler( 'cpp' )

if build_machine.system() == 'windows'
    compile_argument_list = [ '/EHsc', '/MD', '/nologo', '/GS-', '/O2', '/Ot' ]

    c_ld = 'Crinkler.exe'

    link_argument_list = [
        '/CRINKLER', '/TINYHEADER', '/TINYIMPORT',
        'kernel32.lib', 'user32.lib', 'gdi32.lib',
        'winspool.lib', 'comdlg32.lib', 'advapi32.lib',
        'shell32.lib', 'ole32.lib', 'oleaut32.lib',
        'uuid.lib', 'odbc32.lib', 'odbccp32.lib',
        'ws2_32.lib', 'mswsock.lib'
    ]

else
    compile_argument_list = []

    link_argument_list = []

endif

fmt = dependency( 'fmt' )
gtest = dependency( 'gtest' )
gtest_main = dependency( 'gtest_main' )
irepeat = dependency( 'irepeat' )

logic_include = include_directories( 'logic/include' )
stdfunc_include = include_directories( 'stdfunc/include' )

stdfunc = declare_dependency(
    compile_args : compile_argument_list,
    include_directories : [ stdfunc_include, logic_include ],
    sources : [ 'stdfunc/src/stdfunc.c', 'stdfunc/src/malloc.c', 'stdfunc/src/ltoa.c' ],
    version : '1.0.0'
)
logic = declare_dependency(
    compile_args : compile_argument_list,
    dependencies : [ stdfunc ],
    include_directories : [ logic_include, stdfunc_include ],
    sources : [ 'logic/src/logic.c' ],
    version : '1.0.1'
)

if build_machine.system() == 'windows'
    executable(
        'main',
        'src/main.c',
        link_args: link_argument_list,
        dependencies : [ logic, stdfunc ],
        include_directories : [ logic_include, stdfunc_include ]
    )

endif

main_test_executable = executable(
    'main_test',
    'test/main_test.cpp',
    dependencies : [ fmt, gtest, gtest_main, logic, irepeat, stdfunc ],
    include_directories : [ logic_include, stdfunc_include ]
)

test(
    'main_test',
    main_test_executable
)
