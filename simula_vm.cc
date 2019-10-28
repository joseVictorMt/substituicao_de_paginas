#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <algorithm>
#include <vector>

using std::cerr, std::cout, std::endl, std::vector, std::pair;

int qtdQuadros( const char* );
pair<int, vector<int>> cadeiaDeReferencias( const char* );

class Simulador {
    public:
        Simulador( const int qtd_quadros, const int qtd_refs, const vector<int> refs ) : 
        qtd_quadros(qtd_quadros), qtd_refs(qtd_refs), refs(refs)
        {
            memoria = vector<int>(qtd_quadros, -1);
        }

        int getQtdQuadros() const {
            return qtd_quadros;
        }

        int getQtdRefs() const {
            return qtd_refs;
        }

        int fifo() {
            limparMemoria();
            return 0;
        }

        int opt() {
            limparMemoria();
            return 0;
        }

        int lru() {
            limparMemoria();
            return 0;
        }

    private:
        int qtd_quadros, qtd_refs;
        vector<int> refs, memoria;

        void limparMemoria() {
            std::for_each( memoria.begin(), memoria.end(), []( auto x ){ x = -1; } );
        }
};

int main( int argc, char* argv[] ) {
    
    if( argc <= 2 ) {
        cerr << "É necessário passar o número de quadros e o arquivo." << endl;
        cout << "Abortando o programa..." << endl;
        exit(-1);
    }

    pair<int, vector<int>> params = cadeiaDeReferencias( argv[2] );

    auto resultado = Simulador( qtdQuadros(argv[1]), params.first, params.second );

    return 0;
}

int qtdQuadros( const char* qtd_quadros ) {
    int quadros;
    std::istringstream ss (qtd_quadros);

    if(!(ss >> quadros) || quadros < 1) {
        cerr << "A quantidade de quadros de memória deve ser um inteiro positivo." << endl;
        cout << "Finalizando o programa..." << endl;
        exit(-2);
    }

    return quadros;
}

pair<int, vector<int>> cadeiaDeReferencias( const char* nome_arquivo ) {

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