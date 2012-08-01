/*  Author: Jakousi Urbina
 *  Date: 2012-07-31
 *
 *  Australian Voting
*/

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* Custom types */
//boletas
typedef struct boleta_t {
    //primera eleccion
    unsigned int first;
    //demas contenido de la boleta
    string * otros;
    //apuntador a la siguiente boleta
    struct boleta_t *siguiente;
}ballot;
typedef ballot *Pila;
typedef ballot *bNodo;
//Eleccion
typedef struct election_t {
    //numero de canditatos
    int size;
    //nombres de candidatos
    string *candidatos;
    //votos por candidatos
    int *votos;
    //total de boletas
    int total;
    //pila de boletas
    Pila pila;
    //ganador
    int winner;
}eleccion;
typedef eleccion *voting;
typedef eleccion *pNodo;
/* Prototipos de funciones */
void initEleccion(voting *election,int n);
void freeNodo(voting *election);
void PushBoleta(voting *election, string boleta);
void ContarVotos(voting *election);
void print(voting *election);
/* Funcion principal */
int main(int argc, char *argv[]){
    string cadena;
    unsigned int inputNumber = 0, aux0, aux1, aux2;
    voting * inputs;
    //Se obtiene numero de casos
    cout << "INPUT\n";
    getline (cin,cadena);
    stringstream(cadena) >> inputNumber;
    //Se espera linea en blanco
    getline (cin,cadena);
    if(cadena.length() > 0) inputNumber = 0;
    if(inputNumber > 0){
        inputs  = new (nothrow) voting[inputNumber];
        if(inputs == 0) cout << "ERROR\n";
        else{
             //Se reciben las boletas
             for(aux0 = 0; aux0 < inputNumber; aux0++){
                aux1 = 0;
                //Se recibe numero de candidatos
                getline (cin,cadena);
                stringstream(cadena) >> aux1;
                if(aux1 > 0 && aux1 < 21){
                       initEleccion(&inputs[aux0],aux1);
                       //Se reciben los nombres de los candidatos
                       for(aux2 = 0; aux2 < aux1; aux2++){
                           getline (cin,cadena);
                           if(cadena.length() > 0 && cadena.length() < 80)
                              inputs[aux0]->candidatos[aux2] = cadena;
                           else
                              inputs[aux0]->candidatos[aux2] = "unknown";
                       }
                       //Se reciben las boletas
                       getline (cin,cadena);
                       aux2 = 0;
                       while(cadena.length() > 0 && aux2 < 1000){
                          if(cadena.length() > 0) 
                             PushBoleta(&inputs[aux0],cadena);
                          getline (cin,cadena);
                          aux2++;
                       }
                }else cout << "ERROR\n";
             } 
        }
    }
    cout << "\nOUTPUT\n";
    for(aux0 = 0; aux0 < inputNumber; aux0++){
       ContarVotos(&inputs[aux0]);
       print(&inputs[aux0]);
    }
    cin.ignore();
    return 0;
}
/* Setea los valores de la estructura eleccion*/
void initEleccion(voting *election,int n){
     pNodo nuevo;
     int i;
     /* Crear un nodo nuevo */
     nuevo = (pNodo)malloc(sizeof(eleccion));
     nuevo->size = n;
     nuevo->candidatos = new string[n];
     nuevo->votos = new int[n];
     for(i = 0; i < n; i++){
          nuevo->votos[i] = 0; 
     }
     nuevo->pila = NULL;
     nuevo->total = 0;
     nuevo->winner = -1;
     //Asigna al apuntador el nuevo nodo creado
     *election = nuevo;
}
/* Libera la memoria ocupada por un estructura eleccion */
void freeNodo(voting *election){
     pNodo nuevo;
     nuevo = *election;
     *election = NULL;
     free(nuevo);
}
/* Agrega una boleta a la eleccion */
void PushBoleta(voting *election, string boleta){
     string otros = "";
     int i, first = 0, space = -1;
     bNodo nuevo;
     nuevo = (bNodo)malloc(sizeof(ballot));
     //Se obtiene la primera eleccion y se saca de la boleta
     space = boleta.find(" ");
     if (space!=string::npos){
        if(space + 1 != boleta.length()) space++;
        otros = boleta.substr(space);
        stringstream(boleta.substr(0, space)) >> first;
     }else{
        stringstream(boleta) >> first;
     }
     //Si hay un candidato seleccionado
     if(first > 0){
       //se genera registro de boleta
       nuevo->first = first;
       nuevo->otros = new string(otros);
       //se agrega a la eleccion
       pNodo auxNew;
       auxNew = *election;
       nuevo->siguiente = auxNew->pila;
       auxNew->pila = nuevo;
       auxNew->total++;
     }
}

