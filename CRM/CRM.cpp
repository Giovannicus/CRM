// 1 import libraries
#include <bits/stdc++.h>
using namespace std;
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

// 2 Create class Client
    // 2.1 attributes (private)
    // 2.1.1 interactions: who, object, budget

const size_t CLIENT_SIZE = 7;
const size_t HISTORY_SIZE = 2;
struct Interaction{
    string first_name;
    string last_name;
    string email;
    string object;
    string income;
};

class Client{
    private:
        string first_name;
        string last_name;
        string email;
        string capital;
        array<Interaction,HISTORY_SIZE> history;
    
    public:
        friend class JSON;
        friend void createClients(array<Client,CLIENT_SIZE>& clients);
    // 2.2 methods
        string getFirstName() const {
            return first_name;
        }
        void setFirstName(const string& s_){
            first_name = s_;
        }
        string getLastName() const {
            return last_name;
        }
        void setLastName(const string& s_){
            last_name = s_;
        }
        string getEmail() const {
            return email;
        }
        void setEmail(const string& s_){
            email = s_;
        }

        void addInteraction(){
            size_t fs;
            string input;
            fs = findFreeSlotInteractions();
            if (fs == HISTORY_SIZE){
                cout << "Sorry, no memory place available for new interactions of this client! Delete some." << endl;
                return;
            }
            cout << "You are going to insert info about whom client interacted with"<< endl;
            cout << "Insert first name:";
            cin >> getHistory()[fs].first_name;
            cout << "Insert last name:";
            cin >> getHistory()[fs].last_name;
            cout << "Insert email:";
            cin >> getHistory()[fs].email;
            cout << "Insert object:";
            cin >> getHistory()[fs].object;
            cout << "Insert income:";
            cin >> getHistory()[fs].income;
        }

        void deleteInteraction(){
            string input1;
            string input2;
            cout << "Type interaction object" << endl;
            getline(cin,input1);
            cout << "Type interaction email" << endl;
            getline(cin,input2);
            for (Interaction& i : getHistory()){
                if (i.email == input2 && i.object == input1){
                    i.first_name = "";
                    i.last_name = "";
                    i.email == "";
                    i.object = "";
                    i.income = "";
                }
            }
        }

        size_t findFreeSlotInteractions(){
            size_t i = 0;
            for (Interaction i_ : history){
                if (i_.object.empty()){
                    return i;
                }
                i++;
            }
            return i;
        }

        void manageInteractions(){
            // this function is called in the menu to add or delete interactions
            int choice;
            cout << "Do you want to...? " << endl;
            cout << "1 add, 2 delete interaction, 3 doesn't matter: ";
            choice = getValidChoice(3);
            switch (choice){
                case 1:
                    addInteraction();
                    break;
                case 2:
                    deleteInteraction();
                    break;
                case 3:
                    cout << "Ok, back to menu" << endl;
                    break;
            }

        }

        void resetClient(){
            // deletes a client, it won't be saved to json
            first_name = "";
            last_name = "";
            email = "";
            capital = "";
        }
        void setClient(){
            // called in menu to modify a client
            cout << "What do you want to modify?" << endl;
            cout << "1 first name, 2 last name, 3 email, 4 capital, 5 interactions, 6 doesn't matter: ";
            int choice = getValidChoice(6);
            string input;
            switch (choice){
                case 1:
                    cout << "Insert new first name: ";
                    getline(cin,input);
                    setFirstName(input);
                    break;
                case 2:
                    cout << "Insert new last name: ";
                    getline(cin,input);
                    setLastName(input);
                    break;    
                case 3:
                    cout << "Insert new email: ";
                    getline(cin,input);
                    setEmail(input);
                    break;
                case 4:
                    cout << "Insert new capital: ";
                    getline(cin,input);
                    setCapital(input);
                    break;
                case 5:
                    manageInteractions();
                    break;    
                case 6:
                    cout << "Ok, back to menu" << endl;
                    break; 

            }

        }
        int getValidChoice(int n){
            // user is ought to insert a valid number between 1 and n
            int choice;
            while (true) {
                string input;
                getline(cin, input); // Legge l'intera riga
                try {
                    choice = stoi(input); // Converti la stringa a intero
                } catch (invalid_argument&) {
                    cout << "Invalid input, try again" << endl;
                    continue;
                }

                if (choice >= 1 && choice <= n) {
                    break; // Input valido, esci dal ciclo
                } else {
                    cout << "Invalid input, try again" << endl;
                }
            }
            return choice;
        }

