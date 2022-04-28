#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

typedef struct{ /* estrutura dos items do cardapio */
    int coditem;
	char nome[31];
    float preco;
    char Status;
    } Item; 

typedef struct{ /* estrutura das informações dos garcons */
    int codgarcon;
	char nome[31];
	char Status;
	} Garcon; 

struct dadospedido{ /* estrutura de dados dos items do pedido */
	int coditem, qtditem;
};

typedef struct dadospedido DPED;

struct Node{ /* estrutura de lista encadeada dos items do pedido */
	DPED item;
	struct Node *prox;
}; 

typedef struct Node node;

struct dadosmesa{ /* estrutura de dados da mesa */
	int codmesa, codgar, totped;
	node listaped;
}; 

typedef struct dadosmesa DMESA;

struct NodeD{ /* estrutura da lista duplamente encadeada de mesas */
	DMESA mesa; 		
	struct NodeD *anterior, *proximo; 
};

typedef struct NodeD LISTAMESA; /* definição do tipo LISTAMESA a partir do tipo 'struct NodeD' */
typedef LISTAMESA *MESAPTR; /* definição do tipo *MESAPTR a partir do tipo LISTAMESA */

void listaItem(node *PEDIDO, Item Cardapio[]);
int notaFiscal(DMESA mesa, Item Cardapio[]);

//!!!!!!!!!!!!!!!!!!!!!FUNCOES DA LISTA DE MESAS!!!!!!!!!!!!!!!!!!!!!!!!!!!

/* insere um mesa no final da lista */
void insere_no_final(MESAPTR *inicio, DMESA mesa){
     
     MESAPTR novo, atual;
     
     novo = malloc(sizeof(LISTAMESA));
     
     if(novo == NULL)
         return;
     
     if(*inicio == NULL){
         novo->mesa = mesa;
         novo->proximo = novo->anterior = NULL;
         *inicio = novo;
     }
     else{
         atual = *inicio; /* atual aponta para o início */
         while(atual->proximo != NULL) /* enquanto o nó atual não for o último */
             atual = atual->proximo;    /* vai para o nó seguinte */
         
         novo->mesa = mesa;
         novo->anterior = atual;  /*o nó anterior ao novo é o nó atual */
         novo->proximo = NULL; /* não há próximo nó, pois o novo será o último */
         atual->proximo = novo; /* o nó atual aponta para o novo nó e não é mais o último */
     }
}   

MESAPTR buscaMesa(MESAPTR *inicio, DMESA mesa){
    
    MESAPTR atual;
	
	if(*inicio == NULL) 
        return NULL; /* retorna NULL caso a lista esteja vazia */
        
    atual = *inicio; /* o nó atual é o do início */
       
    while(atual != NULL && atual->mesa.codmesa != mesa.codmesa) /*se há nó e o mesa do mesmo não é o mesa que se quer retirar*/
        atual = atual->proximo; /*passa para o próximo*/
    
	return atual;  	
        
}



/* remove um nó escolhido pelo usuário */
int retiraMesa(MESAPTR *inicio, DMESA mesa, Item Cardapio[]){
    
    MESAPTR temporario, atual;
    char conf;
    if(*inicio == NULL) 
        return 0; /* retorna 0 caso a lista esteja vazia */
    
    atual = *inicio; /* o nó atual é o do início */

	while(atual != NULL && atual->mesa.codmesa != mesa.codmesa) /*se há nó e o mesa do mesmo não é o mesa que se quer retirar*/
       	atual = atual->proximo; /*passa para o próximo*/
   	
	if(atual == NULL)
       	return 0; /* retorna 0 se não encontrou o mesa */
    
    if(notaFiscal(atual->mesa, Cardapio)==1){

    	if((*inicio)->mesa.codmesa == mesa.codmesa) /* caso o no a ser retirado seja o do inicio */
        	*inicio = (*inicio)->proximo; /* o nó inicial passa a ser o próximo da lista */
       
    	else{
        	temporario = atual; /* temporario aponta para o nó encontrado */
		   	if(atual->anterior != NULL)/* caso haja um nó anterior ao enontrado*/
		       	atual->anterior->proximo = atual->proximo; /* faz o nó anterior apontar para o nó posterior ao encontrado */
		       
		   	if(atual->proximo != NULL) /* caso haja um nó posterior ao enontrado*/
		       	atual->proximo->anterior = atual->anterior; /* faz o nó posterior apontar para o nó anterior ao encontrado */
		        
		   	free(temporario);/* remove o nó encontrado */
		}
	}	
    return 1;
}

