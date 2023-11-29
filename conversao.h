#include <iostream>
#include <vector>
#include <string>
#include <bitset>


#ifndef CONVERSAO_H
#define CONVERSAO_H


struct ResultadoCamadaTransmissora {
    std::vector<std::string> quadros;
    std::vector<std::string> quadrosOriginais;
};


std::string stringToBits(const std::string& input);
std::string bitsToString(const std::string& bits);
ResultadoCamadaTransmissora camada_aplicacao_transmissora(const std::string& input); //enlace transmissora
std::string camada_aplicacao_receptor(const ResultadoCamadaTransmissora& resultado);  //enlace receptor
ResultadoCamadaTransmissora startMeioDeComunicacao(const ResultadoCamadaTransmissora& resultado); //ajeita dados para passar pelo meio de comunicação


#endif // CONVERSAO_H
