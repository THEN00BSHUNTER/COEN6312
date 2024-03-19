#ifndef PROMO_H
#define PROMO_H

#include <string>

using namespace std;

class Promo
{
private:
    int PromoId;
    string Name;
    string Description;
    double DiscountPercentage;
    string StartPeriod;
    string EndPeriod;
    int ProductId;
    int EmployeeId;

public:
    Promo(int promoId, const string &name, const string &description, double discountPercentage, const string &startPeriod, const string &endPeriod, int productId, int employeeId);

    string GetStartPeriod() const;
    string GetEndPeriod() const;
    int GetProductId() const;
    double GetDiscountPercentage() const;
    int GetEmployeeId() const;

    // Functions
    static bool CheckOverlappingPromotions(int employeeId, const string &startPeriod, const string &endPeriod);
    void CreatePromotion(const string &name, const string &description, double discountPercentage, const string &startPeriod, const string &endPeriod, int productId, int employeeId);
    static double ApplyPromotion(double totalPrice, int itemCount);
    void ApplyPromotion(string promoName);
    static bool IsPromotionValid(int promoId);
    static Promo GetPromotionById(int promoId);
    static Promo GetPromotionByName(string promoName);
};

#endif