        string getCapital() const {
            return capital;
        }
        void setCapital(string& c_){
            capital= c_;
        }
        void showInteraction(const Interaction& i) const {
            cout << left << setw(15) << "Firstname"
                        << setw(15) << "Lastname"
                        << setw(25) << "Email"
                        << setw(15) << "Object"
                        << setw(10) << "Income" << endl;
            cout << left << setw(15) << i.first_name
                        << setw(15) << i.last_name
                        << setw(25) << i.email
                        << setw(15) << i.object
                        << setw(10) << i.income << endl;
        }
        
        array<Interaction,HISTORY_SIZE>& getHistory() {
            return history;
        }

        void viewClient(){
            // Called in the menu to display a searched client. Avoids showing blank interactions as they won't  be saved on json
            if (getLastName().empty()){
                return;
            }
            cout << left << setw(15) << "Firstname"
                        << setw(15) << "Lastname"
                        << setw(25) << "Email"
                        << setw(10) << "Capital" << endl;
            cout << left << setw(15) << getFirstName()
                        << setw(15) << getLastName()
                        << setw(25) << getEmail()
                        << setw(10) << getCapital() << endl;
            cout << "Interactions of " << getFirstName() << " " << getLastName() << endl;
            for (Interaction i : getHistory()){
                if (i.first_name.empty()){
                    continue;
                }
                showInteraction(i);
            }
            cout << "\n" << endl;
        }
};

// 3 Graphic interface for command line
    // 3.1 define JSON class (friend of Client) to manage saving and loading from a json file. 
        // Clients list is loaded from a json file as a an array of Client. When execution is over and the program is going to be close,
        // the client list gets updated on the json
    class JSON{
    public:

        void saveJSON(const array<Client,CLIENT_SIZE>& clients,const string& filename){
            // save clients on json
            json j;

            for (const auto& client : clients) {

            json client_json;
            // avoids deleted clients
            if (client.last_name.empty()){
                continue;
            }
            client_json["first_name"] = client.first_name;
            client_json["last_name"] = client.last_name;
            client_json["email"] = client.email;
            client_json["capital"] = client.capital;

            json history_json;
            for (const auto& interaction : client.history) {
                json interaction_json;
                interaction_json["first_name"] = interaction.first_name;
                interaction_json["last_name"] = interaction.last_name;
                interaction_json["email"] = interaction.email;
                interaction_json["object"] = interaction.object;
                interaction_json["income"] = interaction.income;
                if (interaction.object.empty()){
                    continue;
                }
                history_json.push_back(interaction_json);
            }
                client_json["history"] = history_json;

                j.push_back(client_json);
            }

            ofstream file(filename);
            if (file.is_open()) {
                file << j.dump(4);
                file.close();
            } else {
                std::cerr << "No way writing this file: " << filename << "\n";
            }
            
        }

        void loadJSON(array<Client,CLIENT_SIZE>& clients,const string& filename){
            // takes all clients in a json file and load to clients array
            ifstream file(filename);
            if (file.is_open()) {
                json j;
                file >> j;
                file.close();

                size_t index = 0;
                for (const auto& client_json : j) {
                    if (index >= clients.size()) break;

                    Client& client = clients[index++];
                    client.first_name = client_json["first_name"];
                    client.last_name = client_json["last_name"];
                    client.email = client_json["email"];
                    client.capital = client_json["capital"];

                    size_t history_index = 0;
                    for (const auto& interaction_json : client_json["history"]) {
                        if (history_index >= client.history.size()) break;

                        Interaction& interaction = client.history[history_index++];
                        interaction.first_name = interaction_json["first_name"];
                        interaction.last_name = interaction_json["last_name"];
                        interaction.email = interaction_json["email"];
                        interaction.object = interaction_json["object"];
                        interaction.income = interaction_json["income"];
                    }
                }
            } else {
                std::cerr << "No way reading this file: " << filename << "\n";
            }
        }
};
    
    // 3.2 define GI class and its functions
        // 3.2.1 add client in CRM
        // 3.2.2 delete client from CRM
        // 3.2.3 edit a client attributes in CRM
        // 3.2.4 manage client interactions
        // 3.2.5 search and visualize clients    
class GI{
    public:
        void viewAllClients(const array<Client,CLIENT_SIZE>& clients){
            for (Client c : clients){
                c.viewClient();
            }
        }

        size_t findFreeSlot(const array<Client,CLIENT_SIZE>& clients){
            size_t i = 0;
            for (Client c : clients){
                if (c.getLastName().empty()){
                    return i;
                }
                i++;
            }
            return i;
        }


