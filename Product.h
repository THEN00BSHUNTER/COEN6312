#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>

using namespace std;

class Product
{
private:
    int ProductId;
    string Name;
    string Description;
    double Price;
    int SubcategoryId;
    int SupplierId;

public:
    Product(int productId, const string &name, const string &description, double price, int subcategoryId, int supplierId);

    // Functions
    void AddProduct(const string &name, const string &description, double price, int subcategoryId, int supplierId);
    void UpdateProductDetails(int productId, const string &name, const string &description, double price, int subcategoryId, int supplierId);
    void DeleteProduct(int productId);

    int GetProductId() const;
    double GetPrice() const;
    string GetName() const;

    void DisplayProductDetails() const;
    static vector<Product> GetAllProducts();
    static void DisplayAllProducts(const vector<Product> &products);
    static Product GetProductById(int productId);
    static vector<Product> GetProductsBySupplierId(int supplierId);
    static double CalculateTotalPrice(const vector<int> &itemsOrdered);
    static void CheckProductAvailabilityAndNotifySupplier(int productId);
};

#endif
