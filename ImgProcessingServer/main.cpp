#include "imgprocessingserver.hpp"

#include <optional>

int main(int argc, char *argv[])
{    
    std::optional<ImageProcessingServer> server;

    if(argc == 3)
        server.emplace(argv[1], std::stoi(argv[2]));
    else if (argc == 1)
    {
        std::cout << "Iniciando servidor com endereco de ip e porta padrao" << std::endl;
        server.emplace();
    }
    else
    {
        std::cout << "Numero de argumentos invalidos" << std::endl;
        return 1;
    }
    
    return server.value().run();
}