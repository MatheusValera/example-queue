#include<stdio.h>
#include<conio2.h>
#include <stdlib.h>
#include<string.h>
#define MAX 100

struct TpRegistro 
{
	char autores[MAX],titulo[MAX], editora[MAX];
	int pag, ano;
};typedef struct TpRegistro TpRegistro;

struct autor
{
    struct autor *prox;
    char nome[MAX],sobrenome[MAX];
};typedef struct autor Autor;

struct listaAutor
{
    Autor *autor;
    struct listaAutor *prox;
};typedef struct listaAutor Lista;

struct livro
{
    struct livro *prox,*ant;
    char titulo[MAX];
    int ano,paginas;
    Lista *autores;

};typedef struct livro Livro;

struct editora
{
    struct editora *prox;
    char nome[MAX];
    Livro *cabecaL;
};typedef struct editora Editora;

int Desmembrar(char autores[MAX],char matriz[MAX][MAX])
{
	int i,j=0,coluna=0,tam;
	tam = strlen(autores);
	for(i=0;i<tam;i++)
	{
		if(autores[i]!=',' && autores[i]!=';')
		{
			matriz[j][coluna]=autores[i];
			coluna++;
		}
		if(autores[i]==',' || autores[i] == ';')
		{
			matriz[j][coluna] = '\0';
			j++;
			coluna=0;
		}
	}
	return j;
}

Autor *BuscaA(Autor *autor,char nome[])
{
    Autor *aux;
    aux = autor;
    while(aux != NULL && stricmp(aux->nome,nome)!=0)
        aux = aux->prox;
    if(aux != NULL)
        return aux;
    return NULL;
}

void tirar_da_matriz(char Nome[MAX],char Sobrenome[MAX],char matriz[MAX][MAX],int linha)
{
	strcpy(Sobrenome,matriz[linha]);
	strcpy(Nome,matriz[linha+1]);
}

void InserirNaFila(Autor **Autores,char matriz[MAX][MAX],int linhas)
{
	Autor *Busca,*Elem;
	char nome[MAX],sobrenome[MAX];
	int i;
	for(i=0;i<linhas;i+=2)
	{
		nome[0]=sobrenome[0]='\0';
		tirar_da_matriz(nome,sobrenome,matriz,i);
		Busca=BuscaA(*Autores,nome);
		if(Busca==NULL)
		{
			Elem=(Autor*)malloc(sizeof(Autor));
			strcpy(Elem->nome,nome);
			strcpy(Elem->sobrenome,sobrenome);
			Elem->prox=NULL;
			if(*Autores==NULL)
				*Autores=Elem;
			else
			{
				Elem->prox=*Autores;
				*Autores=Elem;
			}
		}
	}
		
}

void InsereAutor(Lista **autor,char autores[],Autor **Autores)
{
	Autor *Busca,*Elem;
	Lista *Aux;
	int linhas,i;
	char matriz[MAX][MAX],nome[MAX], sobrenome[MAX];
	linhas = Desmembrar(autores,matriz);
	for(i=0;i<linhas;i+=2)
	{
		tirar_da_matriz(nome,sobrenome,matriz,i);
		Busca=BuscaA(*Autores,nome);
		if(Busca==NULL)
		{	
			Elem=(Autor*)malloc(sizeof(Autor));
			strcpy(Elem->nome,nome);
			strcpy(Elem->sobrenome,sobrenome);
			Elem->prox=NULL;
			if(*Autores==NULL)
				*Autores=Elem;
			else
			{
				Elem->prox=*Autores;
				*Autores=Elem;
			}
			Aux=(Lista*)malloc(sizeof(Lista));
			Aux->prox=NULL;
			Aux->autor=Elem;
			if(*autor==NULL)
				*autor=Aux;
			else
			{
				Aux->prox=*autor;
				*autor=Aux;
			}
		}
		else
		{
			Aux=(Lista*)malloc(sizeof(Lista));
			Aux->prox=NULL;
			Aux->autor=Busca;
			if(*autor==NULL)
				*autor=Aux;
			else
			{
				Aux->prox=*autor;
				*autor=Aux;
			}
		}		
	}
}

Livro *BuscaLivro(Livro *livro,char titulo[])
{
    Livro *Aux;
    Aux=livro;
    while(Aux!=NULL && stricmp(Aux->titulo,titulo)!=0)
        Aux=Aux->prox;
    if(Aux!=NULL)
        return Aux;
    return NULL;
}

