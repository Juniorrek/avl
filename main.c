#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFERSIZE 100
#define TAMINIVET 100

typedef struct nodo {
	int valor;
	struct nodo *esq;
	struct nodo *dir;
	int altura;
} Nodo;

Nodo *inserir(Nodo *nodo, int valor);
Nodo *rotacaoEsq(Nodo *nodo);
Nodo *rotacaoDir(Nodo *nodo);
void imprimir(Nodo *raiz, int n);
void limparArvore(Nodo *raiz);
int altura(Nodo *N);
Nodo *remover(Nodo *raiz, int valor);
int getBalanceamento(Nodo *N);

void main() {
	Nodo *raiz = NULL;
	char buffer[BUFFERSIZE];

	//Le os dados do stdin e executa as operacoes
	while (fgets(buffer, BUFFERSIZE , stdin)) {
		char *operacao = strtok(buffer, " ");
		int valor = atoi(strtok(NULL, " "));

		if (strcmp(operacao, "i") == 0) {
			raiz = inserir(raiz, valor);
		} else {
			raiz = remover(raiz, valor);
		}
  	}
  	
  	imprimir(raiz, 0);

  	limparArvore(raiz);
}

Nodo *inserir(Nodo *nodo, int valor) {
	// Chegou num lugar vazio => insere
    if (nodo == NULL) {
    	Nodo *n = (Nodo*) malloc(sizeof(Nodo));
	    n->valor = valor;
	    n->esq = NULL;
	    n->dir = NULL;
	    n->altura = 1;

	    return n;
    }
 
    //Verifica se desce pra esquerda ou direita
    if (valor < nodo->valor)
        nodo->esq = inserir(nodo->esq, valor);
    else
        nodo->dir = inserir(nodo->dir, valor);
 
    // Atualiza altura do nodo
    if (altura(nodo->esq) > altura(nodo->dir))
    	nodo->altura = 1 + altura(nodo->esq);
    else
    	nodo->altura = 1 + altura(nodo->dir);
 
    // Fator de balanceamento para verificar possivel desequilibrio
    int balanceamento = altura(nodo->esq) - altura(nodo->dir);
 
    // Casos de desbalanceamento
    // Esq Esq
    if (balanceamento > 1 && valor < nodo->esq->valor)
        return rotacaoDir(nodo);
 
    // Dir Dir
    if (balanceamento < -1 && valor > nodo->dir->valor)
        return rotacaoEsq(nodo);
 
    // Esq Dir
    if (balanceamento > 1 && valor > nodo->esq->valor) {
        nodo->esq =  rotacaoEsq(nodo->esq);
        return rotacaoDir(nodo);
    }
 
    // Dir Esq
    if (balanceamento < -1 && valor < nodo->dir->valor) {
        nodo->dir = rotacaoDir(nodo->dir);
        return rotacaoEsq(nodo);
    }

    return nodo;
}

Nodo *rotacaoEsq(Nodo *nodo) {
    Nodo *dir = nodo->dir;
    Nodo *esq = dir->esq;
 
    // Rotaciona
    dir->esq = nodo;
    nodo->dir = esq;
 
    //  Atualiza alturas
    if (altura(nodo->esq) > altura(nodo->dir))
    	nodo->altura = 1 + altura(nodo->esq);
    else
    	nodo->altura = 1 + altura(nodo->dir);
    if (altura(dir->esq) > altura(dir->dir))
    	dir->altura = 1 + altura(dir->esq);
    else
    	dir->altura = 1 + altura(dir->dir);
 
    return dir;
}

Nodo *rotacaoDir(Nodo *nodo) {
    Nodo *esq = nodo->esq;
    Nodo *dir = esq->dir;
 
    // Rotaciona
    esq->dir = nodo;
    nodo->esq = dir;
 
    // Atualiza alturas
    if (altura(nodo->esq) > altura(nodo->dir))
    	nodo->altura = 1 + altura(nodo->esq);
    else
    	nodo->altura = 1 + altura(nodo->dir);
    if (altura(esq->esq) > altura(esq->dir))
    	esq->altura = 1 + altura(esq->esq);
    else
    	esq->altura = 1 + altura(esq->dir);
 
    return esq;
}

void imprimir(Nodo *raiz, int n) {
    if (raiz != NULL) {
        imprimir(raiz->esq, n + 1);
        printf("%d,%d\n", raiz->valor, n);
        imprimir(raiz->dir, n + 1);
    }
}

void limparArvore(Nodo *raiz) {
    if (raiz != NULL) {
        limparArvore(raiz->esq);
        limparArvore(raiz->dir);
        free(raiz);
        raiz = NULL;
    }
}

int altura(Nodo *N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

Nodo *remover(Nodo *raiz, int valor) {
    if (raiz == NULL)
        return raiz;
 
    // Verifica se desce pra esquerda ou pra direita
    if ( valor < raiz->valor )
        raiz->esq = remover(raiz->esq, valor);
    else if( valor > raiz->valor )
        raiz->dir = remover(raiz->dir, valor);
 
    // Achou o nodo a remover
    else {
        // 1 ou nenhum filho
        if((raiz->esq == NULL) || (raiz->dir == NULL)) {
            Nodo *aux = raiz->esq ? raiz->esq : raiz->dir;
 
            // Nenhum filho
            if (aux == NULL) {
                aux = raiz;
                raiz = NULL;
            } else // Um filho
             	*raiz = *aux;
            free(aux);
        } else {
        	Nodo *pre = raiz->esq;
 			// Pega o predecessor no InOrder
		    while (pre->dir != NULL)
		        pre = pre->dir;

            raiz->valor = pre->valor;
 
            raiz->esq = remover(raiz->esq, pre->valor);
        }
    }
 
    // Se tinha so 1 nodo
    if (raiz == NULL)
      return raiz;
 
    // Atualiza altura
  	if (altura(raiz->esq) > altura(raiz->dir))
    	raiz->altura = 1 + altura(raiz->esq);
    else
    	raiz->altura = 1 + altura(raiz->dir);
 
    // Fator de balanceamento para verificar possivel desequilibrio
    int balanceamento = altura(raiz->esq) - altura(raiz->dir);
 
    // Casos de desbalanceamento
    // esq esq
    if (balanceamento > 1 && getBalanceamento(raiz->esq) >= 0)
        return rotacaoDir(raiz);
 
    // esq dir
    if (balanceamento > 1 && getBalanceamento(raiz->esq) < 0) {
        raiz->esq =  rotacaoEsq(raiz->esq);
        return rotacaoDir(raiz);
    }
 
    // dir dir
    if (balanceamento < -1 && getBalanceamento(raiz->dir) <= 0)
        return rotacaoEsq(raiz);
 
    // dir esq
    if (balanceamento < -1 && getBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDir(raiz->dir);
        return rotacaoEsq(raiz);
    }
 
    return raiz;
}

int getBalanceamento(Nodo *N) {
    if (N == NULL)
        return 0;
    return altura(N->esq) - altura(N->dir);
}