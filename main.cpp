// vectors in c++
// structs in c++
// stof() and stoi() in c++
// getline() in c++
// Map type 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <map>
#include <cstdio>

using namespace ImGui;
using namespace std;


// ---------- Data Models ----------
struct Product {
    int id;
    string name;
    string description;
    double price;
    ImVec4 color; // placeholder for "product image"
};

struct CartItem {
    Product product;
    int quantity;
};

struct OrderInfo {
    char fullName[128] = "";
    char address[256]  = "";
    char country[64]   = "";
    char account[32]   = "";
};

// ---------- Globals ----------
vector<Product> g_products;
map<int, CartItem> g_cart; // id -> item
OrderInfo g_order;
bool g_showToast = false;
double g_toastStartTime = 0.0;

// ---------- Helpers ----------
void InitProducts() {
    g_products = {
        {1, "Wireless Mouse",   "Ergonomic 2.4GHz mouse",     19.99, ImVec4(0.30f,0.55f,0.95f,1)},
        {2, "Mechanical Keyboard","RGB backlit, blue switches", 79.50, ImVec4(0.90f,0.40f,0.40f,1)},
        {3, "USB-C Hub",        "7-in-1 multiport adapter",    34.00, ImVec4(0.35f,0.80f,0.55f,1)},
        {4, "Bluetooth Speaker","Portable, waterproof",        45.75, ImVec4(0.85f,0.65f,0.20f,1)},
        {5, "HD Webcam",        "1080p with dual mics",        59.99, ImVec4(0.60f,0.45f,0.90f,1)},
        {6, "Laptop Stand",     "Aluminum, adjustable",        25.00, ImVec4(0.40f,0.75f,0.85f,1)},
    };
}

void AddToCart(const Product& p, int qty) {
    auto it = g_cart.find(p.id);
    if (it != g_cart.end()) it->second.quantity += qty;
    else g_cart[p.id] = CartItem{p, qty};
}

void RemoveFromCart(int id) { g_cart.erase(id); }

double CartTotal() {
    double total = 0.0;
    for (auto& kv : g_cart) total += kv.second.product.price * kv.second.quantity;
    return total;
}

int CartCount() {
    int c = 0;
    for (auto& kv : g_cart) c += kv.second.quantity;
    return c;
}

// ---------- UI: Product Card ----------
void DrawProductCard(const Product& p, float cardWidth) {
    PushID(p.id);
    BeginGroup();

    // "Image" block (colored rectangle)
    ImVec2 p0 = GetCursorScreenPos();
    ImVec2 p1 = ImVec2(p0.x + cardWidth, p0.y + 90.0f);
    GetWindowDrawList()->AddRectFilled(p0, p1, ColorConvertFloat4ToU32(p.color), 6.0f);
    Dummy(ImVec2(cardWidth, 90.0f));

    Spacing();
    TextUnformatted(p.name.c_str());
    TextDisabled("%s", p.description.c_str());
    TextColored(ImVec4(0.2f, 0.8f, 0.3f, 1.0f), "$%.2f", p.price);
    Spacing();

    // Quantity counter (increments immediately add to cart per click)
    static map<int, int> pendingQty; // per-product pending qty displayed
    int& qty = pendingQty[p.id];
    if (qty < 1) qty = 1;

    TextUnformatted("Qty:");
    SameLine();
    if (Button("-", ImVec2(28, 0))) { if (qty > 1) qty--; }
    SameLine();
    Text("%d", qty);
    SameLine();
    if (Button("+", ImVec2(28, 0))) { qty++; }

    Spacing();
    PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.55f, 0.95f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.65f, 1.0f, 1.0f));
    if (Button("Add to Cart", ImVec2(cardWidth, 0))) {
        AddToCart(p, qty);
        qty = 1;
    }
    PopStyleColor(2);

    EndGroup();
    PopID();
}

// ---------- UI: Products Page ----------
void DrawProductsPage(float availableWidth) {
    Text("🛍  Products");
    Separator();
    Spacing();

    const float cardWidth = 220.0f;
    const float gap       = 16.0f;
    int columns = (int)((availableWidth + gap) / (cardWidth + gap));
    if (columns < 1) columns = 1;

    int i = 0;
    for (const auto& p : g_products) {
        DrawProductCard(p, cardWidth);
        if ((++i % columns) != 0) SameLine(0, gap);
    }
}

