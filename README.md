# spinning-blahaj
A spinning blue shark from ikea.

You can select a camera preset by using <kbd>PageUp</kbd> or <kbd>PageDown

## Setting Up
To run this ridiculously silly C project, you'll need [xmake](https://xmake.io/)

```sh
# optionally, you can set to use the clang compiler instead
# you can read more about setting toolchains here: https://xmake.io/#/toolchain/builtin_toolchains
$ xmake f --cc=clang-cl --cxx=clang-cl -c
$ xmake -y
```

All built files will go into a directory called `build/`

## Credits
- Blahal model by [TechTastic](https://github.com/TechTastic)

## Later Features
- maybe video rendering