        void addClient(array<Client,CLIENT_SIZE>& clients){
            // adds a client in the first location available and checks it already exists            
            string input1;
            string input2;
            string input3;
            string cap;
            size_t fs = findFreeSlot(clients);

            if (fs == CLIENT_SIZE){
                cout << "Sorry, no more space available for adding. Delete someone!" << endl;
                return;
            }

            cout << "Insert first name:";
            getline(cin,input1);
            cout << "Insert last name:";
            getline(cin,input2);
            cout << "Insert email:";
            getline(cin,input3);
            for (Client c : clients ){
                if (c.getFirstName() == input1 && c.getLastName() == input2 && c.getEmail() == input3){
                    cout << "Account already exists!"<< endl;
                    return;
                }
            }

            clients[fs].setFirstName(input1);
            clients[fs].setLastName(input2);
            clients[fs].setEmail(input3);
            cout << "Insert capital:";
            getline(cin,input1);
            clients[fs].setCapital(input1);
            cout << "Client created! You can add interactions by its omonimous command" << endl;
        }

        array<string,2> firstChance(string action, void (Client::*_func)(),string msg, array<Client,CLIENT_SIZE>& clients,string item, string (Client::*func)() const){
            string input;
            // take a choice
            cout << "Type the " << item << " you want to " << action << ": ";
            getline(cin,input);
            size_t i = 0;
            vector<size_t> idx;
            // search 
            for (Client c : clients){
                if ((c.*func)() == input){
                    c.viewClient();
                    idx.push_back(i);
                }
                i++;
            }
            // get a result or try again
            if(idx.size()==0 || input.empty()){
                string s = "No client corresponding to " + input + " has been found";
                cout << s << endl;
                array<string,2> ss;
                ss[0] = "%";
                ss[1] = input;
                return ss;
            }else if(idx.size()==1){
                (clients[idx[0]].*_func)();
                string s = input + msg;
                cout << s << endl;
                array<string,2> ss;
                ss[0] = s;
                ss[1] = input;
                return ss;
            }else{
                array<string,2> ss;
                ss[0] = "";
                ss[1] = input;
                return ss;
            }
        }

        array<string,3> secondChance(string action, void (Client::*_func)(), string msg, array<Client,CLIENT_SIZE>& clients, string (Client::*func1)() const,
                                                                string item2, string (Client::*func2)() const,const string& input1){
            string input2;
            // take a choice
            cout << "Type the " << item2 << " you want to "<< action <<":";
            getline(cin,input2);
            size_t i = 0;
            vector<size_t> idx;
            // search 
            for (Client c : clients){
                if (((c.*func1)() == input1) && ((c.*func2)() == input2)){
                    c.viewClient();
                    idx.push_back(i);
                }
                i++;
            }
            // get a result or try again
            if(idx.size()==0 || input2.empty()){
                string s = "No client corresponding to " + input1 + " " + input2 + " has been found";
                cout << s << endl;
                array<string,3> ss;
                ss[0] = "%";
                ss[1] = input1;
                ss[2] = input2;
                return ss;
            }else if(idx.size()==1){
                (clients[idx[0]].*_func)();
                string s = input1 + " " + input2 + msg;
                cout << s << endl;
                array<string,3> ss;
                ss[0] = s;
                ss[1] = input1;
                ss[2] = input2;
                return ss;
            }else{
                array<string,3> ss;
                ss[0] = "";
                ss[1] = input1;
                ss[2] = input2;
                return ss;
            }
        }

        void thirdChance(string action, void (Client::*_func)(), string msg, array<Client,CLIENT_SIZE>& clients, string (Client::*func1)() const,
                                                                 string (Client::*func2)() const,
                                                                string item3, string (Client::*func3)() const,
                                                                const string& input, const string& input2){
            string input3;
            // take a choice
            cout << "Type the " << item3 << " you want to "<< action <<":";
            getline(cin,input3);
            size_t i = 0;
            vector<size_t> idx;
            // search 
            for (Client c : clients){
                if (((c.*func1)() == input) && ((c.*func2)() == input2) && ((c.*func3)() == input3)){
                    c.viewClient();
                    idx.push_back(i);
                }
                i++;
            }
            // get a result or try again
            if(idx.size()==0|| input3.empty()){
                string s = "No client corresponding to " + input + " " + input2 + " " + input3 + " has been found";
                cout << s << endl;
                return;
            }else if(idx.size()==1){
                (clients[idx[0]].*_func)();
                string s = input + " " + input2 + " " + input3 + msg;
                cout << s << endl;
                return;
            }else{
                return;
            }
        }

