#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H
#include "NodoArbol.h"
#include <iostream>
using namespace std;
template<class T>
class ArbolBinario {
private:
    enum {IZQUIERDO, DERECHO};
    NodoArbol<T> *raiz;
    NodoArbol<T> *actual;
public:
    ArbolBinario();
    void put(T dato);
    T search(T dato);
    ~ArbolBinario();
    void balance(NodoArbol<T> *nodo, int rama, bool nuevo);
    void RDD(NodoArbol<T> *nodo);
    void RDI(NodoArbol<T> *nodo);
    void RSI(NodoArbol<T> *nodo);
    void RSD(NodoArbol<T> *nodo);
protected:
    void put(T dato, NodoArbol<T> *r);
    T search(T dato, NodoArbol<T> *r);
};
template<class T>
ArbolBinario<T>::ArbolBinario() { raiz = nullptr; }     //Constructor del arbol
template<class T>
ArbolBinario<T>::~ArbolBinario() { }                    //Destructor del arbol
/**
 * Busca un dato en el árbol. Si no esta el dato en el árbol
 * tira     una excepción
 * @param clave Valor a buscar
 * @return el valor buscado
 */
template<class T>
T ArbolBinario<T>::search(T dato) { return search(dato, raiz); }
template<class T>
T ArbolBinario<T>::search(T dato, NodoArbol<T> *r) {
    if (r == nullptr)
        throw 404;
    if (dato == r->getDato())
        return r->getDato();
    if (dato > r->getDato())
        return search(dato, r->getDer());
    if (dato < r->getDato())
        return search(dato, r->getIzq());
}
/**
 * Agrega un dato al árbol
 * @param clave Clave para agregar el dato
 * @param dato Dato a agregar
 */
