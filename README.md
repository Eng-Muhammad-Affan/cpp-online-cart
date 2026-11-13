{
  "name": "test-cart-fltk",
  "version": "1.0.0",
  "dependencies": ["fltk"]
}

when cache become invalid run the following command to revalidate it 

```bash 
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=/home/affan/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_CXX_COMPILER=g++
```

## Manual app 

```cpp
// #include <FL/Fl.H>
// #include <FL/Fl_Window.H>
// #include <FL/Fl_Box.H>
// #include <FL/Fl_Button.H>
// #include <FL/Fl_Input.H>
// #include <FL/Fl_Output.H>
// #include <FL/Fl_Browser.H>
// #include <FL/Fl_Multiline_Output.H>
// #include <FL/fl_ask.H>

// #include <iostream>
// #include <sstream>
// #include <string>
// #include <iomanip>
// #include <cstdlib>
// #include <cctype>
// #include <limits>

// using namespace std;

// // ========================================================
// // Constants
// // ========================================================

// const int MAX_PRODUCTS = 10;
// const int MAX_CART = 10;

// const double DISCOUNT_LIMIT = 10000.00;
// const double DISCOUNT_RATE = 0.10;

// // ========================================================
// // Data Structures
// // ========================================================

// struct Product {
//     int id;
//     string name;
//     double price;
//     int stock;
// };

// struct CartItem {
//     int productId;
//     int quantity;
// };

// // ========================================================
// // Application Data
// // ========================================================

// Product products[MAX_PRODUCTS];
// CartItem cart[MAX_CART];

// int productCount = 0;
// int cartCount = 0;

// // ========================================================
// // GUI Widgets
// // ========================================================

// Fl_Window* mainWin = nullptr;

// Fl_Browser* productList = nullptr;
// Fl_Browser* cartList = nullptr;

// Fl_Input* idInput = nullptr;
// Fl_Input* qtyInput = nullptr;
// Fl_Input* nameInput = nullptr;

// Fl_Output* subtotalOut = nullptr;
// Fl_Output* discountOut = nullptr;
// Fl_Output* totalOut = nullptr;

// Fl_Multiline_Output* messageOut = nullptr;

// // ========================================================
// // Utility Functions
// // ========================================================

// string money(double value) {
//     ostringstream oss;
//     oss << fixed << setprecision(2) << value;
//     return oss.str();
// }

// string toStr(int value) {
//     return to_string(value);
// }

// string trim(const string& value) {
//     size_t start = value.find_first_not_of(" \t\n\r");

//     if (start == string::npos)
//         return "";

//     size_t end = value.find_last_not_of(" \t\n\r");

//     return value.substr(start, end - start + 1);
// }

// void setMessage(const string& message) {
//     messageOut->value(message.c_str());
// }

// bool parsePositiveInt(const char* input, int& result) {
//     string value = trim(input);

//     if (value.empty())
//         return false;

//     for (char ch : value) {
//         if (!isdigit(static_cast<unsigned char>(ch)))
//             return false;
//     }

//     try {
//         size_t pos = 0;
//         long number = stol(value, &pos);

//         if (pos != value.length() ||
//             number <= 0 ||
//             number > numeric_limits<int>::max()) {
//             return false;
//         }

//         result = static_cast<int>(number);
//         return true;
//     }
//     catch (...) {
//         return false;
//     }
// }

// void clearProductInputs() {
//     idInput->value("");
//     qtyInput->value("");
// }

// void clearAllInputs() {
//     clearProductInputs();
//     nameInput->value("");
// }

// // ========================================================
// // Product and Cart Lookup
// // ========================================================

// int findProduct(int id) {
//     for (int i = 0; i < productCount; i++) {
//         if (products[i].id == id)
//             return i;
//     }

//     return -1;
// }

// int findInCart(int id) {
//     for (int i = 0; i < cartCount; i++) {
//         if (cart[i].productId == id)
//             return i;
//     }

//     return -1;
// }

// // ========================================================
// // Initialize Products
// // ========================================================

// void loadProducts() {
//     products[0] = {1, "T-Shirt", 1500.00, 10};
//     products[1] = {2, "Jhumka Earrings", 800.00, 25};
//     products[2] = {3, "Shoes", 3500.00, 5};
//     products[3] = {4, "Wrist Watch", 5000.00, 3};
//     products[4] = {5, "Handbag", 2500.00, 8};

//     productCount = 5;
// }

// // ========================================================
// // Shopping Calculations
// // ========================================================

// double cartSubtotal() {
//     double subtotal = 0;

//     for (int i = 0; i < cartCount; i++) {
//         int productIndex = findProduct(cart[i].productId);

//         if (productIndex != -1) {
//             subtotal += products[productIndex].price *
//                         cart[i].quantity;
//         }
//     }

//     return subtotal;
// }

// double calculateDiscount() {
//     double subtotal = cartSubtotal();

//     return subtotal > DISCOUNT_LIMIT
//         ? subtotal * DISCOUNT_RATE
//         : 0;
// }

// double calculateTotal() {
//     return cartSubtotal() - calculateDiscount();
// }

// // ========================================================
// // Refresh Product List
// // ========================================================

// void refreshProducts() {
//     productList->clear();

//     productList->add(
//         "ID  |  PRODUCT NAME       |  PRICE (Rs.)  |  AVAILABLE STOCK"
//     );

//     for (int i = 0; i < productCount; i++) {
//         ostringstream line;

//         line << products[i].id
//              << "   |  "
//              << products[i].name
//              << "  |  "
//              << money(products[i].price)
//              << "  |  ";

//         if (products[i].stock > 0)
//             line << products[i].stock << " units";
//         else
//             line << "OUT OF STOCK";

//         productList->add(line.str().c_str());
//     }
// }

// // ========================================================
// // Refresh Cart and Order Summary
// // ========================================================

// void refreshCart() {
//     cartList->clear();

//     if (cartCount == 0) {
//         cartList->add("Your cart is empty.");
//     }
//     else {
//         for (int i = 0; i < cartCount; i++) {
//             int p = findProduct(cart[i].productId);

//             if (p == -1)
//                 continue;

//             double itemTotal =
//                 products[p].price * cart[i].quantity;

//             ostringstream line;

//             line << products[p].name
//                  << "  |  "
//                  << cart[i].quantity
//                  << " x Rs. "
//                  << money(products[p].price)
//                  << "  |  Rs. "
//                  << money(itemTotal);

//             cartList->add(line.str().c_str());
//         }
//     }

//     subtotalOut->value(money(cartSubtotal()).c_str());
//     discountOut->value(money(calculateDiscount()).c_str());
//     totalOut->value(money(calculateTotal()).c_str());
// }

// // ========================================================
// // Add Product to Cart
// // ========================================================

// void addToCart() {
//     int id, quantity;

//     if (!parsePositiveInt(idInput->value(), id)) {
//         setMessage("Please enter a valid positive Product ID.");
//         return;
//     }

//     if (!parsePositiveInt(qtyInput->value(), quantity)) {
//         setMessage("Please enter a valid positive quantity.");
//         return;
//     }

//     int p = findProduct(id);

//     if (p == -1) {
//         setMessage("Product not found. Please check the Product ID.");
//         return;
//     }

//     if (products[p].stock == 0) {
//         setMessage(products[p].name + " is currently out of stock.");
//         return;
//     }

//     int c = findInCart(id);

//     int existingQuantity =
//         (c != -1) ? cart[c].quantity : 0;

//     int availableQuantity =
//         products[p].stock - existingQuantity;

//     if (quantity > availableQuantity) {
//         setMessage(
//             "Insufficient stock. Only " +
//             toStr(availableQuantity) +
//             " additional units can be added."
//         );
//         return;
//     }

//     if (c == -1 && cartCount >= MAX_CART) {
//         setMessage("Cart limit reached. Remove an item first.");
//         return;
//     }

//     if (c != -1) {
//         cart[c].quantity += quantity;

//         setMessage(
//             products[p].name +
//             " quantity successfully increased in your cart."
//         );
//     }
//     else {
//         cart[cartCount].productId = id;
//         cart[cartCount].quantity = quantity;

//         cartCount++;

//         setMessage(
//             products[p].name +
//             " successfully added to your cart."
//         );
//     }

//     refreshCart();
//     clearProductInputs();
// }

// // ========================================================
// // Update Cart Quantity
// // ========================================================

// void updateQuantity() {
//     if (cartCount == 0) {
//         setMessage("Your cart is empty.");
//         return;
//     }

//     int id, quantity;

//     if (!parsePositiveInt(idInput->value(), id)) {
//         setMessage("Please enter a valid Product ID.");
//         return;
//     }

//     if (!parsePositiveInt(qtyInput->value(), quantity)) {
//         setMessage("Please enter a valid quantity.");
//         return;
//     }

//     int c = findInCart(id);

//     if (c == -1) {
//         setMessage("This product does not exist in your cart.");
//         return;
//     }

//     int p = findProduct(id);

//     if (p == -1) {
//         setMessage("Product information could not be found.");
//         return;
//     }

//     if (quantity > products[p].stock) {
//         setMessage(
//             "Insufficient stock. Maximum available quantity: " +
//             toStr(products[p].stock)
//         );
//         return;
//     }

//     cart[c].quantity = quantity;

//     setMessage("Cart quantity successfully updated.");

//     refreshCart();
//     clearProductInputs();
// }

// // ========================================================
// // Remove Product from Cart
// // ========================================================

// void removeFromCart() {
//     if (cartCount == 0) {
//         setMessage("Your cart is already empty.");
//         return;
//     }

//     int id;

//     if (!parsePositiveInt(idInput->value(), id)) {
//         setMessage("Please enter a valid Product ID.");
//         return;
//     }

//     int c = findInCart(id);

//     if (c == -1) {
//         setMessage("This product does not exist in your cart.");
//         return;
//     }

//     string removedName = products[findProduct(id)].name;

//     for (int i = c; i < cartCount - 1; i++) {
//         cart[i] = cart[i + 1];
//     }

//     cartCount--;

//     setMessage(removedName + " successfully removed from your cart.");

//     refreshCart();
//     clearProductInputs();
// }

// // ========================================================
// // Checkout
// // ========================================================

// void checkout() {
//     if (cartCount == 0) {
//         setMessage("Your cart is empty. Add products before checkout.");
//         return;
//     }

//     string customerName = trim(nameInput->value());

//     if (customerName.empty()) {
//         setMessage("Please enter the customer's name before checkout.");
//         return;
//     }

//     // Verify stock before processing the order.
//     for (int i = 0; i < cartCount; i++) {
//         int p = findProduct(cart[i].productId);

//         if (p == -1 || cart[i].quantity > products[p].stock) {
//             setMessage(
//                 "Checkout failed. Stock validation failed for a cart item."
//             );
//             return;
//         }
//     }

//     double subtotal = cartSubtotal();
//     double discount = calculateDiscount();
//     double grandTotal = calculateTotal();

//     ostringstream receipt;

//     receipt << "============================================\n";
//     receipt << "              E-STORE RECEIPT               \n";
//     receipt << "============================================\n\n";

//     receipt << "Customer Name: " << customerName << "\n";
//     receipt << "--------------------------------------------\n";

//     receipt << left
//             << setw(22) << "Product"
//             << right
//             << setw(7) << "Qty"
//             << setw(15) << "Amount (Rs.)"
//             << "\n";

//     receipt << "--------------------------------------------\n";

//     for (int i = 0; i < cartCount; i++) {
//         int p = findProduct(cart[i].productId);

//         double itemTotal =
//             products[p].price * cart[i].quantity;

//         receipt << left
//                 << setw(22) << products[p].name
//                 << right
//                 << setw(7) << cart[i].quantity
//                 << setw(15) << money(itemTotal)
//                 << "\n";
//     }

//     receipt << "--------------------------------------------\n";

//     receipt << left << setw(29) << "Subtotal:"
//             << right << "Rs. " << money(subtotal) << "\n";

//     receipt << left << setw(29) << "Discount:"
//             << right << "Rs. " << money(discount) << "\n";

//     receipt << left << setw(29) << "Grand Total:"
//             << right << "Rs. " << money(grandTotal) << "\n";

//     receipt << "============================================\n";
//     receipt << "       Thank you for shopping with us!      \n";
//     receipt << "============================================\n";

//     // Complete transaction only after successful validation.
//     for (int i = 0; i < cartCount; i++) {
//         int p = findProduct(cart[i].productId);

//         products[p].stock -= cart[i].quantity;
//     }

//     cartCount = 0;

//     refreshProducts();
//     refreshCart();

//     nameInput->value("");

//     setMessage(receipt.str());
// }

// // ========================================================
// // Clear Cart
// // ========================================================

// void clearCart() {
//     if (cartCount == 0) {
//         setMessage("Your cart is already empty.");
//         return;
//     }

//     int result = fl_choice(
//         "Are you sure you want to remove all products from your cart?",
//         "Cancel",
//         "Clear Cart",
//         nullptr
//     );

//     if (result != 1)
//         return;

//     cartCount = 0;

//     refreshCart();

//     setMessage("All products have been removed from your cart.");
// }

// // ========================================================
// // GUI Callbacks
// // ========================================================

// void cbAdd(Fl_Widget*, void*) {
//     addToCart();
// }

// void cbUpdate(Fl_Widget*, void*) {
//     updateQuantity();
// }

// void cbRemove(Fl_Widget*, void*) {
//     removeFromCart();
// }

// void cbCheckout(Fl_Widget*, void*) {
//     checkout();
// }

// void cbClearCart(Fl_Widget*, void*) {
//     clearCart();
// }

// void cbClearInputs(Fl_Widget*, void*) {
//     clearAllInputs();
//     setMessage("Input fields cleared.");
// }

// void cbExit(Fl_Widget*, void*) {
//     int result = fl_choice(
//         "Are you sure you want to exit the shopping application?",
//         "Cancel",
//         "Exit",
//         nullptr
//     );

//     if (result == 1)
//         mainWin->hide();
// }

// // ========================================================
// // Main Application
// // ========================================================

// int main(int argc, char** argv) {

//     loadProducts();

//     const int W = 1000;
//     const int H = 680;

//     mainWin = new Fl_Window(
//         W,
//         H,
//         "E-Store | Shopping Cart Management System"
//     );

//     mainWin->color(FL_WHITE);

//     // ----------------------------------------------------
//     // Application Header
//     // ----------------------------------------------------

//     Fl_Box* title = new Fl_Box(
//         0, 10, W, 40,
//         "E-STORE SHOPPING CART SYSTEM"
//     );

//     title->labelfont(FL_BOLD);
//     title->labelsize(23);

//     Fl_Box* subtitle = new Fl_Box(
//         0, 48, W, 25,
//         "Browse products, manage your cart and complete your purchase."
//     );

//     subtitle->labelsize(12);

//     // ----------------------------------------------------
//     // Products Panel
//     // ----------------------------------------------------

//     Fl_Box* productLabel = new Fl_Box(
//         20, 85, 460, 25,
//         "AVAILABLE PRODUCTS"
//     );

//     productLabel->labelfont(FL_BOLD);
//     productLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

//     productList = new Fl_Browser(
//         20, 110, 460, 270
//     );

//     productList->type(FL_HOLD_BROWSER);
//     productList->textsize(12);

//     // ----------------------------------------------------
//     // Shopping Cart Panel
//     // ----------------------------------------------------

//     Fl_Box* cartLabel = new Fl_Box(
//         520, 85, 460, 25,
//         "SHOPPING CART"
//     );

//     cartLabel->labelfont(FL_BOLD);
//     cartLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

//     cartList = new Fl_Browser(
//         520, 110, 460, 190
//     );

//     cartList->type(FL_HOLD_BROWSER);
//     cartList->textsize(12);

//     // ----------------------------------------------------
//     // Order Summary
//     // ----------------------------------------------------

//     Fl_Box* summaryLabel = new Fl_Box(
//         520, 310, 460, 25,
//         "ORDER SUMMARY"
//     );

//     summaryLabel->labelfont(FL_BOLD);
//     summaryLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

//     Fl_Box* subtotalLabel = new Fl_Box(
//         520, 340, 140, 25,
//         "Subtotal (Rs.):"
//     );

//     subtotalLabel->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);

//     subtotalOut = new Fl_Output(
//         670, 340, 310, 25
//     );

//     Fl_Box* discountLabel = new Fl_Box(
//         520, 370, 140, 25,
//         "Discount (Rs.):"
//     );

//     discountLabel->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);

//     discountOut = new Fl_Output(
//         670, 370, 310, 25
//     );

//     Fl_Box* totalLabel = new Fl_Box(
//         520, 400, 140, 25,
//         "Grand Total (Rs.):"
//     );

//     totalLabel->labelfont(FL_BOLD);
//     totalLabel->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);

//     totalOut = new Fl_Output(
//         670, 400, 310, 25
//     );

//     totalOut->textfont(FL_BOLD);
//     totalOut->textsize(14);

//     // ----------------------------------------------------
//     // Customer and Product Inputs
//     // ----------------------------------------------------

//     Fl_Box* actionLabel = new Fl_Box(
//         20, 415, 460, 25,
//         "SHOPPING ACTIONS"
//     );

//     actionLabel->labelfont(FL_BOLD);
//     actionLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

//     new Fl_Box(20, 450, 100, 25, "Product ID:");

//     idInput = new Fl_Input(
//         120, 450, 100, 25
//     );

//     new Fl_Box(240, 450, 80, 25, "Quantity:");

//     qtyInput = new Fl_Input(
//         320, 450, 100, 25
//     );

//     new Fl_Box(440, 450, 100, 25, "Customer:");

//     nameInput = new Fl_Input(
//         540, 450, 200, 25
//     );

//     // ----------------------------------------------------
//     // Action Buttons
//     // ----------------------------------------------------

//     Fl_Button* addButton = new Fl_Button(
//         20, 490, 145, 32,
//         "Add to Cart"
//     );

//     addButton->callback(cbAdd);

//     Fl_Button* updateButton = new Fl_Button(
//         175, 490, 145, 32,
//         "Update Quantity"
//     );

//     updateButton->callback(cbUpdate);

//     Fl_Button* removeButton = new Fl_Button(
//         330, 490, 145, 32,
//         "Remove Item"
//     );

//     removeButton->callback(cbRemove);

//     Fl_Button* checkoutButton = new Fl_Button(
//         485, 490, 145, 32,
//         "Checkout"
//     );

//     checkoutButton->callback(cbCheckout);

//     Fl_Button* clearCartButton = new Fl_Button(
//         640, 490, 145, 32,
//         "Clear Cart"
//     );

//     clearCartButton->callback(cbClearCart);

//     Fl_Button* clearInputsButton = new Fl_Button(
//         795, 490, 185, 32,
//         "Clear Inputs"
//     );

//     clearInputsButton->callback(cbClearInputs);

//     // ----------------------------------------------------
//     // Messages and Receipt
//     // ----------------------------------------------------

//     Fl_Box* messageLabel = new Fl_Box(
//         20, 535, 960, 25,
//         "APPLICATION MESSAGES / PURCHASE RECEIPT"
//     );

//     messageLabel->labelfont(FL_BOLD);
//     messageLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

//     messageOut = new Fl_Multiline_Output(
//         20, 560, 960, 90
//     );

//     messageOut->value(
//         "Welcome to E-Store!\n"
//         "Enter a Product ID and Quantity to add products to your cart."
//     );

//     messageOut->textfont(FL_COURIER);
//     messageOut->textsize(12);

//     // ----------------------------------------------------
//     // Exit Button
//     // ----------------------------------------------------

//     Fl_Button* exitButton = new Fl_Button(
//         840, 10, 140, 30,
//         "Exit Application"
//     );

//     exitButton->callback(cbExit);

//     // ----------------------------------------------------
//     // Finalize Window
//     // ----------------------------------------------------

//     mainWin->end();

//     mainWin->size_range(1000, 680);

//     mainWin->show(argc, argv);

//     refreshProducts();
//     refreshCart();

//     return Fl::run();
// }
```