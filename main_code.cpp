#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>
using namespace std;

const int MAX = 100;
const string fName = "inventory.data";

const int NUM_CATEGORIES = 10;
const string CATEGORIES[NUM_CATEGORIES] = {
    "Fresh Produce", "Meat & Poultry", "Seafood", "Beverage", "Candies & Chocolates", "Dairy", "Dry Goods", "Bakery", "Personal Care", "Cleaning Supplies";
};
struct Product{
    int id;
    int categoryIndex
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

//Function Prototypes :)
void dispMenu();
void addProduct();
void updProduct();
void delProduct();
void searchProduct();
void viewAll(); // lagyan ba to ng another void function to view by category?
//void viewByCategory(int categoryIndex);
void checkStock();
void processOrder();
void saveFile();
void loadFile();
int selectCategory();
int  findProduct(int id);//kung gusto nila magsearch using ID (1001-1100)
int  genId(int  categoryIndex);//para sa int id, 1000 basis natin? like 1001-1100 para 100 items?
bool confirm(const string& message);
string getStockLevels(int quantity);


int main() {
    loadFile();
    
    int choice;
    
    do {
        dispMenu();
        cout << "Enter choice: ";
        cin >> choice;
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please Enter a number: ";
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
                cout << "\nData saved. Goodbye!\n\n";
                break;
            default:
                cout << "\nInvalid choice! Try again.\n\n";
        }
    } while(choice != 0);
    
    return 0;
}

void dispMenu(){
    cout << "\n========================================\n";
    cout << "     INVENTORY MANAGEMENT SYSTEM\n";
    cout << "==========================================\n";
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

//FOR ADDING PRODUCTS
void addProduct(){
    if(productCount >= MAX){
        cout << "\n[ERROR!] Inventory is full! You cannot add more products.\n";
        return;
    }
    
    cout << "\n--- ADD NEW PRODUCT ---\n";
    int categoryIndex = selectCategory();
    
    Product newProduct;
    newProduct.categoryIndex = categoryIndex;
    newProduct.id = genId(categoryIndex);
    
    cout << "Generated ID: " << newProduct.id << " (" << CATEGORIES[categoryIndex] << ") \n" << endl;
    
/* cout << "Enter Category: ";
    getline(cin, newProduct.category);
    while(newProduct.category.empty()){
        cout << "Category cannot be empty. Enter again: ";
        getline(cin, newProduct.category);
    } gawin ko munang comment to kasi hindi na user defined ang categories*/
    
    //Name of Product
    cout << "Please Enter Product Name: ";
    getline(cin, newProduct.name);
    while(newProduct.name.empty()){
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, newProduct.name);
    }
    
    //Price
    cout << "Please Enter Price (PHP): ";
    cin >> newProduct.price;
    while(cin.fail() || newProduct.price < 0){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid price. Enter a positive number: ";
        cin >> newProduct.price;
    }
    
    //Quantity(limit of 100 per product)
    cout << "Enter Quantity (MAXIMUM is 100): ";
    cin >> newProduct.quantity;
    while(cin.fail() || newProduct.quantity < 0 || newProduct.quantity > 100){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid quantity! Enter 0-100: ";
        cin >> newProduct.quantity;
    }
    
    //productCount++
    inv[productCount] = newProduct;
    productCount++;
    
    //intention = display updated list + remaining slots
    cout << "\n[SUCCESS!] Your product has been added successfully!\n";
    
    cout << "- - - - - - - - - - - - - - -\n";
    cout << "Here is your updated" << CATEGORIES[categoryIndex] << "list: \n";
    viewByCategory(categoryIndex);
    
    cout << "- - - - - - - - - - - - - - -\n";
    cout << "Slots remaining: " << (MAX - productCount) << "/" << MAX << endl;
    cout << "- - - - - - - - - - - - - - -\n"; //natatawa akoo parang ang pangit nito

    saveFile();
}

