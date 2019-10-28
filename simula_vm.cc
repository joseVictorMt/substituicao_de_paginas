#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using std::cerr, std::cout, std::distance, std::endl, std::find, std::for_each, std::vector, std::pair;

int qtdQuadros( const char* );
pair<int, vector<int>> cadeiaDeReferencias( const char* );
void imprime(const vector<int>);

class Simulador {
    public:
        Simulador( const int qtd_quadros, const int qtd_refs, const vector<int> refs ) : 
        qtd_quadros(qtd_quadros), qtd_refs(qtd_refs), refs(refs) {}

        int getQtdQuadros() const {
            return qtd_quadros;
        }

        int getQtdRefs() const {
            return qtd_refs;
        }

        int fifo() {
            vector<int> memoria(qtd_quadros, -1);
            int faltas_de_pagina, contador;
            faltas_de_pagina = contador = 0;
            
            for( auto x: refs ) {
                if(contador == qtd_quadros) {
                    contador = 0;
                }

                if( naoEstaNaMemoria(x, memoria) ) {
                    memoria[contador] = x;
                    faltas_de_pagina++;
                    contador++;
                }
            }

            return faltas_de_pagina;
        }

        int lru() {
            return 0;
        }

        int opt() {
            vector<int> memoria(qtd_quadros, -1);
            int faltas_de_pagina = 0;

            for( auto x = refs.begin(); x != refs.end(); x++ ) {
                if( naoEstaNaMemoria(*x, memoria) ) {
                    substituiMenosUsada(x, memoria, OPT);
                    faltas_de_pagina++;
                }
            }

            return faltas_de_pagina;
        }

    private:
        int qtd_quadros, qtd_refs;
        vector<int> refs;
        const static bool OPT = 1, LRU = 0;

        bool naoEstaNaMemoria( const int valor, vector<int> const &memoria ) const {
            return find(memoria.begin(), memoria.end(), valor) == memoria.end();
        }

        void substituiMenosUsada( vector<int>::iterator &pagina, vector<int> &memoria, int algoritmo ) {
            vector<int>::iterator troca, maior = pagina, achado, inicio, fim;

            if( algoritmo == OPT ) {
                inicio = pagina + 1;
                fim = refs.end();
            } else if( algoritmo == LRU ) {
                inicio = refs.begin();
                fim = pagina + 1;
            } else {
                cout << "Algoritmo indefinido (bug)" << endl;
                exit(-4);
            }

            for( auto x = memoria.begin(); x < memoria.end(); x++ ) {
                achado = find(inicio, fim, *x);
                if( achado == refs.end() ) {
                    troca = x;
                    break;
                }

                if( distance(pagina, maior) < distance(pagina, achado) ) {
                    maior = achado;
                    troca = x;
                }
            }

            *troca = *pagina;
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


    imprime( {rstd.getQtdQuadros(), rstd.getQtdRefs(), rstd.fifo(), rstd.lru(), rstd.opt()} );

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

void imprime( const vector<int> params ) {
    cout << params[0] << " quadros     "
        << params[1] << " refs: "
        << "FIFO:   " << params[2] << " PFs, "
        << "LRU:    " << params[3] << " PFs, "
        << "OPT:    " << params[4] << " PFs"
        << endl;
}