/* deleta um nó apartir do início da lista */
char elimina_no(MESAPTR *inicio){
    
    MESAPTR temporario;
    int c;
    
    temporario = *inicio; /* nó do início passa a ser temporário */
    c = (*inicio)->mesa.codmesa; /* pega o mesa que estava no nó */
    *inicio = (*inicio)->proximo; /* o nó seguinte passa a ser o do início */
    free(temporario); /* deleta o nó temporário */
    
    return c;
}

int esta_vazia(MESAPTR lista){
    return lista == NULL;
    
}

void lista_Mesa(MESAPTR lista){
     
     if(esta_vazia(lista))
         printf("\n\nA lista esta vazia.\n\n");
     else{
         printf("\nMesas Abertas:\n\n");
         while(lista != NULL){
             printf(" [ %d ]", lista->mesa.codmesa);
             lista = lista->proximo;
         }
     }
}

//!!!!!!!!!!!!!!!!!!!!!FUNCOES AUXILIARES !!!!!!!!!!!!!!!!!!!!!!!!!!!

void ler_Garcon( int cod )
{
	FILE * fp;
    fp = fopen( "Garcons.dat", "r+b");
    Garcon reg;
    rewind(fp);
    while(1){
        if(fread(&reg, sizeof(reg), 1, fp)!= 1)break; /*Sair do laço*/
        if(reg.Status=='*') continue; /*Passa ao próximo*/
        if( reg.codgarcon == cod )
        {
            printf("\nNome do garcon: %s\n\n", reg.nome);
			break;
        }
	}
    fclose(fp);
}


int tam_Cardapio()
{
    Item a;
	FILE * arq = NULL;
    int tam = 0;
    
	arq = fopen( "Cardapio.dat", "rb");
    while( fread( &a, sizeof(Item), 1, arq ) )
        tam++;
    fclose(arq);
    return tam;
}

void ler_Cardapio(Item Cardapio[])
{
    Item a;
	FILE * arq = NULL;
    int tam = 0;
    
	arq = fopen( "Cardapio.dat", "rb");
    
    while( fread( &a, sizeof(Item), 1, arq ) )
    {
        if(a.Status=='*') continue;
        Cardapio[tam]=a;
        tam++;
    }
    fclose(arq);
}

int vazia(node *PEDIDO)
{
	if(PEDIDO->prox == NULL)
		return 1;
 	else
  		return 0;
}

//!!!!!!!!!!!!!!!!!!!!!FUNCOES DA LISTA DE ITENS DO PEDIDO!!!!!!!!!!!!!!!!!!!!!!!!!!!

node *alocaItem()
{
	node *novo=(node *) malloc(sizeof(node));
	if(!novo){
		printf("Sem memoria disponivel!\n");
		exit(1);
 	}
	else{
  		printf("Codigo do item pedido ou 0 para retornar: "); scanf("%d", &novo->item.coditem);
		if (novo->item.coditem>0){
  		printf("Quantidade do item pedido: "); scanf("%d", &novo->item.qtditem);
    }
	return novo;
    }
}

Item buscaCoditem(int cod, Item Cardapio[])
{
	Item a;
	a=Cardapio[0];
	int i=0;
	int s = tam_Cardapio();
	while( a.coditem != cod && i<s){
  		i++;
  		a=Cardapio[i];
    }
    if(i==s)
    	a.coditem=0;
   	return a;
}

float calcTotal(node *PEDIDO, Item Cardapio[])
{
	 float total = 0;
	 Item a;
	 
	 node *tmp;
	 tmp = PEDIDO->prox;
	 while( tmp != NULL){
	  	a=buscaCoditem(tmp->item.coditem, Cardapio);
	  	total=total+a.preco*tmp->item.qtditem;
	  	tmp = tmp->prox;
	 	}
	 return total;
}