void updProduct(){//enter product id, display product info, prompt user to add new info

    /*MY INTENTION = enter id ex: 1001 = pork, 1002 = ground pork, automatic na dapat lalabas yung category dito*/
    
    if (productCount == 0){
        cout << "\nNo Products Found in the Inventory\n";
        return;
    }
    
    int id;
    cout << "\n--- UPDATE PRODUCT ---\n";
    cout << "Please enter product ID: ";
    cin >> id;
    
    while (cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please product ID: ";
        cin >> id;
    }
    
    int index = findProduct(id);
    if (index == -1){
        cout << "[ERROR!] Product not found. \n";
        return;
    }
    
    //pwede to na hindi nakaswitch kasi pwede na isa-isa sila magkakaroon ng condition
    cout << "\nCurrent Product Information:\n";
    cout << "  Product Name    :" << inv[index].name<< '\n';
    cout << "  ID              :" << inv[index].id << '\n';
    cout << "  Category        :" << CATEGORIES[inv[index].categoryIndex] << '\n';
    //taas constant, itong baba pwede iupdate
    cout << "  Price           : PHP " << inv[index].price << '\n'
    cout << "  Quantity        :" << inv[index].quantity << '\n';
    
    //INTENTION = UPDATE PRICE OR QUANTITY
    cout << "\nDo you want to make an update?\n";
    cout << "  [1] Price \n";
    cout << "  [2] Quantity\n";
    cout << "  [0] Cancel/Exit\n";
    cout << "Enter your Choice: ";
    
    int choice;
    cin >> choice;
    
    while(cin.fail){
        cin.clear();
        cin.ignore(1000,\n);
        cout << "Invalid Input! Please choose between 1, 2, and 0: ";
        cin >> choice;
    }
    
    switch (choice){
        //pwede pa ito ayusin kasi baka ienter nila same price, hindi mag enter, basta ganon
        case 1: 
        cout << "Please enter a new price: PHP ";
        cin >> inv[index].price;
        cout << "Price has been updated successfully!\n";
        break;
        
        case 2:
        cout << "Please enter a new quantity: ";
        cin >> inv[index].quantity;
        cout << "Quantity has been updated successfully! \n";
        break;
        
        case 0: 
        cout << "Thank you! No changes were made. \n";
        break;
    }
    
    saveFile();
    cout<< "\n[SUCESS!] Products updated successfully! \n";
    
}

void delProduct(){//enter product id, confirm, delete
    if (productCount == 0){
        cout << "\nNo Products Found in the Inventory\n";
        return;
    }
    
    int id;
    cout << "\n--- DELETE PRODUCT ---\n";
    cout << "Please enter product ID: ";
    cin >> id;
    
    while (cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please product ID: ";
        cin >> id;
    }
    
    int index = findProduct(id);
    if (index == -1){
        cout << "[ERROR!] Product not found.\n";
        return;
    }
    
    //tamad ako kaya copypasta na lang yung code na nauna
    cout << "\nCurrent Product Information:\n";
    cout << "  Product Name    :" << inv[index].name<< '\n';
    cout << "  ID              :" << inv[index].id << '\n';
    cout << "  Category        :" << CATEGORIES[inv[index].categoryIndex] << '\n';
    cout << "  Price           : PHP " << inv[index].price << '\n'
    cout << "  Quantity        :" << inv[index].quantity << '\n';
    
    //INTENTION = CONFIRMING THE DELETION
    char confirm;
    cout << "\nAre you sure you want to delete this product? : (y/n) \n";
    cin >> confirm;
    
    if (confirm == 'y'|| confirm == 'Y'){
        for (int i = index; i < productCount-1; i++){
            inv[i] = inv[i+1];
        }
    
    productCount--;
    
    saveFile();
    cout << "[SUCESS!] Products has been updated successfully!";
    } else {
        cout << "Deletion cancelled. No updates has been made.";
    }
    cout << "Slots remaining: " << (MAX - productCount) << "/" << MAX << "\n";

}

void searchProduct(){//enter product id, display info if found  int id;
    bool found = false;
     // ask user for product ID to search
    cout << "Enter Product ID: ";
    cin >> id;
    // loop through all products to find matching ID
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            cout << "\nProduct Found!\n";
            cout << "ID: " << products[i].id << endl;
            cout << "Name: " << products[i].name << endl;
            cout << "Price: " << products[i].price << endl;
            found = true;
        }
    }
    // if no product was found after loop
    if (!found) {
        char choice;
        cout << "Product not found. Add product? (Y/N): ";
        cin >> choice;

        if (choice == 'Y' || choice == 'y') {
            addProduct();
        }
    }
}


