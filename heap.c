#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

#define TAM_INI 50
#define FACT_REDIM 2

void swap(void** arr, size_t pos1, size_t pos2);
void upheap(void** arr, size_t pos, cmp_func_t cmp);
void downheap(void** arr, size_t n,size_t pos, cmp_func_t cmp);
void heapify(void* arr, size_t cant, cmp_func_t cmp);
bool heap_redimensionar(heap_t* heap, size_t nuevo_tam);

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos*/
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	if(cant<=1) return;
	heapify(elementos,cant,cmp);
	for(size_t i = cant; i!=0;i--){
		swap(elementos,0,i-1);
		downheap(elementos,i-1,0,cmp);
	}
}

/** TAD COLA DE PRIORIDAD MAX-HEAP**/

struct heap{
	size_t cant;
	void** datos;
	size_t cap;
	cmp_func_t cmp;
};

/* Crea un heap. Recibe la función de comparación a utilizar*/
heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(heap==NULL) return NULL;
	void* datos = malloc(sizeof(void*)*TAM_INI);
	if(datos==NULL){
		free(heap);
		return NULL;
	}
	heap->datos=datos;
	heap->cant=0;
	heap->cap=TAM_INI;
	heap->cmp=cmp;
	return heap;
}

/* Constructor alternativo del heap. 
 * Recibe una función de comparación y un arreglo
 * de valores para inicializar el heap. */
heap_t *heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = heap_crear(cmp);
	if(heap==NULL) return NULL;
	for(size_t i = 0; i<n; i++){
		heap_encolar(heap,arreglo[i]);
	}
	return heap;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.*/
void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if(destruir_elemento!=NULL){
		for(size_t i = 0; i < heap->cant ; i++){
			destruir_elemento(heap->datos[i]);
		}
	}
	free(heap->datos);
	free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap){
	return heap->cant;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0*/
bool heap_esta_vacio(const heap_t *heap){
	return heap->cant==0;
}



/* Agrega un elemento al heap. El elemento no puede ser NULL.*/
bool heap_encolar(heap_t *heap, void *elem){
	if(elem==NULL)return false;
	if(heap->cant==heap->cap){
		if(!heap_redimensionar(heap,heap->cap*FACT_REDIM))return false;
	}
	heap->datos[heap->cant]=elem;
	upheap(heap->datos,heap->cant,heap->cmp);
	heap->cant+=1;
	return true;
}

/* Devuelve el elemento con máxima prioridad.*/
void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap))return NULL;
	return heap->datos[0];
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.*/
void *heap_desencolar(heap_t *heap){
	if(heap_esta_vacio(heap))return NULL;
	void* elem = NULL;
	if(heap_cantidad(heap)==1){
		elem = heap->datos[0];
		heap->datos[0] = NULL;
		heap->cant-=1;
		return elem;
	}
	size_t pos_ult = heap->cant-1;
	swap(heap->datos,0,pos_ult);
	elem = heap->datos[pos_ult];
	heap->datos[pos_ult]=NULL;
	heap->cant-=1;
	downheap(heap->datos,heap->cant,0,heap->cmp);
	size_t cap_aux = heap->cap/4;
	if(heap->cant<=cap_aux&&cap_aux>=TAM_INI){
		heap_redimensionar(heap,heap->cap/FACT_REDIM);
	}
	return elem;
}

/** FUNCIONES AUXILIARES **/

/* Swap recibe un vector de punteros y dos posiciones del mismo.
 * Intercambia las direcciones de los punteros */
void swap(void** arr, size_t pos1, size_t pos2){
	void* puntero = arr[pos1];
	arr[pos1]=arr[pos2];
	arr[pos2]=puntero;
	return;
}

/*Función auxiliar para evaluar y conservar la condición de heap 
 * en una posicón válida del mismo "hacia arriba" */
void upheap(void** arr, size_t pos, cmp_func_t cmp){
	if(pos==0){
		return;
	}
	size_t pos_padre = (pos-1)/2;
	if(cmp(arr[pos],arr[pos_padre])<0)return;
	swap(arr,pos,pos_padre);
	upheap(arr,pos_padre,cmp);
}

/*Función auxiliar para evaluar y conservar la condición de heap
 * en una posición válida del mismo "hacia abajo" */
void downheap(void** arr, size_t n, size_t pos, cmp_func_t cmp){
	if(pos>=n)return;
	size_t izq = (2*pos)+1;
	size_t der = (2*pos)+2;
	size_t pos_max = pos;
	if(izq<n&&cmp(arr[izq],arr[pos_max])>0) pos_max = izq;
	if(der<n&&cmp(arr[der],arr[pos_max])>0) pos_max = der;
	if(pos!=pos_max){
		swap(arr,pos,pos_max);
		downheap(arr,n,pos_max,cmp);
	}
	return;
}

void heapify(void* arr, size_t cant, cmp_func_t cmp){
	for(size_t i = cant ; i!=0 ; i--){
		downheap(arr,cant, i-1,cmp);
	}
}

bool heap_redimensionar(heap_t* heap, size_t nuevo_tam){
	void* nuevo_datos = realloc(heap->datos, sizeof(void*)*nuevo_tam);
	if(nuevo_datos==NULL) return false;
	heap->cap=nuevo_tam;
	heap->datos=nuevo_datos;
	return true;
}