int insereItem(node *PEDIDO, Item Cardapio[])
{
	node *novo=alocaItem();
	novo->prox = NULL;

	if(novo->item.coditem==0 || novo->item.qtditem==0)
 		return 0;
 		
 	if(buscaCoditem(novo->item.coditem,Cardapio).coditem==0){
		printf("CODIGO INEXISTENTE ! Tecle ENTER para retornar\n");
		getch();
		return 0;
	}
	node *tmp;
 
	if(vazia(PEDIDO))
		PEDIDO->prox=novo;
	else{
		tmp = PEDIDO->prox;
		while( tmp != NULL && tmp->item.coditem!=novo->item.coditem){
		tmp = tmp->prox;
  	}	
  	if(tmp==NULL){	
   		node *tmp = PEDIDO->prox;
   		while(tmp->prox != NULL)
    		tmp = tmp->prox;
   		tmp->prox = novo;
   }
  	else{
		tmp->item.qtditem += novo->item.qtditem;
  	}
 	}
	return novo->item.coditem;
}

void listaItem(node *PEDIDO, Item Cardapio[])
{
	 int tam;
	 int i;
	 Item a;
	 float t;
	 
	 if(vazia(PEDIDO)){
		printf("Lista vazia!\n\n");
		return ;
	 }
	 
	 node *tmp;
	 tmp = PEDIDO->prox;
	
	 printf("Item Cod Descricao do Item         Qtd V.Unit Total R$\n");
	 tam = 1;
	 while( tmp != NULL){
	  	i=tmp->item.coditem;
	  	a=buscaCoditem(i, Cardapio);
	  	t=a.preco*tmp->item.qtditem;
	  	printf("%4d %3d %-25s %3d %6.2f %8.2f\n", tam, tmp->item.coditem, a.nome, tmp->item.qtditem, a.preco, t);
	  	tmp = tmp->prox;
	  	tam++;
	 	}
	 printf("\nTOTAL A PAGAR EM R$: %.2f\n", calcTotal(PEDIDO, Cardapio));
	 printf("\n\n");
}

int notaFiscal(DMESA mesa, Item Cardapio []){
	system("cls");
	printf("=======================================================\n");
	printf("                 Restaurante Zanconatto's              \n\n");
	printf("        ****     NOTA FISCAL ELETRONICA    ****        \n\n");
	printf("=======================================================\n");
	ler_Garcon(mesa.codgar);
	listaItem(&mesa.listaped, Cardapio);
	printf("Confirma o fechamento da mesa, [s] ou [n]? ");
	if(toupper(getch())=='S')
	{
		printf("SIM");
		getch();
		return 1;
	}
	else
	{
		printf("NAO");
		getch();
		return 0;	
	}
}

void liberaItens(node *PEDIDO)
{
	if(!vazia(PEDIDO)){
		node *proxNode,
		*atual;
  
  		atual = PEDIDO->prox;
  		while(atual != NULL){
   			proxNode = atual->prox;
   			free(atual);
   			atual = proxNode;
  		}
 	}
}

node *buscaItem(node *PEDIDO, int opt)
{
	int count, tam;
 	node *tmp;
 	tmp = PEDIDO->prox;
 	if (opt==0){
   		printf("Digite o codigo do item que deseja encontrar? ");
   		scanf("%d", &opt);
 	}
 
 	while( tmp != NULL && tmp->item.coditem!=opt ){
  		tmp = tmp->prox;
 	}
 	if(tmp!=NULL)  
   		printf("Codigo do item: %d\nQuantidade do item: %d\n", tmp->item.coditem, tmp->item.qtditem);
 	else
   		printf("Codigo nao encontrado\n");
 	getch();
 	return tmp;	
}

node *alteraItem(node *PEDIDO, int opt)
{
	int count, tam;
 	node *tmp;
 	tmp = PEDIDO->prox;
 	while( tmp != NULL && tmp->item.coditem!=opt ){
  		tmp = tmp->prox;
 	}
 	if(tmp!=NULL)  
 	{
	    printf("Digite a nova quantidade do item %d: ", tmp->item.coditem);
 		scanf("%d", &tmp->item.qtditem);
    }
	else{		
		printf("Codigo nao encontrado\n");
 		getch();
 	}
 	return tmp;	
}

node *retiraInicio(node *PEDIDO)
{
	if(PEDIDO->prox == NULL){
  		printf("Lista ja esta vazia\n");
  		return NULL;
 	}
	else{
  		node *tmp = PEDIDO->prox;
  		PEDIDO->prox = tmp->prox;
  		return tmp;
 	}
}

