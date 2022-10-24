#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define CCP_NO_INI 2
#define POS_NULA NULL
#define ELE_NULA NULL



int cantBits(int i);
char* tobinary(int i);

typedef void * TClave; //prioridad
typedef void * TValor; //dato a almacenar

typedef struct entrada {
TClave clave;
TValor valor;
} * TEntrada;

typedef struct nodo {
TEntrada entrada;
struct nodo * padre;
struct nodo * hijo_izquierdo;
struct nodo * hijo_derecho;
} * TNodo;

typedef struct cola_con_prioridad {
int cantidad_elementos;
TNodo raiz;
int (*comparador)(TEntrada, TEntrada);
} * TColaCP;
void enlazar(TNodo padre,TNodo hijo,int lado);
//privadas
void intercambiar(TColaCP cola, TNodo padre, TNodo hijo){
    //guardo abuelo y si el padre es su hijo izq o der
    TNodo abuelo=padre->padre;
    int ladoPadre=0;
    //guardo el otro hijo del padre y de que lado va cada uno
    TNodo otroHijo;
    int ladoHijo=0;
    int ladoOtroHijo=0;
    if(padre->hijo_izquierdo!=hijo){
        ladoHijo=1;
        otroHijo=padre->hijo_izquierdo;
    }else{
            ladoOtroHijo=1;
            otroHijo=padre->hijo_derecho;
    }
    if(abuelo!=NULL){
        if(abuelo->hijo_izquierdo!=padre)
            ladoPadre=1;
    }
    //enlazo cada uno con quien va
    enlazar(padre,hijo->hijo_izquierdo,0);
    enlazar(padre,hijo->hijo_derecho,1);

    enlazar(hijo,otroHijo,ladoOtroHijo);
    enlazar(hijo,padre,ladoHijo);
    enlazar(abuelo,hijo,ladoPadre);
}
void enlazar(TNodo padre,TNodo hijo,int lado){
    if(padre!=NULL){
        if(lado==0){
            padre->hijo_izquierdo=hijo;
        }
        else padre->hijo_derecho=hijo;
    }
    if(hijo!=NULL)
        hijo->padre=padre;
}
//funciones
TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)){
    TColaCP newCola=(TColaCP) malloc(sizeof(struct cola_con_prioridad));
    //Seteo todo vacio
    newCola->cantidad_elementos=0;
    newCola->raiz=NULL;
    newCola->comparador=f;
    return newCola;
};
int cp_insertar(TColaCP cola, TEntrada entr){
    (cola)->cantidad_elementos=cola->cantidad_elementos+1;
    //busco ultima hoja donde se insertará

    char* ruta=tobinary(cola->cantidad_elementos);
    ruta=ruta+1;
    TNodo ultHoja=(TNodo) malloc(sizeof(struct nodo));
    ultHoja->entrada=entr;
    ultHoja->hijo_derecho=NULL;
    ultHoja->hijo_izquierdo=NULL;
  //  int lado=0;


    TNodo padreUltHoja=cola->raiz;
    if(padreUltHoja!=NULL){
        while(*(ruta+1)!='\0'){
            if(*ruta=='0')
                padreUltHoja=padreUltHoja->hijo_izquierdo;
            else padreUltHoja=padreUltHoja->hijo_derecho;
            ruta=ruta+1;
        }
        enlazar(padreUltHoja,ultHoja,(*ruta)-48);
/*      if((*ruta)=='0'){
            padreUltHoja->hijo_izquierdo=ultHoja;
    //        lado=0;
        }
        else{
            padreUltHoja->hijo_derecho=ultHoja;
    //        lado=1;
        }
        ultHoja->padre=padreUltHoja;
*/
    }else cola->raiz=ultHoja;

    //falta hacer free de ruta;

    int burbujeo=0;
    if(cola->cantidad_elementos>1){
        if((cola->comparador(padreUltHoja->entrada,ultHoja->entrada))==1)
            burbujeo=1;
    }
    if(burbujeo==1){
        TNodo padre=padreUltHoja;
        TNodo hijo=ultHoja;
        while(burbujeo==1){
            intercambiar(cola,padre,hijo);
            hijo=padre;
            padre=padre->padre;
            if(cola->comparador(padre->entrada,hijo->entrada)>=0)
                burbujeo=0;
        }
    }


}


char* tobinary(int i){
    int bits=cantBits(i);
    char* devolver=(char*) malloc(sizeof(char)*(bits+1));
    devolver[bits]=0;
    int digito=bits-1;
    while(i!=0 && digito!=-1){
        int resto=i%2;
        devolver[digito--]=resto+48;
        i=i/2;
    }
    return devolver;
}
int cantBits(int i){
    int bits=0;
    while(i!=0){
        i=i/2;
        bits++;
    }
    return bits;
}


TEntrada cp_eliminar(TColaCP cola);

int cp_cantidad(TColaCP cola);
void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada) );
int f(TEntrada t1,TEntrada t2){
    int* clave1=(int*)t1->clave;
    int* clave2=(int*)t2->clave;
    int retornar=0;
    if((*clave1)>(*clave2))
        retornar=1;
    else if((*clave1)<(*clave2))
        retornar=-1;
    else retornar=0;
    return retornar;
}
int main(){
    TEntrada t1=(TEntrada) malloc(sizeof(struct entrada));
    TEntrada t2=(TEntrada) malloc(sizeof(struct entrada));
    int clave1=5;
    int clave2=10;
    int valor1=1;
    int valor2=2;
    t1->clave=&clave1;
    t2->clave=&clave2;

    t1->valor=&valor1;
    t2->valor=&valor2;
    int (*ptr)(TEntrada,TEntrada)=&f;
    TColaCP cola= crear_cola_cp(ptr);

    TEntrada t3=(TEntrada) malloc(sizeof(struct entrada));

    int clave3=15;

    int valor3=3;
    t3->clave=&clave3;
    t3->valor=&valor3;

    TEntrada t4=(TEntrada) malloc(sizeof(struct entrada));

    int clave4=20;

    int valor4=4;
    t4->clave=&clave4;
    t4->valor=&valor4;

    cp_insertar(cola,t2);
    cp_insertar(cola,t1);

    int* valor=cola->raiz->entrada->valor;
    printf("%d ",*valor);
    valor=cola->raiz->hijo_izquierdo->entrada->valor;
    printf("%d ",*valor);


/*






    cp_insertar(cola,t1);
    if(cola->raiz!=NULL){
        printf("hay raiz");
        if(cola->raiz->hijo_izquierdo!=NULL)
            printf("hay 2do hijo");
    }
*/
//    cp_insertar(cola,t3);

//    TNodo n1=cola->raiz;
//    TNodo n2=n1->hijo_izquierdo;
//    TNodo n3=n1->hijo_derecho;
/*
    if(n2->padre!=n1)
        printf("error");
    else printf("correcto");
    if(n3->padre!=n1)
        printf("error");
    else printf("correcto");

    intercambiar(cola,n1,n2);
    if(n2->hijo_izquierdo==n1)
        printf("correcto");
    else printf("error");
    if(n1->padre!=n2)
        printf("error");
    else printf("correcto");
*/
//    int* salida1=(int*) n1->entrada->valor;
//    int* salida2=(int*) n1->hijo_izquierdo->entrada->valor;
//    int* salida3=(int*) n1->hijo_derecho->entrada->valor;



//    printf("%d\n",*salida1);
//    printf("%d\n",*salida2);
 //   printf("%d",*salida3);

}