void InserirLivro(Livro **livro,TpRegistro Elem,Autor **Autores)
{
    Livro *aux,*busca;
    busca = BuscaLivro(*livro,Elem.titulo);
    if(busca==NULL)
    {
        aux = (Livro *)malloc(sizeof(Livro));
        aux->prox = NULL;
        aux->ant = NULL;
        aux->paginas = Elem.pag;
        strcpy(aux->titulo,Elem.titulo);
        aux->ano = Elem.ano;
        aux->autores = NULL;
        if(*livro == NULL)
            *livro = aux;
        else
        {
            aux->prox = *livro;
            (*livro)->ant = aux;
            *livro = aux;
        }
        busca = aux;
    }
    InsereAutor(&busca->autores,Elem.autores,*&Autores);
}

Editora *BuscaE(Editora *pE,char nomeE[])
{
	Editora *aux;
	aux = pE;
	while(aux != NULL && stricmp(aux->nome,nomeE)!=0)
		aux = aux->prox;
	if(aux != NULL)
		return aux;
    return NULL;
}

void InserirEditora(Editora **Lista, TpRegistro Elem,Autor **Autores)
{
    Editora *Busca,*Elemento;
    Busca=BuscaE(*Lista,Elem.editora);
    if(Busca==NULL)
    {
        Elemento=(Editora*)malloc(sizeof(Editora));
        strcpy(Elemento->nome,Elem.editora);
        Elemento->prox=NULL;
        Elemento->cabecaL=NULL;
        if(*Lista==NULL)
            *Lista=Elemento;
        else
        {
            Elemento->prox = *Lista;
            *Lista = Elemento;
        }
        Busca=Elemento;
    }
    InserirLivro(&Busca->cabecaL,Elem,*&Autores);
}

void imprime()
{
	
	FILE *arqBin = fopen("arquivo.dat","rb");
     TpRegistro reg;
    fread(&reg,sizeof(TpRegistro),1,arqBin);
    while (!feof(arqBin)){
    	
    	printf("%s\n%s\n%s\t%d\t%d\n\n",reg.autores, reg.titulo, reg.editora, reg.ano, reg.pag );
        fread(&reg,sizeof(TpRegistro),1,arqBin);
    	
    }
    getch();
}

void LimpaMat(char matriz[MAX][MAX])
{
	int x,y;
	for(y=0;y<MAX;y++)
		for(x=0;x<MAX;x++)
			matriz[x][y] = '\0';
}

void gera_arq_bin(Autor **Autores)
{
	char matriz[MAX][MAX];
	int linhas;
	FILE *arq = fopen("livros.txt","r");
    TpRegistro reg;
    FILE *arqBin = fopen("arquivo.dat","wb");
    	fscanf(arq,"%[^|]|%[^|]|%[^|]|%d|%d\n", &reg.autores, &reg.titulo, &reg.editora, &reg.ano, &reg.pag);
    while(!feof(arq)){
    	LimpaMat(matriz);
    	linhas=Desmembrar(reg.autores,matriz);
    	InserirNaFila(*&Autores,matriz,linhas);
    	fwrite(&reg,sizeof(TpRegistro),1,arqBin);
    	fscanf(arq,"%[^|]|%[^|]|%[^|]|%d|%d\n", &reg.autores, &reg.titulo, &reg.editora, &reg.ano, &reg.pag);
    }
    linhas=Desmembrar(reg.autores,matriz);
    InserirNaFila(*&Autores,matriz,linhas);
    fclose(arq);
    fclose(arqBin);	
}

void gera_Lista(Editora **Cabeca,Autor **Autores)
{
	TpRegistro reg;
    FILE *arqBin = fopen("arquivo.dat","rb");
    	fread(&reg,sizeof(TpRegistro),1,arqBin);
    while(!feof(arqBin)){
		InserirEditora(*&Cabeca,reg,*&Autores);
    	fread(&reg,sizeof(TpRegistro),1,arqBin);
    };
    fclose(arqBin);
}

void exibeA(Autor *Autores)
{
	Autor *aux;
	aux = Autores;
	while(aux != NULL)
	{
		printf("\n%s %s",aux->nome,aux->sobrenome);
		aux=aux->prox;
	}
}

