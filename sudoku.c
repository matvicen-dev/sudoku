/*
UNIVERSIDADE FEDERAL DA GRANDE DOURADOS (UFGD)
FACULDADE DE CIENCIAS EXATAS E TECNOLOGIA (FACET)
LABORATORIO DE PROGRAMACAO II

SUDOKU

GABRIEL TRES CEOLIN, MATEUS ALEM VICENTE
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ERROR_FILE_MSG "Nao foi possivel abrir o arquivo!\n"
#define INVALID_OPTION "Opcao invalida! Tente novamente!"

enum boolean
{
	FALSO = 0,
	VERDADEIRO = 1
};

/* Prototipos */
FILE *carregar(char quadro[9][9]);
FILE *carregar_continuar_jogo(char quadro[9][9], char *nome_arquivo);
void carregar_novo_jogo(char quadro[9][9], char *nome_arquivo);
int determinar_quadrante(int x, int y);
FILE *criar_arquivo_binario(char quadro[9][9]);
int e_valido(const char quadro[9][9], int x, int y, int valor);
int e_valido_na_coluna(const char quadro[9][9], int y, int valor);
int e_valido_no_quadro3x3(const char quadro[9][9], int x, int y, int valor);
int e_valido_na_linha(const char quadro[9][9], int x, int valor);
int existe_campo_vazio(const char quadro[9][9]);
void imprimir(const char quadro[9][9]);
void jogar();
void resolver(FILE *, char quadro[9][9]);
void resolver_um_passo(char quadro[9][9]);
void salvar_jogada_bin(FILE *fb, char quadro[9][9]);

/* Funcoes auxiliares */
int fim_x(int quadr);
int fim_y(int quadr);
int le_opcao();
void gen_random(char *s, const int len);
int ini_x(int quadr);
int ini_y(int quadr);
void menu();
void menu_arquivo();

/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * MAIN
 * /////////////////////////////////////////////////////////////////////////////
 */
int main()
{

	// inicia o jogo
	jogar();

	return 0;
}

