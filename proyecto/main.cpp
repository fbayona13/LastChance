#include <iostream>
#include <string.h>
#include <fstream>
#include "Arbol/ArbolBinario.h"
#include "par.h"
using namespace std;
int main(int argc, char **argv) {
    par busca, palabras;
    string linea, palabra, Codif, simbolo;
    ifstream diccionario, entrada;
    ofstream outfile;
    unsigned char aux;
    bool mayuscula = false;
    int condicion;

    auto *arbol = new ArbolBinario<par>;
    entrada >> noskipws;

    for(int i = 0;i < argc;i++) {
        if (strcmp(argv[i], "-c") == 0) { condicion = 1;}
        if (strcmp(argv[i], "-d") == 0) { condicion = 2;}
        if (strcmp(argv[i], "-l") == 0) { diccionario.open(argv[i + 1]);}
        else
        if (strcmp(argv[i], "-i") == 0) { entrada.open(argv[i + 1]);}
        else
        if (strcmp(argv[i], "-o") == 0) { outfile.open(argv[i + 1]);}
    }
    if(condicion == 1) {
        while (getline(diccionario, linea)) {
            unsigned long count = linea.find('\t');
            palabras.set(linea.substr(0, count), linea.substr(count + 1, linea.length()));
            arbol->put(palabras);
        }
    }
    if(condicion == 2) {
        while (getline(diccionario, linea)) {
            unsigned long count = linea.find('\t');
            palabras.set(linea.substr(count + 1, linea.length()), linea.substr(0, count));
            arbol->put(palabras);
        }
    }
    while (entrada >> aux){
        if(aux<=90 && aux>=65||aux==193||aux==201||aux==205||aux==211||aux==218){ aux += 32; mayuscula = true; }
        if((aux<=122 && aux>=97)||aux==225||aux==233||aux==237||aux==243||aux==250){ palabra += aux; }
        else{
            if(palabra != "") {
                busca.set(palabra, "");
                Codif = arbol->search(busca).getDecodificado();
                if (mayuscula) {
                    if((Codif[0]<=122 && Codif[0]>=97)||Codif[0]==225||Codif[0]==233||Codif[0]==237||Codif[0]==243||Codif[0]==250){
                        Codif[0] = (char) (Codif[0] - 32);
                    }
                    mayuscula = false;
                }
                outfile << Codif;
                palabra = "";
            }
            outfile << aux;
        }
    }
    outfile.close();
    return 0;
}