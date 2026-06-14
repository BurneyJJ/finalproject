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
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a number: ";
            cin >> choice;
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

    void dispMenu(){
    //kelangan ng ui/ux, parang header o title ng project :)
    //dito yung menu ([1] add product, [2] update product, eme)
    cout << "\n========================================\n";
    cout << "     INVENTORY MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "   [1] Add Product\n";
    cout << "   [2] Update Product\n";
    cout << "   [3] Delete Product\n";
    cout << "   [4] Search Product\n";
    cout << "   [5] View All Products\n";
    cout << "   [6] Check Stock Status\n";
    cout << "   [7] Process Customer Order\n";
    cout << "   [0] Exit & Save\n";
    cout << "========================================\n";
    cout << "   Slots Available: " << (MAX - productCount) << "/" << MAX << endl;
    cout << "========================================\n";
    }

void addProduct(){
    //check if productCount >= MAX(100)
    if(productCount >= MAX){
        cout << "\n[ERROR] Inventory is full! Cannot add more products.\n";
        return;
    }
    
    //genId();
    Product newProduct;
    newProduct.id = genId();
    
    cout << "\n--- ADD NEW PRODUCT ---\n";
    cout << "Generated ID: " << newProduct.id << endl;
    
    cin.ignore();
    
    //user input
    //Category
    cout << "Enter Category: ";
    getline(cin, newProduct.category);
    while(newProduct.category.empty()){
        cout << "Category cannot be empty. Enter again: ";
        getline(cin, newProduct.category);
    }
    
    //Name of Product
    cout << "Enter Product Name: ";
    getline(cin, newProduct.name);
    while(newProduct.name.empty()){
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, newProduct.name);
    }
    
    //Price
    cout << "Enter Price (PHP): ";
    cin >> newProduct.price;
    while(cin.fail() || newProduct.price < 0){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid price. Enter a positive number: ";
        cin >> newProduct.price;
    }
    
    //Quantity(limit of 100 per product)
    cout << "Enter Quantity (max 100): ";
    cin >> newProduct.quantity;
    while(cin.fail() || newProduct.quantity < 0 || newProduct.quantity > 100){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid quantity. Enter 0-100: ";
        cin >> newProduct.quantity;
    }
    
    //productCount++
    inv[productCount] = newProduct;
    productCount++;
    
    //display amount of product slots left
    cout << "\n[SUCCESS] Product added successfully!\n";
    cout << "Slots remaining: " << (MAX - productCount) << "/" << MAX << endl;
    
    //saveFile();
    saveFile();
}

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
    if (productCount == 0){
        cout << "No products in inventory.\n";
        return;
    }
    cout << left << setw(10) << "ID" 
         << setw(20) << "Name" 
         << setw(15) << "Quantity" 
         << setw(15) << "Status" << endl;
    for (int i = 0; i < productCount; i++){
        string status;
        
        if (inv[i].quantity == 0)
            status = "OUT OF STOCK";
        else if (inv[i].quantity <= 30)
            status = "LOW STOCK";
        else if (inv[i].quantity <= 75)
            status = "IN STOCK";
        else status = "FULL STOCK";
        
        cout << left << setw(10) << inv[i].id
             << setw(20) << inv[i].name
             << setw(15) << inv[i].quantity
             << setw(15) << status << endl;
    }
}

void processOrder(){//customer name input, list all products, enter [product ID, quantity], add another item?, order receipt (display)
                    //save to file "transactions.log" (Customer Name, PHP, Order Info)
}

void saveFile(){//save inventory info ofstream file(fName)
    ofstream file(fName);
    
    if (!file.is_open()) {
        cout << "Error: Could not open file for saving.\n";
        return;
    }

    for (int i = 0; i < productCount; i++) {
        file << inv[i].id << endl;
        file << inv[i].name << endl;
        file << inv[i].category << endl;
        file << inv[i].price << endl;
        file << inv[i].quantity << endl;
    }

    file.close();
}

void loadFile(){//load fName
    ifstream file(fName);
    
    if (!file.is_open()) {
        return; 
    }

    productCount = 0; 

    while (file >> inv[productCount].id && productCount < MAX) {
        
        file.ignore(); 

        getline(file, inv[productCount].name);
        getline(file, inv[productCount].category);
        
        file >> inv[productCount].price;
        file >> inv[productCount].quantity;
        
        file.ignore(); 

        productCount++; 
    }

    file.close();
}

bool confirm(const string& message){
    char c;
    cout << message << "(y/n): ";
    cin >> c;
    while (cin.fail()||(tolower(c) != 'y' && tolower(c) != 'n')){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Enter y or n: ";
        cin >> c;
    }
    cin.ignore();
    
    return (tolower(c) == 'y');
}
