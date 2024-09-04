#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <limits>
#include <functional>
#include <iomanip>

class DatabaseManager {
public:
    static int GetLastID(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo: " << filepath << std::endl;
            return -1;
        }
        std::string line, lastID;
        while (getline(file, line)) {
            std::stringstream ss(line);
            getline(ss, lastID, ',');
        }
        return lastID.empty() ? 0 : std::stoi(lastID);
    }

    static void AppendToFile(const std::string& filepath, const std::string& data) {
        std::ofstream file(filepath, std::ios::app);
        if (file.is_open()) {
            file << data << std::endl;
        } else {
            std::cerr << "Erro ao abrir o arquivo: " << filepath << std::endl;
        }
    }

    static void DisplayFileContents(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo: " << filepath << std::endl;
            return;
        }
        std::string line;
        std::cout << "______________________________" << std::endl;
        while (getline(file, line)) {
            std::cout << line << std::endl;
            std::cout << "______________________________" << std::endl;
        }
    }
};

void ClearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void Pause(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

bool Confirm(const std::string& prompt) {
    std::string option;
    std::cout << prompt << " (S/N): ";
    std::cin >> option;
    return option == "S" || option == "s";
}

std::string InputWithClear(const std::string& prompt) {
    std::string input;
    do {
        std::cout << prompt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(std::cin, input);
        ClearScreen();
    } while (input.empty());
    return input;
}

void AddSuppliers() {
    std::string name = InputWithClear("Nome: ");
    std::string company = InputWithClear("Empresa: ");
    std::string contact = InputWithClear("Contato: ");
    std::string description = InputWithClear("Descrição: ");

    std::cout << "______________________________" << std::endl;
    std::cout << "Nome: " << name << std::endl;
    std::cout << "Empresa: " << company << std::endl;
    std::cout << "Contato: " << contact << std::endl;
    std::cout << "Descrição: " << description << std::endl;
    std::cout << "______________________________" << std::endl;

    if (Confirm("Deseja salvar este fornecedor?")) {
        int lastID = DatabaseManager::GetLastID(".Database/Fornecedores.csv") + 1;
        if (lastID == -1) return;
        std::stringstream data;
        data << lastID << "," << name << "," << company << "," << contact << "," << description;
        DatabaseManager::AppendToFile(".Database/Fornecedores.csv", data.str());
        ClearScreen();
        std::cout << "Fornecedor salvo com sucesso, aguarde...";
        Pause(2);
    }

    ClearScreen();
    std::cout << "Voltando para o menu...";
    Pause(1);
}

void ReadSuppliers() {
    ClearScreen();
    DatabaseManager::DisplayFileContents(".Database/Fornecedores.csv");
    std::cout << "[S] Para Sair: ";
    std::string option;
    std::cin >> option;
    ClearScreen();
    std::cout << "Voltando para o menu, aguarde...";
    Pause(2);
}

void AddStockItem() {
    std::string item = InputWithClear("Produto: ");
    std::string quantity = InputWithClear("Quantidade: ");
    std::string expiry = InputWithClear("Validade: ");
    std::string pricePerKG = InputWithClear("Valor por KG: ");
    std::string alertLimit = InputWithClear("Limite de alerta: ");

    std::cout << "______________________________" << std::endl;
    std::cout << "Item: " << item << std::endl;
    std::cout << "Quantidade: " << quantity << std::endl;
    std::cout << "Valor por KG: R$ " << pricePerKG << std::endl;
    std::cout << "Validade: " << expiry << std::endl;
    std::cout << "Alerta: " << alertLimit << std::endl;
    std::cout << "______________________________" << std::endl;

    if (Confirm("Deseja salvar este produto?")) {
        int lastID = DatabaseManager::GetLastID(".Database/stock.csv") + 1;
        if (lastID == -1) return;
        std::stringstream data;
        data << lastID << "," << item << "," << quantity << "," << pricePerKG << "," << expiry << "," << alertLimit;
        DatabaseManager::AppendToFile(".Database/stock.csv", data.str());
        ClearScreen();
        std::cout << "Produto salvo com sucesso, aguarde...";
        Pause(2);
    }

    ClearScreen();
    std::cout << "Voltando ao menu principal...";
    Pause(1);
}

void ReadStock() {
    ClearScreen();
    DatabaseManager::DisplayFileContents(".Database/stock.csv");
    std::cout << "[S] Para Sair: ";
    std::string option;
    std::cin >> option;
    ClearScreen();
    std::cout << "Voltando para o menu, aguarde...";
    Pause(2);
}

std::string VerifyLogin(const std::string& userLogin, const std::string& userPassword) {
    std::ifstream loginReader(".Database/users.csv");
    if (!loginReader) {
        std::cerr << "Erro ao abrir o arquivo: .Database/users.csv" << std::endl;
        return "Erro";
    }

    std::string output, id, login, password, role;
    while (getline(loginReader, output)) {
        std::stringstream ss(output);
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

void AddLogin() {
    std::string login = InputWithClear("Login: ");
    std::string password = InputWithClear("Senha: ");
    std::string role = InputWithClear("Cargo: ");

    std::cout << "______________________________" << std::endl;
    std::cout << "Login: " << login << std::endl;
    std::cout << "Senha: " << password << std::endl;
    std::cout << "Cargo: " << role << std::endl;
    std::cout << "______________________________" << std::endl;

    if (Confirm("Deseja cadastrar este login?")) {
        int lastID = DatabaseManager::GetLastID(".Database/users.csv") + 1;
        if (lastID == -1) return;
        std::stringstream data;
        data << lastID << "," << login << "," << password << "," << role;
        DatabaseManager::AppendToFile(".Database/users.csv", data.str());
        ClearScreen();
        std::cout << "Login cadastrado com sucesso, aguarde...";
        Pause(2);
    }

    ClearScreen();
    std::cout << "Voltando ao menu principal...";
    Pause(1);
}

void ReadLogins() {
    ClearScreen();
    DatabaseManager::DisplayFileContents(".Database/users.csv");
    std::cout << "[S] Para Sair: ";
    std::string option;
    std::cin >> option;
    ClearScreen();
    std::cout << "Voltando para o menu, aguarde...";
    Pause(2);
}

enum class MenuOptionAdmin {
    AddSupplier = 1,
    ReadSuppliers,
    AddStock,
    ReadStock,
    AddLogin,
    ReadLogins,
    Exit
};

enum class MenuOptionEmployee {
    ReadSuppliers = 1,
    ReadStock,
    Exit
};

void ShowAdminMenu() {
    std::cout << "1 - Adicionar Fornecedor" << std::endl;
    std::cout << "2 - Ler Fornecedores" << std::endl;
    std::cout << "3 - Adicionar Produto ao Estoque" << std::endl;
    std::cout << "4 - Ler Produtos em Estoque" << std::endl;
    std::cout << "5 - Adicionar Login" << std::endl;
    std::cout << "6 - Ler Logins" << std::endl;
    std::cout << "7 - Sair" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void ShowEmployeeMenu() {
    std::cout << "1 - Ler Fornecedores" << std::endl;
    std::cout << "2 - Ler Produtos em Estoque" << std::endl;
    std::cout << "3 - Sair" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void HandleAdminMenu() {
    MenuOptionAdmin option;
    int choice;
    do {
        ShowAdminMenu();
        std::cin >> choice;
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
                std::cout << "Saindo, aguarde..." << std::endl;
                Pause(2);
                break;
            default:
                std::cout << "Opção inválida, tente novamente." << std::endl;
                Pause(2);
                break;
        }
        ClearScreen();
    } while (option != MenuOptionAdmin::Exit);
}

void HandleEmployeeMenu() {
    MenuOptionEmployee option;
    int choice;
    do {
        ShowEmployeeMenu();
        std::cin >> choice;
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
                std::cout << "Saindo, aguarde..." << std::endl;
                Pause(2);
                break;
            default:
                std::cout << "Opção inválida, tente novamente." << std::endl;
                Pause(2);
                break;
        }
        ClearScreen();
    } while (option != MenuOptionEmployee::Exit);
}

int main() {
    ClearScreen();
    std::string login = InputWithClear("Login: ");
    std::string password = InputWithClear("Senha: ");

    ClearScreen();
    std::string role = VerifyLogin(login, password);

    if (role == "Admin") {
        HandleAdminMenu();
    } else if (role == "Funcionario") {
        HandleEmployeeMenu();
    } else {
        std::cout << "Credenciais inválidas, encerrando o programa." << std::endl;
        Pause(2);
    }

    return 0;
}
