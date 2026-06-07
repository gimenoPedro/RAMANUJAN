/*******************************************************
 * PROJETO TCC - ESP32-S3
 *******************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


/***********  TEMPO PARA PASSAGEM DE ESTADOS **********/

int Δt = 500;
int Δt_primeiro = 500;   //tempo para primeiro estado, o que tem a descrição scroll do programa
int Δt_diferente = 2000;  //casos especiais

/**************** CARACTERES ESPECIAIS ****************/

byte setaCima[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000
};

byte setaBaixo[8] = {
  B00000,
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};

/**************** ENDEREÇOS I2C ****************/

#define LCD1_ADDRESS 0x27
#define LCD2_ADDRESS 0x26

LiquidCrystal_I2C lcd1(LCD1_ADDRESS, 16, 2);
LiquidCrystal_I2C lcd2(LCD2_ADDRESS, 16, 2);

/**************** PINOS I2C ****************/

#define I2C_SDA 4
#define I2C_SCL 5

/**************** LEDs ****************/

#define LED_STATUS 6

#define LED_STAGE_EXPLICACAO_PROGRAMA 15
#define LED_STAGE_EXPLICACAO_TECLADO 16
#define LED_STAGE_SELECAO_GRANDEZAS 8
#define LED_STAGE_CALCULO_GRANDEZAS 18
#define LED_STAGE_RESULTADO 17

/**************** DEFINIÇÃO TECLADO ****************/

