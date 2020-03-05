#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *const DIR_ARQ = "arquivo";
const char *const MOD_ARQ = "w+";
const int MAX_POS = 10;

typedef struct{
	int chave;
	int valor;
}Registro;

int abreArquivo(FILE **arquivo){
  	*arquivo = fopen(DIR_ARQ, MOD_ARQ);
  	if(*arquivo == NULL){
  		printf("Erro na abertura do arquivo!");
  		return 0;
  	}else{
  		return 1;
  	}
}

void fechaArquivo(FILE **arquivo){
	fclose(*arquivo);
}

void menuInicial(){
	printf("\nEscolha uma opção válida\n");
	printf("0 - Fechar programa\n");
	printf("1 - Consultar\n");
	printf("2 - Inserir\n");
	printf("3 - Remover\n\n");
	printf("Digite a opção correspondente: ");
}

void imprimir(Registro registro){
    printf("\nChave: %d", registro.chave); 
    printf("\nValor: %d ", registro.valor); 
    printf("\n"); 
}

int calculaHash(Registro r){
	// Função para calcular a posição
	return r.chave - 1;
}

void cadastrar(Registro registro, FILE **arquivo){
    printf("Digite o código: "); 
    scanf("%d%*c", &registro.chave); 
    printf("Digite o valor: "); 
    scanf("%d%*c", &registro.valor);

    fseek(*arquivo, calculaHash(registro) * sizeof(registro), SEEK_SET); 
    if(!fwrite(&registro, sizeof(registro), 1, *arquivo))
    	printf("\nErro no cadastro\n");
}

void remover(Registro r, int deslocamento, FILE **arquivo){
	fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
	fread(&r, sizeof(r), 1, *arquivo);

	if(r.chave == -1 && r.valor == -1){
		printf("\nPosição vazia\n");
	}else{
		r.chave = -1;
		r.valor = -1;
		if(!fwrite(&r, sizeof(r), 1, *arquivo))
			printf("\nErro na remoção\n");
		else
			printf("\nRemovido %d posição\n", deslocamento+1);
	}
}

void consultar(Registro r, int deslocamento, FILE **arquivo){
	fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
	fread(&r, sizeof(r), 1, *arquivo);

	if(r.chave == -1 && r.valor == -1){
		printf("\nPosição vazia\n");
	}else{
		imprimir(r);
	}
}

void inicializa(FILE **arquivo){
	Registro registro;

	for(int i = 0; i < MAX_POS; i++){
		registro.chave = -1;
		registro.valor = -1;
		fwrite(&registro, sizeof(registro), 1, *arquivo);
	}
}

int main(void)
{
  	FILE *pont_arq;
  	Registro registro;
  	int opcao;

	if(abreArquivo(&pont_arq) == 0)
  		return 1;

  	inicializa(&pont_arq);

  	do{
  		menuInicial();
  		scanf("%d%*c",&opcao);
  		printf("\n");
  		switch(opcao){
        	case 0:{
        		break;
        	}
            case 1:{
            	int posicao = 1;
            	printf("Digite a posição para consulta: ");
            	scanf("%d%*c", &posicao);
            	consultar(registro, posicao-1, &pont_arq);
                break;
            }
            case 2:{
            	registro.chave = -1;
            	registro.valor = -1;
            	cadastrar(registro, &pont_arq);
                break;
            }
            case 3:{     
                int posicao = 1;
            	printf("Digite a posição para remoção: ");
            	scanf("%d%*c", &posicao);
            	remover(registro, posicao-1, &pont_arq);
                break;
            }  
            default:{
            	printf("Opção inválida\n");
            	break;
            }   
        }
  	}while(opcao != 0);
  	
  	printf("\n");
  	fechaArquivo(&pont_arq);
  	return(0);
}

