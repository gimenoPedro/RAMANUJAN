#include <LiquidCrystal.h>
#include <Keypad.h>
#include <string.h>
#include <stdlib.h>

// ==== Configuração do LCD ====
const int rs = A4, en = A5, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ==== Configuração do LCD2 ====
const int rs_2 = A2, en_2 = A3, d4_2 = 2, d5_2 = 3, d6_2 = 4, d7_2 = 5;
LiquidCrystal lcd_2(rs_2, en_2, d4_2, d5_2, d6_2, d7_2);


// ==== Configuração do teclado matricial ====
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
 {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ==== Controle de telas ====
enum Estado {
  TELA_INICIAL,
  TELA_BEMVINDO,
  TELA_NAV1,
  TELA_NAV2,
  TELA_NAV3,
  TELA_NAV4,
  TELA_NAV5,
  TELA_SELECAO_GRANDEZA,
  TELA_APOS_SELECIONAR,
  TELA_NAVEGACAO_GRANDEZA,
  TELA_ENTRADA_VALOR,
  TELA_CONFIRMACAO_GRANDEZA,
  TELA_ADICIONAR_OUTRA,
  TELA_SELECAO_CALCULO,
  TELA_APOS_SELECIONAR2,
  TELA_NAVEGACAO_OBJETIVO,
  TELA_RESULTADO,
  TELA_MENSAGEM_INSUFICIENTE,
  TELA_ADD_OUTRA_GRANDEZA,
  TELA_extra_num1,
  TELA_extra_num1_pt2,
  TELA_Extra_num2,
  TELA_extra_num3,
  TELA_extra_num3_pt2,
  TELA_CALC_OUTRA_GRANDEZA,
};
Estado estado = TELA_INICIAL;
Estado estadoAnterior = TELA_INICIAL;

// ==== Controle de tempo ====
unsigned long ultimaTroca = 0;
bool mostrandoPrimeiroTexto = true;

// ==== Controle da seleção de grandeza ====
const char* grandezas[3] = {
  "Forca",
   "Massa",
   "Aceleracao"};   
int indiceGrandeza = 0;
int ultimaGrandezaIndex = -1;

// ==== Controle da seleção de objetivo ====
const char* objetivos[3] = {"Calcular F", "Calcular m", "Calcular a"};
int indiceObjetivo = 0;
int ultimoObjetivoIndex = -1;

// ==== Variáveis das grandezas ====
float valorForca = 0;
float valorMassa = 0;
float valorAceleracao = 0;

bool flagForca = false;
bool flagMassa = false;
bool flagAceleracao = false;

// ==== Resultado calculado ====
float resultado = 0;

// ==== Entrada de valor ====
char inputBuffer[16];
int inputIndex = 0;

// ==== Flags de tela ====
bool telaConfirmacaoMostrada = false;
bool telaAdicionarMostrada = false;
bool telaResultadoMostrada = false;
bool telaMensagemMostrada = false;
bool telaExtraNum1 = false;
bool telaExtraNum1Msg = false;
bool telaExtraNum2 = false;
bool telaExtraNum2Msg = false;
bool telaExtraNum3Msg = false;
bool telaExtraNum3 = false;
bool telaExtraNum3msg = false;

// Caracteres especiais para o LCD //

byte setaCima[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

byte setaBaixo[8] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};

byte setaEsquerda[8] = {
  B00000,
  B00000,
  B00100,
  B01000,
  B11111,  
  B01000,
  B00100,
  B00000
};


byte setaDireita[8] = {
  B00000,
  B00000,
  B00100,
  B00010,
  B11111,  
  B00010,
  B00100,
  B00000
};


byte charOK[8] = {
  B00000,
  B00001,  
  B00010,
  B10010,  
  B01010,
  B00100,
  B00100,
  B00000
};


byte charNO[8] = {
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000
};


// ==== Funções auxiliares ====
void printCentralizado(const char* texto, int linha) {
  int col = (16 - strlen(texto)) / 2;
  lcd.setCursor(col, linha);
  lcd.print(texto);
}

void printCentralizado_2(const char* texto, int linha) {
  int col = (16 - strlen(texto)) / 2;
  lcd_2.setCursor(col, linha);
  lcd_2.print(texto);
}

void ledrON(){
  digitalWrite(A0, HIGH);
}

void ledrOFF(){
  digitalWrite(A0, LOW);
}

void ledgON(){
  digitalWrite(A1, HIGH);
}

void ledgOFF(){
  digitalWrite(A1, LOW);
}

void atualizarLEDSporEstado(Estado novoEstado){

  switch (novoEstado){

  case TELA_INICIAL:
    ledgON();
    ledrOFF();
    break;

  case TELA_BEMVINDO:
    ledgON();
    ledrOFF();
    break;

  case TELA_NAV1:
    ledgOFF();
    ledrON();
    break; 

  case TELA_NAV2:
    ledgOFF();
    ledrON();
    break;

  case TELA_NAV3:
    ledgOFF();
    ledrON();
    break;

  case TELA_NAV4:
    ledgOFF();
    ledrON();
    break;

  case TELA_NAV5:
    ledgON();
    ledrOFF();
    break;
  
  case TELA_SELECAO_GRANDEZA:
    ledgOFF();
    ledrON();
    break;

  case TELA_APOS_SELECIONAR:
    ledgOFF();
    ledrON();
    break;
  
  case TELA_NAVEGACAO_GRANDEZA:
    ledgON();
    ledrOFF();
    break;

  case TELA_ENTRADA_VALOR:
    ledgON();
    ledrOFF();
    break;

  case TELA_CONFIRMACAO_GRANDEZA:
    ledgOFF();
    ledrON();
    break;

  case TELA_ADICIONAR_OUTRA:
    ledgON();
    ledrOFF();
    break;

  case TELA_SELECAO_CALCULO:
    ledgOFF();
    ledrON();
    break;

  case TELA_APOS_SELECIONAR2:
    ledgOFF();
    ledrON();
    break;

  case TELA_NAVEGACAO_OBJETIVO:
    ledgON();
    ledrOFF();
    break;

  case TELA_RESULTADO:
    ledgOFF();
    ledrON();
    break;

  case TELA_MENSAGEM_INSUFICIENTE:
    ledgOFF();
    ledrON();
    break;

  case TELA_ADD_OUTRA_GRANDEZA:
    ledgON();
    ledrOFF();
    break;

  case TELA_extra_num1:
    ledgON();
    ledrOFF();
    break;

  case TELA_extra_num1_pt2:
    ledgOFF();
    ledrON();
    break;

  case TELA_Extra_num2:
    ledgOFF();
    ledrON();
    break;

  case TELA_CALC_OUTRA_GRANDEZA:
    ledgON();
    ledrOFF();
    break;

  case TELA_extra_num3:
    ledgON();
    ledrOFF();
    break;

  case TELA_extra_num3_pt2:
    ledgOFF();
    ledrON();
    break;
  }
}

void mostrarTelaInicial(bool primeira) {
  lcd.clear();
  if (primeira) {
    printCentralizado("RAMANUJAN", 0);
    printCentralizado("CALCLADR INTLGNT", 1);
  } else {
    printCentralizado("PROJETO TCC", 0);
    printCentralizado("PEDRO GIMENO", 1);
  }

  printCentralizado_2("verde interaja",0);
  printCentralizado_2("vermelho espere",1);
  atualizarLEDSporEstado(estado);
}

void atualizarPonteiros() {}

void confirmarValor() {
  float valorConvertido = atof(inputBuffer);

  if (strcmp(grandezas[indiceGrandeza], "Forca") == 0) {
    valorForca = valorConvertido;
    flagForca = true;
  } else if (strcmp(grandezas[indiceGrandeza], "Massa") == 0) {
    valorMassa = valorConvertido;
    flagMassa = true;
  } else if (strcmp(grandezas[indiceGrandeza], "Aceleracao") == 0) {
    valorAceleracao = valorConvertido;
    flagAceleracao = true;
  }

  atualizarPonteiros();

  telaConfirmacaoMostrada = false;
  telaAdicionarMostrada = false;
  estado = TELA_CONFIRMACAO_GRANDEZA;
  ultimaTroca = millis();
}

void setup() {
  lcd.begin(16, 2);
  mostrarTelaInicial(true);
  ultimaTroca = millis();


  lcd_2.begin(16, 2);   // inicializa o segundo LCD
  lcd_2.clear();        // limpa o LCD 2


  lcd.createChar(0, setaCima);
  lcd.createChar(1, setaBaixo);
  lcd.createChar(2, setaEsquerda);
  lcd.createChar(3, setaDireita);
  lcd.createChar(4, charOK);
  lcd.createChar(5, charNO);

  lcd_2.createChar(0, setaCima);
  lcd_2.createChar(1, setaBaixo);
  lcd_2.createChar(2, setaEsquerda);
  lcd_2.createChar(3, setaDireita);
  lcd_2.createChar(4, charOK);
  lcd_2.createChar(5, charNO);


}

void resetFlagsAuxiliares() 
{
   telaConfirmacaoMostrada = false;
  telaAdicionarMostrada = false;
  telaResultadoMostrada = false;
  telaMensagemMostrada = false;
  telaExtraNum1 = false;
  telaExtraNum1Msg = false;
  telaExtraNum2 = false;
  telaExtraNum2Msg = false;
  telaExtraNum3Msg = false;
}


void resetFlagsParaSelecaoCalculo() {
    telaConfirmacaoMostrada = false;
    telaAdicionarMostrada = false;
    telaResultadoMostrada = false;
    telaMensagemMostrada = false;
}


void loop() {

  if (estado != estadoAnterior) {
  atualizarLEDSporEstado(estado);
  estadoAnterior = estado;
}

  char tecla = keypad.getKey();

  switch (estado) {
    case TELA_INICIAL:
      if (millis() - ultimaTroca >= 3000) {
        mostrandoPrimeiroTexto = !mostrandoPrimeiroTexto;
        mostrarTelaInicial(mostrandoPrimeiroTexto);
        ultimaTroca = millis();
      }
      if (tecla) {
        lcd.clear();
        printCentralizado("BEM-VINDO", 0);
        lcd.setCursor(0, 1);
        lcd.print(F("C PARA CONTINUAR"));
        estado = TELA_BEMVINDO;
      }
      break;



    case TELA_BEMVINDO:

      resetFlagsAuxiliares();

      if (tecla == 'C') {
        lcd.clear();
        printCentralizado("COMO NAVEGAR O", 0);
        printCentralizado("MENU (AGUARDE)", 1);
        estado = TELA_NAV1;
        ultimaTroca = millis();
      }
      break;

    case TELA_NAV1:
      if (millis() - ultimaTroca >= 2000) {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.write((byte)2);
        lcd.setCursor(4,0);
        lcd.print(F("(*)"));
        lcd.setCursor(9,0);
        lcd.print(F("(#)"));
        lcd.setCursor(13,0);
        lcd.write((byte)3);
        printCentralizado("ESQ      DIR", 1);

        lcd_2.clear();
        lcd_2.setCursor(0,0);
        lcd_2.write((byte)2);
        lcd_2.setCursor(2,0);
        lcd_2.print(F("*"));
        lcd_2.setCursor(0,1);
        lcd_2.write((byte)3);
        lcd_2.setCursor(2,1);
        lcd_2.print(F("#"));

        estado = TELA_NAV2;
        ultimaTroca = millis();
      }
      break;

    case TELA_NAV2:
      if (millis() - ultimaTroca >= 2000) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("P/ CIMA  P/BAIXO"));
        lcd.setCursor(1, 1);
        lcd.print(F("(A)"));
        lcd.setCursor(5,1);
        lcd.write((byte)0);
        lcd.setCursor(10,1);
        lcd.write((byte)1);
        lcd.setCursor(12,1);
        lcd.print(F("(B)"));

        lcd_2.setCursor(6,0);
        lcd_2.write((byte)0);
        lcd_2.setCursor(8,0);
        lcd_2.print(F("A"));
        lcd_2.setCursor(6,1);
        lcd_2.write((byte)1);
        lcd_2.setCursor(8,1);
        lcd_2.print(F("B"));


        estado = TELA_NAV3;
        ultimaTroca = millis();
      }
      break;

    case TELA_NAV3:
      if (millis() - ultimaTroca >= 2000) {
        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print(F("CONFIRMAR (C)"));
        lcd.setCursor(14,0);
        lcd.write((byte)4);
        lcd.setCursor(1,1);
        lcd.print(F("DELETAR (D)"));
        lcd.setCursor(13,1);
        lcd.write((byte)5);

        lcd_2.setCursor(12,0);
        lcd_2.write((byte)4);
        lcd_2.setCursor(14,0);
        lcd_2.print(F("C"));
        lcd_2.setCursor(12,1);
        lcd_2.write((byte)5);
        lcd_2.setCursor(14,1);
        lcd_2.print(F("D"));

        estado = TELA_NAV4;
        ultimaTroca = millis();
      }
      break;

    case TELA_NAV4:
      if (millis() - ultimaTroca >= 2000) {
        lcd.clear();
        printCentralizado("DESEJA VOLTAR", 0);
        lcd.setCursor(0, 1);
        lcd.print(F("SIM (C) NAO (D)"));
        estado = TELA_NAV5;
      }
      break;

    case TELA_NAV5:
      if (tecla == 'C') {
        lcd.clear();
        printCentralizado("COMO NAVEGAR O", 0);
        printCentralizado("MENU (AGUARDE)", 1);
        estado = TELA_NAV1;
        ultimaTroca = millis();
      } else if (tecla == 'D') {
        lcd.clear();
        printCentralizado("NAVEGUE A LISTA", 0);

        lcd.setCursor(1, 1);
        lcd.print(F("COM */#"));
        lcd.setCursor(9,1);
        lcd.write((byte)2);
        lcd.setCursor(12,1);
        lcd.write((byte)3);

        estado = TELA_SELECAO_GRANDEZA;
        ultimaTroca = millis();
      }
      break;

    case TELA_SELECAO_GRANDEZA:
      if (millis() - ultimaTroca >= 2000) {
        lcd.clear();
        printCentralizado("APOS SELECIONAR", 0);
        lcd.setCursor(0, 1);
        lcd.print(F("CONFIRME COM C"));
        estado = TELA_NAVEGACAO_GRANDEZA;
        ultimaTroca = millis();
      }
      break;

    case TELA_NAVEGACAO_GRANDEZA:
      if (ultimaGrandezaIndex != indiceGrandeza) {
        lcd.clear();

        lcd.setCursor(1, 0);
        lcd.write((byte)2);
        lcd.setCursor(3,0);
        lcd.print(F("(*)"));
        lcd.setCursor(11,0);
        lcd.write((byte)3);
        lcd.setCursor(13,0);
        lcd.print(F("(#)"));

        printCentralizado(grandezas[indiceGrandeza], 1);
        ultimaGrandezaIndex = indiceGrandeza;
      }

      if (tecla == '*') {
        indiceGrandeza = (indiceGrandeza + 2) % 3;
      }
      if (tecla == '#') {
        indiceGrandeza = (indiceGrandeza + 1) % 3;
      }
      if (tecla == 'C') {
        lcd.clear();
        printCentralizado(grandezas[indiceGrandeza], 0);
        lcd.setCursor(0,1);
        lcd.print(F("Valor: "));
        inputIndex = 0;
        inputBuffer[0] = '\0';
        estado = TELA_ENTRADA_VALOR;
      }
      break;

    case TELA_ENTRADA_VALOR:
      if (tecla >= '0' && tecla <= '9') {
        if (inputIndex < 15) {
          inputBuffer[inputIndex++] = tecla;
          inputBuffer[inputIndex] = '\0';
          lcd.setCursor(7,1);
          lcd.print("        ");
          lcd.setCursor(7,1);
          lcd.print(inputBuffer);
        }
      } else if (tecla == 'D' && inputIndex > 0) { // BACKSPACE
        inputIndex--;
        inputBuffer[inputIndex] = '\0';
        lcd.setCursor(7,1);
        lcd.print("        ");
        lcd.setCursor(7,1);
        lcd.print(inputBuffer);
      } else if (tecla == 'C' && inputIndex > 0) {
        confirmarValor();
      }
      break;

    case TELA_CONFIRMACAO_GRANDEZA:
      if (!telaConfirmacaoMostrada) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("VALOR CONFIRMADO"));

        lcd.setCursor(0,1);
        if (strcmp(grandezas[indiceGrandeza], "Massa") == 0) {
          lcd.print(F("m = "));
          lcd.print(valorMassa);
        } else if (strcmp(grandezas[indiceGrandeza], "Forca") == 0) {
          lcd.print(F("F = "));
          lcd.print(valorForca);
        } else if (strcmp(grandezas[indiceGrandeza], "Aceleracao") == 0) {
          lcd.print(F("a = "));
          lcd.print(valorAceleracao);
        }
        telaConfirmacaoMostrada = true;
        ultimaTroca = millis();
      }
      if (millis() - ultimaTroca >= 1000) {
        lcd.clear();
        printCentralizado("ADICIONAR OUTRA", 0);
        lcd.setCursor(0,1);
        lcd.print(F("GRANDEZA? (C/D)"));
        estado = TELA_ADICIONAR_OUTRA;
      }
      break;

    case TELA_ADICIONAR_OUTRA:
      if (tecla == 'C') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("<- (*)    -> (#)"));
        printCentralizado(grandezas[indiceGrandeza], 1);
        estado = TELA_NAVEGACAO_GRANDEZA;
      } else if (tecla == 'D') {
        lcd.clear();
        printCentralizado("ESCOLHA A", 0);
        lcd.setCursor(0,1);
        lcd.print(F("GRANDEZA P/ CALC"));
        estado = TELA_SELECAO_CALCULO;
        ultimaTroca = millis();
      }
      break;

    case TELA_SELECAO_CALCULO:
      if (millis() - ultimaTroca >= 2000) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("APOS SELECION R"));
        lcd.setCursor(0,1);
        lcd.print(F("CONFIRME COM C"));
        estado = TELA_APOS_SELECIONAR2;
        ultimaTroca = millis();
      }
      break;

    case TELA_APOS_SELECIONAR2:
      if (millis() - ultimaTroca >= 2000) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("<- (*)    -> (#)"));
        printCentralizado(objetivos[indiceObjetivo], 1);
        estado = TELA_NAVEGACAO_OBJETIVO;
        ultimoObjetivoIndex = -1;
      }
      break;

    case TELA_NAVEGACAO_OBJETIVO:
      if (ultimoObjetivoIndex != indiceObjetivo) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.write((byte)2);
        lcd.setCursor(3,0);
        lcd.print(F("(*)"));
        lcd.setCursor(11,0);
        lcd.write((byte)3);
        lcd.setCursor(13,0);
        lcd.print(F("(#)"));

        printCentralizado(objetivos[indiceObjetivo], 1);
        ultimoObjetivoIndex = indiceObjetivo;
      }
      if (tecla == '*') {
        indiceObjetivo = (indiceObjetivo + 2) % 3;
      }
      if (tecla == '#') {
        indiceObjetivo = (indiceObjetivo + 1) % 3;
      }
      if (tecla == 'C') {
        bool podeCalcular = false;
        if (indiceObjetivo == 0) podeCalcular = flagMassa && flagAceleracao;
        else if (indiceObjetivo == 1) podeCalcular = flagForca && flagAceleracao;
        else if (indiceObjetivo == 2) podeCalcular = flagForca && flagMassa;

        if (podeCalcular) {
          if (indiceObjetivo == 0) resultado = valorMassa * valorAceleracao;
          else if (indiceObjetivo == 1) resultado = valorForca / valorAceleracao;
          else if (indiceObjetivo == 2) resultado = valorForca / valorMassa;
          estado = TELA_RESULTADO;
          ultimaTroca = millis();
        } else {
          estado = TELA_MENSAGEM_INSUFICIENTE;
          ultimaTroca = millis();
        }
      }
      break;

    case TELA_RESULTADO:
      if (!telaResultadoMostrada) {
        lcd.clear();
        printCentralizado("RESULTADO", 0);
        lcd.setCursor(0, 1);
        if (indiceObjetivo == 0) lcd.print(F("F = "));
        else if (indiceObjetivo == 1) lcd.print(F("m = "));
        else if (indiceObjetivo == 2) lcd.print(F("a = "));
        lcd.print(resultado);
        telaResultadoMostrada = true;
        ultimaTroca = millis();
      }
      if (millis() - ultimaTroca >= 1000) {
        estado = TELA_ADD_OUTRA_GRANDEZA;
        ultimaTroca = millis();
      }
      break;

    case TELA_MENSAGEM_INSUFICIENTE:
      if (!telaMensagemMostrada) {
        lcd.clear();
        printCentralizado("SEM DADOS", 0);
        printCentralizado("SUFICIENTES", 1);
        telaMensagemMostrada = true;
        ultimaTroca = millis();
      }
      if (millis() - ultimaTroca >= 2000) {
        estado = TELA_ADD_OUTRA_GRANDEZA;
        ultimaTroca = millis();
      }
      break;

    case TELA_ADD_OUTRA_GRANDEZA:
{
    static bool telaAddOutraMostrada = false;

    if (!telaAddOutraMostrada) {
        lcd.clear();
        printCentralizado("ADICION R OUTRA", 0);
        printCentralizado("GRANDEZA? (C/D)", 1);
        telaAddOutraMostrada = true;
    }

    if (tecla == 'C') {
        telaAddOutraMostrada = false; // reset para próxima vez
        resetFlagsParaSelecaoCalculo();
        estado = TELA_extra_num1;
        ultimaTroca = millis();
    } 
    else if (tecla == 'D') {
        telaAddOutraMostrada = false; // reset para próxima vez
        estado = TELA_CALC_OUTRA_GRANDEZA;
    }
}
break;

  case TELA_extra_num1:
  {
        static bool telaExtraNum1 = false;
    
    if ((!telaExtraNum1) && ((millis() - ultimaTroca >= 2000))) {
        lcd.clear();
        printCentralizado("Aguarde",0);
        printCentralizado("bits em trabalho",1);
        telaExtraNum1 = true;
        estado = TELA_extra_num1_pt2;
        ultimaTroca = millis();
    }
  }

  case TELA_extra_num1_pt2:{

    static bool telaExtraNum1Msg = false;

    if ( (!telaExtraNum1Msg) && (millis() - ultimaTroca >= 5000) ) 
    {
      lcd.clear();
      printCentralizado("Agora, use */#",0);
      printCentralizado("para navegar",1);
      estado = TELA_NAVEGACAO_GRANDEZA;
      ultimaTroca = millis();
      telaExtraNum1Msg = true;
    }
      break;

  }

  case TELA_CALC_OUTRA_GRANDEZA:
{
    static bool telaCalcOutraMostrada = false;

    if (!telaCalcOutraMostrada) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("CALCULAR OUTRA"));
        lcd.setCursor(0,1);
        lcd.print(F("GRANDEZA? (C/D)"));
        telaCalcOutraMostrada = true;
    }

    if (tecla == 'C') {
        telaCalcOutraMostrada = false; // reset para próxima vez
        resetFlagsParaSelecaoCalculo(); 
        estado = TELA_Extra_num2;
        ultimaTroca = millis();
    } 
    else if (tecla == 'D') {
    telaCalcOutraMostrada = false;
    estado = TELA_extra_num3;
    ultimaTroca = millis();
}

}
break;

    case TELA_Extra_num2: 
    {
      static bool telaExtraNum2 = false;
      static bool telaExtraNum2Msg = false;
      static bool telaExtraNum3Msg = false;

    if(!telaExtraNum2){
      lcd.clear();
      printCentralizado("Aguarde",0);
      printCentralizado("bits em trabalho",1);
      telaExtraNum2 = true;
    }
    if(((!telaExtraNum2Msg) && (millis() -ultimaTroca >= 3000))) {
      lcd.clear();
      printCentralizado("ESCOLHA QUAL",0);
      printCentralizado("GRANDEZA CALCLAR",1);
      telaExtraNum2Msg = true;
    }

    if(((!telaExtraNum3Msg) && (millis() -ultimaTroca >= 4500))) {
      lcd.clear();
      printCentralizado("CONFIRME DPS",0);
      printCentralizado("DE SELECIONAR",1);
      telaExtraNum3Msg = true;
    }

    if (millis() - ultimaTroca >= 5500) {
        estado = TELA_APOS_SELECIONAR2;
        ultimaTroca = millis();
      }

    break;

    }

    case TELA_extra_num3:{

      static bool telaExtraNum3 = false;
      

      if (((!telaExtraNum3) && (millis() -ultimaTroca >= 2000))) {
        lcd.clear();
        printCentralizado("Aguarde",0);
        printCentralizado("bits em trabalho",1);;
        telaExtraNum3 = true;
        estado = TELA_extra_num3_pt2;
        ultimaTroca = millis();
      }
         
      break;
    }
    
    case TELA_extra_num3_pt2:

        static bool telaExtraNum3msg = false;

        if (((!telaExtraNum3msg) && (millis() -ultimaTroca >= 5000))){
          lcd.clear();
          printCentralizado("BEM-VINDO",0);
          printCentralizado("C PARA CONTINUAR",1);
          estado = TELA_BEMVINDO;
          ultimaTroca = millis();
          telaExtraNum3msg = true;

      }

      break;
  }
}
