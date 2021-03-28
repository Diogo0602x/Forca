#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>
#include <conio.h>
#include "conio.h"

struct tForca{
	char palavra_oculta[30];
	char chutes[26];
	int contador_chutes = 0;
};

struct tForca forca;

int menu();
void banco_palavras();
void adicionar_palavras();
void chutes();
int chutes_errados();
void jogo_forca();
int letras_chutadas(char letra);
int acertou();
int enforcou();
int jogar_novamente();
void resetar_variaveis();
void desenho_quadro();
void bem_vindo();
void desenho_forca();
void desenhou_ganhou();
void desenho_perdeu();
void desenho_feliz();
void logo_forca();

int main(){
	desenho_quadro();
	desenho_forca();
	bem_vindo();
	logo_forca();
	getchar();
	int opcao_menu;			//Menu de opções
		do{
			opcao_menu = menu();		//Função menu
			switch(opcao_menu){
				case 0:
					clrscr();
					getchar();
					desenho_quadro();
					desenho_forca();
					logo_forca();
					gotoxy(7,3);textcolor(7);printf("Saindo... \n");		//Mensagem ao escolher a opção para fechar o console
					gotoxy(6,26);
					break;
				case 1:
					do{
						clrscr();
						getchar();
						desenho_quadro();
						banco_palavras();		//Função para escolher a palavra secreta
						resetar_variaveis();	//Gambiarra para resetar os dados das variaveis
						do{
							jogo_forca();		//Chamada de funções necessárias para o jogo
							
							chutes();			//Chamada de funções necessárias para controlar os chutes
						}while(!acertou() && !enforcou());		//Booleanos para controle da partida	
						
						if(acertou()){	
							clrscr();
							getchar();
							desenho_quadro();	
							desenhou_ganhou();
							desenho_feliz();
							gotoxy(7,3);textcolor(7);printf("Palavra: %s",forca.palavra_oculta);
							gotoxy(7,4);textcolor(7);printf("Parabens, voce ganhou!");
						}else{
							clrscr();
							getchar();
							desenho_quadro();	
							desenho_forca();
							desenho_perdeu();
							gotoxy(7,3);textcolor(7);printf("Perdeu...");
							gotoxy(7,4);textcolor(7);printf("Palavra: %s",forca.palavra_oculta);
						}
					}while(jogar_novamente() == 'S');
					break;
				case 2:
					clrscr();
					getchar();
					desenho_quadro();
					adicionar_palavras();
					break;
				default:
					clrscr();
					getchar();
					desenho_quadro();
					printf("Opcao invalida. \n");
					break;
			}
		}while(opcao_menu != 0);
}

int menu(){
	clrscr();
	desenho_quadro();
	desenho_forca();
	logo_forca();
	
	int op;
	
	gotoxy(7,3);textcolor(7);printf("[1] - Iniciar jogo. \n");
	gotoxy(7,4);textcolor(7);printf("[2] - Adicionar palavra. \n");
	gotoxy(7,5);textcolor(7);printf("[0] - Exit. \n");

	gotoxy(7,7);textcolor(7);printf("Escolha a sua opcao: ");
	_setcursortype(2);
	scanf("%d",&op);
	
	return op;	
}

void banco_palavras(){	
	FILE* f;

	f = fopen("bancoPalavras.txt","r");
	
	if(f == 0){
		gotoxy(7,3);textcolor(7);printf("Banco de palavras indisponivel no momento!");
		exit(1);
	}		
	int quantidade_palavras;
	fscanf(f,"%d",&quantidade_palavras);

	srand(time(0));
	int numero_randomico = rand() % quantidade_palavras;
	
	for(int i = 0; i < numero_randomico; i++){
		fscanf(f, "%s",&forca.palavra_oculta);
	}
	fclose(f);	
}

void adicionar_palavras(){
	char palavra_nova[30];
	int quantidade = 0;
	
	FILE* f;
	
	//do{
		gotoxy(7,3);textcolor(7);printf("Informe a palavra nova:");
		_setcursortype(2);
		scanf("%s",&palavra_nova);
	//}while(palavra_nova < 65|| palavra_nova > 122);
		for(int i = 0; i<=strlen(palavra_nova); i++){
			palavra_nova[i]=toupper(palavra_nova[i]);
		}
	
	f = fopen("bancoPalavras.txt","r+");
	
	fscanf(f,"%d",&quantidade);
	quantidade++;				
			
	fseek(f, 0, SEEK_SET);
	fprintf(f, "%d", quantidade);
	
	fseek(f, 0, SEEK_END);
	fprintf(f, "\n%s", palavra_nova);
	
	fclose(f);
		
	gotoxy(7,5);textcolor(7);printf("PALAVRA: %s\n",palavra_nova);
	gotoxy(7,6);textcolor(7);printf("Sucesso ao adicionar uma nova palavra ao banco de dados!\n");
}

