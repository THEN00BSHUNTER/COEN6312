#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>

using namespace std;

class Discount
{
private:
    int DiscountId;
    string Name;
    string Description;
    string StartPeriod;
    string EndPeriod;
    int ProductId;
    double Percentage;

public:
    Discount(int discountId, const string &name, const string &description, const string &startPeriod, const string &endPeriod, int productId, double percentage);

    int GetProductId() const;
    double GetPercentage() const;

    // Functions
    void CreateDiscount(const string &name, const string &description, const string &startPeriod, const string &endPeriod, int productId, double percentage);
    bool CheckOverlappingDiscounts(int productId, const string &startPeriod, const string &endPeriod);
    void ApplyDiscount(string discountName);

    static bool IsDiscountValid(int discountId);
    static Discount GetDiscountById(int discountId);
    static Discount GetDiscountByName(string discountName);
};

#endif
