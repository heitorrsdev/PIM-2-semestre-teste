#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <limits>
#include <functional>
#include <iomanip>
using namespace std;

// Gerenciador de banco de dados para manipulação de arquivos
class DatabaseManager {
public:
    // Obtém o último ID registrado no arquivo
    static int GetLastID(const string& filepath) {
        ifstream file(filepath);
        if (!file) {
            cerr << "Erro ao abrir o arquivo: " << filepath << endl;
            return -1;
        }
        string line, lastID;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, lastID, ',');
        }
        return lastID.empty() ? 0 : stoi(lastID);
    }

    // Adiciona dados ao final do arquivo
    static void AppendToFile(const string& filepath, const string& data) {
        ofstream file(filepath, ios::app);
        if (file.is_open()) {
            file << data << endl;
        } else {
            cerr << "Erro ao abrir o arquivo: " << filepath << endl;
        }
    }

    // Exibe o conteúdo do arquivo no console
    static void DisplayFileContents(const string& filepath) {
        ifstream file(filepath);
        if (!file) {
            cerr << "Erro ao abrir o arquivo: " << filepath << endl;
            return;
        }
        string line;
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;
        while (getline(file, line)) {
            cout << line << endl;
            cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;
        }
    }
};

// Limpa a tela usando código de escape ANSI
void ClearScreen() {
    cout << "\033[2J\033[1;1H";
}

// Pausa a execução do programa por um determinado número de segundos
void Pause(int seconds) {
    this_thread::sleep_for(chrono::seconds(seconds));
}

// Solicita uma confirmação do usuário
bool Confirm(const string& prompt) {
    string option;
    cout << prompt << " (S/N): ";
    cin >> option;
    return option == "S" || option == "s";
}

// Obtém entrada do usuário com limpeza de buffer
string InputWithClear(const string& prompt) {
    string input;
    while (input.empty()) {
        cout << prompt;
        getline(cin, input);
        ClearScreen();
    }
    return input;
}

// Função para adicionar fornecedores ao sistema
void AddSuppliers() {
    string name = InputWithClear("Nome: ");
    string company = InputWithClear("Empresa: ");
    string contact = InputWithClear("Contato: ");
    string description = InputWithClear("Descrição: ");

    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;
    cout << "Nome: " << name << endl;
    cout << "Empresa: " << company << endl;
    cout << "Contato: " << contact << endl;
    cout << "Descrição: " << description << endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;

    if (Confirm("Deseja salvar este fornecedor?")) {
        int lastID = DatabaseManager::GetLastID(".Database/Fornecedores.csv") + 1;
        if (lastID == -1) return;
        stringstream data;
        data << lastID << "," << name << "," << company << "," << contact << "," << description;
        DatabaseManager::AppendToFile(".Database/Fornecedores.csv", data.str());
        ClearScreen();
        cout << "Fornecedor salvo com sucesso, aguarde...";
        Pause(2);
    }

    ClearScreen();
    cout << "Voltando para o menu...";
    Pause(1);
}

// Função para ler os fornecedores cadastrados
void ReadSuppliers() {
    ClearScreen();
    DatabaseManager::DisplayFileContents(".Database/Fornecedores.csv");
    cout << "[S] Para Sair: ";
    string option;
    cin >> option;
    ClearScreen();
    cout << "Voltando para o menu, aguarde...";
    Pause(2);
}

// Função para adicionar itens ao estoque
void AddStockItem() {
    string item = InputWithClear("Produto: ");
    string quantity = InputWithClear("Quantidade: ");
    string expiry = InputWithClear("Validade: ");
    string pricePerKG = InputWithClear("Valor por KG: ");
    string alertLimit = InputWithClear("Limite de alerta: ");

    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;
    cout << "Item: " << item << endl;
    cout << "Quantidade: " << quantity << endl;
    cout << "Valor por KG: R$ " << pricePerKG << endl;
    cout << "Validade: " << expiry << endl;
    cout << "Alerta: " << alertLimit << endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;

    if (Confirm("Deseja salvar este produto?")) {
        int lastID = DatabaseManager::GetLastID(".Database/stock.csv") + 1;
        if (lastID == -1) return;
        stringstream data;
        data << lastID << "," << item << "," << quantity << "," << pricePerKG << "," << expiry << "," << alertLimit;
        DatabaseManager::AppendToFile(".Database/stock.csv", data.str());
        ClearScreen();
        cout << "Produto salvo com sucesso, aguarde...";
        Pause(2);
    }

    ClearScreen();
    cout << "Voltando ao menu principal...";
    Pause(1);
}

// Função para ler os itens do estoque
void ReadStock() {
    ClearScreen();
    DatabaseManager::DisplayFileContents(".Database/stock.csv");
    cout << "[S] Para Sair: ";
    string option;
    cin >> option;
    ClearScreen();
    cout << "Voltando para o menu, aguarde...";
    Pause(2);
}

// Verifica as credenciais de login e retorna o cargo do usuário
string VerifyLogin(const string& userLogin, const string& userPassword) {
    ifstream loginReader(".Database/users.csv");
    // if (!loginReader) {
    //     cerr << "Erro ao abrir o arquivo: .Database/users.csv" << endl;
    //     return "Erro";
    // }

    string output, id, login, password, role;
    
    while (getline(loginReader, output)) {
        stringstream ss(output);
        getline(ss, id, ',');
        getline(ss, login, ',');
        getline(ss, password, ',');
        getline(ss, role, ',');

        if (userLogin == login && userPassword == password) {
            return role;
        }
    }
    return "Login não encontrado";
}

