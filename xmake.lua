local project_name = "RISC_EMU"
local target_dir = "xmake-build"
local extension = ".bin"

set_project(project_name)
set_version("v0.2")

add_rules("mode.debug")

local RISCV_Bin = "Rom_pro/build/asm_pro.bin"

-- define toolchain
toolchain("Complie")
    -- mark as standalone toolchain
    set_kind("standalone")
    -- set toolset
    set_toolset("cc", "g++")
    set_toolset("cxx", "g++")
    set_toolset("ld", "g++")

target(project_name)
    set_toolchains("Complie")
    set_kind("binary")
    set_targetdir(target_dir)
    set_objectdir(target_dir .. "/obj")
    set_dependir(target_dir .. "/dep")
    set_extension(extension)

    set_warnings("all")
    set_languages("c11", "cxx17")
    set_optimize("fastest")
    add_files("./src/**.cc")
    add_includedirs("./include", "./include/utils", "./include/ISA")

on_run(function(target)
    os.exec("%s %s", target_dir.."/"..project_name..extension, "./Rom_pro/build/os.bin")
end)
