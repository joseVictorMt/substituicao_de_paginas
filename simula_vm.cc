#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <deque>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cerr, std::cout, std::distance, std::endl, std::find;
using std::for_each, std::unordered_map, std::unordered_set, std::vector, std::pair;

int qtdQuadros( const char* );
pair<int, vector<int>> cadeiaDeReferencias( const char* );
void imprime( const vector<int>& );

// -------------------------------------- Classe do Simulador --------------------------------------

class Simulador {
    public:
        Simulador( const int &qtd_quadros, const int &qtd_refs, const vector<int> &refs ) : 
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

                    if ( memoria.find( refs[i] ) == memoria.end() ) {
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
            int faltas_de_pagina = 0;
            auto momento_de_exec = mapaDeMomentos();

            for( int i = 0; i < qtd_refs; i++ ) {

                if ( memoria.size() < qtd_quadros ) {
                    
                    if ( memoria.find( refs[i] ) == memoria.end() ) {
                        faltas_de_pagina++;
                        memoria.insert( refs[i] );
                    }

                } else {
                    
                    if ( memoria.find( refs[i] ) == memoria.end() ) {
                        faltas_de_pagina++;

                        int mais_tardio = 0, escolhida;
                        for( auto mem_pag : memoria ) {
                            if ( momento_de_exec[mem_pag].empty() ) {
                                escolhida = mem_pag;
                                break;
                            }
                            
                            else if( mais_tardio < momento_de_exec[mem_pag].front() ) {
                                escolhida = mem_pag;
                                mais_tardio = momento_de_exec[mem_pag].front();
                            }
                        }

                        memoria.erase(escolhida);
                        memoria.insert( refs[i] );
                    }

                }

                if( !momento_de_exec[refs[i]].empty() && momento_de_exec[refs[i]].front() == i) {
                    momento_de_exec[refs[i]].pop();
                }
            }
            
            return faltas_de_pagina;
        }

    private:
        int qtd_quadros, qtd_refs;
        vector<int> refs;


        auto mapaDeMomentos() -> unordered_map< int, std::queue<int> > {
            unordered_map<int, std::queue<int>> momentos_de_exec;
            for( int i = 0; i < qtd_refs; ++i ) {
                momentos_de_exec[refs[i]].push(i);
            }

            return momentos_de_exec;
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

auto cadeiaDeReferencias( const char* nome_arquivo ) -> pair< int, vector<int> > {

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

    return { qtd_refs, refs };
}

void imprime( const vector<int> &resultado ) {
    cout << resultado[0] << " quadros,     "
        << resultado[1] << " refs: "
        << "FIFO:   " << resultado[2] << " PFs, "
        << "LRU:    " << resultado[3] << " PFs, "
        << "OPT:    " << resultado[4] << " PFs"
        << endl;
}
