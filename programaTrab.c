#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Struct do cabeçalho do arquivo binário
typedef struct {
    char status;
    long int topo;
    long int proxByteOffset;
    int nroRegArq;
    int nroRegRem;
    char descreveIdentificador[23];
    char descreveYear[27];
    char descreveFinancialLoss[28];
    char codDescreveCountry;
    char descreveCountry[26];
    char codDescreveType;
    char descreveType[38];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[38];
    char codDescreveDefense;
    char descreveDefense[67];
} Cabecalho;

// Struct do registro de dados
typedef struct {
    // Campos de controle
    char removido;
    int tamanhoRegistro;
    int prox;

    // Campos de tamanho fixo
    int idAttack;
    int year;
    int financialLoss;

    // Campos de tamanho variável
    char *country;
    char *attackType;
    char *TargetIndustry;
    char *defenseMechanism;
} Registro;

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

// Função da funcionalidade 1 (escreve o arquivo csv no arquivo binário seguindo as especificações)
void funcionalidade1(char nomeCSV[], char nomeBin[]) {
    // Abre os arquivos csv e binário
    FILE *csv = fopen(nomeCSV, "r");
    FILE *bin = fopen(nomeBin, "wb");

    // Verifica se os arquivos foram abertos corretamente
    if(!csv || !bin) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

        // Cria o cabeçalho
        Cabecalho cabecalho;

        // Inicializa os campos do cabeçalho
        cabecalho.status = '0';
        cabecalho.topo = -1;
        cabecalho.proxByteOffset = 0;
        cabecalho.nroRegArq = 0;
        cabecalho.nroRegRem = 0;
        memcpy(cabecalho.descreveIdentificador, "IDENTIFICADOR DO ATAQUE", 23);
        memcpy(cabecalho.descreveYear, "ANO EM QUE O ATAQUE OCORREU", 27);
        memcpy(cabecalho.descreveFinancialLoss, "PREJUIZO CAUSADO PELO ATAQUE", 28);
        cabecalho.codDescreveCountry = '1';
        memcpy(cabecalho.descreveCountry, "PAIS ONDE OCORREU O ATAQUE", 26);
        cabecalho.codDescreveType = '2';
        memcpy(cabecalho.descreveType, "TIPO DE AMEACA A SEGURANCA CIBERNETICA", 38);
        cabecalho.codDescreveTargetIndustry = '3';
        memcpy(cabecalho.descreveTargetIndustry, "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE", 38);
        cabecalho.codDescreveDefense = '4';
        memcpy(cabecalho.descreveDefense, "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA", 67);
    
        // Escreve o cabeçalho no arquivo binário
        fwrite(&cabecalho.status, sizeof(char), 1, bin);
        fwrite(&cabecalho.topo, sizeof(long int), 1, bin);
        fwrite(&cabecalho.proxByteOffset, sizeof(long int), 1, bin);
        fwrite(&cabecalho.nroRegArq, sizeof(int), 1, bin);
        fwrite(&cabecalho.nroRegRem, sizeof(int), 1, bin);
        fwrite(cabecalho.descreveIdentificador, sizeof(char), 23, bin);
        fwrite(cabecalho.descreveYear, sizeof(char), 27, bin);
        fwrite(cabecalho.descreveFinancialLoss, sizeof(char), 28, bin);
        fwrite(&cabecalho.codDescreveCountry, sizeof(char), 1, bin);
        fwrite(cabecalho.descreveCountry, sizeof(char), 26, bin);
        fwrite(&cabecalho.codDescreveType, sizeof(char), 1, bin);
        fwrite(cabecalho.descreveType, sizeof(char), 38, bin);
        fwrite(&cabecalho.codDescreveTargetIndustry, sizeof(char), 1, bin);
        fwrite(cabecalho.descreveTargetIndustry, sizeof(char), 38, bin);
        fwrite(&cabecalho.codDescreveDefense, sizeof(char), 1, bin);
        fwrite(cabecalho.descreveDefense, sizeof(char), 67, bin);

    char linha[1024];
    while(fgets(linha, sizeof(linha), csv)) {
        Registro registro;
        registro.removido = '0';
        registro.prox = -1;

        registro.country = NULL;
        registro.attackType = NULL;
        registro.TargetIndustry = NULL;
        registro.defenseMechanism = NULL;

        char *campos = strtok(linha, ",");
        int campoAtual = 0;

        while (campos != NULL) {
            switch (campoAtual) {
                case 0:
                    registro.idAttack = (strcmp(campos, "") == 0) ? -1 : atoi(campos);
                    break;
                case 1:
                    registro.year = (strcmp(campos, "") == 0) ? -1 : atoi(campos);
                    break;
                case 2:
                    registro.financialLoss = (strcmp(campos, "") == 0) ? -1 : atoi(campos);
                    break;
                case 3:
                    if(strcmp(campos, "") == 0) {
                        registro.country = NULL;
                    } else {
                        registro.country = malloc(strlen(campos) + 1);
                        strcpy(registro.country, campos);
                    }
                    break;
                case 4:
                    if(strcmp(campos, "") == 0) {
                        registro.attackType = NULL;
                    } else {
                        registro.attackType = malloc(strlen(campos) + 1);
                        strcpy(registro.attackType, campos);
                    }
                    break;
                case 5:
                    if(strcmp(campos, "") == 0) {
                        registro.TargetIndustry = NULL;
                    } else {
                        registro.TargetIndustry = malloc(strlen(campos) + 1);
                        strcpy(registro.TargetIndustry, campos);
                    }
                    break;  
                case 6:
                    if(strcmp(campos, "") == 0) {
                        registro.defenseMechanism = NULL;
                    } else {
                        registro.defenseMechanism = malloc(strlen(campos) + 1);
                        strcpy(registro.defenseMechanism, campos);
                    }
                    break;                 
            }
            campos = strtok(NULL, ",");
            campoAtual++;
        }

        int tamanhoCampoVariavel = 0;
        tamanhoCampoVariavel += (registro.country != NULL) ? strlen(registro.country) : 0;
        tamanhoCampoVariavel += (registro.attackType != NULL) ? strlen(registro.attackType) : 0;
        tamanhoCampoVariavel += (registro.TargetIndustry != NULL) ? strlen(registro.TargetIndustry) : 0;
        tamanhoCampoVariavel += (registro.defenseMechanism != NULL) ? strlen(registro.defenseMechanism) : 0;
        registro.tamanhoRegistro = sizeof(registro.removido) + sizeof(registro.tamanhoRegistro) + sizeof(registro.idAttack) + sizeof(registro.year) + sizeof(registro.financialLoss) + tamanhoCampoVariavel;
        cabecalho.nroRegArq++;

        fwrite(&registro.removido, sizeof(char), 1, bin);
        fwrite(&registro.tamanhoRegistro, sizeof(int), 1, bin);
        fwrite(&registro.prox, sizeof(int), 1, bin);
        fwrite(&registro.idAttack, sizeof(int), 1, bin);
        fwrite(&registro.year, sizeof(int), 1, bin);
        fwrite(&registro.financialLoss, sizeof(int), 1, bin);
        fwrite(registro.country, sizeof(char), strlen(registro.country), bin);
        fwrite(registro.attackType, sizeof(char), strlen(registro.attackType), bin);
        fwrite(registro.TargetIndustry, sizeof(char), strlen(registro.TargetIndustry), bin);
        fwrite(registro.defenseMechanism, sizeof(char), strlen(registro.defenseMechanism), bin);

        free(registro.country);
        free(registro.attackType);
        free(registro.TargetIndustry);
        free(registro.defenseMechanism);
    }

    fseek(bin, 0, 0);
    cabecalho.status = '1'; // Atualiza o status para consistente
    fwrite(&cabecalho.status, sizeof(char), 1, bin);
    fwrite(&cabecalho.topo, sizeof(long int), 1, bin);
    fwrite(&cabecalho.proxByteOffset, sizeof(long int), 1, bin);
    fwrite(&cabecalho.nroRegArq, sizeof(int), 1, bin);
    fwrite(&cabecalho.nroRegRem, sizeof(int), 1, bin);

    fclose(csv);
    fclose(bin);

    binarioNaTela(nomeBin);
}

int main() {
    int func;
    scanf("%d", &func);

    if(func == 1) {
        char nomeCSV[100], nomeBin[100];
        scanf("%s %s", nomeCSV, nomeBin);
        funcionalidade1(nomeCSV, nomeBin);
    }

    return 0;
}
