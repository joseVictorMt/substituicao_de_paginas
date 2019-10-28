#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::cerr, std::cout, std::endl, std::vector, std::pair;

int qtdQuadros( char* );
pair<int, vector<int>> cadeiaDeReferencias( char* );

class Simulador {
    public:
        Simulador(int qtd_quadros, pair<int,vector<int>> params) : 
            qtd_quadros(qtd_quadros), qtd_refs(params.first), refs(params.second) {}

    private:
        int qtd_quadros, qtd_refs;
        vector<int> refs;
};

int main( int argc, char* argv[] ) {
    
    if( argc <= 2 ) {
        cerr << "É necessário passar o número de quadros e o arquivo." << endl;
        cout << "Abortando o programa..." << endl;
        exit(-1);
    }

    auto resultado = Simulador( qtdQuadros(argv[1]), cadeiaDeReferencias(argv[2]) );

    return 0;
}

int qtdQuadros( char* qtd_quadros ) {
    int quadros;
    std::istringstream ss (qtd_quadros);

    if(!(ss >> quadros) || quadros < 1) {
        cerr << "A quantidade de quadros de memória deve ser um inteiro positivo." << endl;
        cout << "Finalizando o programa..." << endl;
        exit(-2);
    }

    return quadros;
}

pair<int, vector<int>> cadeiaDeReferencias( char* nome_arquivo ) {

    std::ifstream arquivo (nome_arquivo);
    if( !arquivo.is_open() ) {
        cerr << "Não foi possível abrir o arquivo '" << nome_arquivo << "'." << endl;
        cout <<"Abortando o programa..." << endl;
        exit(-3);
    }

    vector<int> refs;
    int ref, qtd_refs = 0;
    
    while( arquivo >> ref ) {
        refs.push_back( ref );
        ++qtd_refs;
    }

    refs.resize(qtd_refs);

    arquivo.close();

    return pair<int, vector<int>> (qtd_refs, refs);
}