        void manageClient(array<Client,CLIENT_SIZE>& clients,string action,  void (Client::*_func)(),string msg){
            // A model function for searching for a target client to perform operations on.
            // It is a model for delete, edit and search client functions of the menu.
            // Once a clients is found with the following procedure, you can submit a function that will be applied to the found client.
            int chose;
            string input;
            cout << "Would you like to "<< action <<" by:"<< endl;
            cout << "1 firstname, 2 lastname, 3 back to menu "<< endl;

            chose = getValidChioce(3);
            array<string,2> s;
            array<string,3> ss;
            // Allows searching by first or last name. 
            // If multiple clients match the initial search, it prompts for further details until a single client is identified.
            switch (chose) {
                case 1:
                    s = firstChance(action,_func,msg,clients,"firstname", &Client::getFirstName);
                    if (!s[0].empty()){
                        break;
                    } else {
                        ss = secondChance(action,_func,msg,clients, &Client::getFirstName,
                                                                            "lastname", &Client::getLastName,s[1]);
                        if(!ss[0].empty()){
                            break;
                        }else{
                            thirdChance(action,_func,msg,clients, &Client::getFirstName,
                                                                         &Client::getLastName,
                                                                         "email",&Client::getEmail,
                                                                            ss[1], ss[2]);
                            break;
                        }
                    }

                case 2:
                    s = firstChance(action,_func,msg,clients,"lastname", &Client::getLastName);
                    if (!s[0].empty()){
                        break;
                    } else {
                        ss = secondChance(action,_func,msg,clients, &Client::getLastName,
                                                                            "firstname", &Client::getFirstName,s[1]);
                        if(!ss[0].empty()){
                            break;
                        }else{
                            thirdChance(action,_func,msg,clients, &Client::getLastName,
                                                                          &Client::getFirstName,
                                                                         "email",&Client::getEmail,
                                                                            ss[1], ss[2]);
                            break;
                        }
                    }
                case 3:
                    cout << "Ok, back to menu\n" << endl;
                    break;
            }
        }

        void deleteClient(array<Client,CLIENT_SIZE>& clients,void (Client::*_func)()){
            manageClient(clients,"delete",_func," has been deleted!");
        }
        void editClient(array<Client,CLIENT_SIZE>& clients,void (Client::*_func)()){
            manageClient(clients,"modify",_func," has been modified!");
        }
        void searchClient(array<Client,CLIENT_SIZE>& clients,void (Client::*_func)()){
            manageClient(clients,"search",_func," found!");
        }

        int getValidChioce(int n){
            int choice;
            while (true) {
                string input;
                getline(cin, input);
                try {
                    choice = stoi(input);
                } catch (invalid_argument&) {
                    cout << "Invalid input, try again" << endl;
                    continue;
                }

                if (choice >= 1 && choice <= n) {
                    break; 
                } else {
                    cout << "Invalid input, try again" << endl;
                }
            }
            return choice;
        }
    // 3.3 create a menu
        void menu(array<Client,CLIENT_SIZE>& clients){
            cout << "Welcome 2 our CRM platform!" << endl;
            cout << "What would you like to do?" << endl;
            bool willin = true;
            int choice;
            while(willin){
                cout << "1 add client, 2 delete client, 3 edit client, 4 search client, 5 view all clients, 6 reset, 7 exit: ";
                choice = getValidChioce(7);
                switch (choice){
                    case 1:
                        addClient(clients);
                        break;
                    case 2:
                        deleteClient(clients,&Client::resetClient);
                        break;
                    case 3:
                        editClient(clients,&Client::setClient);
                        break;
                    case 4:
                        searchClient(clients,&Client::viewClient);
                        break;
                    case 5:
                        viewAllClients(clients);
                        break;
                    case 6:
                        createClients(clients);
                        break;
                    case 7:
                        // Exit
                        willin = false;
                        break;        
                    default:
                        cout << "Invalid input. Insert a 1-7 number" << endl;
                        choice = getValidChioce(7);
                        break;
                }
            }
            cout << "Exiting" << endl;
        }
};

