add_rules("mode.debug", "mode.release")

add_requires("raylib")

target("spinning-blahaj")
    set_kind("binary")
    add_files("src/*.c")

    add_headerfiles("src/**.h")
    add_includedirs("src/")

    add_packages("raylib")