char teclas[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte linhas[4] = { 1, 2, 42, 41 };
byte colunas[4] = { 40, 39, 38, 37 };

Keypad teclado = Keypad(makeKeymap(teclas), linhas, colunas, 4, 4);

/**************** DEFINIÇÃO DOS ESTADOS ****************/

#define ESTADO_BEM_VINDO 0
#define ESTADO_QUALQUER_TECLA 1
#define ESTADO_INSTR1 2
#define ESTADO_INSTR2 3
#define ESTADO_INSTR3 4
#define ESTADO_INSTR4 5
#define ESTADO_INSTR5 6
#define ESTADO_INSTR6 7
#define ESTADO_INSTR7 8
#define ESTADO_INSTR8 9
#define ESTADO_INSTR9 10
#define ESTADO_INSTR10 11
#define ESTADO_TECLD1 12
#define ESTADO_TECLD2 13
#define ESTADO_TECLD3 14
#define ESTADO_TECLD4 15
#define ESTADO_SELEC1 16
#define ESTADO_SELEC2 17
#define ESTADO_SELEC3 18
#define ESTADO_SELEC4 19
#define ESTADO_SELEC5 20
#define ESTADO_SELEC6 21
#define ESTADO_SELEC7 22
#define ESTADO_CALCUL1 23
#define ESTADO_CALCUL2 24
#define ESTADO_CALCUL3 25
#define ESTADO_CALCUL4 26
#define ESTADO_CALCUL5 27
#define ESTADO_RESULT1 28
#define ESTADO_RESULT2 29
#define ESTADO_INFOR1  30

int estadoAtual = ESTADO_BEM_VINDO;
int estadoAnterior = -1;

/**************** DEFINIÇÕES AUXILIARES LED ****************/

#define VERMELHO 0
#define VERDE 1

/**************** CONTROLE DE TEMPO ****************/

unsigned long tempoInicioEstado = 0;

/**************** TEXTO ROLANTE ****************/

char textoRolante[] =
  "Projeto TCC - Ramanujan: Calculadora inteligente para problemas de fisica do ensino fundamental.    ";

int tamanhoTexto = sizeof(textoRolante) - 1;
int posicaoScroll = 16;

unsigned long ultimaTrocaScroll = 0;
unsigned long intervaloScroll = 400;

/***************************************************************
 * ESCREVE COMO LINHA 32x1 (LCD1 + LCD2)
 ***************************************************************/
void escreverLinha32x1(int linha, const char* texto) {

  char buffer[33];

  for (int i = 0; i < 32; i++)
    buffer[i] = ' ';

  buffer[32] = '\0';

  for (int i = 0; i < 32 && texto[i] != '\0'; i++)
    buffer[i] = texto[i];

  lcd1.setCursor(0, linha);
  for (int i = 0; i < 16; i++)
    lcd1.print(buffer[i]);

  lcd2.setCursor(0, linha);
  for (int i = 16; i < 32; i++)
    lcd2.print(buffer[i]);
}

/***************************************************************
 * CENTRALIZA TEXTO EM 32 COLUNAS
 ***************************************************************/
void escreverCentralizado32x1(int linha, const char* texto) {

  int tamanho = strlen(texto);
  int espacos = (32 - tamanho) / 2;

  char buffer[33];

  for (int i = 0; i < 32; i++)
    buffer[i] = ' ';

  buffer[32] = '\0';

  for (int i = 0; i < tamanho && (i + espacos) < 32; i++)
    buffer[i + espacos] = texto[i];

  escreverLinha32x1(linha, buffer);
}
/***************************************************************
 * CONTROLE LED VERDE/VERMELHO
 ***************************************************************/
void LEDstatus(int cor) {

  if (cor == VERDE) {
    digitalWrite(LED_STATUS, HIGH);
  } else if (cor == VERMELHO) {
    digitalWrite(LED_STATUS, LOW);
  }
}
/***************************************************************
 * CONTROLE DOS LEDs AZUIS (1 a 5)
 ***************************************************************/
void LEDazul(int numero) {
  // Primeiro desliga todos
  digitalWrite(LED_STAGE_EXPLICACAO_PROGRAMA, LOW);
  digitalWrite(LED_STAGE_EXPLICACAO_TECLADO, LOW);
  digitalWrite(LED_STAGE_SELECAO_GRANDEZAS, LOW);
  digitalWrite(LED_STAGE_CALCULO_GRANDEZAS, LOW);
  digitalWrite(LED_STAGE_RESULTADO, LOW);
  // Liga apenas o escolhido
  switch (numero) {
    case 1:
      digitalWrite(LED_STAGE_EXPLICACAO_PROGRAMA, HIGH);
      break;
    case 2:
      digitalWrite(LED_STAGE_EXPLICACAO_TECLADO, HIGH);
      break;
    case 3:
      digitalWrite(LED_STAGE_SELECAO_GRANDEZAS, HIGH);
      break;
    case 4:
      digitalWrite(LED_STAGE_CALCULO_GRANDEZAS, HIGH);
      break;
    case 5:
      digitalWrite(LED_STAGE_RESULTADO, HIGH);
      break;
    case 0:
    default:
      // nenhum ligado
      break;
  }
}

/**************************************************
*LISTA DE GRANDEZAS
**************************************************/

/*************** LISTA DE GRANDEZAS ***************/

const char* lista_grandezas[] = {
  "Forca",
  "Massa",
  "Aceleracao",
  "Deslocamnto",
  "Trabalho",
  "PosicaoIni",
  "PosicaoFin",
  "VarPosicao",
  "TempoIni",
  "TempoFin",
  "VarTempo",
  "VelocidMed",
  "VelocidIni",
  "VelocidFin",
  "VarVelocid",
  "AceleraMed",
  "Forca Peso",
  "Potencia",
  "Pressao",
  "Area",
  "EnergCinet",
  "Velocidade",
  "Altura",
  "EnergPtGr",
  "EnergMec",
  "Densidade",
  "Volume",
  "VolumeDesl",
  "Empuxo",
  "TemperatC",
  "TemperatF",
  "TemperatK",
  "VolumeIni",
  "VolumeFin",
  "AreaIni",
  "Areafin",
  "ComprIni",
  "ComprFin",
  "VarVolum",
  "VarArea",
  "VarCompr",
  "TemperIni",
  "TemperFin",
  "VarTemper",
  "CoefDltL",
  "CoefDltA",
  "CoefDltV",
  "QtdCalorS",
  "CalorEsp",
  "QtdCalorL",
  "CalorLat",
  "CapacCalr",
  "ComprOnda",
  "Frequencia",
  "Periodo",
  "CargaIni",
  "CargaFin",
  "VarCarga",
  "CorrentElet",
  "Resistencia",
  "Comprimento",
  "Resistividd",
  "TensaoElet",
  "PotElet"
};





const char* lista_simbolos[] = {
  "F",
  "m",
  "a",
  "d",
  "W",
  "So",
  "S",
  "dS",
  "to",
  "t",
  "dt",
  "Vm",
  "Vo",
  "V",
  "dV",
  "am",
  "P",
  "Pot",
  "Pres",
  "A",
  "Ec",
  "v",
  "h",
  "Epg",
  "Em",
  "d",
  "Vol",
  "Vd",
  "E",
  "Tc",
  "Tf",
  "Tk",
  "Vi",
  "Vf",
  "Ai",
  "Af",
  "Li",
  "Lf",
  "dVol",
  "dA",
  "dL",
  "Ti",
  "Tf",
  "dT",
  "alfa",
  "beta",
  "gama",
  "Qs",
  "c",
  "Ql",
  "L",
  "C",
  "lamb",
  "f",
  "T",
  "Qi",
  "Qf",
  "dQ",
  "i",
  "R",
  "l",
  "p",
  "U",
  "PotEl"

};

const int TOTAL_GRANDEZAS = sizeof(lista_grandezas) / sizeof(lista_grandezas[0]);

int indiceGrandeza = 0;
int indiceAnterior = -1;  // controla atualização da tela\

float lista_valores[TOTAL_GRANDEZAS] = { 0 };
String listaResultados[TOTAL_GRANDEZAS];
int totalResultados = 0;


/***************************************************************
 * CONTROLE/ARMAZENAGEM/CALCULO GRANDEZAS
 ***************************************************************/
//estado 20
String valorDigitado = "";  // armazena número digitado

int faseCalculo = 0;
float resultadoCalculado = 0;

int indiceCalculo = 0;
int indiceAnteriorCalculo = -1;

int equacaoSelecionada = -1;

bool emCalculo[TOTAL_GRANDEZAS] = {false};

bool grandezaAtiva[TOTAL_GRANDEZAS] = { false };

struct Termo {
  float constante;
  int vars[4];
  float potencias[4];
  int numVars;
};

struct Equacao {
  int resultado;    // índice da grandeza resultado
  Termo termos[4];  // lista de termos
  int numTermos;    
};

#define TOTAL_EQUACOES 113

Equacao equacoes[TOTAL_EQUACOES] = {

  // =========================
  // F = m * a
  // =========================
  {
    0, // F
    {
      {1.0, {1,2}, {1,1}, 2}   // m^1 * a^1
    },
    1
  },

  // =========================
  // a = F / m
  // =========================
  {
    2, // a
    {
      {1.0, {0,1}, {1,-1}, 2}  // F^1 * m^-1
    },
    1
  },

  // =========================
  // m = F / a
  // =========================
  {
    1, // m
    {
      {1.0, {0,2}, {1,-1}, 2}  // F^1 * a^-1
    },
    1
  },

  // =========================
  // W = F * d
  // =========================
  {
    4, // W
    {
      {1.0, {0,3}, {1,1}, 2}   // F^1 * d^1
    },
    1
  },

  // =========================
  // d = W / F   
  // =========================
  {
    3, // d
    {
      {1.0, {4,0}, {1,-1}, 2}  // W^1 * F^-1
    },
    1
  },

  // =========================
// F = W / d   
// =========================
{
  0, // F
  {
    {1.0, {4,3}, {1,-1}, 2}  // W^1 * d^-1
  },
  1
},

{
  7, // dS
  {
    {1.0, {6}, {1}, 1},   // +S
    {-1.0, {5}, {1}, 1}   // -So
  },
  2
},

{
  6, // S
  {
    {1.0, {7}, {1}, 1},   // +dS
    {1.0, {5}, {1}, 1}    // +So
  },
  2
},

{
  5, // So
  {
    {1.0, {6}, {1}, 1},   // +S
    {-1.0, {7}, {1}, 1}   // -dS
  },
  2
},

{
  10, // dt
  {
    {1.0, {9}, {1}, 1},   // +t
    {-1.0, {8}, {1}, 1}   // -to
  },
  2
},

{
  9, // t
  {
    {1.0, {10}, {1}, 1},  // +dt
    {1.0, {8}, {1}, 1}    // +to
  },
  2
},

{
  8, // to
  {
    {1.0, {9}, {1}, 1},   // +t
    {-1.0, {10}, {1}, 1}  // -dt
  },
  2
},

{
  11, // vm  
  {
    {1.0, {7,10}, {1,-1}, 2}  // dS^1 * dt^-1
  },
  1
},

{
  7, // dS
  {
    {1.0, {11,10}, {1,1}, 2}  // vm^1 * dt^1
  },
  1
},

{
  10, // dt
  {
    {1.0, {7,11}, {1,-1}, 2}  // dS^1 * vm^-1
  },
  1
},

{
  15, // am
  {
    {1.0, {14,10}, {1,-1}, 2}  // dV^1 * dt^-1
  },
  1
},


{
  14, // dV
  {
    {1.0, {15,10}, {1,1}, 2}  // am^1 * dt^1
  },
  1
},


{
  10, // dt
  {
    {1.0, {14,15}, {1,-1}, 2}  // dV^1 * am^-1
  },
  1
},


{
  14, // dV
  {
    {1.0, {13}, {1}, 1},   // +V
    {-1.0, {12}, {1}, 1}   // -Vo
  },
  2
},


{
  13, // V
  {
    {1.0, {14}, {1}, 1},   // +dV
    {1.0, {12}, {1}, 1}    // +Vo
  },
  2
},


{
  12, // Vo
  {
    {1.0, {13}, {1}, 1},   // +V
    {-1.0, {14}, {1}, 1}   // -dV
  },
  2
},


{
  6, // S
  {
    {1.0, {5}, {1}, 1},             // So
    {1.0, {12,10}, {1,1}, 2},       // Vo * dt
    {0.5, {15,10}, {1,2}, 2}         // (1/2) * am * dt^2
  },
  3
},


{
  5, // So
  {
    {1.0, {6}, {1}, 1},              // S
    {-1.0, {12,10}, {1,1}, 2},      // -Vo * dt
    {-0.5, {15,10}, {1,2}, 2}       // -(1/2) * am * dt^2
  },
  3
},


{
  12, // Vo
  {
    {1.0, {6,10}, {1,-1}, 2},        // S * dt^-1
    {-1.0, {5,10}, {1,-1}, 2},       // -So * dt^-1
    {-0.5, {15,10}, {1,1}, 2}        // -(1/2) * am * dt
  },
  3
},


{
  15, // am
  {
    {2.0, {6,10}, {1,-2}, 2},        // 2 * S * dt^-2
    {-2.0, {5,10}, {1,-2}, 2},       // -2 * So * dt^-2
    {-2.0, {12,10}, {1,-1}, 2}       // -2 * Vo * dt^-1
  },
  3
},

// =========================
// P = m * g  (g = 10)
// =========================
{
  16, // P
  {
    {10.0, {1}, {1}, 1}  // 10 * m^1
  },
  1
},


// =========================
// m = P / g  (g = 10)
// =========================
{
  1, // m
  {
    {0.1, {16}, {1}, 1}  // 0.1 * P^1
  },
  1
},


// =========================
// Pot = W / dt
// =========================
{
  17, // Pot
  {
    {1.0, {4,10}, {1,-1}, 2}  // W^1 * dt^-1
  },
  1
},


// =========================
// W = Pot * dt
// =========================
{
  4, // W
  {
    {1.0, {17,10}, {1,1}, 2}  // Pot^1 * dt^1
  },
  1
},


// =========================
// dt = W / Pot
// =========================
{
  10, // dt
  {
    {1.0, {4,17}, {1,-1}, 2}  // W^1 * Pot^-1
  },
  1
},

// =========================
// Pres = F / A
// =========================
{
  18, // Pres
  {
    {1.0, {0,19}, {1,-1}, 2}  // F^1 * A^-1
  },
  1
},


// =========================
// F = Pres * A
// =========================
{
  0, // F
  {
    {1.0, {18,19}, {1,1}, 2}  // Pres^1 * A^1
  },
  1
},


// =========================
// A = F / Pres
// =========================
{
  19, // A
  {
    {1.0, {0,18}, {1,-1}, 2}  // F^1 * Pres^-1
  },
  1
},


// =========================
// Ec = (1/2) * m * v^2
// =========================
{
  20, // Ec
  {
    {0.5, {1,21}, {1,2}, 2}  // 0.5 * m^1 * v^2
  },
  1
},


// =========================
// m = 2 * Ec / v^2
// =========================
{
  1, // m
  {
    {2.0, {20,21}, {1,-2}, 2}  // 2 * Ec^1 * v^-2
  },
  1
},


// =========================
// Epg = m * g * h  (g = 10)
// =========================
{
  23, // Epg
  {
    {10.0, {1,22}, {1,1}, 2}  // 10 * m^1 * h^1
  },
  1
},

// =========================
// m = Epg / (g * h)
// =========================
{
  1, // m
  {
    {0.1, {23,22}, {1,-1}, 2}  // 0.1 * Epg^1 * h^-1
  },
  1
},


// =========================
// h = Epg / (g * m)
// =========================
{
  22, // h
  {
    {0.1, {23,1}, {1,-1}, 2}  // 0.1 * Epg^1 * m^-1
  },
  1
},

// =========================
// Em = Ec + Epg
// =========================
{
  24, // Em
  {
    {1.0, {20}, {1}, 1},   // +Ec
    {1.0, {23}, {1}, 1}    // +Epg
  },
  2
},


// =========================
// Ec = Em - Epg
// =========================
{
  20, // Ec
  {
    {1.0, {24}, {1}, 1},    // +Em
    {-1.0, {23}, {1}, 1}    // -Epg
  },
  2
},


// =========================
// Epg = Em - Ec
// =========================
{
  23, // Epg
  {
    {1.0, {24}, {1}, 1},    // +Em
    {-1.0, {20}, {1}, 1}    // -Ec
  },
  2
},


// =========================
// d = m / Vol
// =========================
{
  25, // d (densidade)
  {
    {1.0, {1,26}, {1,-1}, 2}  // m^1 * Vol^-1
  },
  1
},


// =========================
// m = d * Vol
// =========================
{
  1, // m
  {
    {1.0, {25,26}, {1,1}, 2}  // d^1 * Vol^1
  },
  1
},


// =========================
// Vol = m / d
// =========================
{
  26, // Vol
  {
    {1.0, {1,25}, {1,-1}, 2}  // m^1 * d^-1
  },
  1
},

// =========================
// E = d * Vd * g  (g = 10)
// =========================
{
  28, // E (Empuxo)
  {
    {10.0, {25,27}, {1,1}, 2}  // 10 * d^1 * Vd^1
  },
  1
},


// =========================
// d = E / (g * Vd)
// =========================
{
  25, // d (densidade)
  {
    {0.1, {28,27}, {1,-1}, 2}  // 0.1 * E^1 * Vd^-1
  },
  1
},


// =========================
// Vd = E / (g * d)
// =========================
{
  27, // Vd (volume deslocado)
  {
    {0.1, {28,25}, {1,-1}, 2}  // 0.1 * E^1 * d^-1
  },
  1
},

{
  30, // Tf
  {
    {1.8, {29}, {1}, 1},   // (9/5)*Tc
    {32.0, {}, {}, 0}      // +32
  },
  2
},


{
  29, // Tc
  {
    {5.0/9.0, {30}, {1}, 1},        // (5/9)*Tf
    {-32.0 * (5.0/9.0), {}, {}, 0}  // -(32 * 5/9)
  },
  2
},


{
  31, // Tk
  {
    {1.0, {29}, {1}, 1},   // Tc
    {273.0, {}, {}, 0}     // +273
  },
  2
},



{
  29, // Tc
  {
    {1.0, {31}, {1}, 1},   // Tk
    {-273.0, {}, {}, 0}    // -273
  },
  2
},


{
  40, // dL
  {
    {1.0, {37}, {1}, 1},   // +Lf
    {-1.0, {36}, {1}, 1}   // -Li
  },
  2
},


{
  37, // Lf
  {
    {1.0, {40}, {1}, 1},   // +dL
    {1.0, {36}, {1}, 1}    // +Li
  },
  2
},


{
  36, // Li
  {
    {1.0, {37}, {1}, 1},   // +Lf
    {-1.0, {40}, {1}, 1}   // -dL
  },
  2
},



{
  39, // dA
  {
    {1.0, {35}, {1}, 1},   // +Af
    {-1.0, {34}, {1}, 1}   // -Ai
  },
  2
},


{
  35, // Af
  {
    {1.0, {39}, {1}, 1},   // +dA
    {1.0, {34}, {1}, 1}    // +Ai
  },
  2
},


{
  34, // Ai
  {
    {1.0, {35}, {1}, 1},   // +Af
    {-1.0, {39}, {1}, 1}   // -dA
  },
  2
},


{
  38, // dV
  {
    {1.0, {33}, {1}, 1},   // +Vf
    {-1.0, {32}, {1}, 1}   // -Vi
  },
  2
},


{
  33, // Vf
  {
    {1.0, {38}, {1}, 1},   // +dV
    {1.0, {32}, {1}, 1}    // +Vi
  },
  2
},


{
  32, // Vi
  {
    {1.0, {33}, {1}, 1},   // +Vf
    {-1.0, {38}, {1}, 1}   // -dV
  },
  2
},

{
  43, // dT
  {
    { 1.0, {42}, {1}, 1 },   // +Tf
    {-1.0, {41}, {1}, 1 }    // -Ti
  },
  2
},

{
  42, // Tf
  {
    {1.0, {43}, {1}, 1},   // +dT
    {1.0, {42}, {1}, 1}    // +Ti
  },
  2
},


{
  41, // Ti
  {
    { 1.0, {42}, {1}, 1 },   // +Tf
    {-1.0, {43}, {1}, 1 }    // -dT
  },
  2
},

// =========================
// dL = Li * alfa * dT
// =========================
{
  40, // dL
  {
    {1.0, {36,44,43}, {1,1,1}, 3}  // Li^1 * alfa^1 * dT^1
  },
  1
},

// =========================
// Li = dL / (alfa * dT)
// =========================
{
  36, // Li
  {
    {1.0, {40,44,43}, {1,-1,-1}, 3}  // dL^1 * alfa^-1 * dT^-1
  },
  1
},

// =========================
// alfa = dL / (Li * dT)
// =========================
{
  44, // alfa
  {
    {1.0, {40,36,43}, {1,-1,-1}, 3}  // dL^1 * Li^-1 * dT^-1
  },
  1
},

// =========================
// dT = dL / (Li * alfa)
// =========================
{
  43, // dT
  {
    {1.0, {40,36,44}, {1,-1,-1}, 3}  // dL^1 * Li^-1 * alfa^-1
  },
  1
},

// =========================
// dA = Ai * beta * dT
// =========================
{
  39, // dA
  {
    {1.0, {34,45,43}, {1,1,1}, 3}
  },
  1
},

// =========================
// Ai = dA / (beta * dT)
// =========================
{
  34, // Ai
  {
    {1.0, {39,45,43}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// beta = dA / (Ai * dT)
// =========================
{
  45, // beta
  {
    {1.0, {39,34,43}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// dT = dA / (Ai * beta)
// =========================
{
  43, // dT
  {
    {1.0, {39,34,45}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// dVol = Vi * gama * dT
// =========================
{
  38, // dVol
  {
    {1.0, {32,46,43}, {1,1,1}, 3}
  },
  1
},

// =========================
// Vi = dVol / (gama * dT)
// =========================
{
  32, // Vi
  {
    {1.0, {38,46,43}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// gama = dVol / (Vi * dT)
// =========================
{
  46, // gama
  {
    {1.0, {38,32,43}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// dT = dVol / (Vi * gama)
// =========================
{
  43, // dT
  {
    {1.0, {38,32,46}, {1,-1,-1}, 3}
  },
  1
},


// =========================
// alfa = beta / 2
// =========================
{
  44, // alfa
  {
    {0.5, {45}, {1}, 1}  // 0.5 * beta
  },
  1
},

// =========================
// beta = 2 * alfa
// =========================
{
  45, // beta
  {
    {2.0, {44}, {1}, 1}  // 2 * alfa
  },
  1
},

// =========================
// alfa = gama / 3
// =========================
{
  44, // alfa
  {
    {1.0/3.0, {46}, {1}, 1}  // (1/3) * gama
  },
  1
},

// =========================
// gama = 3 * alfa
// =========================
{
  46, // gama
  {
    {3.0, {44}, {1}, 1}  // 3 * alfa
  },
  1
},

// =========================
// Qs = m * c * dT
// =========================
{
  47, // Qs
  {
    {1.0, {1,48,43}, {1,1,1}, 3}
  },
  1
},

// =========================
// m = Qs / (c * dT)
// =========================
{
  1, // m
  {
    {1.0, {47,48,43}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// c = Qs / (m * dT)
// =========================
{
  48, // c
  {
    {1.0, {47,1,43}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// dT = Qs / (m * c)
// =========================
{
  43, // dT
  {
    {1.0, {47,1,48}, {1,-1,-1}, 3}
  },
  1
},

// =========================
// Ql = m * L
// =========================
{
  49, // Ql
  {
    {1.0, {1,50}, {1,1}, 2}
  },
  1
},

// =========================
// m = Ql / L
// =========================
{
  1, // m
  {
    {1.0, {49,50}, {1,-1}, 2}
  },
  1
},

// =========================
// L = Ql / m
// =========================
{
  50, // L
  {
    {1.0, {49,1}, {1,-1}, 2}
  },
  1
},

// =========================
// C = m * c
// =========================
{
  51, // C
  {
    {1.0, {1,48}, {1,1}, 2}
  },
  1
},

// =========================
// m = C / c
// =========================
{
  1, // m
  {
    {1.0, {51,48}, {1,-1}, 2}
  },
  1
},

// =========================
// c = C / m
// =========================
{
  48, // c
  {
    {1.0, {51,1}, {1,-1}, 2}
  },
  1
},

// =========================
// Qs = C * dT
// =========================
{
  47, // Qs
  {
    {1.0, {51,43}, {1,1}, 2}
  },
  1
},

// =========================
// C = Qs / dT
// =========================
{
  51, // C
  {
    {1.0, {47,43}, {1,-1}, 2}
  },
  1
},

// =========================
// dT = Qs / C
// =========================
{
  43, // dT
  {
    {1.0, {47,51}, {1,-1}, 2}
  },
  1
},

// =========================
// v = lamb * f
// =========================
{
  21, // v
  {
    {1.0, {52,53}, {1,1}, 2}
  },
  1
},

// =========================
// lamb = v / f
// =========================
{
  52, // lamb
  {
    {1.0, {21,53}, {1,-1}, 2}
  },
  1
},

// =========================
// f = v / lamb
// =========================
{
  53, // f
  {
    {1.0, {21,52}, {1,-1}, 2}
  },
  1
},


// =========================
// f = 1 / T
// =========================
{
  53, // f
  {
    {1.0, {54}, {-1}, 1}
  },
  1
},

// =========================
// T = 1 / f
// =========================
{
  54, // T
  {
    {1.0, {53}, {-1}, 1}
  },
  1
},

// =========================
// dQ = Qf - Qi
// =========================
{
  57, // dQ
  {
    {1.0, {56}, {1}, 1},   // +Qf
    {-1.0, {55}, {1}, 1}   // -Qi
  },
  2
},

// =========================
// Qf = dQ + Qi
// =========================
{
  56, // Qf
  {
    {1.0, {57}, {1}, 1},   // +dQ
    {1.0, {55}, {1}, 1}    // +Qi
  },
  2
},

// =========================
// Qi = Qf - dQ
// =========================
{
  55, // Qi
  {
    {1.0, {56}, {1}, 1},   // +Qf
    {-1.0, {57}, {1}, 1}   // -dQ
  },
  2
},

// =========================
// i = dQ / dt
// =========================
{
  58, // i
  {
    {1.0, {57,10}, {1,-1}, 2}  // dQ^1 * dt^-1
  },
  1
},

// =========================
// dQ = i * dt
// =========================
{
  57, // dQ
  {
    {1.0, {58,10}, {1,1}, 2}  // i^1 * dt^1
  },
  1
},

// =========================
// dt = dQ / i
// =========================
{
  10, // dt
  {
    {1.0, {57,58}, {1,-1}, 2}  // dQ^1 * i^-1
  },
  1
},


// =========================
// R = l * p / A
// =========================
{
  59, // R
  {
    {1.0, {60,61,19}, {1,1,-1}, 3}   // l^1 * p^1 * A^-1
  },
  1
},

// =========================
// l = R * A / p
// =========================
{
  60, // l
  {
    {1.0, {59,19,61}, {1,1,-1}, 3}   // R^1 * A^1 * p^-1
  },
  1
},

// =========================
// p = R * A / l
// =========================
{
  61, // p
  {
    {1.0, {59,19,60}, {1,1,-1}, 3}   // R^1 * A^1 * l^-1
  },
  1
},

// =========================
// A = l * p / R
// =========================
{
  19, // A
  {
    {1.0, {60,61,59}, {1,1,-1}, 3}   // l^1 * p^1 * R^-1
  },
  1
},


// =========================
// U = R * i
// =========================
{
  62, // U
  {
    {1.0, {59,58}, {1,1}, 2}   // R^1 * i^1
  },
  1
},

// =========================
// R = U / i
// =========================
{
  59, // R
  {
    {1.0, {62,58}, {1,-1}, 2}   // U^1 * i^-1
  },
  1
},

// =========================
// i = U / R
// =========================
{
  58, // i
  {
    {1.0, {62,59}, {1,-1}, 2}   // U^1 * R^-1
  },
  1
},

// =========================
// PotEl = U * i
// =========================
{
  63, // PotEl
  {
    {1.0, {62,58}, {1,1}, 2}   // U^1 * i^1
  },
  1
},

// =========================
// U = PotEl / i
// =========================
{
  62, // U
  {
    {1.0, {63,58}, {1,-1}, 2}   // PotEl^1 * i^-1
  },
  1
},

// =========================
// i = PotEl / U
// =========================
{
  58, // i
  {
    {1.0, {63,62}, {1,-1}, 2}   // PotEl^1 * U^-1
  },
  1
},

};

bool podeCalcular(int g) {
  //evita recursão infinita 
  if (emCalculo[g]) {
    return false;
  }
  emCalculo[g] = true;
  for (int i = 0; i < TOTAL_EQUACOES; i++) {
    if (equacoes[i].resultado == g) {
      bool pode = true;
      for (int t = 0; t < equacoes[i].numTermos; t++) {
        for (int v = 0; v < equacoes[i].termos[t].numVars; v++) {
          int var = equacoes[i].termos[t].vars[v];
          // se a variável ainda não foi definida
          if (!grandezaAtiva[var]) {
            // tenta ver se dá pra calcular ela
            if (!podeCalcular(var)) {
              pode = false;
              break;
            }
          }
        }
        if (!pode) break;
      }
      // se encontrou uma equação válida, já retorna
      if (pode) {
        emCalculo[g] = false;
        return true;
      }
    }
  }

  //  libera antes de sair
  emCalculo[g] = false;

  return false;
}

void mostrarValoresAtuais(){
  lcd2.setCursor(0,0);
  int coluna = 0;
  for(int i = 0; i < TOTAL_GRANDEZAS; i++){
    if(grandezaAtiva[i]){
      char buffer[10];
      if(lista_valores[i] == (int)lista_valores[i]){
        snprintf(buffer, sizeof(buffer), "%s=%d ",
        lista_simbolos[i],
        (int)lista_valores[i]); }
      else{
        snprintf(buffer, sizeof(buffer), "%s=%.2f ",
        lista_simbolos[i],
        lista_valores[i]); }
      // evita estourar 16 colunas
      if(coluna + strlen(buffer) > 16){
        break; }
      lcd2.print(buffer);
      coluna += strlen(buffer); } } }

float calcularGrandeza(int g){
  if (emCalculo[g]) {   // evita recursão infinita
    return NAN;}
  emCalculo[g] = true;
  for(int i = 0; i < TOTAL_EQUACOES; i++){
    if(equacoes[i].resultado == g){
      float resultado = 0;
      bool pode = true;
      for(int t = 0; t < equacoes[i].numTermos; t++){
        float termoValor = equacoes[i].termos[t].constante;
        for(int v = 0; v < equacoes[i].termos[t].numVars; v++){
          int var = equacoes[i].termos[t].vars[v];
          float pot = equacoes[i].termos[t].potencias[v];
          if(!grandezaAtiva[var]){  // se não tiver valor, tenta calcular recursivamente
            if(podeCalcular(var)){
              lista_valores[var] = calcularGrandeza(var);
              grandezaAtiva[var] = true; }
            else {
              pode = false;
              break; } }
          if (isnan(lista_valores[var])) { //proteção 1: valor inválido 
            emCalculo[g] = false;
            return NAN; }
          if (lista_valores[var] == 0 && pot < 0) {  //proteção 2: divisão por zero 
            emCalculo[g] = false;
            return NAN; }
          termoValor *= pow(lista_valores[var], pot); }
        if(!pode) break;
        resultado += termoValor;}
      if(pode){
        emCalculo[g] = false;
        return resultado; } } }
  emCalculo[g] = false;
  return NAN; }

void resetSistema() {

  // Zera valores das grandezas
  for (int i = 0; i < TOTAL_GRANDEZAS; i++) {
    lista_valores[i] = 0;
    grandezaAtiva[i] = false;
    emCalculo[i] = false; }
  // Limpa resultados
  for (int i = 0; i < TOTAL_GRANDEZAS; i++) {
    listaResultados[i] = ""; }

  totalResultados = 0;

  // Reset de navegação
  indiceGrandeza = 0;
  indiceCalculo = 0;
  indiceAnterior = -1;
  indiceAnteriorCalculo = -1;

  // Reset auxiliares
  valorDigitado = "";
  resultadoCalculado = 0;
  faseCalculo = 0;
}


/***************************************************************
 * SETUP
 ***************************************************************/
void setup() {

  Wire.begin(I2C_SDA, I2C_SCL);

  lcd1.init();
  lcd1.backlight();

  lcd2.init();
  lcd2.backlight();

  lcd1.createChar(0, setaCima);
  lcd1.createChar(1, setaBaixo);

  lcd2.createChar(0, setaCima);
  lcd2.createChar(1, setaBaixo);

  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_STAGE_EXPLICACAO_PROGRAMA, OUTPUT);
  pinMode(LED_STAGE_EXPLICACAO_TECLADO, OUTPUT);
  pinMode(LED_STAGE_SELECAO_GRANDEZAS, OUTPUT);
  pinMode(LED_STAGE_CALCULO_GRANDEZAS, OUTPUT);
  pinMode(LED_STAGE_RESULTADO, OUTPUT);

  // 🔴 Começa com tudo desligado
  LEDstatus(VERMELHO);
  LEDazul(0);  // 0 = nenhum LED azul ligado

  lcd1.clear();
  lcd2.clear();

  estadoAtual = ESTADO_BEM_VINDO;
  estadoAnterior = -1;  // força executar ação de entrada

  tempoInicioEstado = millis();
}

/******************************************************************************************************************************************************************
 * LOOP PRINCIPAL
 *****************************************************************************************************************************************************************/
void loop() {

  char tecla = teclado.getKey();

  switch (estadoAtual) {

      /************************** ESTADO 1 **************************/
    case ESTADO_BEM_VINDO:
      // ===== AÇÃO DE ENTRADA  =====
      if (estadoAtual != estadoAnterior) {
        estadoAnterior = estadoAtual;
        lcd1.clear();
        lcd2.clear();
        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul
        tempoInicioEstado = millis();
        ultimaTrocaScroll = millis();
        posicaoScroll = 0; }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====
      escreverCentralizado32x1(0, "SEJA BEM-VINDO, AGUARDE");
      if (millis() - ultimaTrocaScroll >= intervaloScroll) {
        ultimaTrocaScroll = millis();
        char buffer[33];
        for (int i = 0; i < 32; i++) {
          int indice = (posicaoScroll + i) % tamanhoTexto;
          buffer[i] = textoRolante[indice]; }
        buffer[32] = '\0';
        escreverLinha32x1(1, buffer);
        posicaoScroll++;
        if (posicaoScroll >= tamanhoTexto)
          posicaoScroll = 0; }

      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt_primeiro) {
        estadoAtual = ESTADO_QUALQUER_TECLA; }
      break;

    /************************** ESTADO 2 **************************/
    case ESTADO_QUALQUER_TECLA:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERDE);  // 🟢 LED verde
        LEDazul(1);        // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====


      escreverLinha32x1(0, "Pressione qualquer tecla para");
      escreverLinha32x1(1, "continuar e ver as instrucoes");

      // ===== TRANSIÇÃO DE ESTADO =====

      if (tecla) {

        estadoAtual = ESTADO_INSTR1;
      }

      break;

    /************************** ESTADO 3 **************************/
    case ESTADO_INSTR1:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====


      escreverLinha32x1(0, "Como dito, temos o objetivo");
      escreverLinha32x1(1, "resolver problemas de fisica");

      // ===== TRANSIÇÃO DE ESTADO =====

      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR2;
      }

      break;


    /************************** ESTADO 4 **************************/
    case ESTADO_INSTR2:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Para isso, usaremos estes 2");
      escreverLinha32x1(1, "displays para   comunicacao");


      // ===== TRANSIÇÃO DE ESTADO =====

      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR3;
      }

      break;


    /************************** ESTADO 5 **************************/
    case ESTADO_INSTR3:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Alem disso, temos 2 LEDs, verde");
      escreverLinha32x1(1, "e vermelho como indicadores");


      // ===== TRANSIÇÃO DE ESTADO =====

      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR4;
      }

      break;



    /************************** ESTADO 6 **************************/
    case ESTADO_INSTR4:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Se o LED verde   estiver");
      escreverLinha32x1(1, "ligado voce deve interagir");


      // ===== TRANSIÇÃO DE ESTADO =====

      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR5;
      }

      break;


    /************************** ESTADO 7 **************************/
    case ESTADO_INSTR5:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Para interagir  com o prorgama,");
      escreverLinha32x1(1, "ha este teclado 4x4");


      // ===== TRANSIÇÃO DE ESTADO =====

      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR6;
      }

      break;

      /************************** ESTADO 8 **************************/
    case ESTADO_INSTR6:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Se o LED vermelho estiver ligado");
      escreverLinha32x1(1, ",como agora,apenas aguarde");


      // ===== TRANSIÇÃO DE ESTADO =====

      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR7;
      }

      break;

      /************************** ESTADO 9 **************************/
    case ESTADO_INSTR7:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Os LEDs azuis, diferentemente,");
      escreverLinha32x1(1, "indicam a fase atual do programa");


      // ===== TRANSIÇÃO DE ESTADO =====

      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR8;
      }

      break;

    /************************** ESTADO 10 **************************/
    case ESTADO_INSTR8:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Agora, vamos detalhar como, de");
      escreverLinha32x1(1, "fato o programa funciona:");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR9;
      }

      break;

      /************************** ESTADO 11 **************************/
    case ESTADO_INSTR9:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "1o, insira os valores conhecidos");
      escreverLinha32x1(1, " do enunciado usando o teclado");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_INSTR10;
      }


      break;

      /************************** ESTADO 12 **************************/
    case ESTADO_INSTR10:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(1);           // 🔵 Primeiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Entao, SELECIONE A GRANDEZA e");
      escreverLinha32x1(1, "dps SELECIONE seu VALOR NUMERICO");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_TECLD1;
      }


      break;

      /************************** ESTADO 13 **************************/
    case ESTADO_TECLD1:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(2);           // 🔵 Segundo LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Para isso vc usara o TECLADO,");
      escreverLinha32x1(1, "sendo que temos TECLAS ESPECIAIS");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_TECLD2;
      }

      break;

      /************************** ESTADO 14 **************************/
    case ESTADO_TECLD2:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(2);           // 🔵 Segundo LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      lcd1.setCursor(0, 0);
      lcd1.print("Como setas ALTO");
      lcd1.write(byte(0));  // ↑
      lcd2.setCursor(0, 0);
      lcd2.print("e BAIXO ");
      lcd2.write(byte(1));  // ↓
      escreverLinha32x1(1, "usaremos A e B, respectivamente");


      // ===== TRANSIÇÃO DE ESTADO =====


      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_TECLD3;
      }
      break;

      /************************** ESTADO 15 **************************/
    case ESTADO_TECLD3:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(2);           // 🔵 Segundo LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Como CONFIRMAR/SIM e DELETAR/NAO");
      escreverLinha32x1(1, "usaremos C e D, respectivamente");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt) {

        estadoAtual = ESTADO_TECLD4;
      }
      break;


      /************************** ESTADO 16 **************************/
    case ESTADO_TECLD4:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERDE);  // 🟢 LED Verde
        LEDazul(2);        // 🔵 Segundo LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Quer ver as instrucoes de novo?");
      escreverLinha32x1(1, "SIM (tecla C)     NAO (tecla D)");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (tecla == 'C') {
        estadoAtual = ESTADO_INSTR1;
      } else if (tecla == 'D') {

        estadoAtual = ESTADO_SELEC1;
      }

      break;


      /************************** ESTADO 17 **************************/
    case ESTADO_SELEC1:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(3);           // 🔵 Terceiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====


      escreverLinha32x1(0, "Lista de GRANDEZAS sera exibida");
      lcd1.setCursor(0, 1);
      lcd1.print("use");
      lcd1.setCursor(4, 1);
      lcd1.write(byte(0));  // Seta para cima
      lcd1.setCursor(5, 1);
      lcd1.print("/");
      lcd1.setCursor(6, 1);
      lcd1.write(byte(1));  // Seta para baixo
      lcd1.setCursor(8, 1);
      lcd1.print("(A/B)");
      lcd2.setCursor(0, 1);
      lcd2.print("para selecionar");



      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt_primeiro) {

        estadoAtual = ESTADO_SELEC2;
      }

      break;


      /************************** ESTADO 18 **************************/
    case ESTADO_SELEC2:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(3);           // 🔵 Terceiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Pressione C para confirmar e");
      escreverLinha32x1(1, "depois inserir o valor numerico");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt_primeiro) {

        estadoAtual = ESTADO_INFOR1;
      }

      break;


      /************************** ESTADO 19 **************************/
    case ESTADO_SELEC3:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERDE);  // 🟢  LED verde
        LEDazul(3);        // 🔵 Terceiro LED azul

        tempoInicioEstado = millis();

        indiceAnterior = -1;
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      lcd2.setCursor(0, 0);
      lcd2.print("Selecionar: A/B");
      lcd2.setCursor(0, 1);
      lcd2.print("confirmar: C");


      if (tecla == 'A') {  // subir

        indiceGrandeza--;

        if (indiceGrandeza < 0)
          indiceGrandeza = TOTAL_GRANDEZAS - 1;
      }

      else if (tecla == 'B') {  // descer

        indiceGrandeza++;

        if (indiceGrandeza >= TOTAL_GRANDEZAS)
          indiceGrandeza = 0;
      }

      // ATUALIZA LCD SOMENTE SE NECESSÁRIO
      if (indiceGrandeza != indiceAnterior) {

        indiceAnterior = indiceGrandeza;

        // ----- PRIMEIRA LINHA -----
        lcd1.setCursor(0, 0);
        lcd1.print("Lista grandezas");
        lcd1.write(byte(0));  // seta ↑

        // SEGUNDA LINHA FORMATADA
        char linha[17];  // 16 caracteres + \0

        snprintf(linha, 16, "%s (%s)",
                 lista_grandezas[indiceGrandeza],
                 lista_simbolos[indiceGrandeza]);

        // Limpa linha inteira
        lcd1.setCursor(0, 1);
        lcd1.print("                ");

        // Escreve texto
        lcd1.setCursor(0, 1);
        lcd1.print(linha);

        // Seta ↓ sempre na última coluna
        lcd1.setCursor(15, 1);
        lcd1.write(byte(1));
      }

      // ===== TRANSIÇÃO DE ESTADO =====
      if (tecla == 'C') {

        estadoAtual = ESTADO_SELEC4;
      }

      break;


      /************************** ESTADO 20 **************************/
    case ESTADO_SELEC4:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERDE);  // 🟢  LED verde
        LEDazul(3);        // 🔵 Terceiro LED azul

        tempoInicioEstado = millis();

        valorDigitado = "";
        ;

        // primeira linha do LCD1 (fixa)
        lcd1.setCursor(0, 0);
        lcd1.print(lista_grandezas[indiceGrandeza]);

        // mostra simbolo inicial
        lcd1.setCursor(0, 1);
        lcd1.print(lista_simbolos[indiceGrandeza]);
        lcd1.print(" = ");

        //LCD2 escreve apenas uma vez
        lcd2.setCursor(0, 0);
        lcd2.print("Digite o valor e");

        lcd2.setCursor(0, 1);
        lcd2.print("confirme com C");
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      if (tecla) {

        // números
        if (tecla >= '0' && tecla <= '9') {
          valorDigitado += tecla;
        }

        // ponto decimal
        if (tecla == '*') {
          valorDigitado += '.';
        }


        // deletar
        if (tecla == 'D') {
          if (valorDigitado.length() > 0) {
            valorDigitado.remove(valorDigitado.length() - 1);
          }
        }

        // atualizar segunda linha
        lcd1.setCursor(0, 1);
        lcd1.print("                ");  // limpa linha

        lcd1.setCursor(0, 1);
        lcd1.print(lista_simbolos[indiceGrandeza]);
        lcd1.print(" = ");
        lcd1.print(valorDigitado);
      }

      // ===== TRANSIÇÃO DE ESTADO =====
      if (tecla == 'C') {
        if (valorDigitado.length() > 0) {
          lista_valores[indiceGrandeza] = valorDigitado.toFloat();
          grandezaAtiva[indiceGrandeza] = true;

          estadoAtual = ESTADO_SELEC5;
        }
      }

      break;

      /************************** ESTADO 21 **************************/
    case ESTADO_SELEC5:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          lcd2.clear();

          LEDstatus(VERMELHO);
          LEDazul(3);

          tempoInicioEstado = millis();

          char buffer[17];
          float valor = lista_valores[indiceGrandeza];

          if (valor == (int)valor) {
            snprintf(buffer, 16, "%s=%d",
                     lista_simbolos[indiceGrandeza],
                     (int)valor);
          } else {
            snprintf(buffer, 16, "%s=%.2f",
                     lista_simbolos[indiceGrandeza],
                     valor);
          }

          if (totalResultados < TOTAL_GRANDEZAS) {

            listaResultados[totalResultados] = String(buffer);
            totalResultados++;
          }

          // ===== PRINTA TODOS OS RESULTADOS (UMA VEZ) =====

          int linha = 0;
          int coluna = 0;

          for (int i = 0; i < totalResultados; i++) {

            String texto = listaResultados[i] + " ";

            if (coluna + texto.length() > 16) {
              linha++;
              coluna = 0;
            }

            if (linha > 1) break;

            lcd2.setCursor(coluna, linha);
            lcd2.print(texto);

            coluna += texto.length();
          }
        }

        // ===== LÓGICA CONTÍNUA DO ESTADO =====

        lcd1.setCursor(0, 0);
        lcd1.print("Valor inserido");

        lcd1.setCursor(0, 1);
        lcd1.print("olhe ao lado ->");

        // ===== TRANSIÇÃO DE ESTADO =====

        if (millis() - tempoInicioEstado >= Δt_primeiro) {
          estadoAtual = ESTADO_SELEC6;
        }

        break;
      }

      /************************** ESTADO 22 **************************/
    case ESTADO_SELEC6:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          //lcd2.clear();

          LEDstatus(VERDE);  // 🟢 LED verde
          LEDazul(3);        // 🔵 Terceiro LED azul

          tempoInicioEstado = millis();
        }

        // ===== LÓGICA CONTÍNUA DO ESTADO =====

        lcd1.setCursor(0, 0);
        lcd1.print("Deseja inserir");
        lcd1.setCursor(0, 1);
        lcd1.print("outra grandeza?");


        // ===== TRANSIÇÃO DE ESTADO =====

        if (tecla == 'C') {
          estadoAtual = ESTADO_SELEC3;
        }

        if (tecla == 'D') {
          estadoAtual = ESTADO_SELEC7;
        }

        break;
      }
      /************************** ESTADO 23 **************************/
    case ESTADO_SELEC7:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          //lcd2.clear();

          LEDstatus(VERMELHO);  // 🔴 LED vermelho
          LEDazul(3);           // 🔵 Terceiro LED azul

          tempoInicioEstado = millis();
        }

        // ===== LÓGICA CONTÍNUA DO ESTADO =====

        lcd1.setCursor(0, 0);
        lcd1.print("Todos os valores");
        lcd1.setCursor(0, 1);
        lcd1.print("estao ao lado ->");

        // ===== TRANSIÇÃO DE ESTADO =====


        if (millis() - tempoInicioEstado >= Δt_diferente) {
          estadoAtual = ESTADO_CALCUL1;
        }

        break;
      }


      /************************** ESTADO 24 **************************/
    case ESTADO_CALCUL1:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          lcd2.clear();

          LEDstatus(VERMELHO);  // 🔴 LED vermelho
          LEDazul(4);           // 🔵 Quarto LED azul

          tempoInicioEstado = millis();
        }

        // ===== LÓGICA CONTÍNUA DO ESTADO =====

        escreverLinha32x1(0, "Agora voce ira  selecionar qual");
        escreverLinha32x1(1, "GRANDEZA quer   CALCULAR");

        // ===== TRANSIÇÃO DE ESTADO =====

        if (millis() - tempoInicioEstado >= Δt_primeiro) {
          estadoAtual = ESTADO_CALCUL2;
        }

        break;
      }

      /************************** ESTADO 25 **************************/
    case ESTADO_CALCUL2:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          lcd2.clear();

          LEDstatus(VERDE);  // 🟢  LED verde
          LEDazul(3);        // 🔵 Terceiro LED azul

          tempoInicioEstado = millis();

          indiceAnteriorCalculo = -1;
        }

        // ===== LÓGICA CONTÍNUA DO ESTADO =====

        lcd2.setCursor(0, 0);
        lcd2.print("Selecionar: A/B");
        lcd2.setCursor(0, 1);
        lcd2.print("confirmar: C");


        if (tecla == 'A') {  // subir

          indiceCalculo--;

          if (indiceCalculo < 0)
            indiceCalculo = TOTAL_GRANDEZAS - 1;
        }

        else if (tecla == 'B') {  // descer

          indiceCalculo++;

          if (indiceCalculo >= TOTAL_GRANDEZAS)
            indiceCalculo = 0;
        }

        // ATUALIZA LCD SOMENTE SE NECESSÁRIO
        if (indiceCalculo != indiceAnteriorCalculo) {

          indiceAnteriorCalculo = indiceCalculo;

          // ----- PRIMEIRA LINHA -----
          lcd1.setCursor(0, 0);
          lcd1.print("Qual qr calclr?");
          lcd1.write(byte(0));  // seta ↑

          // SEGUNDA LINHA FORMATADA
          char linha[17];  // 16 caracteres + \0

          snprintf(linha, 16, "%s (%s)",
                   lista_grandezas[indiceCalculo],
                   lista_simbolos[indiceCalculo]);

          // Limpa linha inteira
          lcd1.setCursor(0, 1);
          lcd1.print("                ");

          // Escreve texto
          lcd1.setCursor(0, 1);
          lcd1.print(linha);

          // Seta ↓ sempre na última coluna
          lcd1.setCursor(15, 1);
          lcd1.write(byte(1));
        }

        // ===== TRANSIÇÃO DE ESTADO =====
        if (tecla == 'C') {

          estadoAtual = ESTADO_CALCUL3;
        }

        break;
      }


      /************************** ESTADO 26 **************************/
    case ESTADO_CALCUL3:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          lcd2.clear();

          LEDstatus(VERMELHO);
          LEDazul(4);

          tempoInicioEstado = millis();
        }

        // ===== LÓGICA CONTÍNUA =====

        if (!podeCalcular(indiceCalculo)) {

          escreverLinha32x1(0, "Nao foi possivel calcular com");
          escreverLinha32x1(1, "os dados inseridos");

          if (millis() - tempoInicioEstado >= Δt_diferente) {
            estadoAtual = ESTADO_CALCUL5;
          }

        } else {

          lcd1.setCursor(0, 0);
          lcd1.print("Dados OK");

          lcd2.setCursor(0, 0);
          lcd2.print("Calculando...");

          if (millis() - tempoInicioEstado >= Δt_diferente) {

            
            resultadoCalculado = calcularGrandeza(indiceCalculo);

            estadoAtual = ESTADO_CALCUL4;
          }
        }

        break;
      }

      /************************** ESTADO 27 **************************/
    case ESTADO_CALCUL4:
{
    // ===== AÇÃO DE ENTRADA =====
    if (estadoAtual != estadoAnterior) {
        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);   // 🔴 LED vermelho
        LEDazul(5);             // 🔵 LED azul

        faseCalculo = 0;

        // ===== MOSTRA INÍCIO =====
        lcd1.setCursor(0, 0);
        lcd1.print("Calculando...");

        mostrarValoresAtuais();
          
        tempoInicioEstado = millis();
    }

    // ===== APÓS 2.5s MOSTRA RESULTADO =====
    if (faseCalculo == 0 && millis() - tempoInicioEstado > 2500) {

        lcd1.setCursor(0, 1);
        lcd1.print("Resultado:");

        // Verifica se resultadoCalculado é válido
        if (isnan(resultadoCalculado)) {
            lcd2.setCursor(0, 1);
            lcd2.print("ERRO: calculo");
            grandezaAtiva[indiceCalculo] = false;  // não ativa
            lista_valores[indiceCalculo] = 0;      // opcional, limpa valor
        } else {
            lcd2.setCursor(0, 1);
            lcd2.print(lista_simbolos[indiceCalculo]);
            lcd2.print("=");

            // Mostra resultado com formatação
            if (resultadoCalculado == (int)resultadoCalculado) {
                lcd2.print((int)resultadoCalculado);
            } else {
                lcd2.print(resultadoCalculado, 2);
            }

            // ===== SALVA RESULTADO =====
            lista_valores[indiceCalculo] = resultadoCalculado;
            grandezaAtiva[indiceCalculo] = true;

            // ===== SALVA NA LISTA DE RESULTADOS =====
            char buffer[17];
            if (resultadoCalculado == (int)resultadoCalculado) {
                snprintf(buffer, 16, "%s=%d",
                         lista_simbolos[indiceCalculo],
                         (int)resultadoCalculado);
            } else {
                snprintf(buffer, 16, "%s=%.2f",
                         lista_simbolos[indiceCalculo],
                         resultadoCalculado);
            }

            if (totalResultados < TOTAL_GRANDEZAS) {
                listaResultados[totalResultados] = String(buffer);
                totalResultados++;
            }
        }

        faseCalculo = 1;
        tempoInicioEstado = millis();
    }

    // ===== TRANSIÇÃO FINAL =====
    if (faseCalculo == 1 && millis() - tempoInicioEstado > 5000) {
        estadoAtual = ESTADO_RESULT1;
    }

    break;
}
      /************************** ESTADO 28 **************************/
    case ESTADO_CALCUL5:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          lcd2.clear();

          LEDstatus(VERDE);  // 🟢 LED verde
          LEDazul(4);        // 🔵 Quarto LED azul

          tempoInicioEstado = millis();
        }

        // ===== LÓGICA CONTÍNUA DO ESTADO =====

        escreverLinha32x1(0, "Deseja adicionar outra grandeza?");
        escreverLinha32x1(1, "SIM (tecla C)     NAO (tecla D)");

        // ===== TRANSIÇÃO DE ESTADO =====

        if (tecla == 'C') {
          estadoAtual = ESTADO_SELEC3;
        }

        if (tecla == 'D') {
          estadoAtual = ESTADO_CALCUL1;
        }

        break;
      }

              /************************** ESTADO 29 **************************/
    case ESTADO_RESULT1:
      {

        // ===== AÇÃO DE ENTRADA =====
        if (estadoAtual != estadoAnterior) {

          estadoAnterior = estadoAtual;

          lcd1.clear();
          lcd2.clear();

          LEDstatus(VERMELHO);  // 🔴 LED vermelho
          LEDazul(5);           // 🔵 Quinto LED azul

          tempoInicioEstado = millis();
        }

        // ===== LÓGICA CONTÍNUA DO ESTADO =====

        escreverLinha32x1(0, "   {{{(>_<)}}}      (*^_^*)");
        escreverLinha32x1(1, "Grandeza calculada com sucesso");

        // ===== TRANSIÇÃO DE ESTADO =====
        if (millis() - tempoInicioEstado >= Δt_diferente) {
          estadoAtual = ESTADO_RESULT2;
        }

        break;
      }



      /************************** ESTADO 30 **************************/
      case ESTADO_RESULT2:
                            {
    // ===== AÇÃO DE ENTRADA =====
    if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERDE);  // 🟢 LED verde (interação permitida)
        LEDazul(5);        // 🔵 Quinto LED azul (fase final)

        tempoInicioEstado = millis();
    }

    // ===== LÓGICA CONTÍNUA DO ESTADO =====

    escreverLinha32x1(0, "Quer comecar o programa do zero?");
    escreverLinha32x1(1, "(C) SIM         (D) continuar");

    // ===== TRANSIÇÃO DE ESTADO =====

    if (tecla == 'C') {

        resetSistema();  //  reset

        estadoAtual = ESTADO_BEM_VINDO;
        estadoAnterior = -1;  //  força executar ação de entrada corretamente
    }

    if (tecla == 'D') {

        estadoAtual = ESTADO_SELEC3;  // continua com dados atuais
    }

    break;
}

  /************************** ESTADO 31 **************************/
    case ESTADO_INFOR1:

      // ===== AÇÃO DE ENTRADA =====
      if (estadoAtual != estadoAnterior) {

        estadoAnterior = estadoAtual;

        lcd1.clear();
        lcd2.clear();

        LEDstatus(VERMELHO);  // 🔴 LED vermelho
        LEDazul(3);           // 🔵 Terceiro LED azul

        tempoInicioEstado = millis();
      }

      // ===== LÓGICA CONTÍNUA DO ESTADO =====

      escreverLinha32x1(0, "Use (D) para apagar o valor, e");
      escreverLinha32x1(1, "use (*) para inserir virgula");


      // ===== TRANSIÇÃO DE ESTADO =====
      if (millis() - tempoInicioEstado >= Δt_primeiro) {

        estadoAtual = ESTADO_SELEC3;
      }   

      break;





  }
}
