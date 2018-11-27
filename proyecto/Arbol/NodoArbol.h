#ifndef NODOARBOL_H
#define NODOARBOL_H
template<class T>
class NodoArbol {
private:
    NodoArbol<T> *izq;
    NodoArbol<T> *der;
    NodoArbol<T> *padre;
    T dato;
public:
    int FE;
    NodoArbol() { izq = nullptr; der = nullptr; padre = nullptr; FE = 0; }
    NodoArbol(T d) { izq = nullptr; der = nullptr; padre = nullptr; dato = d; FE = 0; }
    T getDato() const { return dato; }
    void setDato(T dato) { this->dato = dato; }
    void setIzq(NodoArbol<T> *r) { this->izq = r; }
    void setDer(NodoArbol<T> *r) { this->der = r; }
    void setPad(NodoArbol<T> *r) { this->padre = r; }
    NodoArbol *getIzq() const { return izq; }
    NodoArbol *getDer() const { return der; }
    NodoArbol *getPad() const { return padre; }
};
#endif //NODOARBOL_H