// Função para adicionar novos logins ao sistema
void AddLogin() {
    string login = InputWithClear("Login: ");
    string password = InputWithClear("Senha: ");
    string role = InputWithClear("Cargo: ");

    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;
    cout << "Login: " << login << endl;
    cout << "Senha: " << password << endl;
    cout << "Cargo: " << role << endl;
    cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+--+" << endl;

    if (Confirm("Deseja cadastrar este login?")) {
        int lastID = DatabaseManager::GetLastID(".Database/users.csv") + 1;
        if (lastID == -1) return;
        stringstream data;
        data << lastID << "," << login << "," << password << "," << role;
        DatabaseManager::AppendToFile(".Database/users.csv", data.str());
        ClearScreen();
        cout << "Login cadastrado com sucesso, aguarde...";
        Pause(2);
    }

    ClearScreen();
    cout << "Voltando ao menu principal...";
    Pause(1);
}

// Função para ler os logins cadastrados
void ReadLogins() {
    ClearScreen();
    DatabaseManager::DisplayFileContents(".Database/users.csv");
    cout << "[S] Para Sair: ";
    string option;
    cin >> option;
    ClearScreen();
    cout << "Voltando para o menu, aguarde...";
    Pause(2);
}

// Enumeração para as opções do menu do administrador
enum class MenuOptionAdmin {
    AddSupplier = 1,
    ReadSuppliers,
    AddStock,
    ReadStock,
    AddLogin,
    ReadLogins,
    Exit
};

// Enumeração para as opções do menu do funcionário
enum class MenuOptionEmployee {
    ReadSuppliers = 1,
    ReadStock,
    Exit
};

// Exibe o menu para o administrador
void ShowAdminMenu() {
    cout << "1 - Adicionar Fornecedor" << endl;
    cout << "2 - Ler Fornecedores" << endl;
    cout << "3 - Adicionar Produto ao Estoque" << endl;
    cout << "4 - Ler Produtos em Estoque" << endl;
    cout << "5 - Adicionar Login" << endl;
    cout << "6 - Ler Logins" << endl;
    cout << "7 - Sair" << endl;
    cout << "Escolha uma opção: ";
}

// Exibe o menu para o funcionário
void ShowEmployeeMenu() {
    cout << "1 - Ler Fornecedores" << endl;
    cout << "2 - Ler Produtos em Estoque" << endl;
    cout << "3 - Sair" << endl;
    cout << "Escolha uma opção: ";
}

// Gerencia o menu do administrador
void HandleAdminMenu() {
    MenuOptionAdmin option;
    int choice;
    while (option != MenuOptionAdmin::Exit) {
        ShowAdminMenu();
        cin >> choice;
        option = static_cast<MenuOptionAdmin>(choice);
        ClearScreen();
        switch (option) {
            case MenuOptionAdmin::AddSupplier:
                AddSuppliers();
                break;

            case MenuOptionAdmin::ReadSuppliers:
                ReadSuppliers();
                break;

            case MenuOptionAdmin::AddStock:
                AddStockItem();
                break;

            case MenuOptionAdmin::ReadStock:
                ReadStock();
                break;

            case MenuOptionAdmin::AddLogin:
                AddLogin();
                break;

            case MenuOptionAdmin::ReadLogins:
                ReadLogins();
                break;

            case MenuOptionAdmin::Exit:
                cout << "Saindo, aguarde..." << endl;
                Pause(2);
                break;

            default:
                cout << "Opção inválida, tente novamente." << endl;
                Pause(2);
                break;
        }
        ClearScreen();

    }
}

// Gerencia o menu do funcionário
void HandleEmployeeMenu() {
    MenuOptionEmployee option;
    int choice;
    while (option != MenuOptionEmployee::Exit) {
        ShowEmployeeMenu();
        cin >> choice;
        option = static_cast<MenuOptionEmployee>(choice);
        ClearScreen();
        switch (option) {
            case MenuOptionEmployee::ReadSuppliers:
                ReadSuppliers();
                break;
            case MenuOptionEmployee::ReadStock:
                ReadStock();
                break;
            case MenuOptionEmployee::Exit:
                cout << "Saindo, aguarde..." << endl;
                Pause(2);
                break;
            default:
                cout << "Opção inválida, tente novamente." << endl;
                Pause(2);
                break;
        }
        ClearScreen();
    }
}

// Função principal
int main() {
    ClearScreen();
    bool unvalidRole = true;
    while (unvalidRole)
    {
        ClearScreen();
        string login = InputWithClear("Login: ");
        string password = InputWithClear("Senha: ");

        ClearScreen();
        string role = VerifyLogin(login, password);

        if (role == "Admin") {
            unvalidRole = false;
            HandleAdminMenu();
        } else if (role == "Funcionario") {
            unvalidRole = false;
            HandleEmployeeMenu();
        } else {
            cout << "Credenciais inválidas! Informe os dados novamente." << endl;
            Pause(5.5);
        }
    }
    

    return 0;
}
