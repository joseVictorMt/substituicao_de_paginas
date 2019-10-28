#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using std::cerr, std::cout, std::endl, std::for_each, std::vector, std::pair;

int qtdQuadros( const char* );
pair<int, vector<int>> cadeiaDeReferencias( const char* );
void imprime(int, int, int, int, int);

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
            int faltas_de_pagina, contador;
            faltas_de_pagina = contador = 0;

            for( auto x: refs ) {
                if(contador == qtd_quadros) {
                    contador = 0;
                }
                
                if( naoEstaNaMemoria(x) ) {
                    memoria[contador] = x;
                    faltas_de_pagina++;
                    contador++;
                }
            }

            limparMemoria();
            return faltas_de_pagina;
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
            for_each( memoria.begin(), memoria.end(), []( auto x ){ x = -1; } );
        }

        bool naoEstaNaMemoria( int valor ) {
            return std::find(memoria.begin(), memoria.end(), valor) == memoria.end();
        }
};

int main( int argc, char* argv[] ) {
    
    if( argc <= 2 ) {
        cerr << "É necessário passar o número de quadros e o arquivo." << endl;
        cout << "Abortando o programa..." << endl;
        exit(-1);
    }

    pair<int, vector<int>> params = cadeiaDeReferencias( argv[2] );

    auto rstd = Simulador( qtdQuadros(argv[1]), params.first, params.second );

    imprime( rstd.getQtdQuadros(), rstd.getQtdRefs(), rstd.fifo(), rstd.lru(), rstd.opt() );

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

void imprime( int qtd_quadros, int qtd_refs, int pfs_fifo, int pfs_lru, int pfs_opt) {
    cout << qtd_quadros << " quadros     "
        << qtd_refs << " refs: "
        << "FIFO:   " << pfs_fifo << " PFs, "
        << "LRU:    " << pfs_lru << " PFs, "
        << "OPT:    " << pfs_opt << " PFs"
        << endl;
}