#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

struct Aluno {
    string matricula;
    string cpf;
    string nome;
    double nota;
    int idade;
    string curso;
    string cidade;
};

struct No {
    Aluno aluno;
    No* anterior;
    No* proximo;
};

class Lista {
private:
    No* inicio;
    No* fim;

public:
    Lista() {
        inicio = NULL;
        fim = NULL;
    }

    bool existeCPFouMatricula(const string& cpf, const string& matricula) {
        No* atual = inicio;
        while (atual != NULL) {
            if (atual->aluno.cpf == cpf || atual->aluno.matricula == matricula) {
                return true;
            }
            atual = atual->proximo;
        }
        return false;
    }

    void inserirOrdenado(Aluno a) {
        if (existeCPFouMatricula(a.cpf, a.matricula)) {
            cout << "ERRO: Já existe aluno com o CPF ou Matrícula informados.\n";
            return;
        }

        No* novo = new No;
        novo->aluno = a;
        novo->anterior = NULL;
        novo->proximo = NULL;

        if (inicio == NULL) { // lista vazia
            inicio = fim = novo;
            return;
        }

        No* atual = inicio;
        while (atual != NULL && atual->aluno.nome < a.nome) {
            atual = atual->proximo;
        }

        if (atual == NULL) { // insere no fim
            fim->proximo = novo;
            novo->anterior = fim;
            fim = novo;
        }
        else if (atual == inicio) { // insere no início
            novo->proximo = inicio;
            inicio->anterior = novo;
            inicio = novo;
        }
        else { // insere no meio
            novo->proximo = atual;
            novo->anterior = atual->anterior;
            atual->anterior->proximo = novo;
            atual->anterior = novo;
        }
    }

    No* buscarPorMatricula(const string& matricula) {
        No* atual = inicio;
        while (atual != NULL) {
            if (atual->aluno.matricula == matricula) return atual;
            atual = atual->proximo;
        }
        return NULL;
    }

    No* buscarPorCPF(const string& cpf) {
        No* atual = inicio;
        while (atual != NULL) {
            if (atual->aluno.cpf == cpf) return atual;
            atual = atual->proximo;
        }
        return NULL;
    }

    void remover(No* no) {
        if (no == NULL) return;

        if (no == inicio && no == fim) { // único elemento
            inicio = fim = NULL;
        }
        else if (no == inicio) { // início
            inicio = inicio->proximo;
            inicio->anterior = NULL;
        }
        else if (no == fim) { // fim
            fim = fim->anterior;
            fim->proximo = NULL;
        }
        else { // meio
            no->anterior->proximo = no->proximo;
            no->proximo->anterior = no->anterior;
        }
        delete no;
    }

    void exibirAluno(const Aluno& a) {
        cout << "Matrícula: " << a.matricula << "\n"
             << "CPF: " << a.cpf << "\n"
             << "Nome: " << a.nome << "\n"
             << "Nota: " << a.nota << "\n"
             << "Idade: " << a.idade << "\n"
             << "Curso: " << a.curso << "\n"
             << "Cidade: " << a.cidade << "\n";
    }

    void exibirLista() {
        No* atual = inicio;
        while (atual != NULL) {
            exibirAluno(atual->aluno);
            cout << "-----------------------\n";
            atual = atual->proximo;
        }
    }
};

// Função para ler CSV
void carregarCSV(const string& nomeArquivo, Lista& lista) {
    ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo.\n";
        return;
    }

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        Aluno a;
        string notaStr, idadeStr;

        getline(ss, a.matricula, ',');
        getline(ss, a.cpf, ',');
        getline(ss, a.nome, ',');
        getline(ss, notaStr, ',');
        getline(ss, idadeStr, ',');
        getline(ss, a.curso, ',');
        getline(ss, a.cidade, ',');

        a.nota = atof(notaStr.c_str());
        a.idade = atoi(idadeStr.c_str());

        lista.inserirOrdenado(a);
    }
    arquivo.close();
}

int main() {
    Lista lista;

    carregarCSV("alunos.csv", lista);

    int opcao;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1 - Buscar por matrícula\n";
        cout << "2 - Buscar por CPF\n";
        cout << "3 - Exibir todos os alunos\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore();

        if (opcao == 1) {
            string matricula;
            cout << "Digite a matrícula: ";
            getline(cin, matricula);
            No* encontrado = lista.buscarPorMatricula(matricula);
            if (encontrado != NULL) {
                lista.exibirAluno(encontrado->aluno);
                cout << "Deseja remover este aluno? (s/n): ";
                char resp;
                cin >> resp;
                if (resp == 's' || resp == 'S') {
                    lista.remover(encontrado);
                    cout << "Aluno removido.\n";
                }
            } else {
                cout << "Aluno não encontrado.\n";
            }
        }
        else if (opcao == 2) {
            string cpf;
            cout << "Digite o CPF: ";
            getline(cin, cpf);
            No* encontrado = lista.buscarPorCPF(cpf);
            if (encontrado != NULL) {
                lista.exibirAluno(encontrado->aluno);
                cout << "Deseja remover este aluno? (s/n): ";
                char resp;
                cin >> resp;
                if (resp == 's' || resp == 'S') {
                    lista.remover(encontrado);
                    cout << "Aluno removido.\n";
                }
            } else {
                cout << "Aluno não encontrado.\n";
            }
        }
        else if (opcao == 3) {
            lista.exibirLista();
        }
    } while (opcao != 0);

    return 0;
}
