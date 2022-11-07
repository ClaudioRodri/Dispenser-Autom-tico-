/**************************************************************************************************************************


Projeto de Dispenser Automático de Ração 

Utilizando o arduino, o programa gerência o trabalho de um rele, que controla se o motor está ligado ou não, e por quanto tempo.

Foram usados também um teclado, e um display de 7 segmentos.
***************************************************************************************************************************/

#include <LiquidCrystal.h> //biblioteca para configurar o LCD
#include <Keypad.h> //biblioteca para configurar o teclado 

#define RELE 13 //associdando o pino 13 do arduino ao rele de ativação do motor 
#define CLEAR_TIME 3
int qntd;
int horario;
int segundos = 0;
int opc =0;

int relogio[3] = {0 ,0 ,0}; //relogio
int hora [2] = {0,0}; // hora de comer
int horaAlimentacao = 0;
int QuantidadeAlimentacao = 0;
// se algum botão (A,B,C ou D for presionado)
// será estabelecido um controle por aqui!
bool btnSelecionado = false; 

void clearLCD();
void createMenuLCD();
void setSegundos();
void setLCDText(char text[]);
void setRelogio();
void setQuantidade();

const byte linhas = 4; //4 linhas
const byte colunas = 4; //4 colunas

//teclado matricial
char matrizteclado[linhas][colunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pinoslinhas[linhas] = {2,3,4,5}; //pinos utilizados nas linhas
byte pinoscolunas[colunas] = {6,7,8,9}; //pinos utilizados nas colunas

//inicializando o teclado
Keypad teclado = Keypad( makeKeymap(matrizteclado), pinoslinhas, pinoscolunas, linhas, colunas );


LiquidCrystal lcd_1(A0, A1, A2, A3, A4, A5);

void setup() {
  pinMode(RELE, OUTPUT); //fala que o rele é uma saída
  lcd_1.begin(16, 2);
  lcd_1.setCursor(3,0);
  setLCDText("Bem-vindo");
  lcd_1.setCursor(3,1);
  Serial.begin(9600);
}


void loop(){
   char apertatecla = teclado.getKey(); // verifica se alguma tecla foi pressionada
  if(apertatecla == 'A') {
    clearLCD();
    lcd_1.print("Digite a hora: ");
    btnSelecionado = true;
    opc = 1;
    setHoraAlimentacao();
  }
  else if(apertatecla == 'B') {
     clearLCD();
     btnSelecionado = true;
     opc = 2;
  }
  else if(apertatecla == '#') {
     clearLCD();
     btnSelecionado = false;
     createMenu();
  }
  else if(apertatecla == 'C') {
     clearLCD();
     btnSelecionado = true;
     opc = 3;
     lcd_1.print("Digite o tempo: ");
     setQuantidade();
  }
  
  if(segundos == CLEAR_TIME){
     createMenu();
  }
  setSegundos();
  setRelogio();
  int variavel = 0;
  variavel = horaAlimentacao + QuantidadeAlimentacao;     
  if(relogio[2] == horaAlimentacao){
     digitalWrite(RELE,HIGH);
  }else if(relogio[2] == variavel){
     digitalWrite(RELE,HIGH);
  }
}
   

void clearLCD(){
    lcd_1.clear();
}

void setSegundos(){
    delay(1000);
    segundos++;
  if(opc == 2){
    mostrarRelogio();
    lcd_1.setCursor(0,1);
    lcd_1.print("Ativara em:");
    lcd_1.setCursor(12,1);
    lcd_1.print(horaAlimentacao);
  }
}

void setLCDText(char text[]){
  lcd_1.print(text);
}
 
/* 
   Função para criar o menu
   se btnSelecionado (apertando a letra 'A'),
   o menu será criado instataneamente se não, só depois 
   de CLEAR_TIME segundos
*/
void createMenu(){
  if(btnSelecionado == false){
     clearLCD();
     setLCDText("A-Hora");
     lcd_1.setCursor(7,0);
     setLCDText("B-Relogio");
     lcd_1.setCursor(2,1);
     setLCDText("C-Quantidade");
    // voltar no menu -- outra função
  }
}

void setQuantidade(){
  int TempoAlimentacao[30];
  for (int i = 0; i <= 30; i++) TempoAlimentacao[i] = i;
  int index = 0;
  while(teclado.getKey() != '*'){
     char apertatecla = teclado.getKey();
      if (apertatecla && apertatecla != '*') {
          if(apertatecla == 'D'){
            if(index <= 30){
              clearLCD();
              lcd_1.setCursor(8,0);
          lcd_1.print(TempoAlimentacao[index]);
                index++;
            } else index = 0;
          }
        }
  
  } 
  QuantidadeAlimentacao = TempoAlimentacao[index -1];
  clearLCD();
  lcd_1.setCursor(0,0);
  lcd_1.print("Tempo: ");
  lcd_1.setCursor(0,1);
  lcd_1.print("Voltar Menu - #");
  lcd_1.setCursor(8,0);
  lcd_1.print(QuantidadeAlimentacao);
    
}
  
  void setRelogio(){
  relogio[2] = relogio[2] + 1;
  if(relogio[2] == 60) {
    relogio[1] = relogio[1] + 1;
    relogio[2] = 0;
  }
  if(relogio[1] == 60){
     relogio[0] = relogio[0] + 1;
     relogio[1] = 0;
   }
  if(relogio[0] > 24)relogio[0] = 0;
}

  void mostrarRelogio(){
  lcd_1.setCursor(5,0);
  lcd_1.print(relogio[0]);
  lcd_1.setCursor(8,0);
  lcd_1.print(relogio[1]);
  lcd_1.setCursor(11,0);
  lcd_1.print(relogio[2]);
  
}


  void setHoraAlimentacao(){
  int horasAlimentacao[60];
  // Laço for para popular horasAlimentação
    for(int i = 0; i <= 60;i++)horasAlimentacao[i]= i;
  int index = 0;
  while(teclado.getKey() != '*'){
     char apertatecla = teclado.getKey();
      if (apertatecla && apertatecla != '*') {
          if(apertatecla == 'D'){
            if(index <= 60){
              clearLCD();
              lcd_1.setCursor(8,0);
          lcd_1.print(horasAlimentacao[index]);
                index = index + 5;
            } else index = 0;
          }
        }
  }
  horaAlimentacao = horasAlimentacao[index - 5];
  clearLCD();
  lcd_1.setCursor(0,0);
  lcd_1.print("Hora salva: ");
  lcd_1.setCursor(0,1);
  lcd_1.print("Voltar Menu - #");
  lcd_1.setCursor(13,0);
  lcd_1.print(horaAlimentacao);
  
   

}