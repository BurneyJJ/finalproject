#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>
using namespace std;

const int MAX = 100;
const int NUM_CATEGORIES = 10;
const string fName = "inventory.data";
const string CATEGORIES[NUM_CATEGORIES] = {
    "Fresh Produce", "Meat & Poultry", "Seafood", "Beverage", "Sweets", "Dairy", "Dry Goods", "Bakery", "Personal Care", "Cleaning Supplies"
};
struct Product{
    int id;
    int categoryIndex;
    string name;
    double price;
    int quantity;
};
struct OrderItem{
    int productIndex;
    int quantity;
};
Product inv[MAX];
int productCount = 0;

void productInfo(int i);
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
void printHeader();
void sortByID();
int selectCategory();
int findProduct(int id);
int genId(int categoryIndex);
int getInput(const string& prompt, int min, int max);
string stockStatus(int qty);
string tolowerCase(string s);
bool confirm(const string& message);

int main() {
    loadFile();
    sortByID();
    int choice;
    do {
        dispMenu();
        choice = getInput("Enter choice: ", 0, 7);
        
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
                cout << "\nData saved. Goodbye!\n\n";
                break;
            default:
                cout << "\n[!] Invalid choice! Try again.\n\n";
        }
    } while(choice != 0);
    
    return 0;
}

void dispMenu(){
    cout << "\n========================================\n"
         << "     INVENTORY MANAGEMENT SYSTEM"
         << "\n========================================\n"
         << "   [1] Add Product\n"
         << "   [2] Update Product\n"
         << "   [3] Delete Product\n"
         << "   [4] Search Product\n"
         << "   [5] View All Products\n"
         << "   [6] Check Stock Status\n"
         << "   [7] Process Customer Order\n"
         << "   [0] Exit & Save\n"
         << "========================================\n"
         << "          Slots Available: " << (MAX - productCount) << "/" << MAX << endl
         << "========================================\n";
}

void addProduct(){
    Product newProduct;
    
    do{
        if(productCount >= MAX){
            cout << "\n[!] Inventory is full! You cannot add more products.\n";
            return;
        }
        newProduct.categoryIndex = selectCategory();
        newProduct.id = genId(newProduct.categoryIndex);
        
        cout << "Generated ID: " << newProduct.id << " (" 
             << CATEGORIES[newProduct.categoryIndex] << ")" << endl;
             
        cin.ignore();
        cout << "Please Enter Product Name: ";
        getline(cin, newProduct.name);
        while(newProduct.name.empty()){
            cout << "[!] Name cannot be empty. Enter again: ";
            getline(cin, newProduct.name);
        }
        
        for(int i = 0; i < productCount; i++){
            if(tolowerCase(inv[i].name) == tolowerCase(newProduct.name)){
                cout << "[!] Product already exists.\n";
                return;
            }
        }
    
        newProduct.price = getInput("Enter Price (PHP): ", 0, 1000);
        newProduct.quantity = getInput("Enter Quantity (MAX 100): ", 0, 100);
        inv[productCount] = newProduct;
        productCount++;
        sortByID();
        cout << "========================================================\n"
             << "[SUCCESS!] Your product has been added successfully!\n"
             << "Slots remaining: " << (MAX - productCount) << "/" << MAX << endl
             << "========================================================\n";
        saveFile();
    }while(confirm("Do you still want to add more to your inventory? "));
}

void updProduct(){
    if (productCount == 0){
        cout << "\n[!] No products found in the inventory.\n";
        return;
    }
    int id = getInput("Enter Product ID: ", 1001, 11000);
    
    int index = findProduct(id);
    if (index == -1){
        cout << "[!] Product not found.\n";
        return;
    }
    productInfo(index);
    
    cout << "\nDo you want to make an update?\n";
    cout << "  [1] Price \n";
    cout << "  [2] Quantity\n";
    cout << "  [0] Cancel/Exit\n";
    
    int choice = getInput("Enter Your Choice: ", 0, 2);
    
    switch (choice){
        case 1: 
            inv[index].price = getInput("Enter New Price: ", 0, 1001);
            cout << "[SUCCESS!] Price has been updated successfully!\n";
            break;
        case 2:
            inv[index].quantity = getInput("Enter New Quantity[MAX 100]: ", 0, 100);
            cout << "[SUCCESS!] Quantity has been updated successfully!\n";
            break;
        case 0: 
            cout << "Thank you! No changes were made.\n";
            break;
    }
    saveFile();
}

