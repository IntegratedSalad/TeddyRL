
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include "app/app.hpp"

void printResourcePath()
{
    std::cout << resourcePath() << std::endl;
}

int main(int argc, const char** argv)
{
    App app = App(argv[0]);
    app.run();

    return EXIT_SUCCESS;
}

