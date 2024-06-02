add_rules("mode.debug", "mode.release")

add_requires("raylib", "imgui")

target("spinning-blahaj")
    set_kind("binary")
    add_files("src/*.cpp")

    add_headerfiles("src/**.h", "src/*.hpp")
    add_includedirs("src/")

    add_packages("raylib", "imgui")