void delProduct(){
    if (productCount == 0){
        cout << "\n[!] No products found in the inventory.\n";
        return;
    }
    
    int id = getInput("Enter Product ID: ", 1001, 11000);
    
    int index = findProduct(id);
    if (index == -1){
        cout << "[!] Product not found.\n";
        return;
    }
    productInfo(index);
    
    if (confirm("Are you sure you want to delete this product? ")){
        for (int i = index; i < productCount-1; i++){
            inv[i] = inv[i+1];
        }
        productCount--;
        sortByID();
        saveFile();
        cout << "[SUCCESS!] Product has been updated!";
    } else cout << "Deletion cancelled. No updates has been made.";
    cout << "Slots remaining: " << (MAX - productCount) << "/" << MAX << "\n";
}

void searchProduct(){
    bool found = false;
    cout << "How would you like to search a product? \n";
    cout << "  [1] ID \n";
    cout << "  [2] Name\n";
    int choice = getInput("Enter Your Choice: ", 0, 2);
    
    switch (choice){
        case 1:{
            int id = getInput("Enter Product ID: ", 1001, 11000);
            int index = findProduct(id);
            if (index != -1) {
                cout << "\nProduct Found!\n";
                productInfo(index);
                found = true;
            };
        }break;
        case 2:{
            string productName;
            cin.ignore();
            cout << "Please enter product name: ";
            getline(cin, productName);
            for (int i = 0; i < productCount; i++) {
                if (tolowerCase(inv[i].name) == tolowerCase(productName)) {
                    cout << "\nProduct Found!\n";
                    productInfo(i);
                    found = true;
                }
            }
        }break;
    }
    
    if (!found) {
        char choice;
        cout << "[!] Product not found. Add product? (Y/N): ";
        cin >> choice;
        if (choice == 'Y' || choice == 'y') addProduct();
    }
}

void viewAll(){
    if (productCount == 0){
        cout << "[!] No products found in the inventory.\n";
        return;
    }
    sortByID();
    printHeader();
    cout << "\n=====================================================================\n";
    for (int i = 0; i < productCount; i++) {
        cout << setw(10) << inv[i].id << "|"
             << setw(19) << CATEGORIES[inv[i].categoryIndex] << "|"
             << setw(19) << inv[i].name << "|" 
             << setw(9) << inv[i].price << "|" 
             << setw(14) << inv[i].quantity  << endl;
    }
}

void checkStock(){
    if (productCount == 0){
        cout << "[!] No products found in the inventory.\n";
        return;
    }
    printHeader();
        cout << setw(15) << "|STATUS"
             << "\n========================================================================================\n";
    for (int i = 0; i < productCount; i++){
        cout << left << setw(10) << inv[i].id << "|"
             << setw(19) << CATEGORIES[inv[i].categoryIndex] << "|"
             << setw(19) << inv[i].name << "|"
             << setw(9) << inv[i].price << "|"
             << setw(14) << inv[i].quantity << "|"
             << setw(9) << stockStatus(inv[i].quantity) << endl;
    }
}

