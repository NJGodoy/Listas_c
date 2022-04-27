#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

#define ERROR NULL;

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if(!lista)
		return ERROR;
	
	nodo_t *nodo = malloc(sizeof(nodo_t));

	if(!nodo)
		goto salida_free;

	nodo->elemento = elemento;
	nodo->siguiente = NULL;

	if(lista_vacia(lista)){
		lista->nodo_fin = nodo;
		lista->nodo_inicio = nodo;
	}
	else{
		lista->nodo_fin->siguiente = nodo;
		lista->nodo_fin = nodo;
	}

	lista->cantidad++;

	return lista;

salida_free:
        lista_destruir(lista);
        return ERROR;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if(!lista)
		return ERROR;
	
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if(!nodo)
		goto salida_free;

	nodo->elemento = elemento;

	if(posicion == 0){
		nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo;
		lista->cantidad++;
		return lista;
	}

	if(posicion == 1){
		nodo->siguiente = lista->nodo_inicio->siguiente;
		lista->nodo_inicio->siguiente = nodo;
		if(lista_tamanio(lista) == 1)
			lista->nodo_fin = nodo;
	
		lista->cantidad++;
		return lista;
	}

	if(posicion >= lista_tamanio(lista)){
		nodo->siguiente = NULL;
		if(lista_tamanio(lista) == 0)
			lista->nodo_inicio = nodo;
		if(lista->nodo_fin)
			lista->nodo_fin->siguiente = nodo;

		lista->nodo_fin = nodo;
		lista->cantidad++;
		return lista;
	}

	nodo_t *nodo_anterior = lista->nodo_inicio;
	for(size_t i = 0; i < posicion - 1; i++)
		nodo_anterior = nodo_anterior->siguiente;
	
	nodo->siguiente = nodo_anterior->siguiente;
	nodo_anterior->siguiente = nodo;
	lista->cantidad++;

	return lista;

salida_free:
        lista_destruir(lista);
        return ERROR;
}

void *lista_quitar(lista_t *lista)
{
	if(lista_vacia(lista))
		return ERROR;
	
	nodo_t *penultimo = lista->nodo_inicio;
	void *elemento;

	if(penultimo->siguiente == NULL){
		elemento = lista->nodo_inicio->elemento;
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
		lista->cantidad--;
		free(penultimo);
		return elemento;
	}

	while(penultimo->siguiente != lista->nodo_fin)
		penultimo = penultimo->siguiente;
	
	elemento = lista->nodo_fin->elemento;
	free(lista->nodo_fin);
	lista->nodo_fin = penultimo;
	lista->nodo_fin->siguiente = NULL;
	lista->cantidad--;

	return elemento;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if(lista_vacia(lista))
		return ERROR;
	

	nodo_t *nodo;
	void *elemento;

	if(posicion == 0){
		nodo = lista->nodo_inicio->siguiente;
		elemento = lista->nodo_inicio->elemento;
		free(lista->nodo_inicio);
		lista->nodo_inicio = nodo;
		lista->cantidad--;
		return elemento;
	}

	if(posicion >= lista_tamanio(lista) - 1)
		return lista_quitar(lista);
	
	nodo_t *nodo_anterior = lista->nodo_inicio;
	for(size_t i = 0; i < posicion - 1; i++)
		nodo_anterior = nodo_anterior->siguiente;
	
	nodo = nodo_anterior->siguiente;
	elemento = nodo->elemento;
	nodo_anterior->siguiente = nodo->siguiente;
	free(nodo);
	lista->cantidad--;

	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if(lista_vacia(lista) || posicion > lista->cantidad - 1)
		return ERROR;
	
	if(posicion == 0)
		return lista->nodo_inicio->elemento;
	
	if(posicion == lista->cantidad - 1)
		return lista->nodo_fin->elemento;
	
	nodo_t *nodo = lista->nodo_inicio;
	for(size_t i = 0; i < posicion; i++)
		nodo = nodo->siguiente;
	

	return	nodo->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if(lista_vacia(lista) || !comparador)
		return ERROR;
	
	nodo_t *nodo = lista->nodo_inicio;
	while(nodo){
		if(comparador(nodo->elemento, contexto) == 0)
			return nodo->elemento;
		nodo = nodo->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if(lista_vacia(lista))
		return ERROR;

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if(lista_vacia(lista))
		return ERROR;

	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if(lista_tamanio(lista) == 0)
		return true;

	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if(lista == NULL)
		return 0;

	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if(!lista_vacia(lista)){
		nodo_t *nodo = lista->nodo_inicio;
		while(nodo){
			nodo_t *nodo_siguiente = nodo->siguiente;
			free(nodo);
			nodo = nodo_siguiente;
		}
	}
	if(lista)
		free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if(!funcion)
		goto destruir_normal;
	
	if(!lista_vacia(lista)){
		nodo_t *nodo = lista->nodo_inicio;
		while(nodo){
			nodo_t *nodo_siguiente = nodo->siguiente;
			funcion(nodo->elemento);
			free(nodo);
			nodo = nodo_siguiente;
		}
	}
	if(lista)
		free(lista);
	
	return;

destruir_normal:
        lista_destruir(lista);
        return;
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if(!lista)
		return ERROR;

	lista_iterador_t *iterador = calloc(1, sizeof(lista_iterador_t));
	if(!iterador)
		return ERROR;

	iterador->corriente = lista->nodo_inicio;
	iterador->lista = lista;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if(!iterador)
		return false;

	if(iterador->corriente)
		return true;

	return false;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if(!iterador || !iterador->corriente)
		return false;

	iterador->corriente = iterador->corriente->siguiente;

	if(!iterador->corriente){
		return false;
	}

	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if(!iterador)
		return ERROR;
	
	if(!iterador->corriente)
		return NULL;
	
	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{

	if(lista_vacia(lista) || !funcion)
		return 0;

	size_t iteraciones = 0;
	nodo_t *nodo = lista->nodo_inicio;
	while(nodo && funcion(nodo->elemento, contexto)){
		nodo = nodo->siguiente;
		iteraciones++;
	}
	return iteraciones;
}