/* -----------------------------------------------------------------------------
 * CARREGAR
 * Inicializa o SUDOKU a partir de um novo jogo ou estado de jogo anterior
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE *carregar(char quadro[9][9])
{
	int opcao;

	menu_arquivo();
	opcao = le_opcao();

	// TODO Função incompleta

	FILE *fb = NULL;
	char name[32];

	switch (opcao)
	{

	// carregar novo sudoku
	case 1:
		printf("Informe o nome do arquivo: ");
		scanf("%s", name);
		carregar_novo_jogo(quadro, name);
		fb = criar_arquivo_binario(quadro);
		break;
	// continuar jogo
	case 2:
		printf("Informe o nome do arquivo: ");
		scanf("%s", name);
		fb = carregar_continuar_jogo(quadro, name);
		break;

	// retornar ao menu anterior
	case 9:
		break;

	default:
		break;
	}

	return fb;
}

/* -----------------------------------------------------------------------------
 * CARREGAR CONTINUAR JOGO
 * Carrega um estado de jogo a partir de um arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE *carregar_continuar_jogo(char quadro[9][9], char *nome_arquivo)
{
	// TODO
	FILE *fp = fopen(nome_arquivo, "rb+");
	if (fp == NULL)
	{
		printf("%s", ERROR_FILE_MSG);
		exit(1);
	}

	char *n = malloc(4);
	char buf;

	fgets(n, sizeof(int), fp);

	fseek(fp, 4 + 81 * atoi(n), SEEK_SET);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fread(&buf, 1, 1, fp);
			quadro[i][j] = buf;
		}
	}

	free(n);

	return fp;
}

/* -----------------------------------------------------------------------------
 * CARREGAR NOVO JOGO
 * Carrega um novo jogo do Sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void carregar_novo_jogo(char quadro[9][9], char *nome_arquivo)
{
	// TODO

	FILE *fb = fopen(nome_arquivo, "r");
	if (fb == NULL)
	{
		printf("%s", ERROR_FILE_MSG);
	}

	char line[18];
	int i = 0;
	while (fgets(line, sizeof(line), fb))
	{
		if (strlen(line) >= 8)
		{
			for (int j = 0; j < 9; j++)
			{
				quadro[i][j] = line[j * 2];
			}
			i++;
		}
	}

	fclose(fb);
}

/* -----------------------------------------------------------------------------
 * CRIAR ARQUIVO BINARIO
 * Criar arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE *criar_arquivo_binario(char quadro[9][9])
{
	// TODO
	char name[4];
	gen_random(name, 4);
	FILE *fb = fopen(name, "wb+");
	int n = 0;
	char c;

	fwrite(&n, sizeof(int), 1, fb);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fwrite(&quadro[i][j], 1, 1, fb);
		}
	}

	return fb;
}

/* -----------------------------------------------------------------------------
 * DETERMINAR QUADRANTE
 * Dado as posicoes x e y, determina o quadrante do quadro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int determinar_quadrante(int x, int y)
{
	if (x < 3 && y < 3)
		return 1;
	else if (x < 3 && y < 6)
		return 2;
	else if (x < 3 && y < 9)
		return 3;
	else if (x < 6 && y < 3)
		return 4;
	else if (x < 6 && y < 6)
		return 5;
	else if (x < 6 && y < 9)
		return 6;
	else if (x < 9 && y < 3)
		return 7;
	else if (x < 9 && y < 6)
		return 8;
	else
		return 9;
}

/* -----------------------------------------------------------------------------
 * E VALIDO
 * Determina se um valor na posicao x e y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido(const char quadro[9][9], int x, int y, int valor)
{

	// verifica as tres condicoes
	if (!e_valido_na_coluna(quadro, y, valor))
		return FALSO;
	if (!e_valido_na_linha(quadro, x, valor))
		return FALSO;
	if (!e_valido_no_quadro3x3(quadro, x, y, valor))
		return FALSO;
	if (quadro[x][y] != '0')
		return FALSO;

	return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA COLUNA
 * Verifica se um valor na coluna y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_na_coluna(const char quadro[9][9], int y, int valor)
{
	// TODO
	for (int i = 0; i < 9; i++)
	{
		if (quadro[i][y] - '0' == valor)
		{
			return FALSO;
		}
	}
	return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA LINHA
 * Verifica se um valor na linha x e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_na_linha(const char quadro[9][9], int x, int valor)
{
	// TODO
	for (int j = 0; j < 9; j++)
	{
		if (quadro[x][j] - '0' == valor)
		{
			return FALSO;
		}
	}
	return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NO QUADRO 3X3
 * Verifica se um valor e valido no quadrante da posicao x, y
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_no_quadro3x3(const char quadro[9][9], int x, int y, int valor)
{
	// TODO
	int n;
	n = determinar_quadrante(x, y);

	int xi, yi, xf, yf;
	xi = ini_x(n);
	xf = fim_x(n);
	yi = ini_y(n);
	yf = fim_y(n);

	for (int i = xi; i <= xf; i++)
	{
		for (int j = yi; j <= yf; j++)
		{
			if (quadro[i][j] - '0' == valor)
			{
				return FALSO;
			}
		}
	}

	return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * EXISTE CAMPO VAZIO
 * Verifica se existe um campo nao preenchido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int existe_campo_vazio(const char quadro[9][9])
{
	int i, j;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (quadro[i][j] == '0')
				return VERDADEIRO;
		}
	}

	return FALSO;
}

/* -----------------------------------------------------------------------------
 * IMPRIMIR
 * Imprime o quadro inteiro do sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void imprimir(const char quadro[9][9])
{
	int i, j;

	//	puts("~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("    1 2 3   4 5 6   7 8 9");
	for (i = 0; i < 9; i++)
	{
		if (i % 3 == 0)
			puts("  -------------------------");
		for (j = 0; j < 9; j++)
		{

			if (j == 0)
				printf("%d | ", i + 1);
			else if (j % 3 == 0)
				printf("| ");

			if (quadro[i][j] == '0')
				printf("- ");
			else
				printf("%c ", quadro[i][j]);
		}
		puts("|");
	}
	puts("  -------------------------");
}

/* -----------------------------------------------------------------------------
 * JOGAR
 * Realiza toda a logica do jogo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void jogar()
{
	int opcao;
	char quadro[9][9] = {{'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
						 {'0', '0', '0', '0', '0', '0', '0', '0', '0'}};

	FILE *fb = NULL;

	opcao = 0;

	while (opcao != 9)
	{
		// imprime na tela o quadro atual
		imprimir(quadro);

		// apresenta um menu com as opcoes
		menu();
		opcao = le_opcao();

		switch (opcao)
		{

		// carregar sudoku
		case 1:
			fb = carregar(quadro);

			if (fb != NULL)
			{
				fclose(fb);
				fb = criar_arquivo_binario(quadro);
			}
			break;

		// preencher quadro com um valor
		case 2:
		{
			int x, y, valor;
			char c;

			printf("Entre com a posicao e o valor (linha, coluna, valor): ");
			scanf("%d %d %d", &x, &y, &valor);

			if (e_valido(quadro, x - 1, y - 1, valor))
			{
				sprintf(&c, "%d", valor);
				quadro[x - 1][y - 1] = c;
				salvar_jogada_bin(fb, quadro);
			}
			else
				puts("Valor ou posicao incorreta! Tente novamente!");
		}
		break;

		// resolver 1 passo
		case 3:
			resolver_um_passo(quadro);
			salvar_jogada_bin(fb, quadro);
			puts("Um passo resolvido!");
			break;

		// resolver o sudoku
		case 4:
			resolver(fb, quadro);
			break;

		case 9:
			puts("Programa finalizado ..");
			break;

		default:
			puts(INVALID_OPTION);
			break;
		}
	}
}

/* -----------------------------------------------------------------------------
 * RESOLVER
 * Resolve o sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolver(FILE *fb, char quadro[9][9])
{

	// enquanto houver posicoes vazias no quadro
	while (existe_campo_vazio(quadro))
	{
		resolver_um_passo(quadro);
		salvar_jogada_bin(fb, quadro);
	}
}

/* -----------------------------------------------------------------------------
 * RESOLVER UM PASSO
 * Preenche apenas um campo vazio
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolver_um_passo(char quadro[9][9])
{
	// TODO
	int last = 0;
	char c;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (quadro[i][j] == '0')
			{
				for (int k = 1; k <= 9; k++)
				{
					if (e_valido(quadro, i, j, k))
					{
						if (last == 0)
						{
							last = k;
						}
						else
						{
							last = 0;
							break;
						}
					}
				}
				if (last != 0)
				{
					sprintf(&c, "%d", last);
					quadro[i][j] = c;
					return;
				}
			}
		}
	}
}

/* -----------------------------------------------------------------------------
 * SALVAR JOGADA BINARIO
 * Salva o estado atual do quadro no arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void salvar_jogada_bin(FILE *fb, char quadro[9][9])
{
	// TODO
	char *n = malloc(4);
	char c;
	fseek(fb, 0, SEEK_SET);
	fgets(n, sizeof(int), fb);
	fseek(fb, 0, SEEK_SET);
	char *num = malloc(4);
	sprintf(num, "%d", atoi(n) + 1);
	fputs(num, fb);

	fseek(fb, 0, SEEK_END);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fwrite(&quadro[i][j], 1, 1, fb);
		}
	}

	free(n);
	free(num);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 							FUNCOES AUXILIARES
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -----------------------------------------------------------------------------
 * FIM X
 * Indice final da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_x(int quadr)
{
	switch (quadr)
	{
	case 1:
	case 2:
	case 3:
		return 2;

	case 4:
	case 5:
	case 6:
		return 5;

	default:
		return 8;
	}
}

/* -----------------------------------------------------------------------------
 * FIM Y
 * Indice final da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_y(int quadr)
{
	switch (quadr)
	{
	case 1:
	case 4:
	case 7:
		return 2;

	case 2:
	case 5:
	case 8:
		return 5;

	default:
		return 8;
	}
}

/* -----------------------------------------------------------------------------
 * GEN_RANDOM
 * Gera uma cadeira de caracteres randomica de tamanho len
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void gen_random(char *s, const int len)
{
	srand(time(NULL));
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	int i;
	// int main log n () to
		//forname(a, h);
		//txt to format and conquer anything bc_ ++
		//im the nerd boy

	for (i = 0; i < len; ++i)
	{
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = 0;
}

/* -----------------------------------------------------------------------------
 * INI X
 * Indice inicial da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_x(int quadr)
{
	switch (quadr)
	{
	case 1:
	case 2:
	case 3:
		return 0;

	case 4:
	case 5:
	case 6:
		return 3;

	default:
		return 7;
	}
}

/* -----------------------------------------------------------------------------
 * INI Y
 * Indice inicial da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_y(int quadr)
{
	switch (quadr)
	{
	case 1:
	case 4:
	case 7:
		return 0;

	case 2:
	case 5:
	case 8:
		return 3;

	default:
		return 7;
	}
}

/* -----------------------------------------------------------------------------
 * LE OPCAO
 * Imprime a mensagem a faz a leitura da opcao
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int le_opcao()
{
	int opcao;

	printf("Opcao: ");
	scanf("%d", &opcao);

	return opcao;
}

/* -----------------------------------------------------------------------------
 * MENU
 * Imprime o menu de opcoes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu()
{
	puts("\n~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("[1] - Carregar jogo");
	puts("[2] - Jogar");
	puts("[3] - Resolver um passo");
	puts("[4] - Resolver");
	puts("[9] - Finalizar");
	puts("--------");
}

/* -----------------------------------------------------------------------------
 * MENU ARQUIVO
 * Imprime o menu de opcoes do arquivo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu_arquivo()
{
	puts("\n~~~~~ MENU ARQUIVO ~~~~~");
	puts("[1] - Novo jogo");
	puts("[2] - Continuar jogo");
	puts("[9] - Retornar ao menu anterior");
	puts("--------");
}
/*GABRIEL TRES CEOLIN, MATEUS ALEM VICENTE*/
