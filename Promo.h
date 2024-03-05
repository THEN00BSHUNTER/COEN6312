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

public:
    Promo(int promoId, const string &name, const string &description, double discountPercentage, const string &startPeriod, const string &endPeriod, int productId);

    string GetStartPeriod() const;
    string GetEndPeriod() const;
    int GetProductId() const;
    double GetDiscountPercentage() const;

    // Functions
    void CreatePromotion(const string &name, const string &description, double discountPercentage, const string &startPeriod, const string &endPeriod, int productId);
    void ApplyPromotion(string promoName);
    static bool IsPromotionValid(int promoId);
    static Promo GetPromotionById(int promoId);
    static Promo GetPromotionByName(string promoName);
};

#endif