void viewAll(){//if productCount == 0, none found; otherwise, list all products if (productCount == 0) {
    // check kung walang laman ang product list
    cout << "No products." << endl;
     }
    else {
    // loop through all products
    for (int i = 0; i < productCount; i++) {

        // display product details per item
        cout << "ID: " << products[i].id << endl;
        cout << "Name: " << products[i].name << endl;
        cout << "Price: " << products[i].price << endl;
        cout << "Category: " << products[i].category << endl;

        // spacing lang para readable output
        cout << endl;
    }
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

void processOrder(){
    if (productCount == 0){
        cout << "No products in inventory.\n";
        return;
    }
    string customerName;
    OrderItem cart[MAX];
    int cartCount = 0;
    
    cout << "Customer Name: ";
    getline(cin, customerName);
    
    viewAll();
    
    do{
        int id;
        int qty;
        
        cout << "\nEnter Product ID: ";
        cin >> id;
        
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Enter a product ID: ";
            cin >> id;
        }
        
        int index = findProduct(id);
        if (index == -1){
            cout << "Product not found.\n";
            continue;
        }
        
        cout << "Quantity to Buy: ";
        cin >> qty;
        
        while (cin.fail() || qty <= 0){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Enter a positive number: ";
            cin >> qty;
        }
        
        if (cartCount >= MAX){
                cout << "Order is full.\n";
                break;
        }
        
        if (qty > inv[index].quantity){
            cout << "Not enough stock! Available: " << inv[index].quantity << endl;
            continue;
        } else {
            cart[cartCount].productIndex = index;
            cart[cartCount].quantity = qty;
            cartCount++;
            cout << "Added to order.\n";
        }
    }while(confirm("Add another item? "));
    
    if (cartCount == 0){
        cout << "No items ordered.\n";
        return;
    }
    
    double total = 0;
    
    cout << "\nORDER SUMMARY\n";
    
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
    
    cout << "\nTOTAL: PHP " << fixed << setprecision(2) << total << endl;
    
    while(confirm("Would you like to remove or edit an item? ")){
        int editId;
        int cartIndex = -1;
        
        cout << "Enter Product ID: ";
        cin >> editId;
        
        for (int i = 0; i < cartCount; i++){
            int p = cart[i].productIndex;
            
            if (inv[p].id == editId){
                cartIndex = i;
                break;
            }
        }
        
        if (cartIndex == -1) cout << "Product not found in order.\n";
        else{
            if (confirm("Remove product entirely?")){
                for(int i = cartIndex; i < cartCount - 1; i++){
                    cart[i] = cart[i + 1];
                }
                cartCount--;
                
                if(cartCount == 0){
                    cout << "Order is now empty.\n";
                    return;
                }
                cout << "Product removed.\n";
            } else {
                int newQuantity;
                
                cout << "Enter new quantity: ";
                cin >> newQuantity;
                
                int p = cart[cartIndex].productIndex;
                
                while(cin.fail()||newQuantity <= 0||newQuantity>inv[p].quantity){
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Enter again: ";
                    cin >> newQuantity;
                }
                cart[cartIndex].quantity = newQuantity;
                cout << "Quantity updated.\n";
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
        cout << "Payment Amount: PHP ";
        cin >> payment;
        
        while(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid amount. Enter again: ";
            cin >> payment;
        }
        
        if (payment < total) cout << "Insufficient payment.\n";
    }while(payment < total);
    
    double change = payment - total;
    
    for (int i = 0; i < cartCount; i++){
        int p = cart[i].productIndex;
        inv[p].quantity -= cart[i].quantity;
    }
    
    saveFile();
    
    cout << "RECEIPT\n";
    cout << "Customer: " << customerName << endl;
    
    cout << left << setw(10) << "Name" 
         << setw(20) << "Quantity" 
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
         << "Change : PHP " << change << endl;
         
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
    
    cout << "\nTransaction completed successfully.\n";
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