/* Realiza el conteo de votos */
void ContarVotos(voting *election){
     bNodo nuevo;
     int aux,i,ganador = false, empate = true, maxvotos, minvotos;
     pNodo auxNew;
     auxNew = *election;
     //Si solo hay un candidato este gano
     if(auxNew->size == 1){
       auxNew->winner = 0;
       return;
     }
     //recuperamos pila de boleta
     nuevo = auxNew->pila;
     for(i = 0; i < auxNew->size; i++){
          //reinicializamos los votos
          auxNew->votos[i] = 0; 
     }
     while(nuevo){
       //solo se consideran los primeros votos correctos
       aux = nuevo->first;
       if(aux > 0){
         auxNew->votos[aux-1]++;
       }else{
       //Existe error
       auxNew->winner = -3;
       return;
     }
       nuevo = nuevo->siguiente;
     }
     maxvotos = 0;
     minvotos = auxNew->total;
     //Se busca al ganador absoluto
     for(i = 0; i < auxNew->size; i++){
          if(auxNew->votos[i] * 2 > auxNew->total){
            auxNew->winner = ++i;
            break;
          }else if(auxNew->votos[i] > 0){
                //se localiza el maximo numero d evotos y el minimo
                if(auxNew->votos[i] > maxvotos)
                  maxvotos = auxNew->votos[i];
                if(auxNew->votos[i] < minvotos)
                  minvotos = auxNew->votos[i];
          }
     }
     if(auxNew->winner > 0) return;
     if(maxvotos == minvotos){
     //hay empate
     auxNew->winner = -2;
       return;
     }
     //Si hya un minimo de votos se eliminan de las boletas los candidatos
     nuevo = auxNew->pila;
     while(nuevo){
       //solo se consideran los primeros votos correctos
       aux = nuevo->first;
       if(aux > 0){
         while(auxNew->votos[aux-1] <= minvotos){
             //Se obtiene la primera eleccion y se saca de la boleta
             string boleta = *(nuevo->otros);
             string otros =  "";
             int space = boleta.find(" "), first = -1;
             if (space!=string::npos){
                if(space + 1 != boleta.length()) space++;
                otros = boleta.substr(space);
                stringstream(boleta.substr(0, space)) >> first;
             }else{
                stringstream(boleta) >> first;
             }
             //Si hay un candidato seleccionado
             if(first > 0){
               //se genera registro de boleta
               nuevo->first = first;
               nuevo->otros = new string(otros);
             }
             aux = nuevo->first;
         }
       }else{
       //Existe error
       auxNew->winner = -3;
       return;
     }
       nuevo = nuevo->siguiente;
     }
     //Se vuelven a contar los votos
      ContarVotos(election);
}
/* Imprime el resultado de la eleccion */
void print(voting *election){
     pNodo nuevo;
     int i;
     nuevo = *election;
     if(nuevo->winner > 0)
        //si hay bandera positiva hay un solo ganador
        cout << nuevo->candidatos[nuevo->winner - 1] << "\n\n";
     else if(nuevo->winner == -2){
       //si hay empate s eimprimen los candidatatos con votos
       for(i = 0; i < nuevo->size; i++){
          if(nuevo->votos[i] > 0) cout << nuevo->candidatos[i] << "\n";
       }
     }else{
           cout << "Error\n\n";
     }
     freeNodo(election);
}
