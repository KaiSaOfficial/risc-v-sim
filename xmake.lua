add_rules("mode.debug", "mode.release")

-- define toolchain
toolchain("Complie")
    -- mark as standalone toolchain
    set_kind("standalone")
    -- set toolset
    set_toolset("cc", "g++")
    set_toolset("cxx", "g++")
    set_toolset("ld", "g++")

target("All")
    set_toolchains("Complie")
    set_kind("binary")
    add_files("./src/*.cc")
    add_includedirs("./include")
