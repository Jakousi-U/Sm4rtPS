/*  Author: Jakousi Urbina
 *  Date: 2012-07-31
 *
 *  
*/

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* Custom types */
//boletas
typedef struct field_t {
    //lados del campo
    int n;
    int m;
    //celdas del campo
    int * celdas;
    //apuntador a la siguiente boleta
    struct field_t *siguiente;
}campo;
typedef campo *Cola;
typedef campo *cNodo;
/* Prototipos de funciones */
void initCampo(Cola *head, Cola *tail,int n,int m);
void freeNodo(Cola *field);
void print(Cola *head);
/* Funcion principal */
int main(int argc, char *argv[]){
    int n = 0,m = 0, i,j;
    string cadena;
    Cola inicio = NULL, fin = NULL;
    //Se obtiene los campos
    cout << "INPUT\n";
    do{
      n = 0;
      m = 0;
      getline (cin,cadena);
      stringstream(cadena) >> n >> m ;
      if(n > 0 && n < 101 && m > 0 && m < 101){
           initCampo(&inicio, &fin, n, m);
           char c;
           i = 0;
           j = 0;
           do{
             c = getchar();
             if(c == '*'){
                fin->celdas[i] = -1;
                i++;
             }
             else if(c=='\n' || c==EOF){
                i = ++j * m;
             }else{
               i++;
             }
           }while(j < n);
      }
    }while(n > 0 && n < 101 && m > 0 && m < 101);
    cout << "\nOUTPUT\n\n";
    i = 1;
    while(inicio){
       cNodo nuevo;
       nuevo = inicio;
       inicio = inicio-> siguiente;
       cout << "FIELD #" << i++ << ":\n";
       print(&nuevo);
    }
    cin.ignore();
    return 0;
}
//Genera un nuevo campo
void initCampo(Cola *head, Cola *tail,int n,int m){
     cNodo nuevo;
     int i;
     /* Crear un nodo nuevo */
     nuevo = (cNodo)malloc(sizeof(campo));
     nuevo->n = n;
     nuevo->m = m;
     nuevo->celdas = new int[n*m];
     nuevo->siguiente = NULL;
     //inicializamos celdas
     for(i = 0; i < n*m; i++){
          nuevo->celdas[i] = 0; 
     }
     //sea ajustan apuntadores
     if(*head == NULL){
        *head = nuevo;
     }
     if(*tail){
         cNodo aux;
         aux = *tail;
         aux->siguiente = nuevo;
     }
     *tail = nuevo;
}
/* Libera la memoria ocupada por un estructura eleccion */
void freeNodo(Cola *field){
     cNodo nuevo;
     nuevo = *field;
     field = NULL;
     free(nuevo);
}
/* Imprime el contenido del campo */
void print(Cola *head){
     cNodo nuevo;
     int i, aux, j;
     nuevo = *head;
     aux = nuevo->n*nuevo->m;
     //Se evaluan las celdas
     for(i = 0; i < aux; i++){
          //Se checa si es una mina
          if(nuevo->celdas[i] == -1){
          /* se checan una a una las celdas adyacentes si no son minas (-1) se 
          aumenta en uno su valor, se toma preucacion en caso de que la celda 
          con mina este en un extremo lateral del tablero */
             j = i - nuevo->m - 1;
               if(j >= 0 && j < aux && i%nuevo->m != 0)
                  if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
             j = i - nuevo->m;
               if(j >= 0 && j < aux)
                 if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
             j = i - nuevo->m + 1;
              if(j >= 0 && j < aux && (i+1)%nuevo->m != 0)
                 if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
             j = i - 1;
               if(j >= 0 && j < aux && i%nuevo->m != 0)
                 if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
             j = i + 1;
               if(j >= 0 && j < aux && (i+1)%nuevo->m != 0)
                 if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
             j = i + nuevo->m - 1;
               if(j >= 0 && j < aux && i%nuevo->m != 0)
                 if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
             j = i + nuevo->m;
               if(j >= 0 && j < aux)
                 if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
             j = i + nuevo->m + 1;
               if(j >= 0 && j < aux && (i+1)%nuevo->m != 0)
                 if(nuevo->celdas[j] != -1)
                     nuevo->celdas[j]++;
          }            
     }
     //Se imprimen las celdas
     for(i = 0; i < aux; i++){
          if(i % nuevo->m == 0 && i > 0) cout << "\n";
          //Se checa si es una mina
          if(nuevo->celdas[i] == -1)
              cout << "*";
          else
              cout << nuevo->celdas[i];
     }
     cout << "\n\n";
     freeNodo(head);
}

