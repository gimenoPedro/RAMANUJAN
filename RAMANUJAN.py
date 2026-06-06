# --------------------------------------------------------------------------------------------------------------------------------------------------
#
#  TCC - Calculadora inteligente para problemas de física do ensino fundamental - Ramanujan
#  Nome: Pedro Gimeno de Oliveira
#  Data de início: 23/12/2024
#  Data de finalização: 
# 
#
#
#  EXPLICAÇÃO DO CÓDIGO
#
#  O código tem como objetivo trabalhar com grandezas físicas e seus cálculos baseados em fórmulas físicas
#  do ensino fundamental. Ele é dividido em quatro etapas principais, com o uso de flags, ponteiros e valores inseridos pelo
#  usuário para controlar a lógica de execução e os cálculos.
#
#
#
#  FLAGS - Indicadores de quais grandezas se tem valores numéricos correspondentes
#
#  As flags são variáveis booleanas que indicam se uma determinada grandeza foi inserida pelo usuário. Elas ajudam o programa
#  a identificar quais dados estão disponíveis para realizar os cálculos. Por exemplo:
#  - flags["Massa"] será True se o usuário inseriu um valor para Massa.
#  - Se a flag de uma grandeza estiver False, significa que o valor correspondente ainda não foi definido.
#
# 
#
#  PONTEIROS - Indicadores de quais grandezas podem ser calculadas a partir das grandezas que se tem valores numéricos 
#  correspondentes
#
# Os ponteiros são também variáveis booleanas, mas diferentemente das flags, eles indicam quais grandezas podem ser calculadas
# com base nos valores já disponíveis. Por exemplo:
# - Se flags["Massa"] e flags["Aceleração"] forem True, então ponteiros["Força"] será definido como True, indicando que a 
# Força pode ser calculada.
#
#
#
#  4 DIVISÕES DO PROGRAMA - Para simplficar a visualização e programação do programa, o mesmo foi dividio em 4 partes : a entrada
#  ou coleta de dados do usuário, atualização das flags e dos ponteiros, cálculo ou não das grandezas desejadas pelo usuáiro e
#  a saída dos dados ou exibição dos resultados
#
#  Diamonds (Entrada de Dados):
#  Pergunta ao usuário quais grandezas ele deseja inserir;
#  Solicita o valor numérico correspondente à grandeza escolhida;
#  Oferece a opção de inserir mais grandezas ou seguir com o fluxo do programa.
#
#  Clubs (Avaliação de Flags e Ponteiros):
#  Verifica quais grandezas podem ser calculadas com base nas flags ativadas;
#  Define os ponteiros para indicar quais cálculos são possíveis.
#
#  Hearts (Cálculo das Grandezas):
#  Realiza os cálculos necessários para determinar os valores das grandezas que podem ser derivadas;
#  Utiliza a fórmula 𝐹 = 𝑚⋅𝑎 e suas derivações para calcular as grandezas restantes.
#
#  Spades (Exibição dos Resultados):
#  Permite ao usuário escolher uma grandeza para visualizar o valor;
#  Exibe o valor, seja ele inserido pelo usuário ou calculado pelo programa.
#
# --------------------------------------------------------------------------------------------------------------------------------------------------
#  
#  PROGRESSO
#
#   Fase 1 - Base estrutural e lógica 
#   Fase 2 - Escalabilidade do programa adicionando novas grandezas
#
#  Data                         Versão                          Status                        O que falta / o que adicionou
#   
#  23/12                        1.0                            FUNCIONANDO                          - / -
#  26/12                        1.0 alfa                       FUNCIONANDO                          - / Introdução ao código 
#  27/12                        1.1                            IMCOMPLETO                  implementar barra de rolagem, fazer popup 1 deseparecer quando popup 4 aparecer/ popup 1 agora funciona por botões     
#  03/01                        1.2                            FUNCIONANDO                          - / a barra de rolagem foi substituida por uma combobox, o fluxo dos popups foi alterado porque agora o mesmo popup contém o a informação que antes aparecia em vários popups
#  10/01                        1.3                            IMCOMPLETO                  quando se atribui um valor e outro em seguinda quando se calcula a partir desses valores, está usando o primeiro valor ao inves do segundo / botão para voltar para página anterrio e popup com indicadores de grandezas com cores 
#  13/01                        1.4                            FUNCIONANDO                          - / atualização quando sobrescreve grandezas, botao voltar pagina anterior retirado 
#
#  21/01                        2.0                            FUNCIONANDO                        quando se vai utilizar as duas formulas, isto é, temos m e a, ai calculamos F, ai com isso queremos calcular W e temos d. Desse jeito, após calcular d não é possível calcular W, pois o codigo não entende que uma grandeza que foi calcula a partir de grandezas inseridas como sendo uma grandeza que pode ser utilizada para calcular outras grandezas / agr tem a outra formulas, W=F.d, e também é possível calcular F de duas maneiras diferntes e independetes, além disso os dois popups não estão um sobre o outro agora
#  27/01                        2.1                            FUNCIONANDO                        - / Adicionada fórmulas P = m.g e agora grandezas estão em linhas horizontais de 5 grandezas cada, sendo que a cada sexta grandeza é feita uma nova linha horizontal. Além disso, problema anterior de n conseguir calcular quando utilizamos grandezas que são calculadas de jeitos diferentes "foi" resolvido. Isso porque a funcionalidade do código não foi alterada, mas eu entendi como tem que fazer para funcionar. Ao inserir o valor das grandezas m e a, de fato, é calculado F, porém ao simplesmente colocar a grandeza d não é possível calcular W. Isto porque o programa não calcula grandezas com valores de outras grandezas realizadas pelo programa a partir de infos do usuário para calcular outras grnadezas, ele apenas usa grandezas que o usuário informa para calcular as grandezas que o usuário quer. Ou seja, para conseguir calcular W apenas é necessário inserir o valor de F no programa sendo igual ao valor calculado e indicado nos indicadores, nesse momento, W será calculado.
#  30/01                        2.2                            FUNCIONANDO                        - / barra de listagem em ordem alfabética; adicionada grandezas Δs, s_o, s, Δt, t_o e t; agora esta aparecendo a imagem com nome do projeto e também uma imagem mostrando a versão atual do código; além disso, agora temos simbolos das grandezas aparecendo no popup indicadores, enquanto na barra de rolagem aparece o nome da grande por extenso      
#  03/02                        2.3                            IMCOMPLETO                        ajustar "tique" do popup e adicionar variavel para quando o swtich estiver em MU ou MUV para integrar ele com o código e fazer com que a variavel v puxe o valor de vm no caso / grandeza vₘ = Δs/Δt; novo popup switch cinematica;
#  11/02                        2.4                            IMCOMPLETO                         falta adicionar o segundo caminho para Δs e Δt, apenas tem as formulas para vₘ, falata adicionar caminho para v mu, falta adicionar segunda caminho para t, s e s₀ (no caso em que t₀ = 0 ), / reitrei quadrado com "tique" do popup cinematica; agora o popup cinematica deixa preto a caixa da variavel que nao for usar; adicionada simbolos grandezas velocidade final, inicial e aceleração média;
#                               2.5                            EM PROGRESSO                       - / troquei popup cinematica por um popup que simplesmente age como legenda, e mudei os simbolos de v (MU) e v (MUV) para v mu e v mu;                                    
# --------------------------------------------------------------------------------------------------------------------------------------------------
# 
# FÓRMULAS SENDO TRABALHADAS
# 
#   i)   Segunda Lei de Newton        F = m . a
#  ii)    Fórmula do Trabalho         W = F . d
# iii)    Fórmula força Peso          P = m . g (g = 10)
#  iv)    Variação de posição         Δs = s - s₀         
#   v)    Variação de tempo           Δt = t - t₀
#  vi)    Velocidade média MU         vₘ = Δs / Δt
#
#
# --------------------------------------------------------------------------------------------------------------------------------------------------
#
# GRANDEZAS SENDO CALCULADAS
#
#
#   F = m . a  ou F = W / d            => Força
#   m = F / a  ou m = P / g  (g = 10)  => Massa
#   a = F / m                          => Aceleração
#   W = F . d                          => Trabalho 
#   d = W / F                          => Distância 
#   Δs = s - s₀ ou vₘ . Δt   (falta)   => Variação de posição                   
#   s  = Δs + s₀                       => Posição final
#   s₀ = s - Δs                        => Posição inicial
#   Δt = t - t₀ ou vₘ / Δs  (falta)    => Variação de tempo
#   t  = Δt + t₀                       => Tempo final
#   t₀ = t - Δt                        => Tempo inicial
#   vₘ = Δs / Δt                       => Velocidade média MU
#   
#
# --------------------------------------------------------------------------------------------------------------------------------------------------


