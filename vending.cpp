#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

const int NUM_ITEMS = 6;

struct Item
{
    string code;
    string name;
    int price;      // price in cents
    int quantity;
};

// Display the inventory
void displayInventory(const Item inventory[])
{
    cout << "\n=== SLOT INVENTORY ===\n";

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        cout << left << setw(5) << inventory[i].code
             << setw(15) << inventory[i].name
             << "$" << fixed << setprecision(2)
             << inventory[i].price / 100.0 << "   ";

        if (inventory[i].quantity > 0)
        {
            cout << inventory[i].quantity << " left";
        }
        else
        {
            cout << "SOLD OUT";
        }

        cout << endl;
    }
}

// Display money as dollars and cents
void displayMoney(int cents)
{
    cout << "$" << fixed << setprecision(2) << cents / 100.0;
}

// Insert coins
int insertCoins()
{
    int credit = 0;
    int coin;

    while (true)
    {
        cout << "\nInsert coin (5/10/25/100), or 0 to stop: ";

        if (!(cin >> coin))
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (coin == 0)
        {
            break;
        }

        if (coin == 5 || coin == 10 || coin == 25 || coin == 100)
        {
            credit += coin;

            cout << "Credit: ";
            displayMoney(credit);
            cout << endl;
        }
        else
        {
            cout << "Coin rejected. Please enter 5, 10, 25, or 100.\n";
        }
    }

    return credit;
}

// Return a coin breakdown for a given amount
void showCoinBreakdown(int amount)
{
    int quarters = amount / 25;
    amount %= 25;

    int dimes = amount / 10;
    amount %= 10;

    int nickels = amount / 5;

    if (quarters > 0)
    {
        cout << "  " << quarters << " quarter";
        if (quarters != 1)
            cout << "s";
        cout << endl;
    }

    if (dimes > 0)
    {
        cout << "  " << dimes << " dime";
        if (dimes != 1)
            cout << "s";
        cout << endl;
    }

    if (nickels > 0)
    {
        cout << "  " << nickels << " nickel";
        if (nickels != 1)
            cout << "s";
        cout << endl;
    }

    if (amount == 0 && quarters == 0 && dimes == 0 && nickels == 0)
    {
        cout << "  No change\n";
    }
}

// Find an item by its slot code
int findItem(const Item inventory[], const string& code)
{
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        if (inventory[i].code == code)
        {
            return i;
        }
    }

    return -1;
}

// Print receipt
void printReceipt(const Item& item, int tendered, int change)
{
    cout << "\n--- RECEIPT ---\n";

    cout << "Item:      " << item.name << endl;

    cout << "Price:     ";
    displayMoney(item.price);
    cout << endl;

    cout << "Tendered:  ";
    displayMoney(tendered);
    cout << endl;

    cout << "Change:    ";
    displayMoney(change);
    cout << endl;

    showCoinBreakdown(change);

    cout << "Thank you!\n";
}

// Handle one customer's transaction
void serveCustomer(Item inventory[])
{
    displayInventory(inventory);

    int credit = insertCoins();

    // Customer inserted no money and can leave
    if (credit == 0)
    {
        cout << "\nNo money inserted. Transaction cancelled.\n";
        return;
    }

    string code;

    while (true)
    {
        cout << "\nSelect slot (or CANCEL): ";

        if (!(cin >> code))
        {
            cout << "Invalid input. Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (code == "CANCEL" || code == "cancel")
        {
            cout << "\nRefund: ";
            displayMoney(credit);
            cout << endl;

            cout << "Refund breakdown:\n";
            showCoinBreakdown(credit);

            return;
        }

        int index = findItem(inventory, code);

        // Slot doesn't exist
        if (index == -1)
        {
            cout << "Sorry - that slot does not exist.\n";
            continue;
        }

        // Item sold out
        if (inventory[index].quantity == 0)
        {
            cout << "Sorry - " << inventory[index].code
                 << " is sold out.\n";
            continue;
        }

        // Not enough money
        if (credit < inventory[index].price)
        {
            cout << "Sorry - insufficient credit. You need ";
            displayMoney(inventory[index].price - credit);
            cout << " more.\n";
            continue;
        }

        // Successful purchase
        int change = credit - inventory[index].price;

        inventory[index].quantity--;

        printReceipt(inventory[index], credit, change);

        return;
    }
}

int main()
{
    Item inventory[NUM_ITEMS] =
    {
        {"A1", "Pretzels", 125, 3},
        {"A2", "Trail Mix", 175, 2},
        {"A3", "Chips", 100, 5},
        {"B1", "Cola", 150, 7},
        {"B2", "Water", 100, 6},
        {"B3", "Candy", 75, 4}
    };

    int choice;

    cout << "=============================\n";
    cout << "       VENDING MACHINE       \n";
    cout << "=============================\n";

    while (true)
    {
        cout << "\n1. Start transaction\n";
        cout << "2. Display inventory\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter 1, 2, or 3.\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            continue;
        }

        if (choice == 1)
        {
            serveCustomer(inventory);
        }
        else if (choice == 2)
        {
            displayInventory(inventory);
        }
        else if (choice == 3)
        {
            cout << "\nThank you for using the vending machine!\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}