template<class T>
void ArbolBinario<T>::put(T dato) {
    NodoArbol<T> *padre = NULL;
    actual = raiz;
    // Buscar el dato en el arbol, manteniendo un puntero al nodo padre
    while(actual != nullptr && dato != actual->getDato()) {
        padre = actual;
        if(dato > actual->getDato()) actual = actual->getDer();
        else if(dato < actual->getDato()) actual = actual->getIzq();
    }
    // Si se ha encontrado el elemento, regresar sin insertar
    if(actual != nullptr) throw 200;
    // Si padre es NULL, entonces el arbol estaba vacio, el nuevo nodo sera
    // el nodo raiz
    if(padre == nullptr) raiz = new NodoArbol<T>(dato);
        // Si el dato es menor que el que contiene el nodo padre, lo insertamos
        // en la rama izquierda
    else if(dato < padre->getDato()) {
        auto *nuevo = new NodoArbol<T>(dato);
        padre->setIzq(nuevo);
        balance(padre, IZQUIERDO, true);
    }
        // Si el dato es mayor que el que contiene el nodo padre, lo insertamos
        // en la rama derecha
    else if(dato > padre->getDato()) {
        auto *nuevo = new NodoArbol<T>(dato);
        padre->setDer(nuevo);
        balance(padre, DERECHO, true);
    }
}
template<class T>
void ArbolBinario<T>::put(T dato, NodoArbol<T> *r) {
    T miDato = r->getDato();
    if(miDato == dato)
        throw 200;
    if(dato > miDato) {
        if(r->getDer() != nullptr) {
            put(dato, r->getDer());
        }else{
            auto *nuevo = new NodoArbol<T>(dato);
            r->setDer(nuevo);
        }
        balance(r, DERECHO, true);
    }else{
        if (r->getIzq() != nullptr) {
            put(dato, r->getIzq());
        }else{
            auto *nuevo = new NodoArbol<T>(dato);
            r->setIzq(nuevo);
        }
        balance(r, IZQUIERDO, true);
    }
}
// Equilibrar arbol AVL partiendo del nodo nuevo
template<class T>
void ArbolBinario<T>::balance(NodoArbol<T> *nodo, int rama, bool nuevo) {
    bool salir = false;
    // Recorrer camino inverso actualizando valores de FE:
    while(nodo && !salir) {
        if(nuevo)
            if(rama == IZQUIERDO) nodo->FE--;      // Depende de si añadimos ...
            else                  nodo->FE++;
        else
        if(rama == IZQUIERDO) nodo->FE++;      // ... o borramos
        else                  nodo->FE--;
        if(nodo->FE == 0) salir = true;           // La altura de las rama que
            // empieza en nodo no ha variado,
            // salir de Equilibrar
        else if(nodo->FE == -2) {                 // Rotar a derechas y salir:
            if(nodo->getIzq()->FE == 1) RDD(nodo); // Rotacion doble
            else RSD(nodo);                        // Rotacion simple
            salir = true;
        }
        else if(nodo->FE == 2) {                  // Rotar a izquierdas y salir:
            if(nodo->getDer()->FE == -1) RDI(nodo);// Rotaciin doble
            else RSI(nodo);                        // Rotaciin simple
            salir = true;
        }
        if(nodo->getPad())
            if(nodo->getPad()->getDer() == nodo) rama = DERECHO; else rama = IZQUIERDO;
        nodo = nodo->getPad(); // Calcular FE, siguiente nodo del camino.
    }
}
// Rotacion doble a derechas
template<class T>
void ArbolBinario<T>::RDD(NodoArbol<T> *nodo){
    NodoArbol<T>  *Padre = nodo->getPad();
    NodoArbol<T>  *P = nodo;
    NodoArbol<T>  *Q = P->getIzq();
    NodoArbol<T>  *R = Q->getDer();
    NodoArbol<T>  *B = R->getIzq();
    NodoArbol<T>  *C = R->getDer();
    if(Padre)
        if(Padre->getDer() == nodo) Padre->setDer(R);
        else Padre->setIzq(R);
    else raiz = R;
    // Reconstruir arbol:
    Q->setDer(B);
    P->setIzq(C);
    R->setIzq(Q);
    R->setDer(P);
    // Reasignar padres:
    R->setPad(Padre);
    P->setPad(Q);
    Q->setPad(R);
    if(B) B->setPad(Q);
    if(C) C->setPad(P);
    // Ajustar valores de FE:
    switch(R->FE) {
        case -1: Q->FE = 0; P->FE = 1; break;
        case 0:  Q->FE = 0; P->FE = 0; break;
        case 1:  Q->FE = -1; P->FE = 0; break;
    }
    R->FE = 0;
}
// Rotacion doble a izquierdas
template<class T>
void ArbolBinario<T>::RDI(NodoArbol<T> *nodo){
    NodoArbol<T>  *Padre = nodo->getPad();
    NodoArbol<T>  *P = nodo;
    NodoArbol<T>  *Q = P->getDer();
    NodoArbol<T>  *R = Q->getIzq();
    NodoArbol<T>  *B = R->getIzq();
    NodoArbol<T>  *C = R->getDer();
    if(Padre)
        if(Padre->getDer() == nodo) Padre->setDer(R);
        else Padre->setIzq(R);
    else raiz = R;
    // Reconstruir arbol:
    P->setDer(B);
    Q->setIzq(C);
    R->setIzq(P);
    R->setDer(Q);
    // Reasignar padres:
    R->setPad(Padre);
    P->setPad(Q);
    Q->setPad(R);
    if(B) B->setPad(P);
    if(C) C->setPad(Q);
    // Ajustar valores de FE:
    switch(R->FE) {
        case -1: P->FE = 0; Q->FE = 1; break;
        case 0:  P->FE = 0; Q->FE = 0; break;
        case 1:  P->FE = -1; Q->FE = 0; break;
    }
    R->FE = 0;
}
// Rotacion simple a derechas
template<class T>
void ArbolBinario<T>::RSD(NodoArbol<T> *nodo) {
    NodoArbol<T> *Padre = nodo->getPad();
    NodoArbol<T> *P = nodo;
    NodoArbol<T> *Q = P->getIzq();
    NodoArbol<T> *B = Q->getDer();
    if(Padre)
        if(Padre->getDer() == P) Padre->setDer(Q);
        else Padre->setIzq(Q);
    else raiz = Q;
    // Reconstruir arbol:
    P->setIzq(B);
    Q->setDer(P);
    // Reasignar padres:
    P->setPad(Q);
    if(B) B->setPad(P);
    Q->setPad(Padre);
    // Ajustar valores de FE:
    P->FE = 0;
    Q->FE = 0;
}
// Rotacion simple a izquierdas
template<class T>
void ArbolBinario<T>::RSI(NodoArbol<T> *nodo) {
    NodoArbol<T> *Padre = nodo->getPad();
    NodoArbol<T> *P = nodo;
    NodoArbol<T> *Q = P->getDer();
    NodoArbol<T> *B = Q->getIzq();
    if(Padre)
        if(Padre->getDer() == P) Padre->setDer(Q);
        else Padre->setIzq(Q);
    else raiz = Q;
    // Reconstruir arbol:
    P->setDer(B);
    Q->setIzq(P);
    // Reasignar padres:
    P->setPad(Q);
    if(B) B->setPad(P);
    Q->setPad(Padre);
    // Ajustar valores de FE:
    P->FE = 0;
    Q->FE = 0;
}
#endif //ARBOLBINARIO_H