node *retiraItem(node *PEDIDO, int opt)
{
	int count, tam, pos;
	node *tmp;
	tmp = PEDIDO->prox;
	tam = 1;

	while( tmp != NULL){
		if(tmp->item.coditem==opt)
	  		pos=tam;
	  	tmp = tmp->prox;
	  	tam++;
	}
 
	if(pos>0 && pos<=tam){
  		if(pos==1)
   			return retiraInicio(PEDIDO);
	  	else{
	   		node *atual = PEDIDO->prox,
	     	*anterior=PEDIDO; 
	   		for(count=1 ; count < pos ; count++){
	    		anterior=atual;
	    		atual=atual->prox;
	   		}
	  		anterior->prox=atual->prox;
	  		return atual;
	  	} 
	}
	else{
  		printf("Elemento invalido\n\n");
  		return NULL;
 }
}

//!!!!!!!!!!!!!!!!!!!!!FUNCOES AUXILIARES DE CADASTRO !!!!!!!!!!!!!!!!!!!!!!!!!!!

void Mensagem(char *msg);

/*Colocar uma mensagem na tela*/
void Mensagem(char *msg){
    printf(msg);
    getchar();
}

/*Verificar se o arquivo já existe. Se não existir, ele é criado
se já existir, abre-o em modo de leitura e escrita (r+b)
*/
FILE *Inic(char *ARQ){
	FILE *fp;
    fp= fopen(ARQ, "r+b"); //tentar abrir
    if(fp==NULL){
        fp = fopen(ARQ, "w+b"); // criar o arquivo
        if(fp==NULL){
            printf(" Erro fatal: impossível criar arquivo de dados\n");
            exit(1);
        }
    }
    return fp;
}

/* Faz um menu simples com as opções do vetor de strings.
seleciona a opção, usando o primeiro caracter de cada string.
devolve o primeiro caracter da opção.
*/
char SubMenu(char *Titulo){
    int i;
    char ch;

    char *opcoes[]={
    "1. Inserir registro",
    "2. Alterar registro",
    "3. Apagar registro",
    "4. Listar registros",
    "0. Sair",
    NULL /* ACABARAM AS OPÇÕES*/
	};

    while(1){
        system("cls");/*CLS */
        printf("\n\n\t\t%s\n\n", Titulo);
            for(i=0; opcoes[i]!=NULL; i++)
                printf("\t\t%s\n\n",opcoes[i]);
               
            printf("\n\n\t\tOpcao: ");
            ch = getchar(); fflush(stdin);
            for(i=0; opcoes[i]!= NULL; i++)
                if(opcoes[i][0]==ch)
                    return ch;
    }
}

//!!!!!!!!!!!!!!!!!!!!!FUNCOES DE CADASTRO DE GARCON!!!!!!!!!!!!!!!!!!!!!!!!!!!

/* lê os dados de um registro introduzido pelo usuário*/
void Add_Garcon(Garcon *p){
    printf("Codigo:   "); scanf("%d",&p->codgarcon);
    fflush(stdin);
    printf("Nome:    "); gets(p->nome);
    p->Status = ' ';
    fflush(stdin);
}

/*Mostra na tela os dados existentes no registro*/
void Mostrar_Garcon(Garcon p){
    printf("%3d %-30s\n",p.codgarcon, p.nome);
}

/*Adiciona uma pessoa ao arquivo*/
void Adiciona_Garcon(FILE *fp, Garcon p){
    fseek(fp, 0L, SEEK_END);
    if(fwrite(&p, sizeof(p), 1, fp)!=1)
        Mensagem("Adicionar pessoa: Falhou a escrita do registro");
}




void Inserir_Garcon(FILE *fp){
    Garcon x;
    Add_Garcon(&x);
    Adiciona_Garcon(fp, x);
}

