#include <iostream>  // Incluye la biblioteca estándar de entrada/salida en C++.
#include <vector>    // Incluye la biblioteca para el uso de vectores.
#include <stack>     // Incluye la biblioteca para el uso de pilas.
#include <cstdlib>   // Incluye la biblioteca estándar de C para funciones de utilidad general, como rand y srand.
#include <ctime>     // Incluye la biblioteca estándar de C para funciones relacionadas con el tiempo.

class Laberinto { // Define la clase Laberinto
public: 
    Laberinto(int ancho, int alto); // Declaración del constructor.
    void generar(); // Declaración del método para generar el laberinto.
    void imprimir(); // Declaración del método para imprimir el laberinto.
    bool resolver(); // Declaración del método para resolver el laberinto.
    void imprimirSolucion(); // Declaración del método para imprimir la solución del laberinto.

private: 
    struct Celda { // Define una estructura Celda para representar cada celda del laberinto.
        bool visitado = false; // Indica si la celda ha sido visitada.
        bool arriba = true; // Indica si hay una pared arriba de la celda.
        bool abajo = true; // Indica si hay una pared abajo de la celda.
        bool izquierda = true; // Indica si hay una pared a la izquierda de la celda.
        bool derecha = true; // Indica si hay una pared a la derecha de la celda.
        bool enSolucion = false; // Indica si la celda está en la solución del laberinto.
    };

    int ancho; // Ancho del laberinto.
    int alto; // Alto del laberinto.
    std::vector<std::vector<Celda>> rejilla; // Rejilla que representa el laberinto como una matriz de celdas.

    void eliminarPared(Celda& actual, Celda& siguiente, char direccion); // Declaración del método para eliminar una pared entre dos celdas.
    bool resolverUtil(int x, int y); // Declaración de la función auxiliar para resolver el laberinto.
    void reiniciarVisitados(); // Declaración del método para reiniciar el estado de visitado de todas las celdas.
};

// Implementación del constructor
Laberinto::Laberinto(int ancho, int alto) : ancho(ancho), alto(alto) { 
    rejilla.resize(alto, std::vector<Celda>(ancho)); // Redimensiona la rejilla para que tenga 'alto' filas y 'ancho' columnas.
    srand(time(nullptr)); // Inicializa el generador de números aleatorios con la hora actual.
}

// Implementación del método para generar el laberinto
void Laberinto::generar() {
    std::stack<std::pair<int, int>> pila; // Crea una pila de pares de enteros para el backtracking.
    int inicioX = 0; // Coordenada x de inicio.
    int inicioY = 0; // Coordenada y de inicio.
    rejilla[inicioY][inicioX].visitado = true; // Marca la celda inicial como visitada.
    pila.push({inicioX, inicioY}); // Empuja la celda inicial en la pila.

    while (!pila.empty()) { // Mientras la pila no esté vacía
        int x = pila.top().first; // Obtiene la coordenada x de la celda en la parte superior de la pila.
        int y = pila.top().second; // Obtiene la coordenada y de la celda en la parte superior de la pila.

        std::vector<std::pair<int, int>> vecinos; // Crea un vector para almacenar los vecinos no visitados.

        if (x > 0 && !rejilla[y][x - 1].visitado) vecinos.push_back({x - 1, y}); // Agrega el vecino izquierdo si no ha sido visitado.
        if (x < ancho - 1 && !rejilla[y][x + 1].visitado) vecinos.push_back({x + 1, y}); // Agrega el vecino derecho si no ha sido visitado.
        if (y > 0 && !rejilla[y - 1][x].visitado) vecinos.push_back({x, y - 1}); // Agrega el vecino de arriba si no ha sido visitado.
        if (y < alto - 1 && !rejilla[y + 1][x].visitado) vecinos.push_back({x, y + 1}); // Agrega el vecino de abajo si no ha sido visitado.

        if (!vecinos.empty()) { // Si hay vecinos no visitados
            auto [siguienteX, siguienteY] = vecinos[rand() % vecinos.size()]; // Selecciona un vecino aleatorio.

            // Elimina la pared entre la celda actual y el vecino seleccionado.
            if (siguienteX == x - 1) eliminarPared(rejilla[y][x], rejilla[siguienteY][siguienteX], 'I');
            else if (siguienteX == x + 1) eliminarPared(rejilla[y][x], rejilla[siguienteY][siguienteX], 'D');
            else if (siguienteY == y - 1) eliminarPared(rejilla[y][x], rejilla[siguienteY][siguienteX], 'A');
            else if (siguienteY == y + 1) eliminarPared(rejilla[y][x], rejilla[siguienteY][siguienteX], 'B');

            rejilla[siguienteY][siguienteX].visitado = true; // Marca el vecino seleccionado como visitado.
            pila.push({siguienteX, siguienteY}); // Empuja el vecino seleccionado en la pila.
        } else {
            pila.pop(); // Si no hay vecinos no visitados, hace pop en la pila.
        }
    }

    // Asegura que haya una entrada y una salida en el laberinto.
    rejilla[0][0].izquierda = false;  // Entrada
    rejilla[alto - 1][ancho - 1].derecha = false;  // Salida

    reiniciarVisitados();  // Reinicia el estado de visitado antes de resolver el laberinto.
}

