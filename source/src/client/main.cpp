// Main entry point of client application

int main(int argn, const char **argc)
{
    if (enet_initialize() != 0)
    {
        cerr << "An error occurred while initializing ENet." << endl;
        return EXIT_FAILURE;
    }

    return wxEntry(argc, argv);
}