import tkinter as tk
from tkinter import *
from tkinter import messagebox
from tkinter import ttk
from tkinter.ttk import Combobox
import cv2
import threading


# Base de dados de grandezas e fórmulas
grandezas = ["Massa", "Aceleração", "Força", "Trabalho", "Distância", "Peso", "Posição inicial", 
"Posição final", "Variação de posição", "Tempo inicial", "Tempo final", "Variação de tempo", 
"Velocidade média", "Velocidade", "Velocidade inicial", "Velocidade final", "Variação de velocidade", 
"Aceleração média"] 

# Base de dados para os simbolos das grandezas acimas
simbolos_grandezas = ["m", "a", "F", "W", "d", "P", "s₀", "s", "Δs", "t₀", "t", "Δt", "vₘ",
"v mu", "v₀", "v muv", "Δv", "aₘ" ]

# Flags e ponteiros
flags = {grandeza: False for grandeza in grandezas}
ponteiros = {grandeza: False for grandeza in grandezas}

# Valores das grandezas inseridos
valores = {grandeza: None for grandeza in grandezas}

# Janela de indicadores (global)
janela_indicadores = None
labels_indicadores = {}

def atualizar_cor_simbolo(movimento):
    if movimento == "Movimento Uniforme":
        labels_indicadores["Velocidade final"].config(bg="black", fg="black")  # Texto desaparece
    else:
        labels_indicadores["Velocidade final"].config(bg="white", fg="gray")  

    if movimento == "Movimento Uniformemente Variável":
        labels_indicadores["Velocidade"].config(bg="black", fg="black")  # Texto desaparece
    else:
        labels_indicadores["Velocidade"].config(bg="white", fg="gray") 

