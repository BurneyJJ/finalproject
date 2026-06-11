#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX = 100;
const string fName = "inventory.data";

struct Product{
    int id;
    string name;
    string category;
    double price;
    int quantity;
};

Product inv[MAX];
int productCount = 0;

//Function Prototypes :)
void dispMenu();
void addProduct();
void updProduct();
void delProduct();
void searchProduct();
void viewAll();
void checkStock();
void processOrder();
void saveFile();
void loadFile();
int  findProduct(int id);//kung gusto nila magsearch using ID (1001-1100)
int  genId();//para sa int id, 1000 basis natin? like 1001-1100 para 100 items?
bool confirm(const string& message);//confirmation prompt


int main() {
    
    int choice;
    
    do {
        dispMenu();
        cout << "Enter choice: ";
        cin >> choice;
        while (!(cin >> choice)){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a number: ";
        }
        cin.ignore();
        
        switch (choice){
            case 1: addProduct(); break;
            case 2: updProduct(); break;
            case 3: delProduct(); break;
            case 4: searchProduct(); break;
            case 5: viewAll(); break;
            case 6: checkStock(); break;
            case 7: processOrder(); break;
            case 0:
                saveFile();
                cout << "\nData saved. Goodbye.\n\n";
                break;
            default:
                cout << "\nInvalid choice. Try again.\n\n";
        }
    } while(choice != 0);
    
    return 0;
}
//vvv     okay na to, kayo na sa function definitions @yue @mirabel B)    vvv

void dispMenu(){//dito yung menu ([1] add product, [2] update product, eme)
}

void addProduct(){//add product info
}

void updProduct(){//enter product id, display product info, prompt user to add new info
}

void delProduct(){//enter product id, confirm, delete
}

void searchProduct(){//enter product id, display info if found
}

void viewAll(){//if productCount == 0, none found; otherwise, list all products
}

void checkStock(){//list product info and status (out of stock, low stock, in stock)
}

void processOrder(){//customer name input, list all products, enter [product ID, quantity], add another item?, order receipt (display)
                    //save to file "transactions.log" (Customer Name, PHP, Order Info)
}

void saveFile(){//save inventory info ofstream file(fName)
}

void loadFile(){//load fName
}