void processOrder(){
    if (productCount == 0){
        cout << "[!] No products found in the inventory.\n";
        return;
    }
    string customerName;
    OrderItem cart[MAX];
    int cartCount = 0;
    
    cin.ignore();
    cout << "Customer Name: ";
    getline(cin, customerName);
    
    viewAll();
    
    do{
        int id = getInput("Enter Product ID: ", 1001, 11000);
        
        int index = findProduct(id);
        if (index == -1){
            cout << "[!] Product not found.\n";
            continue;
        }

        int qty = getInput("Quantity to Buy: ", 1, 100);
        if (cartCount >= MAX){
                cout << "[!] Order is full.\n";
                break;
        }
        if (qty > inv[index].quantity){
            cout << "[!] Not enough stock! Available: " << inv[index].quantity << endl;
            continue;
        } else {
            cart[cartCount].productIndex = index;
            cart[cartCount].quantity = qty;
            cartCount++;
            cout << "[SUCCESS!] Added to order!\n";
        }
    }while(confirm("Add another item? "));
    
    if (cartCount == 0){
        cout << "No items ordered.\n";
        return;
    }
    
    double total = 0;
    cout << "=====================================================\n"
         << "                   ORDER SUMMARY                     \n"
         << "=====================================================\n";
    cout << left << setw(10) << "ID" 
         << setw(20) << "Name" 
         << setw(15) << "Quantity" 
         << setw(15) << "Subtotal" << endl; 
         
    for (int i = 0; i < cartCount; i++){
        int p = cart[i].productIndex;
        double subtotal = cart[i].quantity * inv[p].price;
        total += subtotal;
        cout << left << setw(10) << inv[p].id
             << setw(20) << inv[p].name
             << setw(15) << cart[i].quantity
             << setw(15) << fixed << setprecision(2) << subtotal << endl; 
    }
    
    cout << "\nTOTAL: PHP " << fixed << setprecision(2) << total << endl
         << "=====================================================\n";
    
    while(confirm("Would you like to remove or edit an item? ")){
        int cartIndex = -1;
        int editId = getInput("Enter Product ID: ", 1001, 11000);
        
        for (int i = 0; i < cartCount; i++){
            int p = cart[i].productIndex;
            
            if (inv[p].id == editId){
                cartIndex = i;
                break;
            }
        }
        
        if (cartIndex == -1) cout << "[!] Product not found in order.\n";
        else{
            if (confirm("Remove product entirely?")){
                for(int i = cartIndex; i < cartCount - 1; i++){
                    cart[i] = cart[i + 1];
                }
                cartCount--;
                
                if(cartCount == 0){
                    cout << "[SUCCESS!] Order is now empty!\n";
                    return;
                }
                cout << "[SUCCESS!] Product removed!\n";
            } else {
                int p = cart[cartIndex].productIndex;
                int newQuantity= getInput("Enter New Quantity(MAX 100): ", 0, 100);
                cart[cartIndex].quantity = newQuantity;
                cout << "[SUCCESS!] Quantity updated!\n";
            }
        }
    }
    
    total = 0;
    for (int i = 0; i < cartCount; i++){
        int p = cart[i].productIndex;
        total += cart[i].quantity * inv[p].price;
    }
    cout << "\nUpdated Total: PHP " << fixed << setprecision(2) << total << endl;
    
    if (!confirm("Proceed to payment? ")){
        cout << "Order cancelled.\n";
        return;
    }
    
    double payment;
    do{
        payment = getInput("Payment Amount (PHP): ", 0, 100000);
        if (payment < total) cout << "[!] Insufficient payment.\n";
    }while(payment < total);
    
    double change = payment - total;
    
    for (int i = 0; i < cartCount; i++){
        int p = cart[i].productIndex;
        inv[p].quantity -= cart[i].quantity;
    }
    
    saveFile();
    cout << "==========================================================\n"
         << "                       ORDER RECEIPT                      \n"
         << "==========================================================\n";
    cout << "Customer: " << customerName << endl;
    cout << left << setw(20) << "Name" 
         << setw(15) << "Quantity" 
         << setw(15) << "Price" 
         << setw(15) << "Subtotal" << endl; 
    for (int i = 0; i < cartCount; i++){
        int p = cart[i].productIndex;
        double subtotal = cart[i].quantity * inv[p].price;
        cout << left << setw(20) << inv[p].name
             << setw(15) << cart[i].quantity
             << setw(15) << fixed << setprecision(2) << inv[p].price 
             << setw(15) << subtotal << endl;
    }
    cout << "\nTotal  : PHP " << total << endl
         << "Payment: PHP " << payment << endl
         << "Change : PHP " << change << endl
         << "==========================================================\n";
    
    ofstream log("transactions.log", ios::app);
    if(log.is_open()){
        log << fixed << setprecision(2);
        log << "\n-------\n";
        log << "Customer: " << customerName << endl;
        for (int i = 0; i < cartCount; i++){
            int p = cart[i].productIndex;
            double subtotal = cart[i].quantity * inv[p].price;
            log << inv[p].name << " x" << cart[i].quantity 
                << " = PHP " << subtotal << endl;
        }
        log << "Total  : PHP " << total << endl;
        log << "Payment: PHP " << payment << endl;
        log << "Change : PHP " << change << endl;
        log.close();
    }
    cout << "\n[SUCCESS!] Transaction completed successfully!\n";
}

