#include <stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <string.h>

const int MAXNUMREGS = 11;

typedef struct {
    int chave;
    char nome[20];
    int idade;
} Dado;

typedef struct {
    bool ocupado;
    bool primeiravez;
    Dado dado;
} Registro;

void inicializa(FILE **arquivo) {
  Registro r;
  r.primeiravez = true;
  r.ocupado = false;

  for (int i = 0; i < MAXNUMREGS; i++) {
    fwrite(&r, sizeof(Registro), 1, *arquivo);
  }
}

int abreArquivo(FILE **arquivo) {
  *arquivo = fopen("arquivo", "r+");
  if (*arquivo == NULL) {
    *arquivo = fopen("arquivo", "w+");
    if (*arquivo == NULL) {
      return 0;
    } else {
      inicializa(arquivo);
      return 1;
    }
  }

  return 1;
}

void fechaArquivo(FILE **arquivo) {
  fclose(*arquivo);
}

void menuInicial() {
  printf("\nEscolha uma opção válida\n");
  printf("e - Fechar programa\n");
  printf("c - Consultar\n");
  printf("i - Inserir\n");
  printf("r - Remover\n");
  printf("p - Imprimir arquivo\n");
  printf("m - Média de acessos\n");
  printf("Digite a opção correspondente:");
}

int calculaHash(Registro r) {
  //h1 = chave % MAXNUMREGS
  //h2 = max(floor(chave/MAXNUMREGS) % MAXNUMREGS, 1)
  // Função para calcular a posição
  return r.dado.chave - 1;
}

void cadastrar(Registro registro, FILE **arquivo) {
  scanf("%d%*c", &registro.dado.chave);

  fgets(registro.dado.nome, 20, stdin);
  //Limpando enter final do buffer
  size_t ln = strlen(registro.dado.nome) - 1;
  if (registro.dado.nome[ln] == '\n')
    registro.dado.nome[ln] = '\0';

  scanf("%d%*c", &registro.dado.idade);

  registro.ocupado = true;
  registro.primeiravez = false;

  //TODO verificar se a chave já está no arquivo, se sim retornar "chave ja existente: r.dado.chave"
  //TODO se salvar corretamente deve retornar "insercao com sucesso: r.dado.chave"
  //TODO caso o arquivo esteja cheio deve retornar: "insercao de chave sem sucesso - arquivo cheio: r.dado.chave"

  fseek(*arquivo, calculaHash(registro) * sizeof(registro), SEEK_SET);
  if (!fwrite(&registro, sizeof(registro), 1, *arquivo))
    printf("Erro no cadastro\n");
}

void imprimir(Registro registro) {
  printf("\nchave: %d", registro.dado.chave);
  printf("\nnome: %s", registro.dado.nome);
  printf("\nidade: %d", registro.dado.idade);
  printf("\nprimeira: %d", registro.primeiravez);
  printf("\nocupado: %d", registro.ocupado);
  printf("\n");
}

void consultar(Registro r, int chave, int deslocamento, FILE **arquivo) {
  fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
  fread(&r, sizeof(r), 1, *arquivo);

  if (!r.ocupado) {
    printf("chave nao encontrada: %d\n", chave);
  } else {
    imprimir(r);
  }
}


void remover(Registro r, int chave, int deslocamento, FILE **arquivo) {
  fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
  r.ocupado = false;

  //TODO verificar se a chave não estiver no arquivo gerar "chave nao encontrada: r.dado.chave"

  if (!fwrite(&r, sizeof(r), 1, *arquivo))
    printf("Erro na remoção\n");
  else
    printf("chave removida com sucesso: %d\n", chave);
}

void imprimeArquivo(FILE **arquivo) {
  Registro r;
  fseek(*arquivo, sizeof(r), SEEK_SET);

  for (int i = 0; i < MAXNUMREGS; i++) {
    fread(&r, sizeof(Registro), 1, *arquivo);
    printf("%d: ", i);

    if (r.ocupado == false) {
      if (r.primeiravez == false) {
        printf("*\n");
        continue;
      }

      printf("vazio\n");
      continue;
    }

    printf("%d %s %d\n", r.dado.chave, r.dado.nome, r.dado.idade);
  }
}

void mediaAcessos(FILE **arquivo) {
  //TODO calcular o numero médio de acessos a consultas com e sem sucesso;
  float comSucesso = 0;
  float semSucesso = 0;

  printf("%.1f\n", comSucesso);
  printf("%.1f\n", semSucesso);
}


int main(void) {
  FILE *pont_arq;
  Registro registro;
  char opcao;

  if (abreArquivo(&pont_arq) == 0) {
    printf("Erro na abertura do arquivo!");
    exit(-1);
  }

  do {
    menuInicial();
    scanf("%c", &opcao);
    switch (opcao) {
      case 'e': {
        break;
      }
      case 'c': {
        //Deve consultar a chave
        int chave = 1;
        printf("Digite a chave para consulta: ");
        scanf("%d%*c", &chave);
        consultar(registro, chave, chave - 1, &pont_arq);
        break;
      }
      case 'i': {
        //Deve inserir o registro
        cadastrar(registro, &pont_arq);
        break;
      }
      case 'r': {
        //Deve remover a chave
        int chave = 1;
        printf("Digite a chave para remoção: ");
        scanf("%d%*c", &chave);
        remover(registro, chave, chave - 1, &pont_arq);
        break;
      }
      case 'p': {
        //Deve imprimir o arquivo
        imprimeArquivo(&pont_arq);
        break;
      }
      case 'm': {
        //Deve calcular a média de acessos
        mediaAcessos(&pont_arq);
        break;
      }
      default:
        break;
    }
  } while (opcao != 'e');
  fechaArquivo(&pont_arq);
  return (0);
}

