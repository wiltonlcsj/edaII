#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

typedef struct {
  int sucessoTotalAcessos;
  int sucessoTotalQuantidade;
  int falhaTotalAcessos;
  int falhaTotalQuantidade;
} Acessos;

void inicializa(FILE** arquivo) {
  Registro r;
  r.primeiravez = true;
  r.ocupado = false;

  fseek(*arquivo, sizeof(Registro), SEEK_SET);
  for (int i = 0; i < MAXNUMREGS; i++) {
    fwrite(&r, sizeof(Registro), 1, *arquivo);
  }
}

int abreArquivo(FILE** arquivo) {
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

void fechaArquivo(FILE** arquivo) {
  fclose(*arquivo);
}

int buscaChave(int chave, FILE** arquivo, Acessos* acessos, bool remocao) {
  int h1 = (chave % MAXNUMREGS) + 1;
  int v1 = (int)floor(chave / MAXNUMREGS) % MAXNUMREGS;
  int h2 = 1;
  if (v1 > h2) {
    h2 = v1;
  }

  int deslocamento = h1;
  fseek(*arquivo, deslocamento * sizeof(Registro), SEEK_SET);

  int loopsAcesso = 1;

  Registro r;
  fread(&r, sizeof(r), 1, *arquivo);

  if (r.ocupado == false && r.primeiravez == true) {
    if (remocao == false) {
      acessos->falhaTotalAcessos += loopsAcesso;
      acessos->falhaTotalQuantidade += 1;
    }
    return -1;
  } else if (r.ocupado == true && r.dado.chave == chave) {
    if (remocao == false) {
      acessos->sucessoTotalAcessos += loopsAcesso;
      acessos->sucessoTotalQuantidade += 1;
    }
    return deslocamento;
  }

  bool terminou = false;
  while (terminou != true) {
    loopsAcesso++;
    deslocamento += h2;

    if (deslocamento > MAXNUMREGS) {
      deslocamento -= MAXNUMREGS;
    }

    if (deslocamento == h1) {
      deslocamento = -1;
      if (remocao == false) {
        acessos->falhaTotalAcessos += loopsAcesso;
        acessos->falhaTotalQuantidade += 1;
      }
      terminou = true;
    }

    fseek(*arquivo, deslocamento * sizeof(Registro), SEEK_SET);
    fread(&r, sizeof(r), 1, *arquivo);

    if (r.ocupado == false && r.primeiravez == true) {
      if (remocao == false) {
        acessos->falhaTotalAcessos += loopsAcesso;
        acessos->falhaTotalQuantidade += 1;
      }
      deslocamento = -1;
      terminou = true;
    } else if (r.ocupado == true && r.dado.chave == chave) {
      terminou = true;
      if (remocao == false) {
        acessos->sucessoTotalAcessos += loopsAcesso;
        acessos->sucessoTotalQuantidade += 1;
      }
    }
  }

  return deslocamento;
}

int calculaHash(int chave, FILE** arquivo) {
  int h1 = (chave % MAXNUMREGS) + 1;
  int v1 = (int)floor(chave / MAXNUMREGS) % MAXNUMREGS;
  int h2 = 1;

  if (v1 > h2) {
    h2 = v1;
  }

  int deslocamento = h1;
  fseek(*arquivo, deslocamento * sizeof(Registro), SEEK_SET);

  Registro r;
  fread(&r, sizeof(r), 1, *arquivo);

  if (r.ocupado == false && r.primeiravez == true) {
    return deslocamento;
  } else if (r.ocupado == true && r.dado.chave == chave) {
    return -1;
  }

  bool terminou = false;
  while (terminou != true) {
    deslocamento += h2;

    if (deslocamento > MAXNUMREGS) {
      deslocamento -= MAXNUMREGS;
    }

    if (deslocamento == h1) {
      deslocamento = -2;
      terminou = true;
    }

    fseek(*arquivo, deslocamento * sizeof(Registro), SEEK_SET);
    fread(&r, sizeof(r), 1, *arquivo);

    if (r.ocupado == false && r.primeiravez == true) {
      terminou = true;
    } else if (r.ocupado == true && r.dado.chave == chave) {
      terminou = true;
      deslocamento = -1;
    }
  }

  return deslocamento;
}

void cadastrar(FILE** arquivo) {
  Registro r;
  scanf("%d%*c", &r.dado.chave);

  fgets(r.dado.nome, 20, stdin);
  //Limpando enter final do buffer
  size_t ln = strlen(r.dado.nome) - 1;
  if (r.dado.nome[ln] == '\n')
    r.dado.nome[ln] = '\0';

  scanf("%d%*c", &r.dado.idade);

  r.ocupado = true;
  r.primeiravez = false;

  int deslocamento = calculaHash(r.dado.chave, arquivo);

  if (deslocamento == -1) {
    printf("chave ja existente: %d\n", r.dado.chave);
    return;
  } else if (deslocamento == -2) {
    printf("insercao de chave sem sucesso - arquivo cheio: %d\n", r.dado.chave);
    return;
  }

  fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
  if (fwrite(&r, sizeof(r), 1, *arquivo))
    printf("insercao com sucesso: %d\n", r.dado.chave);
}

void imprimir(Registro registro) {
  printf("chave: %d", registro.dado.chave);
  printf("\n%s", registro.dado.nome);
  printf("\n%d", registro.dado.idade);
  printf("\n");
}

void consultar(FILE** arquivo, Acessos* acessos) {
  int chave;
  scanf("%d%*c", &chave);

  int deslocamento = buscaChave(chave, arquivo, acessos, false);

  if (deslocamento == -1) {
    printf("chave nao encontrada: %d\n", chave);
    return;
  }

  Registro r;
  fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
  fread(&r, sizeof(r), 1, *arquivo);
  imprimir(r);
}

void remover(FILE** arquivo) {
  int chave;
  scanf("%d%*c", &chave);

  int deslocamento = buscaChave(chave, arquivo, NULL, true);

  if (deslocamento == -1) {
    printf("chave nao encontrada: %d\n", chave);
    return;
  }

  Registro r;
  fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
  fread(&r, sizeof(r), 1, *arquivo);

  r.ocupado = false;
  fseek(*arquivo, deslocamento * sizeof(r), SEEK_SET);
  if (fwrite(&r, sizeof(r), 1, *arquivo))
    printf("chave removida com sucesso: %d\n", chave);
}

void imprimeArquivo(FILE** arquivo) {
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

void mediaAcessos(FILE** arquivo, Acessos acessos) {
  fseek(*arquivo, sizeof(Registro), SEEK_SET);

  float comSucesso = 0.0;
  float semSucesso = 0.0;

  if (acessos.sucessoTotalQuantidade > 0)
    comSucesso = (float)acessos.sucessoTotalAcessos / (float)acessos.sucessoTotalQuantidade;

  if (acessos.falhaTotalQuantidade > 0)
    semSucesso = (float)acessos.falhaTotalAcessos / (float)acessos.falhaTotalQuantidade;

  printf("%.1f\n", comSucesso);
  printf("%.1f\n", semSucesso);
}


int main(void) {
  FILE* pont_arq;
  char opcao;

  if (abreArquivo(&pont_arq) == 0) {
    printf("Erro na abertura do arquivo!");
    exit(-1);
  }

  Acessos qntAcessos;
  qntAcessos.falhaTotalAcessos = 0;
  qntAcessos.falhaTotalQuantidade = 0;
  qntAcessos.sucessoTotalAcessos = 0;
  qntAcessos.sucessoTotalQuantidade = 0;

  do {
    scanf("%c%*c", &opcao);
    switch (opcao) {
    case 'e': {
      break;
    }
    case 'c': {
      //Deve consultar a chave
      consultar(&pont_arq, &qntAcessos);
      break;
    }
    case 'i': {
      //Deve inserir o registro
      cadastrar(&pont_arq);
      break;
    }
    case 'r': {
      //Deve remover a chave
      remover(&pont_arq);
      break;
    }
    case 'p': {
      //Deve imprimir o arquivo
      imprimeArquivo(&pont_arq);
      break;
    }
    case 'm': {
      //Deve calcular a média de acessos
      mediaAcessos(&pont_arq, qntAcessos);
      break;
    }
    case 'x': {
      //Limpa os registros (remover depois)
      inicializa(&pont_arq);
      break;
    }
    default:
      break;
    }
  } while (opcao != 'e');
  fechaArquivo(&pont_arq);
  return (0);
}