// Implementación del método para eliminar una pared entre dos celdas
void Laberinto::eliminarPared(Celda& actual, Celda& siguiente, char direccion) {
    switch (direccion) { 
        case 'I': // Izquierda
            actual.izquierda = false;
            siguiente.derecha = false;
            break;
        case 'D': // Derecha
            actual.derecha = false;
            siguiente.izquierda = false;
            break;
        case 'A': // Arriba
            actual.arriba = false;
            siguiente.abajo = false;
            break;
        case 'B': // Abajo
            actual.abajo = false;
            siguiente.arriba = false;
            break;
    }
}

// Implementación del método para resolver el laberinto
bool Laberinto::resolver() {
    return resolverUtil(0, 0); // Inicia la resolución del laberinto desde la celda (0, 0).
}

// Implementación de la función auxiliar para resolver el laberinto utilizando backtracking
bool Laberinto::resolverUtil(int x, int y) {
    if (x == ancho - 1 && y == alto - 1) { // Si se ha alcanzado la celda final
        rejilla[y][x].enSolucion = true; // Marca la celda como parte de la solución.
        return true; // Indica que se ha encontrado una solución.
    }

    if (x >= 0 && x < ancho && y >= 0 && y < alto && !rejilla[y][x].visitado) { // Si la celda está dentro del laberinto y no ha sido visitada
        rejilla[y][x].visitado = true; // Marca la celda como visitada.

        // Intenta moverse en cada dirección y marca la celda como parte de la solución si es exitosa.
        if (!rejilla[y][x].arriba && resolverUtil(x, y - 1)) {
            rejilla[y][x].enSolucion = true;
            return true;
        }
        if (!rejilla[y][x].derecha && resolverUtil(x + 1, y)) {
            rejilla[y][x].enSolucion = true;
            return true;
        }
        if (!rejilla[y][x].abajo && resolverUtil(x, y + 1)) {
            rejilla[y][x].enSolucion = true;
            return true;
        }
        if (!rejilla[y][x].izquierda && resolverUtil(x - 1, y)) {
            rejilla[y][x].enSolucion = true;
            return true;
        }
    }
    return false; // Si no se encuentra un camino, retorna falso.
}

// Implementación del método para reiniciar el estado de visitado de todas las celdas
void Laberinto::reiniciarVisitados() {
    for (int y = 0; y < alto; ++y) { // Recorre cada fila del laberinto.
        for (int x = 0; x < ancho; ++x) { // Recorre cada columna del laberinto.
            rejilla[y][x].visitado = false; // Marca cada celda como no visitada.
        }
    }
}

// Implementación del método para imprimir el laberinto con la solución
void Laberinto::imprimirSolucion() {
    for (int y = 0; y < alto; ++y) { // Recorre cada fila del laberinto.
        // Imprime las paredes superiores
        for (int x = 0; x < ancho; ++x) {
            std::cout << (rejilla[y][x].arriba ? "+---" : "+   "); // Imprime '+' y '---' si hay pared arriba.
        }
        std::cout << "+\n";

        // Imprime las paredes izquierdas y celdas
        for (int x = 0; x < ancho; ++x) {
            if (rejilla[y][x].izquierda)
                std::cout << "|"; // Imprime '|' si hay pared a la izquierda.
            else
                std::cout << " "; // Si no hay pared, imprime un espacio.
            if (rejilla[y][x].enSolucion)
                std::cout << " * "; // Imprime '*' si la celda es parte de la solución.
            else
                std::cout << "   "; // Si no es parte de la solución, imprime espacios en blanco.
        }
        std::cout << "|\n";
    }

    // Imprime las paredes inferiores
    for (int x = 0; x < ancho; ++x) {
        std::cout << "+---";
    }
    std::cout << "+\n";
}

// Implementación del método para imprimir el laberinto sin la solución
void Laberinto::imprimir() {
    for (int y = 0; y < alto; ++y) { // Recorre cada fila del laberinto.
        // Imprime las paredes superiores
        for (int x = 0; x < ancho; ++x) {
            std::cout << (rejilla[y][x].arriba ? "+---" : "+   "); // Imprime '+' y '---' si hay pared arriba.
        }
        std::cout << "+\n";

        // Imprime las paredes izquierdas y celdas
        for (int x = 0; x < ancho; ++x) {
            std::cout << (rejilla[y][x].izquierda ? "|   " : "    "); // Imprime '|' si hay pared a la izquierda, seguido de espacios.
        }
        std::cout << "|\n";
    }

    // Imprime las paredes inferiores
    for (int x = 0; x < ancho; ++x) {
        std::cout << "+---";
    }
    std::cout << "+\n";
}

// Función principal
int main() {
    int ancho;
    int alto;
    std::cout << "Elige el ancho del laberinto: ";
    std::cin >> ancho;
    std::cout << "Elige la altura del laberinto: ";
    std::cin >> alto;

    Laberinto laberinto(ancho, alto); // Crea una instancia de la clase Laberinto.
    laberinto.generar(); // Genera el laberinto.
    laberinto.imprimir(); // Imprime el laberinto generado.

    if (laberinto.resolver()) { // Si se encuentra una solución para el laberinto
        std::cout << "\nSolución:\n"; // Imprime "Solución:".
        laberinto.imprimirSolucion(); // Imprime el laberinto con la solución.
    } else {
        std::cout << "\nNo se encontró solución.\n"; // Si no se encuentra una solución, imprime el mensaje correspondiente.
    }

    return 0; // Finaliza la ejecución del programa.
}