void Alterar_Garcon(FILE *fp){
    Garcon x;
    long int n_reg;
    printf("Qual o numero do registro: ");
    scanf("%ld", & n_reg); fflush(stdin);
    if(fseek(fp, (n_reg-1)*sizeof(Garcon), SEEK_SET)!=0){
        Mensagem("Registro inexistente ou problemas no posicionamento!!!");
        return;
    }
    if(fread(&x, sizeof(Garcon), 1, fp)!= 1){
        Mensagem("Problemas na leitura do registro!!!");
        return;
    }
   
    if(x.Status=='*'){
        Mensagem("Um registro apagado não pode ser alterado!!! \n\n");
        return;
    }
   
    printf("\n\n Dados Atuais \n\n");
    Mostrar_Garcon(x);
    printf("\n\n Novos dados \n\n");
    Add_Garcon(&x);
   
    /* recuar um registro no arquivo */
    fseek(fp, -(long) sizeof(Garcon), SEEK_CUR);
    /* reescrever o registro */
    fwrite(&x, sizeof(Garcon), 1, fp);
    fflush(fp); /*despejar os arquivos no disco rígido*/
}

void Apagar_Garcon(FILE *fp){
    Garcon x;
    long int n_reg;
    char resp;
   
    printf("Qual o numero do registro: ");
    scanf("%ld", & n_reg); fflush(stdin);
    if(fseek(fp, (n_reg - 1)*sizeof(Garcon), SEEK_SET)!= 0){
        Mensagem("Registro inexistente ou problemas no registro!!!");
        return;
    }
    if(fread(&x, sizeof(Garcon), 1, fp)!= 1){
        Mensagem("Problema na leitura do registro!!!");
        return;
    }
    if(x.Status=='*'){
        Mensagem("Registro já está apagado!!!\n\n");
    }
    printf("\n\n Dados atuais \n\n");
    Mostrar_Garcon(x);
    printf("\n\n Apagar o registro (s/n)???: "); resp = getchar();
    fflush(stdin);
    if(toupper(resp)!= 'S')return;
   
    x.Status= '*';
    /* recuar um registro no arquivo */
    fseek(fp, -(long) sizeof(Garcon), SEEK_CUR);
    /* reescrever o registro */
    fwrite(&x, sizeof(Garcon), 1, fp);
    fflush(fp); /*Despejar os arquivos no disco rígido*/
}

void Listar_Garcon(FILE *fp){
   
    long int n_Linhas = 0;
    Garcon reg;
    rewind(fp);
    while(1){
        if(fread(&reg, sizeof(reg), 1, fp)!= 1)break; /*Sair do laço*/
        if(reg.Status=='*') continue; /*Passa ao próximo*/
        Mostrar_Garcon(reg);
        n_Linhas++;
        if(n_Linhas%20==0)
            Mensagem("Pressione <Enter> para continuar .  .  .");
    }
    Mensagem("\n\n Pressione <Enter> para continuar .  .  ."); /*No fim da listagem*/
}

//!!!!!!!!!!!!!!!!!!!!!FUNCOES DE CADASTRO DE CARDAPIO!!!!!!!!!!!!!!!!!!!!!!!!!!!

void Add_Cardapio(Item *p){
	printf("Codigo: "); scanf("%d",&p->coditem);
	fflush(stdin);
    printf("Nome:    "); gets(p->nome);
    fflush(stdin);
    printf("Preco: "); scanf("%f",&p->preco);
    p->Status=' ';
    fflush(stdin);
}

/*Mostra na tela os dados existentes no registro*/
void Mostrar_Cardapio(Item p){
    printf("%3d %-30s %10.2f \n",p.coditem, p.nome, p.preco);
}

/*Adiciona uma pessoa ao arquivo*/
void Adiciona_Cardapio(FILE *fp, Item p){
    fseek(fp, 0L, SEEK_END);
    if(fwrite(&p, sizeof(p), 1, fp)!=1)
        Mensagem("Falhou a escrita do registro");
}

void Inserir_Cardapio(FILE *fp){
    Item x;
    Add_Cardapio(&x);
    Adiciona_Cardapio(fp, x);
}

