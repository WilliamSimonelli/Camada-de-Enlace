#include <iostream>
#include "conversao.h"
#include <vector>
#include <string>
#include <bitset>
#include "conversao.h"


int main() {
    // Mensagem a ser enviada
    std::string mensagem;

    while (true)
    {
        std::cout <<"Digite a mensagem que deseja transmitir:" << std::endl;
        std::getline(std::cin, mensagem);

        // Simula a camada de enlace transmissor
        ResultadoCamadaTransmissora resultado  = camada_aplicacao_transmissora(mensagem);
    
        //simula o meio de comunicação
        ResultadoCamadaTransmissora resultadoDoMeio = startMeioDeComunicacao(resultado);
        
        // Simula a camada de enlace receptor
        std::string result = camada_aplicacao_receptor(resultadoDoMeio);

        //resultado final da string enviada
        std::cout << "TRANSMISSAO FINAL:" << std::endl;
        std::cout<<result<<std::endl;

        std::cout << " " << std::endl;
        std::cout << " " << std::endl;
    }
    
    return 0;
}