void excluiLivro(Editora **pEditora,char titulo[MAX])
{
	TpRegistro reg;
	Editora *aux = *pEditora;
	Livro *buscaL;
	Lista *auxA,*excA;
	FILE *excluir = fopen ("arquivo.dat","rb");
	FILE *novo = fopen("excluir.dat","wb");
	
	//Exclui arquivo
	fread(&reg,sizeof(TpRegistro),1,excluir);
	while(!feof(excluir))
	{
		if(stricmp(reg.titulo,titulo)!=0)
			fwrite(&reg,sizeof(TpRegistro),1,novo);
		fread(&reg,sizeof(TpRegistro),1,excluir);
	}
	fclose(novo);
	fclose(excluir);
	remove("arquivo.dat");
	rename("excluir.dat","arquivo.dat");
	
	//Exclui lista dinamica
	while(aux != NULL)
	{
		buscaL = BuscaLivro(aux->cabecaL,titulo);
		if(buscaL != NULL)
		{
			auxA = buscaL->autores;
			while(auxA != NULL)
			{
				excA = auxA;
				auxA = auxA->prox;
				free(excA);
			}
			if(buscaL->ant == NULL)
			{
				aux->cabecaL = buscaL->prox;
				if(buscaL->prox != NULL)
					aux->cabecaL->ant = NULL;
			}
			else
			{
				if(buscaL->prox == NULL)
					buscaL->ant->prox = NULL;
				else
				{
					buscaL->ant->prox = buscaL->prox;
					buscaL->prox->ant = buscaL->ant;
				}
			}
			free(buscaL);
		}
		aux = aux->prox;	
	}
}

void Relatorio1(Editora *E,Autor *A)
{
	Autor *Aux=A;
	Editora *AuxE;
	Lista *achou;
	Autor *blz;
	Livro *livro;
	int flag;
	textcolor(3);
	printf("                                          ***** RELATORIO 1 *****             \n");
	while(Aux!=NULL)
	{
		flag=0;
		AuxE=E;
		while(AuxE!=NULL)
		{
			livro=AuxE->cabecaL;
			while(livro!=NULL)
			{
				achou=livro->autores;
				while(achou!=NULL)
				{
					blz=achou->autor;
					if(blz==Aux)
					{
						printf("........................................................................................................................");
						printf("\n\n  Autor: %s\t Sobrenome: %s\n",Aux->nome,Aux->sobrenome);
						printf("\n\tLivro: %s\tPaginas: %d\tAno: %d",livro->titulo,livro->paginas,livro->ano);
						printf("\n\tEditora: %s",AuxE->nome);
						printf("\n\n........................................................................................................................\n\n");
					}
					achou=achou->prox;
				}
				livro=livro->prox;
			}
			AuxE=AuxE->prox;
		}
		Aux=Aux->prox;
	}
}

void Relatorio2(Editora *E)
{
	Editora *aux;
	Livro *Aux;
	Lista *A;
	aux=E;
	int i;
	textcolor(6);
	printf("                                          ***** RELATORIO 2 *****             \n");
	while(aux!=NULL)
	{
		printf("\n\t\t\t** Editora: %s **",aux->nome);
		Aux=aux->cabecaL;
		i=0;
		while(Aux!=NULL)
		{
			printf("\n\nLivro[%d]: %s \tPaginas: %d \tAno: %d\n",i+1,Aux->titulo,Aux->paginas,Aux->ano);
			A=Aux->autores;
			while(A!=NULL)
			{
				printf("\n\tAutor: %s,%s",A->autor->sobrenome,A->autor->nome);
				A=A->prox;
			}
			Aux=Aux->prox;
			i++;
		}
		aux=aux->prox;
		printf("\n\n.......................................................................................................................\n");
	}	
}

void Relatorio3(Editora *E,Autor *A)
{
	Autor *Aux=A;
	Editora *AuxE;
	Lista *achou;
	Autor *blz;
	Livro *livro;
	int flag;
	textcolor(15);
	printf("                                          ***** RELATORIO 3 *****             \n");
	while(Aux!=NULL)
	{
		flag=0;
		AuxE=E;
		while(AuxE!=NULL)
		{
			livro=AuxE->cabecaL;
			while(livro!=NULL)
			{
				achou=livro->autores;
				while(achou!=NULL)
				{
					blz=achou->autor;
					if(blz==Aux)
						flag++;
					achou=achou->prox;
				}
				livro=livro->prox;
			}
			AuxE=AuxE->prox;
		}
		if(flag == 0)
			printf("\nAutor que nao possui livro: %s, %s",Aux->sobrenome,Aux->nome);
		Aux=Aux->prox;
	}
}

int main(void)
{
	
	Editora *Cabeca=NULL;
	Autor *Autores=NULL;
	gera_arq_bin(&Autores);
	gera_Lista(&Cabeca,&Autores);
	excluiLivro(&Cabeca,"Algoritmos e estruturas de dados");
	excluiLivro(&Cabeca,"Estruturas de dados e seus algoritmos");
	Relatorio1(Cabeca,Autores);
	Relatorio2(Cabeca);
	Relatorio3(Cabeca,Autores);
	getch();
	return 1;
}

//