void Alterar_Cardapio(FILE *fp){
    Item x;
    long int n_reg;
    printf("Qual o numero do registro: ");
    scanf("%ld", & n_reg); fflush(stdin);
    if(fseek(fp, (n_reg-1)*sizeof(Item), SEEK_SET)!=0){
        Mensagem("Registro inexistente ou problemas no posicionamento!!!");
        return;
    }
    if(fread(&x, sizeof(Item), 1, fp)!= 1){
        Mensagem("Problemas na leitura do registro!!!");
        return;
    }
   
    if(x.Status=='*'){
        Mensagem("Um registro apagado não pode ser alterado!!! \n\n");
        return;
    }
   
    printf("\n\n Dados Atuais \n\n");
    Mostrar_Cardapio(x);
    printf("\n\n Novos dados \n\n");
    Add_Cardapio(&x);
   
    // recuar um registro no arquivo
    fseek(fp, -(long) sizeof(Item), SEEK_CUR);
    // reescrever o registro;
    fwrite(&x, sizeof(Item), 1, fp);
    fflush(fp); /*despejar os arquivos no disco rígido*/
}

void Apagar_Cardapio(FILE *fp){
    Item x;
    long int n_reg;
    char resp;
   
    printf("Qual o numero do registro: ");
    scanf("%ld", & n_reg); fflush(stdin);
    if(fseek(fp, (n_reg - 1)*sizeof(Item), SEEK_SET)!= 0){
        Mensagem("Registro inexistente ou problemas no registro!!!");
        return;
    }
    if(fread(&x, sizeof(Item), 1, fp)!= 1){
        Mensagem("Problema na leitura do registro!!!");
        return;
    }
    if(x.Status=='*'){
        Mensagem("Registro já está apagado!!!\n\n");
    }
    printf("\n\n Dados atuais \n\n");
    Mostrar_Cardapio(x);
    printf("\n\n Apagar o registro (s/n)???: "); resp = getchar();
    fflush(stdin);
    if(toupper(resp)!= 'S')return;
   
    x.Status= '*';
    // recuar um registro no arquivo
    fseek(fp, -(long) sizeof(Item), SEEK_CUR);
    // reescrever o registro;
    fwrite(&x, sizeof(Item), 1, fp);
    fflush(fp); /*Despejar os arquivos no disco rígido*/
}

void Listar_Cardapio(FILE *fp){
   
    long int n_Linhas = 0;
    Item reg;
    rewind(fp);
    while(1){
        if(fread(&reg, sizeof(reg), 1, fp)!= 1)break; /*Sair do laço*/
        if(reg.Status=='*') continue; /*Passa ao próximo*/
        Mostrar_Cardapio(reg);
        n_Linhas++;
        if(n_Linhas%20==0)
            Mensagem("Pressione <Enter> para continuar .  .  .");
    }
    Mensagem("\n\n Pressione <Enter> para continuar .  .  ."); /*No fim da listagem*/
}


void menu(){
     system("cls");
     printf("\t <------> RESTAURANTE ZACONATTO's <------>\n");
     printf("\n\t\t\t  OPCOES\n\n");
     printf("\t1 - Abrir uma mesa\n");
     printf("\t2 - Adicionar pedidos a uma mesa\n");
     printf("\t3 - Listar pedidos de uma mesa\n");
	 printf("\t4 - Fechar a conta da mesa\n");
     printf("\t5 - Listar as mesas abertas\n");
     printf("\t6 - Cadastro de Garcons\n");
     printf("\t7 - Cadastro de Cardapio\n");
	 printf("\t0 - sair\n\n");
     
}

