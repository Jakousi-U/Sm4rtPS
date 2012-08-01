/*  Author: Jakousi Urbina
 *  Date: 2012-07-30
 *
 *  Interpreter
*/

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* Custom types */
typedef unsigned int RAM[1000];
typedef unsigned int REGISTRO[10];
typedef struct computer_t {
    bool doit;
    int count;
    RAM memRam;
    REGISTRO registros;
}computadora;
typedef computadora *comp;
typedef computadora *pNodo;
/* Prototipos de funciones */
void initComputadora(comp *compu);
void freeNodo(comp *compu);
void procesar(comp *compu);

//void print(comp *compu);

/* Funcion principal */
int main(int argc, char *argv[]){
    string cadena;
    comp * outputs;
    unsigned int inputNumber = 0;
    int aux0, aux1, aux2;

    //Se obtiene numero de casos
    cout << "INPUT\n";
    getline (cin,cadena);
    stringstream(cadena) >> inputNumber;
    //Se espera linea en blanco
    getline (cin,cadena);
    if(cadena.length() > 0) inputNumber = 0;
    if(inputNumber > 0){
      outputs = new (nothrow) comp[inputNumber];
      if(outputs == 0) cout << "ERROR\n";
      else{
           //Se leen las localidades de RAM
           for(aux0 = 0; aux0 < inputNumber; aux0++){
                outputs[aux0] = NULL;
                initComputadora(&outputs[aux0]);
                aux1 = -1;
                aux2 = 0;
                getline (cin,cadena);
                if(cadena.length() != 3) outputs[aux0]->doit = false;
                while(cadena.length() == 3 && aux2 <  1000){
                  stringstream(cadena) >> aux1;
                  if(aux1 > -1){
                    outputs[aux0]->memRam[aux2++] = aux1;
                    getline (cin,cadena);
                    if(cadena.length() != 3 && cadena.length() != 0) 
                      outputs[aux0]->doit = false;
                    aux1 = -1;
                  }else{ outputs[aux0]->doit = false;
                         cout << "\n";
                         break;
                         }
                  }
           }
      }
      cout << "\nOUTPUT\n";
      //Se ejecutan instrucciones
      for(aux0 = 0; aux0 < inputNumber; aux0++){
        if(outputs[aux0]->doit) {
             procesar(&outputs[aux0]);
             cout << outputs[aux0]->count << "\n\n";
             freeNodo(&outputs[aux0]);
        }else{
             freeNodo(&outputs[aux0]);
             cout << "ERROR" << "\n\n";
        }
      }
    }
    cin.ignore();
    return 0;
}

/* Inicializador de computadora */
void initComputadora(comp *compu){
     int i = 0;
     pNodo nuevo;
     /* Crear un nodo nuevo */
     nuevo = (pNodo)malloc(sizeof(computadora));
     nuevo->doit = true;
     nuevo->count = 0;
     for(i = 0; i < 10; i++)
           nuevo->registros[i] = 0;
     for(i = 0; i < 1000; i++)
           nuevo->memRam[i] = 0;
     //Asigna al apuntador el nuevo nodo creado
     *compu = nuevo;
}
/* Libera la memoria ocupada por un objeto computadora */
void freeNodo(comp *compu){
     pNodo nuevo;
     nuevo = *compu;
     *compu = NULL;
     free(nuevo);
}
/* Funcion para ejecutar y contar las instrucciones */
void procesar(comp *compu){
    pNodo nuevo;
    nuevo = *compu;
    int step = 0, aux = 0,c,d,u;
    while(step != 100 && nuevo->count < 1000){
     step = nuevo->memRam[aux];
     u = step % 10;
     d = ((step % 100) - u) / 10;
     c = (step - d*10 - u) / 100;
     if(step < 100){
             /* 0ds means goto the location in register d unless 
                register s contains 0 */
             if(nuevo->registros[u] != 0) 
               aux = nuevo->registros[d];
             else aux++;
     }else{
           switch(c){
            case 1: //100 means halt
                      break;
            case 2: //2dn means set register d to n (between 0 and 9)
                      nuevo->registros[d] = u;
                      break;
            case 3: //3dn means add n to register d
                      nuevo->registros[d] = 
                        (nuevo->registros[d] + u)
                         % 1000;
                      break;
            case 4: //4dn means multiply register d by n
                      nuevo->registros[d] = 
                        (nuevo->registros[d] * u)
                         % 1000;
                      break;
            case 5: //5ds means set register d to the value of register s
                      nuevo->registros[d] = 
                         nuevo->registros[u];
                      break;
            case 6: //6ds means add the value of register s to register d
                      nuevo->registros[d] = 
                        (nuevo->registros[d] + 
                         nuevo->registros[u])
                         % 1000;
                      break;
            case 7: //7ds means multiply register d by the value of register s
                      nuevo->registros[d] = 
                        (nuevo->registros[d] * 
                         nuevo->registros[u])
                         % 1000;
                      break;
            case 8: /*8da means set register d to the value in RAM whose 
                      address is in register a */
                      nuevo->registros[d] = 
                        nuevo->memRam[nuevo->registros[u]];
                      break;
            case 9: /* 9sa means set the value in RAM whose address is in 
                      register a to the value of register s*/
                      nuevo->memRam[nuevo->registros[u]] =
                       nuevo->registros[d];
                      break;
           }
           aux++;
     }
     nuevo->count++;
     //print(compu);
    }
    nuevo = NULL;
    free(nuevo);
}

/*void print(comp *compu){
     int i = 0;
     pNodo nuevo;
     nuevo = *compu;
      for(i = 0; i < 10; i++)
           cout << i << ":" << nuevo->registros[i] << " - ";
      cout << "\n";
     nuevo = NULL;
     free(nuevo);
}*/
