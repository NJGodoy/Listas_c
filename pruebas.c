#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include "pa2m.h"


void crear_una_lista_me_devuelve_una_lista_len_0()
{
	lista_t *lista = lista_crear();

	pa2m_afirmar(lista != NULL, "La lista creada es válida");
	pa2m_afirmar(lista_primero(lista) == NULL, "El primer elemento es NULL");
	pa2m_afirmar(lista_ultimo(lista) == NULL, "El último elemento es NULL");
	pa2m_afirmar(lista_tamanio(lista) == 0, "El tamanio de la lista es 0");
	pa2m_afirmar(lista_vacia(lista), "lista_vacia() me devuelve true");

	lista_destruir(lista);
}

void agrego_o_quito_elementos_de_lista_NULL()
{
	int elemento = 1;
	pa2m_afirmar(lista_insertar(NULL, &elemento) == NULL,
		"NO puedo agregar elementos a una lista nula");

	pa2m_afirmar(lista_quitar(NULL) == NULL,
		"NO puedo quitar elementos a una lista nula");
}

void creo_una_lista_con_elementos_y_elimino()
{
	lista_t *lista = lista_crear();

	int numeros[] = { 4, 2, 5, 7, 12};
	for(int i = 0; i < 5; i++)
		lista_insertar(lista, numeros + i);
	
	pa2m_afirmar(lista_tamanio(lista) == 5,
			"El tamaño de la lista { 4, 2, 5, 7, 12}  es 5");

	int elemento1 = 10;
	lista_insertar_en_posicion(lista, &elemento1, 3);
	int *num1 = lista_elemento_en_posicion(lista, 3);
	pa2m_afirmar(*num1 == 10,
		"Se inserto el numero 10 en pos 3 { 4, 2, 5, 10, 7, 12}");

	int *num2 = lista_quitar(lista);
	pa2m_afirmar(*num2 == 12,
		"Se quitó el numero 12 al final { 4, 2, 5, 10, 7}");

	int *num3 = lista_quitar_de_posicion(lista, 3);
	pa2m_afirmar(*num3 == 10,
		"Se quitó el numero 10, pasando pos 3 { 4, 2, 5, 7}");

	int elemento2 = 20;
	lista_insertar_en_posicion(lista, &elemento2, 15);
	int *num4 = lista_ultimo(lista);
	pa2m_afirmar(*num4 == 20,
		"Se insertó el numero 20 al final pasando pos > tamaño { 4, 2, 5, 7, 20}");

	int *num5 = lista_quitar_de_posicion(lista, 10);
	pa2m_afirmar(*num5 == 20,
		"Se quitó el numero 20, pasando pos > tamaño { 4, 2, 5, 7}");

	pa2m_afirmar(lista_tamanio(lista) == 4, "El tamaño de la lista es 4");

	lista_destruir(lista);
}

bool contar_elemento(void *elemento, void *contador)
{
	if (elemento && contador)
		(*(int *)contador)++;
	return true;
}

void creo_una_lista_de_elementos_iterables()
{
	lista_t *lista = lista_crear();

	for(int i = 0; i < 5; i++){
		int *numero = malloc(sizeof(int));
		if(!numero){
			lista_destruir_todo(lista, free);
			return;
		}
		*numero = i + 1;
		lista_insertar(lista, numero);
	}


	int contador = 0;
	lista_iterador_t *it = NULL;
	for (it = lista_iterador_crear(lista);
	     lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it)){
		contador++;
		if(contador == 3){
			int *elemento = lista_iterador_elemento_actual(it);
			pa2m_afirmar(*elemento == 3,
				"El elemento actual es el correcto");
		}

	     }
	
	pa2m_afirmar(contador == 5,
		"Se recorrió correctamente la lista con iterador externo");
	lista_iterador_destruir(it);
	
	int contador_int = 0;
	size_t elementos_recorridos = 0;
	elementos_recorridos = lista_con_cada_elemento(lista,contar_elemento,
							(void *)&contador_int);

	pa2m_afirmar(elementos_recorridos == (size_t)contador_int,
		"Se recorrió correctamente la lista con iterador interno");
	lista_destruir_todo(lista, free);
}

void creo_una_cola_inicializada_en_0()
{
	cola_t *cola = cola_crear();
	pa2m_afirmar(cola != NULL, "La cola creada es válida");
	pa2m_afirmar(cola_frente(cola) == NULL, "El frente de la cola es NULL");
	pa2m_afirmar(cola_tamanio(cola) == 0, "El tamaño de la cola es 0");
	pa2m_afirmar(cola_vacia(cola), "cola_vacia() me devuelve true");

	cola_destruir(cola);
}