def switch_cinematica():
    # Criar a janela principal
    root = tk.Tk()
    root.title("Tipo de movimento")
    root.geometry("300x200+450+400")

    # Criar variável para armazenar o estado
    is_uniforme = tk.BooleanVar(value=True)  # Começa como True (Movimento Uniforme)

    # Criar o primeiro label que mostra o tipo de movimento
    my_label = ttk.Label(root, text="Movimento Uniforme", font=("Arial", 10))
    my_label.pack(pady=10)

    # Função que alterna os textos dos labels
    def switcher():
        if is_uniforme.get():
            my_label.config(text="Movimento Uniformemente Variável")
            velocity_label.config(text="v é a velocidade final")
            is_uniforme.set(False)
            atualizar_cor_simbolo("Movimento Uniformemente Variável")  
        else:
            my_label.config(text="Movimento Uniforme")
            velocity_label.config(text="v é a velocidade média")
            is_uniforme.set(True)
            atualizar_cor_simbolo("Movimento Uniforme")  

    # Criar botão para alternar o estado
    my_button = ttk.Button(root, text="Mudar!", command=switcher)
    my_button.pack(pady=10)

    # Criar o segundo label (mensagem) abaixo do botão
    velocity_label = ttk.Label(root, text="v é a velocidade média", font=("Arial", 10))
    velocity_label.pack(pady=10)

    # Iniciar o loop da interface gráfica
    root.mainloop()


    