void saveFile(){
    ofstream file(fName);
    if (!file.is_open()) {
        cout << "[!] Could not open file for saving.\n";
        return;
    }
    for (int i = 0; i < productCount; i++) {
        file << inv[i].id << endl;
        file << inv[i].categoryIndex << endl;
        file << inv[i].name << endl;
        file << inv[i].price << endl;
        file << inv[i].quantity << endl;
    }
    file.close();
}

void loadFile(){
    ifstream file(fName);
    if (!file.is_open()) {
        return; 
    }
    productCount = 0; 
    while (productCount < MAX && file >> inv[productCount].id){
        file >> inv[productCount].categoryIndex;
        file.ignore();
        getline(file, inv[productCount].name);
        file >> inv[productCount].price;
        file >> inv[productCount].quantity;
        file.ignore(); 
        productCount++; 
    }
    file.close();
}

void productInfo(int i){
        cout << "\nCurrent Product Information:\n"
             << "  ID              : " << inv[i].id << '\n'
             << "  Category        : " << CATEGORIES[inv[i].categoryIndex] << '\n'
             << "  Product Name    : " << inv[i].name<< '\n'
             << "  Price (PHP)     : " << inv[i].price << '\n'
             << "  Quantity        : " << inv[i].quantity << '\n';
}

void printHeader(){
    cout << left << setw(10) << "ID"
         << setw(20) << "|CATEGORY"
         << setw(20) << "|NAME"
         << setw(10) << "|PRICE"
         << setw(15) << "|QUANTITY";
}

void sortByID(){
    for(int i = 0; i < productCount - 1; i++){
        for(int j = 0; j < productCount - i - 1; j++){
            if(inv[j].id > inv[j + 1].id){
                Product temp = inv[j];
                inv[j] = inv[j + 1];
                inv[j + 1] = temp;
            }
        }
    }
}

int getInput(const string& prompt, int min, int max){
    int value;
    cout << prompt;
    cin >> value;
    while(cin.fail() || value < min || value > max){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "[!] Invalid input. " << prompt ;
        cin >> value;
    }
    return value;
}

int findProduct(int id){
    for(int i = 0; i < productCount; i++){
        if(inv[i].id == id){
            return i;
        }
    }
    return -1;
}

int selectCategory(){
    cout << "\n=== SELECT CATEGORY ===\n";
    for (auto i = 0; i < NUM_CATEGORIES; i++){
        cout << "[" << i + 1 << "]" << CATEGORIES[i] << endl;
    }
    cout << "=======================\n";
    int choice = getInput("Enter Category Number: ", 1, 10);
    return choice - 1;
}

int genId(int categoryIndex){
    int baseID = (categoryIndex + 1) * 1000;
    int newID = baseID + 1;
    bool taken;
    do{
        taken = false;
        for(int i = 0; i < productCount; i++){
            if(inv[i].id == newID){
                taken = true;
                newID++;
                break;
            }
        }
    }while(taken);
    return newID;
}

string stockStatus(int qty){
    if (qty == 0) return "OUT OF STOCK";
    if (qty <= 20) return "LOW STOCK";
    if (qty <= 75) return "IN STOCK";
    return "FULL STOCK";
}

string tolowerCase(string s){
    for(int i = 0; i < s.length(); i++){
        s[i] = tolower(s[i]);
    }
    return s;
}

bool confirm(const string& message){
    char c;
    cout << message << "(y/n): ";
    cin >> c;
    while (cin.fail()||(tolower(c) != 'y' && tolower(c) != 'n')){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "[!] Invalid input. Enter y or n: ";
        cin >> c;
    }
    cin.ignore();
    return (tolower(c) == 'y');
}