int main(){
    
    MESAPTR lista = NULL;
    MESAPTR mesaAtual = NULL;
	int opcao;
	int cped;
	int optm;
	int i=0;
	char sopcao;
	FILE *fp;
    DMESA mesa;
	Item Cardapio[tam_Cardapio()];
	ler_Cardapio(Cardapio);
	

    menu();
    printf("Escolha uma das opcoes: ");
    scanf("%d", &opcao);
    
    while(opcao != 0){
        
        switch(opcao){

            case 1: //abrir a mesa
                 printf("\nDigite o numero da mesa: ");
                 scanf("\n%d", &mesa.codmesa);
                 printf("\nDigite o codigo do Garcon: ");
                 scanf("\n%d", &mesa.codgar);
                 mesaAtual=buscaMesa(&lista, mesa);
                 if (mesaAtual != NULL){
                 	printf("\nEsta mesa ja esta aberta!\nTecle ENTER para retornar");
                 	getch();
				 }
				 else{
				 	mesa.totped=0;
				 	mesa.listaped.prox = NULL;
                 	insere_no_final(&lista, mesa);
             	 }
				 break;
			
			case 2: //add pedido
                 printf("\nDigite o numero da mesa: ");
                 scanf("\n%d", &mesa.codmesa);
    			 mesaAtual=buscaMesa(&lista, mesa);
    			 if (mesaAtual != NULL){
					 while (insereItem(&mesaAtual->mesa.listaped,Cardapio) != 0){
					 };	
					 mesaAtual->mesa.totped=calcTotal(&mesaAtual->mesa.listaped, Cardapio);
				}
				 else
                 	printf("\nMesa nao encontrada");
				 break;
            
            case 3: //listar pedido
                 printf("\nDigite o numero da mesa: ");
                 scanf("\n%d", &mesa.codmesa);
    			 mesaAtual=buscaMesa(&lista, mesa);
    			 if (mesaAtual != NULL){
    			 	 system("cls");
    			 	 listaItem(&mesaAtual->mesa.listaped, Cardapio);
					 optm=1;
					 cped=1;	
   					 while(optm != 0 && cped!=0){
        				printf("Informe o codigo do item para editar ou 0 para sair: ");
					 	scanf("%d", &cped);
        				if(cped!=0 && optm!=0){
					 		printf("1 - Alterar   2 - Remover   0 - Sair :");
							scanf("%d", &optm);
						
							switch(optm){

            					case 1:	//altera
									alteraItem(&mesaAtual->mesa.listaped, cped);
									mesaAtual->mesa.totped=calcTotal(&mesaAtual->mesa.listaped, Cardapio);
									break;								
								case 2: //retira
									retiraItem(&mesaAtual->mesa.listaped, cped);
									mesaAtual->mesa.totped=calcTotal(&mesaAtual->mesa.listaped, Cardapio);
									break;
								case 0:
									break;	
															 	
					 		}
					 	}
					 }
				}
				 else{
                 	printf("\nMesa nao encontrada");
                 	getch();
                 }
				 break;
            
            case 4: //fechar a mesa
                 printf("\nDigite a mesa a ser fechada: ");
                 scanf("\n%d", &mesa.codmesa);
                 if(! retiraMesa(&lista, mesa, Cardapio)){
                     printf("\nMesa inexistente!\n\n");
                     getch();   
                 }
                 break;
            
            case 5: //lista mesa
                 lista_Mesa(lista);
        		 printf("\n\nInforme o codigo da mesa para editar ou 0 para sair: ");
				 scanf("%d", &mesa.codmesa);
				 if (mesa.codmesa!=0){
            	 	mesaAtual=buscaMesa(&lista, mesa);
            	 	if (mesaAtual != NULL){
            	 		printf("Informe o novo codigo da mesa: ");
            	 		scanf("%d", &mesaAtual->mesa.codmesa);
            	 		printf("Informe o novo codigo do garcon: ");
				 		scanf("%d", &mesaAtual->mesa.codgar);
				  	}
					else{
						printf("\nMesa nao encontrada !!!");
                 		getch();
             		}
			 	 }
				 break;								
				
			case 6: //cadastro de garcon
			    fp=Inic("Garcons.dat");
			    while((sopcao = SubMenu("Cadastro de Garcons"))!= '0')
			        switch(sopcao){
			            case '1': Inserir_Garcon(fp); break;
			            case '2': Alterar_Garcon(fp); break;
			            case '3': Apagar_Garcon(fp); break;
			            case '4': Listar_Garcon(fp); break;
			        }
				break;
			
			case 7: //cadastro de cardapio
				fp=Inic("Cardapio.dat");
				while((sopcao = SubMenu("Cadastro de Cardapio"))!= '0')
			        switch(sopcao){
			            case '1': Inserir_Cardapio(fp); break;
			            case '2': Alterar_Cardapio(fp); break;
			            case '3': Apagar_Cardapio(fp); break;
			            case '4': Listar_Cardapio(fp); break;
			        }
				break;
				
            default:
                 printf("\nOpcao invalida!!!\n\n");
                 getch();
                 break;
        }
        menu();
        printf("Escolha uma das opcoes: ");
        scanf("%d", &opcao);
    }
    printf("\nLimpando lista ...\n\n");
    while(! esta_vazia(lista)){ /* retira todos os nós não apagados para liberar a memória */
        printf("Mesa %d retirada\n", elimina_no(&lista));
    }
    printf("\nLista vazia\n");
    
    getch();
    
    return 0;
}