// ---------- UI: Cart Page ----------
void DrawCartPage() {
    Text("Cart");
    Separator();
    Spacing();

    if (g_cart.empty()) {
        TextDisabled("Your cart is empty. Add products from the Products page.");
        return;
    }

    // Cart table
    if (BeginTable("cart_table", 4,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp)) {
        TableSetupColumn("Product", ImGuiTableColumnFlags_WidthStretch, 3.0f);
        TableSetupColumn("Price",   ImGuiTableColumnFlags_WidthFixed, 90.0f);
        TableSetupColumn("Qty",     ImGuiTableColumnFlags_WidthFixed, 90.0f);
        TableSetupColumn("Action",  ImGuiTableColumnFlags_WidthFixed, 90.0f);
        TableHeadersRow();

        vector<int> toRemove;
        for (auto& kv : g_cart) {
            CartItem& item = kv.second;
            PushID(item.product.id);
            TableNextRow();

            TableSetColumnIndex(0);
            Text("%s", item.product.name.c_str());

            TableSetColumnIndex(1);
            Text("$%.2f", item.product.price);

            TableSetColumnIndex(2);
            if (SmallButton("-")) item.quantity = item.quantity > 1 ? item.quantity - 1 : 0;
            SameLine();
            Text("%d", item.quantity);
            SameLine();
            if (SmallButton("+")) item.quantity++;

            TableSetColumnIndex(3);
            PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.25f, 0.25f, 1.0f));
            if (SmallButton("Remove")) toRemove.push_back(item.product.id);
            PopStyleColor();

            PopID();
        }
        EndTable();

        for (int id : toRemove) RemoveFromCart(id);
    }

    Spacing();
    Separator();
    TextColored(ImVec4(0.2f, 0.85f, 0.3f, 1.0f), "Total: $%.2f", CartTotal());

    Spacing();
    Text("📋 Shipping & Payment Details");
    Separator();

    InputTextWithHint("Full Name",     "John Doe",            g_order.fullName, sizeof(g_order.fullName));
    InputTextWithHint("Address",       "123 Main St, City",   g_order.address,  sizeof(g_order.address));
    InputTextWithHint("Country",       "United States",       g_order.country,  sizeof(g_order.country));
    InputTextWithHint("Account Number","XXXX-XXXX-XXXX-1234", g_order.account,  sizeof(g_order.account));

    bool formOk = strlen(g_order.fullName) > 0 && strlen(g_order.address) > 0 &&
                  strlen(g_order.country) > 0 && strlen(g_order.account) > 0 && !g_cart.empty();

    Spacing();
    BeginDisabled(!formOk);
    PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.65f, 0.35f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.78f, 0.45f, 1.0f));
    if (Button("Pay Now", ImVec2(160, 36))) {
        g_showToast = true;
        g_toastStartTime = GetTime();
        g_cart.clear();
        g_order = OrderInfo{};
    }
    PopStyleColor(2);
    EndDisabled();

    if (!formOk) {
        SameLine();
        TextDisabled("(fill all fields to enable payment)");
    }
}

// ---------- UI: Toast ----------
void DrawToast() {
    if (!g_showToast) return;
    double elapsed = GetTime() - g_toastStartTime;
    if (elapsed > 3.0) { g_showToast = false; return; }

    ImGuiViewport* vp = GetMainViewport();
    ImVec2 pos = ImVec2(vp->WorkPos.x + vp->WorkSize.x - 320.0f,
                        vp->WorkPos.y + 20.0f);

    SetNextWindowPos(pos);
    SetNextWindowBgAlpha(0.92f);
    Begin("##toast", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

    PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 0.95f, 0.45f, 1.0f));
    Text("✅  Order placed successfully!");
    PopStyleColor();
    TextDisabled("Thank you, %s!", "customer");
    End();
}

// ---------- Main ----------
int main() {
    if (!glfwInit()) return 1;
    const char* glsl = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1100, 720, "Online Cart", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    CreateContext();
    StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl);

    ImGuiStyle& style = GetStyle();
    style.WindowRounding    = 8.0f;
    style.FrameRounding     = 5.0f;
    style.ItemSpacing       = ImVec2(8, 6);

    InitProducts();

    int currentPage = 0; // 0 = Products, 1 = Cart

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        NewFrame();

        // Full-screen window
        ImGuiViewport* vp = GetMainViewport();
        SetNextWindowPos(vp->WorkPos);
        SetNextWindowSize(vp->WorkSize);
        Begin("##main", nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

        // Top navigation bar
        TextColored(ImVec4(0.4f, 0.75f, 1.0f, 1.0f), "🛍  MYSHOP");
        SameLine(GetContentRegionAvail().x - 250);
        if (Button("Products", ImVec2(110, 30))) currentPage = 0;
        SameLine();
        char cartLabel[32];
        snprintf(cartLabel, sizeof(cartLabel), "Cart (%d)", CartCount());
        if (Button(cartLabel, ImVec2(110, 30))) currentPage = 1;

        Separator();
        Spacing();

        // Page content
        float availW = GetContentRegionAvail().x;
        if (currentPage == 0) DrawProductsPage(availW);
        else                  DrawCartPage();

        End();

        DrawToast();

        // Render
        Render();
        int w, h; glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.10f, 0.11f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}