void createClients(array<Client,CLIENT_SIZE>& clients){
    // resets clients list to default
    clients[size_t(0)].first_name = "Geeno";
    clients[size_t(0)].last_name = "Palladino";
    clients[size_t(0)].email = "gepa@gmail.com";
    clients[size_t(0)].capital = "5";
    clients[size_t(0)].history[size_t(0)].first_name = "Pinco";
    clients[size_t(0)].history[size_t(0)].last_name = "Pallino";
    clients[size_t(0)].history[size_t(0)].email = "pipa@gmail.com";
    clients[size_t(0)].history[size_t(0)].object = "food";
    clients[size_t(0)].history[size_t(0)].income = "-70";
    clients[size_t(0)].history[size_t(1)].first_name = "Billy";
    clients[size_t(0)].history[size_t(1)].last_name = "Ballo";
    clients[size_t(0)].history[size_t(1)].email = "biba@gmail.com";
    clients[size_t(0)].history[size_t(1)].object = "dance lessons";
    clients[size_t(0)].history[size_t(1)].income = "-80";

    clients[size_t(1)].first_name = "Mario";
    clients[size_t(1)].last_name = "Rossi";
    clients[size_t(1)].email = "maro@gmail.com";
    clients[size_t(1)].capital = "0";
    clients[size_t(1)].history[size_t(0)].first_name = "Pinco";
    clients[size_t(1)].history[size_t(0)].last_name = "Pallino";
    clients[size_t(1)].history[size_t(0)].email = "pipa@gmail.com";
    clients[size_t(1)].history[size_t(0)].object = "food";
    clients[size_t(1)].history[size_t(0)].income = "-70";
    clients[size_t(1)].history[size_t(1)].first_name = "Billy";
    clients[size_t(1)].history[size_t(1)].last_name = "Ballo";
    clients[size_t(1)].history[size_t(1)].email = "biba@gmail.com";
    clients[size_t(1)].history[size_t(1)].object = "dance shoes";
    clients[size_t(1)].history[size_t(1)].income = "90";

    clients[size_t(2)].first_name = "Berto";
    clients[size_t(2)].last_name = "D'Almata";
    clients[size_t(2)].email = "beAl@gmail.com";
    clients[size_t(2)].capital = "-10";
    clients[size_t(2)].history[size_t(0)].first_name = "Pinco";
    clients[size_t(2)].history[size_t(0)].last_name = "Pallino";
    clients[size_t(2)].history[size_t(0)].email = "pipa@gmail.com";
    clients[size_t(2)].history[size_t(0)].object = "beverage";
    clients[size_t(2)].history[size_t(0)].income = "-70";

    // following are good for fast testing search with multiple clients match
    clients[size_t(3)].first_name = "a";
    clients[size_t(3)].last_name = "a";
    clients[size_t(3)].email = "a";
    clients[size_t(3)].capital = "-1";
    clients[size_t(3)].history[size_t(0)].first_name = "Pinco";
    clients[size_t(3)].history[size_t(0)].last_name = "Pallino";
    clients[size_t(3)].history[size_t(0)].email = "pipa@gmail.com";
    clients[size_t(3)].history[size_t(0)].object = "beverage and food";
    clients[size_t(3)].history[size_t(0)].income = "-150";

    clients[size_t(4)].first_name = "a";
    clients[size_t(4)].last_name = "a";
    clients[size_t(4)].email = "w";
    clients[size_t(4)].capital = "-12";
    clients[size_t(4)].history[size_t(0)].first_name = "Pinco";
    clients[size_t(4)].history[size_t(0)].last_name = "Pallino";
    clients[size_t(4)].history[size_t(0)].email = "pipa@gmail.com";
    clients[size_t(4)].history[size_t(0)].object = "beer";
    clients[size_t(4)].history[size_t(0)].income = "-100";

    clients[size_t(5)].first_name = "a";
    clients[size_t(5)].last_name = "b";
    clients[size_t(5)].email = "w";
    clients[size_t(5)].capital = "-2";
    clients[size_t(5)].history[size_t(0)].first_name = "Pinco";
    clients[size_t(5)].history[size_t(0)].last_name = "Pallino";
    clients[size_t(5)].history[size_t(0)].email = "pipa@gmail.com";
    clients[size_t(5)].history[size_t(0)].object = "beer";
    clients[size_t(5)].history[size_t(0)].income = "-10";

}

// 4 create main
    // 4.1 cycle menu til user says enough
    // 4.2 loads CRM clients from json at the begin and saves them to json at the end
int main(){
    JSON* pjson;
    pjson = new JSON;
    GI pgi;
    array<Client, CLIENT_SIZE> clients;

    pjson->loadJSON(clients,"clients.json");
    delete pjson;
    
    pgi.menu(clients);
    pjson = new JSON;
    pjson->saveJSON(clients,"clients.json");
    return 0;
}