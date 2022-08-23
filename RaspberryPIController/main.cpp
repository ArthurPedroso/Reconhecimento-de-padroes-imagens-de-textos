#include "rpicontroller.hpp"

#include <iostream>
#include <optional>

int main(int argc, char *argv[])
{    
    std::optional<RpiController> controller;

    if(argc == 3)
        controller.emplace(argv[1], std::stoi(argv[2]));
    else if (argc == 1)
    {
        std::cout << "Iniciando rpi com endereco de ip e porta padrao" << std::endl;
        controller.emplace();
    }
    else
    {
        std::cout << "Numero de argumentos invalidos" << std::endl;
        return 1;
    }
    
    return controller.value().run();
}