void agrego_o_quito_elementos_de_una_cola_nula()
{
	int elemento = 1;
	pa2m_afirmar(cola_encolar(NULL, &elemento) == NULL,
		"NO puedo encolar en cola NULL");

	pa2m_afirmar(cola_desencolar(NULL) == NULL,
		"NO puedo desencolar en cola NULL");
}

void agrego_o_quito_elementos_de_una_cola()
{
	cola_t *cola = cola_crear();
	char letras[] = { 'a', 'b', 'c', 'd', 'e'};
	for(int i = 0; i < 5; i++)
		cola_encolar(cola, letras + i);
	
	pa2m_afirmar(cola_tamanio(cola) == 5, "Se encolaron todos los elementos");
	
	char *l1 = cola_frente(cola);
	pa2m_afirmar(*l1 == 'a',
		"El frente de la cola { 'a', 'b', 'c', 'd', 'e'} apunta a 'a'");

	char *l2 = cola_desencolar(cola);
	pa2m_afirmar(*l2 == 'a', "Desencolar devuelve 'a'");

	char *l3 = cola_frente(cola);
	pa2m_afirmar(*l3 == 'b',
		"El frente de la cola { 'b', 'c', 'd', 'e'} apunta a 'b'");

	cola_destruir(cola);
}

void creo_una_pila_inicializada_en_0()
{
	pila_t *pila = pila_crear();
	pa2m_afirmar(pila != NULL, "La pila creada es válida");
	pa2m_afirmar(pila_tope(pila) == NULL, "El tope de la pila es NULL");
	pa2m_afirmar(pila_tamanio(pila) == 0, "El tamaño de la pila es 0");
	pa2m_afirmar(pila_vacia(pila), "pila_vacia() me devuelve true");

	pila_destruir(pila);
}

void agrego_o_quito_elementos_de_una_pila_nula()
{
	int elemento = 1;
	pa2m_afirmar(pila_apilar(NULL, &elemento) == NULL,
		"NO puedo apilar en pila NULL");

	pa2m_afirmar(pila_desapilar(NULL) == NULL,
		"NO puedo desapilar en pila NULL");
}

void agrego_o_quito_elementos_de_una_pila()
{
	pila_t *pila = pila_crear();
	char letras[] = { 'a', 'b', 'c', 'd', 'e'};
	for(int i = 0; i < 5; i++)
		pila_apilar(pila, letras + i);
	
	pa2m_afirmar(pila_tamanio(pila) == 5, "Se apilaron todos los elementos");
	
	char *l1 = pila_tope(pila);
	pa2m_afirmar(*l1 == 'e', "El tope de la pila { 'a', 'b', 'c', 'd', 'e'} apunta a 'e'");

	char *l2 = pila_desapilar(pila);
	pa2m_afirmar(*l2 == 'e', "Desapilar devuelve 'e'");

	char *l3 = pila_tope(pila);
	pa2m_afirmar(*l3 == 'd', "El tope de la pila { 'a', 'b', 'c', 'd'} apunta a 'd'");

	pila_destruir(pila);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de lista");

	pa2m_nuevo_grupo("Pruebas de crear lista");
	crear_una_lista_me_devuelve_una_lista_len_0();

	pa2m_nuevo_grupo("Pruebas de agregar o quitar elementos");
	agrego_o_quito_elementos_de_lista_NULL();
	creo_una_lista_con_elementos_y_elimino();

	pa2m_nuevo_grupo("Pruebas del iterador de lista");
	creo_una_lista_de_elementos_iterables();

	pa2m_nuevo_grupo("Pruebas de cola");
	pa2m_nuevo_grupo("Pruebas de crear cola");
	creo_una_cola_inicializada_en_0();
	pa2m_nuevo_grupo("Pruebas de agregar o quitar elementos");
	agrego_o_quito_elementos_de_una_cola_nula();
	agrego_o_quito_elementos_de_una_cola();

	pa2m_nuevo_grupo("Pruebas de pila");
	pa2m_nuevo_grupo("Pruebas de crear pila");
	creo_una_pila_inicializada_en_0();
	pa2m_nuevo_grupo("Pruebas de agregar o quitar elementos");
	agrego_o_quito_elementos_de_una_pila_nula();
	agrego_o_quito_elementos_de_una_pila();


	return pa2m_mostrar_reporte();
}