def nome_janela():
    caminho1 = r"C:\Users\pgo11\OneDrive\Imagens\v24.png"               # Caminhos das imagens
    caminho2 = r"C:\Users\pgo11\OneDrive\Imagens\ramanujan_cover.png"
    imagem1 = cv2.imread(caminho1)      # Carregar as imagens
    imagem2 = cv2.imread(caminho2)
    if imagem1 is None:
        print(f"Erro: Não foi possível carregar a imagem {caminho1}")   # Verificar se as imagens foram carregadas corretamente
        exit(1)
    if imagem2 is None:
        print(f"Erro: Não foi possível carregar a imagem {caminho2}")
        exit(1)
    largura, altura = 700, 150                          # Redimensionar imagens para tamanhos iguais
    imagem1 = cv2.resize(imagem1, (largura, altura))
    imagem2 = cv2.resize(imagem2, (largura, altura))
    cv2.namedWindow("Imagem 1", cv2.WINDOW_NORMAL)  # Criar janelas e definir posições
    cv2.namedWindow("Imagem 2", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("Imagem 1", largura, altura)
    cv2.resizeWindow("Imagem 2", largura, altura)
    cv2.moveWindow("Imagem 1", 50, 200)  # Posição inicial na tela
    cv2.moveWindow("Imagem 2", 50, 50)  # Posição ao lado da primeira
    cv2.imshow("Imagem 1", imagem1) # Exibir imagens
    cv2.imshow("Imagem 2", imagem2)
    while True:         # Loop para manter as janelas ativas
        if cv2.waitKey(1) & 0xFF == 27:  # Pressione 'ESC' para fechar
            break
    cv2.destroyAllWindows()     # Fechar todas as janelas ao sair do loop

   
# Função para centralizar a janela
def centralizar_janela(janela, largura, altura, deslocamento_y=0):
    largura_tela = janela.winfo_screenwidth()
    altura_tela = janela.winfo_screenheight()
    pos_x = ((largura_tela - largura) // 2 ) + 325
    pos_y = ((altura_tela - altura) // 2 + deslocamento_y )  
    janela.geometry(f'{largura}x{altura}+{pos_x}+{pos_y}')

# Dicionário para mapear nomes das grandezas aos seus símbolos
mapa_grandezas = dict(zip(grandezas, simbolos_grandezas))

# Função para criar e atualizar a janela de indicadores
def criar_janela_indicadores():
    global janela_indicadores, labels_indicadores

    janela_indicadores = tk.Toplevel()
    janela_indicadores.title("Indicadores de Grandezas")
    janela_indicadores.geometry("600x200")
    centralizar_janela(janela_indicadores, 600, 200, -300) # Tamanho popup indicadores
    janela_indicadores.resizable(False, False)

    tk.Label(janela_indicadores, text="Indicadores de Grandezas", font=("Arial", 12, "bold")).pack(pady=5)

    # Frame principal para os indicadores
    frame_principal = tk.Frame(janela_indicadores)
    frame_principal.pack(pady=5)

    # Configurar layout das grandezas em grid (5 colunas por linha)
    colunas_por_linha = 6
    for i, grandeza in enumerate(grandezas):
        linha = i // colunas_por_linha  # Determina a linha
        coluna = i % colunas_por_linha  # Determina a coluna

        # Criar um frame para cada grandeza
        frame = tk.Frame(frame_principal, padx=10, pady=5)
        frame.grid(row=linha, column=coluna, sticky="w")  # Adiciona na posição apropriada

        # Nome da grandeza convertido para o símbolo correspondente
        simbolo = mapa_grandezas[grandeza]

        # Label da grandeza (agora com símbolo)
        tk.Label(frame, text=simbolo, font=("Arial",9)).pack()

        # Indicador de valor (inicialmente "Unk")
        labels_indicadores[grandeza] = tk.Label(frame, text="Unk", font=("Arial", 8, "italic"), fg="gray", bg="white", width=10)
        labels_indicadores[grandeza].pack()

# Atualizar indicadores visuais
def atualizar_indicadores():
    for grandeza, valor in valores.items():
        if valor is None:
            labels_indicadores[grandeza].config(text="Unk", fg="gray", bg="white")
        else:
            if flags[grandeza]:  # Atribuído
                labels_indicadores[grandeza].config(text=f"{valor:.2f}", fg="blue", bg="lightblue")
            elif ponteiros[grandeza]:  # Calculado
                labels_indicadores[grandeza].config(text=f"{valor:.2f}", fg="green", bg="lightgreen")

# Função Diamonds: Entrada de Dados
def diamonds():
    def confirmar_entrada():
        grandeza = dropdown_grandezas.get()
        valor = entrada_valor.get()

        if grandeza in grandezas:
            try:
                valor_float = float(valor)
                valores[grandeza] = valor_float
                flags[grandeza] = True
                ponteiros[grandeza] = False  # Se foi atribuído manualmente, não é mais calculado
                clubs()  # Atualizar dependências
                hearts()  # Realizar cálculos com base nos valores mais recentes
                atualizar_indicadores()
                messagebox.showinfo("Sucesso", f"Valor de {grandeza} inserido com sucesso!")
            except ValueError:
                messagebox.showerror("Erro", "Por favor, insira um valor numérico válido.")
        else:
            messagebox.showerror("Erro", "Selecione uma grandeza válida.")
        entrada_valor.delete(0, tk.END)

    def exibir_resultados():
        janela_diamonds.destroy()
        spades()

    janela_diamonds = tk.Tk()
    janela_diamonds.title("Entrada de Dados")
    janela_diamonds.geometry("400x300")
    centralizar_janela(janela_diamonds, 400, 300)

    tk.Label(janela_diamonds, text="Escolha uma grandeza:").pack(pady=5)
    dropdown_grandezas = Combobox(janela_diamonds, values=sorted(grandezas), state="readonly")
    dropdown_grandezas.pack(pady=5)

    tk.Label(janela_diamonds, text="Insira o valor numérico:").pack(pady=5)
    entrada_valor = tk.Entry(janela_diamonds)
    entrada_valor.pack(pady=5)

    tk.Button(janela_diamonds, text="Confirmar", command=confirmar_entrada).pack(pady=10)
    tk.Button(janela_diamonds, text="Ir para o cálculo das grandezas", command=exibir_resultados).pack(pady=10)

    criar_janela_indicadores()

    # Chamar a função nome_janela em uma thread separada
    thread_imagem = threading.Thread(target=nome_janela)
    thread_imagem.daemon = True  # Garantir que a thread termine quando o programa fechar
    thread_imagem.start()

    # Chamar a função nome_janela em uma thread separada
    thread = threading.Thread(target=switch_cinematica)
    thread.daemon = True
    thread.start()
    
    janela_diamonds.mainloop()

# Função Clubs: Atualizar Flags e Ponteiros
def clubs():
    if flags["Força"] and flags["Aceleração"] or flags["Peso"]:
        ponteiros["Massa"] = True

    if flags["Força"] and flags["Massa"]:
        ponteiros["Aceleração"] = True

    if flags["Massa"] and flags["Aceleração"] or flags["Trabalho"] and flags["Distância"]:
        ponteiros["Força"] = True

    if flags["Força"] and flags["Distância"]:
        ponteiros["Trabalho"] = True

    if flags["Força"] and flags["Trabalho"]:
        ponteiros["Distância"] = True

    if flags["Massa"]:
        ponteiros["Peso"] = True

    if flags["Posição inicial"] and flags["Posição final"]:
        ponteiros["Variação de posição"] = True
    
    if flags["Posição inicial"] and flags["Variação de posição"]:
        ponteiros["Posição final"] = True

    if flags["Posição final"] and flags["Variação de posição"]:
        ponteiros["Posição inicial"] = True

    if flags["Tempo inicial"] and flags["Tempo final"]:
        ponteiros["Variação de tempo"] = True
    
    if flags["Tempo inicial"] and flags["Variação de tempo"]:
        ponteiros["Tempo final"] = True

    if flags["Tempo final"] and flags["Variação de tempo"]:
        ponteiros["Tempo inicial"] = True

    if flags["Variação de posição"] and flags["Variação de tempo"]:
        ponteiros["Velocidade média"] = True

# Função Hearts: Realizar cálculos com base nos ponteiros
def hearts():
    if ponteiros["Massa"]:
        if flags["Força"] and flags["Aceleração"]:
            valores["Massa"] = valores["Força"] /  valores["Aceleração"]
        elif flags["Peso"]:
            valores["Massa"] = valores["Peso"] / 10

    if ponteiros["Aceleração"]:
        valores["Aceleração"] = valores["Força"] /  valores["Massa"]

    if ponteiros["Força"]:
        if flags["Massa"] and flags["Aceleração"]:
            valores["Força"] = valores["Massa"] * valores["Aceleração"]
        elif flags["Trabalho"] and flags["Distância"]:
            valores["Força"] = valores["Trabalho"] / valores["Distância"]

    if ponteiros["Trabalho"]:
        valores["Trabalho"] = valores["Força"] * valores["Distância"]

    if ponteiros["Distância"]:
        valores["Distância"] = valores["Trabalho"] / valores["Força"]

    if ponteiros["Peso"]:
        valores["Peso"] = valores["Massa"] * 10

    if ponteiros["Variação de posição"]:
        if valores["Posição final"] == None:
            valores["Posição final"] = 0
        if valores["Posição inicial"] == None:
            valores["Posição inicial"] = 0
        valores["Variação de posição"] = valores["Posição final"] - valores["Posição inicial"]
    
    if ponteiros["Posição final"]:
        if valores["Posição inicial"] == None:
            valores["Posição inicial"] = 0
        if valores["Variação de posição"] == None:
            valores["Variação de posição"] = 0
        valores["Posição final"] = valores["Posição inicial"] + valores["Variação de posição"]

    if ponteiros["Posição inicial"]:
        if valores["Posição final"] == None:
            valores["Posição final"] = 0
        if valores["Variação de posição"] == None:
            valores["Variação de posição"] = 0
        valores["Posição inicial"] = valores["Posição final"] - valores["Variação de posição"]

    if ponteiros["Variação de tempo"]:
        if valores["Tempo final"] == None:
            valores["Tempo final"] = 0
        if valores["Tempo inicial"] == None:
            valores["Tempo inicial"] = 0
        valores["Variação de tempo"] = valores["Tempo final"] - valores["Tempo inicial"]
    
    if ponteiros["Tempo final"]:
        if valores["Tempo inicial"] == None:
            valores["Tempo inicial"] = 0
        if valores["Variação de tempo"] == None:
            valores["Variação de tempo"] = 0
        valores["Tempo final"] = valores["Tempo inicial"] + valores["Variação de tempo"]

    if ponteiros["Tempo inicial"]:
        if valores["Tempo final"] == None:
            valores["Tempo final"] = 0
        if valores["Variação de tempo"] == None:
            valores["Variação de tempo"] = 0
        valores["Tempo inicial"] = valores["Tempo final"] - valores["Variação de tempo"]

    if ponteiros["Velocidade média"]:
        valores["Velocidade média"] = valores["Variação de posição"] / valores["Variação de tempo"]

    atualizar_indicadores()

# Função Spades: Exibição dos Resultados
def spades():
    def mostrar_resultado():
        grandeza = dropdown_grandezas.get()
        if grandeza in grandezas:
            if valores[grandeza] is not None:
                messagebox.showinfo("Resultado", f"O valor de {grandeza} é {valores[grandeza]:.2f}")
            else:
                messagebox.showerror("Erro", f"Não foi possível calcular {grandeza}.")
        else:
            messagebox.showerror("Erro", "Selecione uma grandeza válida.")

    def sair():
        janela_spades.destroy()
        janela_indicadores.destroy()

    janela_spades = tk.Tk()
    janela_spades.title("Exibição de Resultados")
    janela_spades.geometry("400x250")
    centralizar_janela(janela_spades, 400, 250)

    tk.Label(janela_spades, text="Escolha uma grandeza para ver o resultado:").pack(pady=5)
    dropdown_grandezas = Combobox(janela_spades, values=sorted(grandezas), state="readonly")
    dropdown_grandezas.pack(pady=5)

    tk.Button(janela_spades, text="Mostrar Resultado", command=mostrar_resultado).pack(pady=10)

    # Botão para sair do programa
    tk.Button(janela_spades, text="Sair", command=sair).pack(pady=10)

    janela_spades.mainloop()

# Função Principal
def main():
    diamonds()

if __name__ == "__main__":
    main()