void chutes(){
    char chute;

	do{
		gotoxy(7,3);textcolor(7);printf("Qual letra? ");
		_setcursortype(2);
		scanf(" %c", &chute);
		chute = toupper(chute);
	    	if(chute<65 || chute>90){
	    		gotoxy(7,5);printf("Voce nao digitou uma letra!");
			}
	}while(chute<65 || chute>90);
	
    forca.chutes[forca.contador_chutes] = chute;
    forca.contador_chutes++;	
}

int chutes_errados(){
	int erros = 0;
	
	for(int i = 0; i < forca.contador_chutes; i++){
		int existe = 0;
		
		for(int j = 0; j < strlen(forca.palavra_oculta); j++){
			if(forca.chutes[i] == forca.palavra_oculta[j]){
				
				existe = 1;
				break;
			}
		}
		
		if(!existe) erros++;
	}
	
	return erros;	
}

void jogo_forca(){
	
	int erros = chutes_errados();

	//FORCA LINHA HORIZONTAL MAIOR(CHÃO)
	for(int i =47; i<61; i++){		
		gotoxy(i,15);textcolor(7);printf("%c",196);
	}
	//FORCA LINHA HORIZONTAL MENOR(POSTE)
	for(int i =50; i<55; i++){		
		gotoxy(i,6);textcolor(7);printf("%c",196);
	}
	
	//FORCA LINHA VERTICAL MAIOR(POSTE	
	for(int i =7; i<15; i++){
		gotoxy(55,i);textcolor(7);printf("%c",179);
	}
	//BONECO
		gotoxy(49,8);textcolor(7);printf("%c",245);
	if(erros>=1){
		gotoxy(49,9);textcolor(7);printf("O");
	}
	if(erros>=2){
		gotoxy(48,10);textcolor(7);printf("/");
	}
	if(erros>=3){
		gotoxy(48,10);textcolor(7);printf("/%c",179);
	}
	if(erros>=4){
		gotoxy(48,10);textcolor(7);printf("/%c\\",179);
	}
	if(erros>=5){
		gotoxy(48,11);textcolor(7);printf(" %c",179);
	}
	if(erros>=6){
		gotoxy(48,12);textcolor(7);printf("/ ");
	}
	if(erros==7){
		gotoxy(48,12);textcolor(7);printf("/ \\");
	}
		
	gotoxy(55,6);printf("%c",191);//CANTO DIREITO POSTE
	gotoxy(49,6);printf("%c",218);//CANTO ESQUERDO POSTE
	gotoxy(49,7);printf("%c",179);//LINHA VERTICAL MENOR(CORDA)
	
	gotoxy(12,12);_setcursortype(2);
	for(int i = 0; i < strlen(forca.palavra_oculta); i++ ) {
		
		int achou = letras_chutadas(forca.palavra_oculta[i]);
			if (achou){
				printf("%c ", forca.palavra_oculta[i]);
			} else{
				printf("_ ");
			}
		}
		printf("\n");
}

int letras_chutadas(char letra){		//desenha forca // Letras Já chutou
	int achou = 0;
			
	for(int i = 0; i < forca.contador_chutes; i++) {
		if(forca.chutes[i] == letra) {
			achou = 1;
			break;
		}
	}
	
	return achou;
}

int acertou(){
	for(int i = 0; i < strlen(forca.palavra_oculta); i++ ){
		if(!letras_chutadas(forca.palavra_oculta[i])){
			return 0;
		}
	}
	
	return 1;
}

int enforcou(){
	
	return chutes_errados()>= 7;	//dificuldade aqui!
}

int jogar_novamente(){
	char jogar_novamente;
	
	gotoxy(7,5);textcolor(7);printf("Deseja jogar novamente?");
	_setcursortype(2);
	do{
		scanf(" %c",&jogar_novamente);
		jogar_novamente=toupper(jogar_novamente);
	}while(jogar_novamente != 83 && jogar_novamente != 78);
	
	return jogar_novamente;
}

void resetar_variaveis(){
	for(int i = 0; i<=26; i++){
		forca.chutes[i] = '\0';
	}
	forca.contador_chutes = 0;
}

void desenho_quadro(){
	//HORIZONTAIS
	for(int i=6;i<75;i++) {
        gotoxy(i,2);textcolor(6);printf("%c", 205);
        gotoxy(i,25);textcolor(6);printf("%c", 205);
    }
	//VERTICAIS
	for(int i=3;i<25;i++) {
        gotoxy(5,i);textcolor(6);printf("%c", 186);
        gotoxy(75,i);textcolor(6);printf("%c", 186);
    }
	//CANTOS
	gotoxy(5,2);textcolor(6);printf("%c",201);//SUPERIOR ESQUERDO
	gotoxy(75,2);textcolor(6);printf("%c",187);//SUPERIOR DIREITO
	gotoxy(5,25);textcolor(6);printf("%c",200);//INFERIOR ESQUERDO
	gotoxy(75,25);textcolor(6);printf("%c",188);//INFERIOR DIREITO	
}

void bem_vindo(){
	_setcursortype(0);
		//BEM VINDO
	gotoxy(7,3);textcolor(7);printf("Bem-Vindo ao jogo da Forca! \n");
	gotoxy(7,4);textcolor(7);printf("Autores: Pedro Lucas & Diogo Sales\n");
}

