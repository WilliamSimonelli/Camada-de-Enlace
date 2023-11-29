#include "conversao.h"
#include <bitset>
#include <iostream>
#include <vector>
#include <string>

//transforma string para string de bits
std::string stringToBits(const std::string& input) {
    std::string result;
    for (char c : input) {
        result += std::bitset<8>(c).to_string();
    }
    return result;
}

//transforma string de bits para string
std::string bitsToString(const std::string& bits) {
    std::string result;
    for (size_t i = 0; i < bits.length(); i += 8) {
        result += static_cast<char>(std::bitset<8>(bits.substr(i, 8)).to_ulong());
    }
    return result;
}


//calcular o CRC-32 de um quadro de 16 bits
std::string calcularCRC32(const std::string& quadro) {
    // Polinômio gerador do CRC-32 (IEEE 802)
    const std::string polinomioGerador = "100000100110000010001110110110111";

    // Padding de tamanho (grau do polinômio-1)
    std::string quadroCRC = quadro + std::string(polinomioGerador.size() - 1, '0');

    // Calcula o CRC
    for (size_t i = 0; i < quadro.size(); ++i) {
        if (quadroCRC[i] == '1') {
            for (size_t j = 0; j < polinomioGerador.size(); ++j) {
                quadroCRC[j + i] = (quadroCRC[j + i] == polinomioGerador[j]) ? '0' : '1';
            }
        }
    }

    // Resultado do CRC
    return quadroCRC.substr(quadro.size());
}


// Função para verificar e extrair dados originais de quadros com CRC
std::string verifica_CRC(const std::string& quadroComCRC, const std::string& dadosOriginais, int& erros, int& acertos) {
    // Polinômio gerador do CRC-32 (IEEE 802)
    const std::string polinomioGerador = "100000100110000010001110110110111";

    // Verifica o CRC
    std::string quadroCRC = calcularCRC32(dadosOriginais);
    if (quadroCRC != quadroComCRC) {
        // CRC não corresponde
        // std:: cout<<"CRC não corresponde!"<<std::endl;
        erros++;
        return dadosOriginais;
    }
    acertos++;
    // std:: cout<<"corresponde"<<std::endl;
    // Retorna os dados originais
    return dadosOriginais;
}


//monta os quadros
std::vector<std::string> make_quadros(const std::string& input) {
    // Divide a mensagem em quadros (tamanho fixo de 16 bits)
    const size_t tamanhoDoQuadro = 16;
    std::vector<std::string> quadros;

    for (size_t i = 0; i < input.size(); i += tamanhoDoQuadro) {
        quadros.push_back(input.substr(i, tamanhoDoQuadro));
    }
    return quadros;
}



void MeioDeComunicacao(std::vector<std::string>& fluxoBrutoDeBitsPontoB, const std::vector<std::string>& fluxoBrutoDeBitsPontoA ){
    int porcentagemDeErros = 10; //mude a porcentagem de erro como quiser para poder ver quantidades de bits corretos e incorretos

    for(int i = 0; i<fluxoBrutoDeBitsPontoA.size(); i++){
        if((rand()%100 +1 ) >= porcentagemDeErros){
            fluxoBrutoDeBitsPontoB[i] = fluxoBrutoDeBitsPontoA[i];
        }else{
            fluxoBrutoDeBitsPontoB[i] = fluxoBrutoDeBitsPontoA[i] == "0" ? "1" : "0";
        }
    }
}

//ajeita os dados para mandar para o meio de comunicação
ResultadoCamadaTransmissora startMeioDeComunicacao(const ResultadoCamadaTransmissora& resultado){

    ResultadoCamadaTransmissora resultadoAtualizado = resultado; //cópia da original

    std:: cout<<"_________________________________Meio de comunicacao_________________________________"<<std::endl;
    MeioDeComunicacao(resultadoAtualizado.quadros, resultado.quadros);
    MeioDeComunicacao(resultadoAtualizado.quadrosOriginais, resultado.quadrosOriginais);

    return resultadoAtualizado;
}


ResultadoCamadaTransmissora camada_aplicacao_transmissora(const std::string& input) {
    std:: cout<<"_________________________________enlace transmissor_________________________________"<<std::endl;
    
    //transforma em uma string de bits
    std::string bitsEnviados = stringToBits(input);

    std:: cout<<"enquadra bits"<<std::endl;
    //enquadra os bits em string de bits de size 16
    std::vector<std::string> quadros = make_quadros(bitsEnviados);
    std::vector<std::string> quadrosOriginas = quadros;

    //////////////////////////////////////
    // Exibe os quadros originais sem CRC
    //////////////////////////////////////
    // std:: cout<<"Quadros:"<<std::endl;
    // for (const std::string& quadro : quadros) {
    //     std::cout << quadro << std::endl;
    // }

    std:: cout<<"Faz CRC para cada quadro de bits"<<std::endl;
    // Adiciona bits crc(crc com 32 bits)
    for (std::string& quadro : quadros) {
        quadro = calcularCRC32(quadro);
    }

    std::cout << "_________________________________________________________________________________________" << std::endl;

    return {quadros, quadrosOriginas};
}


std::string camada_aplicacao_receptor(const ResultadoCamadaTransmissora& resultado) {
    std::cout << "_________________________________enlace receptor_________________________________" << std::endl;
    ////////////////////////// 
    //Imprime os quadros CRC
    // Caso queria ver
    //////////////////////////
    // std::cout << "Quadros CRC:" << std::endl;
    // for (const std::string& quadro : resultado.quadros) {
    //     std::cout << quadro << std::endl;
    // }

    std::cout << "Verificando quadros do CRC:" << std::endl;
    std::vector<std::string> quadrosOriginais = resultado.quadrosOriginais;

    //Verifica os quadros CRC
    int erros = 0; //quantidade de quadros errados
    int acertos = 0;
    for (size_t indice = 0; indice < resultado.quadros.size(); ++indice) {
        quadrosOriginais[indice] = verifica_CRC(resultado.quadros[indice], resultado.quadrosOriginais[indice], erros, acertos);
    }
    
    
    
    ////////////////////////////////
    // Imprime os quadros originais
    // Caso queria ver
    ////////////////////////////////
    // std::cout << "___________Quadros Originais__________:" << std::endl;
    // for (const std::string& quadroOriginal : resultado.quadrosOriginais) {
    //     std::cout << quadroOriginal << std::endl;
    // }

    // desenquadra os bits e transforma em string
    std:: cout<<"Desenquadra bits"<<std::endl;
    std::string mensagem;
    for (const std::string& quadro : resultado.quadrosOriginais) {
        mensagem = mensagem + bitsToString(quadro);
    }

    std::cout << "_________________________________________________________________________________________" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "Quantidade de quadros ERRADOS de CRC: " << erros << std::endl;
    std::cout << "Quantidade de quadros CORRETOS de CRC: " << acertos << std::endl;
    std::cout << "Quantidade TOTAL de quadros: " << acertos+erros << std::endl;
    std::cout << "Porcentagem de acertos: " << static_cast<double>(acertos) / resultado.quadros.size() * 100 << "%" << std::endl;
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;

     return mensagem; //mensagem final
}