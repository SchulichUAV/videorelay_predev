// Main entry point of client application
#include "../shared/pch.h"

int main(int argc, char **argv)
{
    if (enet_initialize() != 0)
    {
        fputs("An error occurred while initializing ENet.", stderr);
        return EXIT_FAILURE;
    }

    return wxEntry(argc, argv);
}