void desenho_forca(){
	
	//FORCA LINHA HORIZONTAL MAIOR(CHÃO)
	for(int i =47; i<61; i++){		
		gotoxy(i,15);textcolor(7);printf("%c",196);
	}
	//FORCA LINHA HORIZONTAL MENOR(POSTE)
	for(int i =50; i<55; i++){		
		gotoxy(i,6);textcolor(7);printf("%c",196);
	}
	
	//FORCA LINHA VERTICAL MAIOR(POSTE	
	for(int i =7; i<15; i++){
		gotoxy(55,i);textcolor(7);printf("%c",179);
	}
	//BONECO
		gotoxy(49,8);textcolor(7);printf("%c",245);
		gotoxy(49,9);textcolor(7);printf("O");
		gotoxy(48,10);textcolor(7);printf("/%c\\",179);
		gotoxy(48,11);textcolor(7);printf(" %c",179);
		gotoxy(48,12);textcolor(7);printf("/ \\");
	
		
	gotoxy(55,6);printf("%c",191);//CANTO DIREITO POSTE
	gotoxy(49,6);printf("%c",218);//CANTO ESQUERDO POSTE
	gotoxy(49,7);printf("%c",179);//LINHA VERTICAL MENOR(CORDA)
}

void desenhou_ganhou(){

	gotoxy(7,18);textcolor(7);printf("  _____          _   _ _    _  ____  _    _ _	");
	gotoxy(7,19);textcolor(7);printf(" / ____|   /\\   | \\ | | |  | |/ __ \\| |  | | |	");
	gotoxy(7,20);textcolor(7);printf("| |  __   /  \\  |  \\| | |__| | |  | | |  | | |	");
	gotoxy(7,21);textcolor(7);printf("| | |_ | / /\\ \\ | . ` |  __  | |  | | |  | | |	");
	gotoxy(7,22);textcolor(7);printf("| |__| |/ ____ \\| |\\  | |  | | |__| | |__| |_|	");
	gotoxy(7,23);textcolor(7);printf(" \\_____/_/    \\_\\_| \\_|_|  |_|\\____/ \\____/(_)	");

}

void desenho_perdeu(){
	gotoxy(7,18);textcolor(7);printf(" _____  ______ _____  _____  ______ _    _ _ ");
 	gotoxy(7,19);textcolor(7);printf("|  __ \\|  ____|  __ \\|  __ \\|  ____| |  | | | ");
 	gotoxy(7,20);textcolor(7);printf("| |__) | |__  | |__) | |  | | |__  | |  | | | ");
	gotoxy(7,21);textcolor(7);printf("|  ___/|  __| |  _  /| |  | |  __| | |  | | | ");
 	gotoxy(7,22);textcolor(7);printf("| |    | |____| | \\ \\| |__| | |____| |__| |_| ");
 	gotoxy(7,23);textcolor(7);printf("|_|    |______|_|  \\_\\_____/|______|\\____/(_) ");
}

void desenho_feliz(){
	
	//FORCA LINHA HORIZONTAL MAIOR(CHÃO)
	for(int i =47; i<61; i++){		
		gotoxy(i,15);textcolor(7);printf("%c",196);
	}
	//FORCA LINHA HORIZONTAL MENOR(POSTE)
	for(int i =50; i<55; i++){		
		gotoxy(i,6);textcolor(7);printf("%c",196);
	}
	
	//FORCA LINHA VERTICAL MAIOR(POSTE	
	for(int i =7; i<15; i++){
		gotoxy(55,i);textcolor(7);printf("%c",179);
	}
	//BONECO
		gotoxy(49,8);textcolor(7);printf("%c",245);
		gotoxy(49,11);textcolor(7);printf("O");
		gotoxy(48,12);textcolor(7);printf("\\%c/",179);
		gotoxy(48,13);textcolor(7);printf(" %c",179);
		gotoxy(48,14);textcolor(7);printf("/ \\");
	
		
	gotoxy(55,6);printf("%c",191);//CANTO DIREITO POSTE
	gotoxy(49,6);printf("%c",218);//CANTO ESQUERDO POSTE
	gotoxy(49,7);printf("%c",179);//LINHA VERTICAL MENOR(CORDA)
}

void logo_forca(){
	gotoxy(7,19);textcolor(7);printf(" ______ ____  _____   _____          		 ");
	gotoxy(7,20);textcolor(7);printf("|  ____/ __ \\|  __ \\ / ____|   /\\   	 ");
	gotoxy(7,21);textcolor(7);printf("| |__ | |  | | |__) | |       /  \\   	 ");
	gotoxy(7,22);textcolor(7);printf("|  __|| |  | |  _  /| |      / /\\ \\   	 ");
	gotoxy(7,23);textcolor(7);printf("| |   | |__| | | \\ \\| |____ / ____ \\ 	 ");
	gotoxy(7,24);textcolor(7);printf("|_|    \\____/|_|  \\_\\\\_____/_/    \\_\\");
}

