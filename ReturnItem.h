#ifndef RETURNEDITEM_H
#define RETURNEDITEM_H

#include <vector>
#include "sqlite\sqlite3.h"

using namespace std;

class ReturnItem
{
private:
    int ReturnItemId;
    int ReturnId;
    int ProductId;
    int Quantity;

public:
    ReturnItem(int returnItemId, int returnId, int productId, int quantity);
    static vector<ReturnItem> GetReturnItemsByReturnId(int returnId);
};

#endif
