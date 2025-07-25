#include "NamedPipe.hpp"

int main()
{
    NamedPipe named_pipe(fifoname);
    named_pipe.OpenForWrite();

    while (true)
    {
        std::cout << "Please message# ";
        std::string line;
        std::getline(std::cin, line);

        named_pipe.Write(line);
    }

    named_pipe.Close();

    return 0;
}