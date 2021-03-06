CONFIG(debug, debug|release):{
        FILE_POSTFIX = D
        DIR_COMPILEMODE = Debug
}
else:CONFIG(release, debug|release):{
        FILE_POSTFIX =
        DIR_COMPILEMODE = Release
}

win32:{
    DIR_PLATFORM = Win32
    FILE_DLL_EXT = .dll
}
else:linux:{
    DIR_PLATFORM = Linux
    FILE_LIB_PREFIX = lib
    FILE_LIB_EXT = .a
    FILE_DLL_PREFIX = lib
    FILE_DLL_EXT = .so
}
else:mac:{
    DEFINES += MAC
    DIR_PLATFORM = MAC
    FILE_LIB_PREFIX = lib
    FILE_LIB_EXT = .a
    FILE_DLL_PREFIX = lib
    FILE_DLL_EXT = .so
}

DESTDIR  = $$PWD/Bin/$${DIR_PLATFORM}/$${DIR_COMPILEMODE}

LIB_DIR_PATH = $${DESTDIR}

DEFINES += LIB_DIR_PATH=\"\\\"$$LIB_DIR_PATH\\\"\"

CONFIG(release,release|debug){
DEFINES +=QT_NO_DEBUG_OUTPUT
}
