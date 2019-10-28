#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cerr, std::cout, std::endl;

int main( int argc, char* argv[] ) {
    
    if( argc == 1 ) {
        cerr << "Arquivo não passado. Finalizando o programa..." << endl;
        return 0;
    }

    std::ifstream arquivo (argv[1]);
    if( !arquivo.is_open() ) {
        cerr << "Não foi possível abrir" << argv[1];
        cerr <<". Finalizando o programa..." << endl;
        return 0;
    }

    std::vector<int> refs;
    int ref, qtd_refs = 0;
    
    while( arquivo >> ref ) {
        refs.push_back( ref );
        ++qtd_refs;
    }

    refs.resize(qtd_refs);

    return 0;
}