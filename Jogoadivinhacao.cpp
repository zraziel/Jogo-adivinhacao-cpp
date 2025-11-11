#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

// ======== Funções utilitárias ======== //
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausa() {
    cout << "\nPressione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ======== Funções de arquivo ======== //
int carregarRecorde() {
    ifstream arquivo("recorde.txt");
    int recorde = 0;
    if (arquivo.is_open()) {
        arquivo >> recorde;
        arquivo.close();
    }
    return recorde;
}

void salvarRecorde(int novoRecorde) {
    ofstream arquivo("recorde.txt");
    if (arquivo.is_open()) {
        arquivo << novoRecorde;
        arquivo.close();
    }
}

struct Estatisticas {
    int vitorias = 0;
    int derrotas = 0;
};

Estatisticas carregarEstatisticas() {
    ifstream arquivo("estatisticas.txt");
    Estatisticas e;
    if (arquivo.is_open()) {
        arquivo >> e.vitorias >> e.derrotas;
        arquivo.close();
    }
    return e;
}

void salvarEstatisticas(const Estatisticas &e) {
    ofstream arquivo("estatisticas.txt");
    if (arquivo.is_open()) {
        arquivo << e.vitorias << " " << e.derrotas;
        arquivo.close();
    }
}

// ======== Funções principais ======== //
void introducao(const string& nomeJogador) {
    limparTela();
    cout << "===========================================\n";
    cout << "          JOGO DE ADIVINHACAO \n";
    cout << "===========================================\n";
    cout << "  Bem-vindo, " << nomeJogador << "!\n";
    cout << "  Seu objetivo e simples:\n";
    cout << "  Descubra o numero secreto antes que suas\n";
    cout << "  tentativas acabem!\n";
    cout << "-------------------------------------------\n";
    cout << "  Dificuldades:\n";
    cout << "   [1] Facil    1 a 100   | 10 tentativas\n";
    cout << "   [2] Medio    1 a 200   | 12 tentativas\n";
    cout << "   [3] Dificil  1 a 500  | 15 tentativas\n";
    cout << "-------------------------------------------\n";
    cout << "  Boa sorte... voce vai precisar!\n";
    cout << "===========================================\n";
    pausa();
}

void escolherNivel(int &limiteSuperior, int &limiteTentativas) {
    int nivel;
    limparTela();
    cout << "\nEscolha o nivel de dificuldade:\n";
    cout << "1 - Facil (1 a 100)\n";
    cout << "2 - Medio (1 a 200)\n";
    cout << "3 - Dificil (1 a 500)\n";
    cout << "Sua escolha: ";
    cin >> nivel;

    switch (nivel) {
        case 1:
            limiteSuperior = 100;
            limiteTentativas = 10;
            break;
        case 2:
            limiteSuperior = 300;
            limiteTentativas = 12;
            break;
        case 3:
        default:
            limiteSuperior = 500;
            limiteTentativas = 15;
            break;
    }
}

int gerarNumeroSecreto(int limiteSuperior) {
    srand(static_cast<unsigned>(time(0)));
    return rand() % limiteSuperior + 1;
}

int jogarRodada(int limiteSuperior, int limiteTentativas, Estatisticas &stats) {
    int numeroSecreto = gerarNumeroSecreto(limiteSuperior);
    int tentativa, tentativas = 0;
    bool acertou = false;

    limparTela();
    cout << "\nTente adivinhar o numero entre 1 e " << limiteSuperior << "!\n";
    cout << "Voce tem " << limiteTentativas << " tentativas.\n\n";

    while (tentativas < limiteTentativas) {
        cout << "Tentativa " << tentativas + 1 << ": ";
        cin >> tentativa;

        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida! Digite um numero: ";
            cin >> tentativa;
        }

        tentativas++;

        if (tentativa == numeroSecreto) {
            cout << "\n Parabens! Voce acertou o numero em " << tentativas << " tentativas!\n";
            stats.vitorias++;
            acertou = true;
            return (limiteTentativas - tentativas + 1) * 10;
        }

        int diferenca = abs(tentativa - numeroSecreto);
        if (diferenca >= limiteSuperior / 2)
            cout << " Muito longe!\n";
        else if (diferenca >= limiteSuperior / 3)
            cout << " Longe!\n";
        else if (diferenca >= limiteSuperior / 5)
            cout << " Perto!\n";
        else
            cout << " Quase acertando!\n";

        if (tentativa > numeroSecreto)
            cout << "Dica: o numero secreto e MENOR!\n\n";
        else
            cout << "Dica: o numero secreto e MAIOR!\n\n";
    }

    cout << "\n Voce perdeu! O numero secreto era: " << numeroSecreto << "\n";
    stats.derrotas++;
    return 0;
}

void mostrarEstatisticas(const Estatisticas &stats, int recorde) {
    limparTela();
    cout << "========== ESTATISTICAS ==========\n";
    if (stats.vitorias == 0 && stats.derrotas == 0) {
        cout << "Voce ainda nao jogou nenhuma partida!\n";
    } else {
        cout << "Vitorias: " << stats.vitorias << "\n";
        cout << "Derrotas: " << stats.derrotas << "\n";
    }
    cout << "Maior pontuacao: " << recorde << "\n";
    cout << "=================================\n";
    pausa();
}

void mostrarCreditos() {
    limparTela();
    cout << "============ CREDITOS ============\n";
    cout << "Desenvolvido por: Rafa\n";
    cout << "Feito com logica pura em C++\n";
    cout << "=================================\n";
    pausa();
}

// ======== Programa principal ======== //
int main() {
    Estatisticas stats = carregarEstatisticas();
    int recorde = carregarRecorde();
    char opcao;
    string nomeJogador;

    limparTela();
    cout << "===========================================\n";
    cout << "      BEM-VINDO AO JOGO DE ADIVINHACAO!\n";
    cout << "===========================================\n";
    cout << "Antes de comecar, digite seu nome: ";
    cin >> nomeJogador;

    introducao(nomeJogador);

    do {
        limparTela();
        cout << "\n========== MENU PRINCIPAL ==========\n";
        cout << "1 - Jogar\n";
        cout << "2 - Ver estatisticas\n";
        cout << "3 - Creditos\n";
        cout << "4 - Sair\n";
        cout << "===================================\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == '1') {
            int limiteSuperior, limiteTentativas;
            escolherNivel(limiteSuperior, limiteTentativas);
            int pontos = jogarRodada(limiteSuperior, limiteTentativas, stats);

            cout << "\nPontuacao desta rodada: " << pontos << "\n";

            if (pontos > recorde) {
                cout << "\n Novo recorde! Parabens!\n";
                recorde = pontos;
                salvarRecorde(recorde);
            }

            salvarEstatisticas(stats);
            pausa();
        }
        else if (opcao == '2') {
            mostrarEstatisticas(stats, recorde);
        }
        else if (opcao == '3') {
            mostrarCreditos();
        }

    } while (opcao != '4');

    limparTela();
    cout << "===========================================\n";
    cout << "Obrigado por jogar, " << nomeJogador << "! Ate a proxima rodada!\n";
    cout << "===========================================\n";
    return 0;
}
