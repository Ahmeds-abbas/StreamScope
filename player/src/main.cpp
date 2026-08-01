#include <gst/gst.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    gst_init(&argc, &argv);

    if (argc < 2)
    {
        std::cerr << "Usage: streamscope_player <HLS URL>\n";
        return 1;
    }

    const std::string streamUrl = argv[1];

    std::cout << "Stream URL: " << streamUrl << '\n';

    return 0;
}