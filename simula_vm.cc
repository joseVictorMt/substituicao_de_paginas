#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <deque>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cerr, std::cout, std::distance, std::endl, std::find;
using std::for_each, std::unordered_set, std::vector, std::pair;

int qtdQuadros( const char* );
pair<int, vector<int>> cadeiaDeReferencias( const char* );
void imprime(const vector<int>);

// -------------------------------------- Classe do Simulador --------------------------------------

class Simulador {
    public:
        Simulador( const int qtd_quadros, const int qtd_refs, const vector<int> refs ) : 
        qtd_quadros(qtd_quadros), qtd_refs(qtd_refs), refs(refs) {}

        int pegaQtdQuadros() const {
            return qtd_quadros;
        }

        int pegaQtdRefs() const {
            return qtd_refs;
        }

        int fifo() {
            std::deque<int> memoria;
            int faltas_de_pagina = 0;
            
            for( auto x: refs ) {
                if(memoria.size() < qtd_quadros) {
                    if( find(memoria.begin(), memoria.end(), x) == memoria.end() ) {
                        memoria.push_front(x);
                        faltas_de_pagina++;
                    }
                } else {
                    if( find(memoria.begin(), memoria.end(), x) == memoria.end() ) {
                        memoria.pop_back();
                        memoria.push_front(x);
                        faltas_de_pagina++;
                    }
                }
            }

            return faltas_de_pagina;
        }

        int lru() {
            unordered_set<int> memoria;
            std::unordered_map<int, int> mem_indices;
            int faltas_de_pagina = 0;

            for ( int i = 0; i < qtd_refs; i++ ) {

                if ( memoria.size() < qtd_quadros ) {

                    if ( memoria.find( refs[i]) == memoria.end() ) {
                        memoria.insert(refs[i]);
                        faltas_de_pagina++;
                    }

                    mem_indices[refs[i]] = i;

                } else {

                    if ( memoria.find(refs[i]) == memoria.end() ) {
                        int menos_usada_i = qtd_refs + 1, pag_menos_usada;
                        for ( auto pagina = memoria.begin(); pagina != memoria.end(); pagina++ ) {
                            if ( mem_indices[*pagina] < menos_usada_i ) {
                                menos_usada_i = mem_indices[*pagina];
                                pag_menos_usada = *pagina;
                            }
                        }

                        memoria.erase(pag_menos_usada);
                        memoria.insert(refs[i]);
                        faltas_de_pagina++;
                    }

                    mem_indices[refs[i]] = i;
                } 
            }

            return faltas_de_pagina;
        }

        int opt() {
            unordered_set<int> memoria;
            int faltas_de_pagina = 0, troca;
            vector<int>::iterator maior, local;

            for( auto pag_atual = refs.begin(); pag_atual != refs.end(); pag_atual++ ) {
                if( memoria.size() < qtd_quadros ) {
                    if ( memoria.find( *pag_atual ) == memoria.end() ) {
                        memoria.insert( *pag_atual );
                        faltas_de_pagina++;
                    }
                }

                else {
                    if ( memoria.find( *pag_atual ) == memoria.end() ) {
                        maior = pag_atual;
                        for( auto x : memoria ) {
                            local = find(pag_atual+1, refs.end(), x);
                            if( local == refs.end() ) {
                                troca = x;
                                break;
                            }

                            if( distance(pag_atual, maior) < distance(pag_atual, local) ) {
                                maior = local;
                                troca = x;
                            }
                        }

                        memoria.erase(troca);
                        memoria.insert(*pag_atual);
                        faltas_de_pagina++;
                    }
                }
            }

            return faltas_de_pagina;
        }

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

    pair<int, vector<int>> params = cadeiaDeReferencias( argv[2] );

    auto rstd = Simulador( qtdQuadros(argv[1]), params.first, params.second );

    imprime( {rstd.pegaQtdQuadros(), rstd.pegaQtdRefs(), rstd.fifo(), rstd.lru(), rstd.opt()} );

    return 0;
}

// -------------------------------------- Funções auxiliáres --------------------------------------

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

void imprime( const vector<int> resultado ) {
    cout << resultado[0] << " quadros     "
        << resultado[1] << " refs: "
        << "FIFO:   " << resultado[2] << " PFs, "
        << "LRU:    " << resultado[3] << " PFs, "
        << "OPT:    " << resultado[4] << " PFs"
        << endl;
}