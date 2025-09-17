#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

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
    No* proximo;
};

class Lista {
private:
    No* inicio;

public:
    Lista() {
        inicio = NULL;
    }

    bool existeCPF(const string& cpf) {
        No* atual = inicio;
        while (atual != NULL) {
            if (atual->aluno.cpf == cpf) return true;
            atual = atual->proximo;
        }
        return false;
    }

    void inserirOrdenado(Aluno a) {
        if (existeCPF(a.cpf)) {
            cout << "ERRO: Já existe aluno com este CPF.\n";
            return;
        }

        No* novo = new No;
        novo->aluno = a;
        novo->proximo = NULL;

        if (inicio == NULL || a.nome < inicio->aluno.nome) {
            novo->proximo = inicio;
            inicio = novo;
            return;
        }

        No* atual = inicio;
        while (atual->proximo != NULL && atual->proximo->aluno.nome < a.nome) {
            atual = atual->proximo;
        }

        novo->proximo = atual->proximo;
        atual->proximo = novo;
    }

    void exibirLista() {
        No* atual = inicio;
        while (atual != NULL) {
            exibirAluno(atual->aluno);
            cout << "-------------------\n";
            atual = atual->proximo;
        }
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
};

// pega os dois dígitos verificadores do CPF (últimos dois números)
int extrairIndiceCPF(const string& cpf) {
    if (cpf.size() < 2) return 0;

    string doisUltimos = cpf.substr(cpf.size() - 2); 
    return atoi(doisUltimos.c_str()) % 100; // garante índice 0..99
}

// Função para carregar CSV no vetor
void carregarCSV(const string& nomeArquivo, Lista vetor[]) {
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

        int indice = extrairIndiceCPF(a.cpf);
        vetor[indice].inserirOrdenado(a);
    }
    arquivo.close();
}

int main() {
    Lista vetor[100]; // vetor de 100 listas

    clock_t inicio = clock(); // início da medição

    carregarCSV("alunos.csv", vetor);

    clock_t fim = clock(); // fim da medição
    double tempo_exec = double(fim - inicio) / CLOCKS_PER_SEC;

    cout << "Tempo gasto para carregar e inserir: " << tempo_exec << " segundos\n";

    // Exibir todos os alunos de todas as listas
    for (int i = 0; i < 100; i++) {
        if (i == 0) continue; // só para deixar mais limpo
        cout << "\n--- Lista[" << i << "] ---\n";
        vetor[i].exibirLista();
    }

    return 0;
}
