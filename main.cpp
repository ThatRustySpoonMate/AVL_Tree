/*
 * Main.cpp
 *
 *  Created on: 17/05/2022
 *      Author: Ethan Harris
 */

// This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <iomanip> 
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#include "AVL_ADT.h"
#include "Item.h"

using namespace std;


class keyHolder {
protected:
    int keyEven;
    int keyOdd;

public:

    keyHolder() {
        keyEven = 0;
        keyOdd = 1;
    }

    int getKey(bool polarity) {
        if (polarity) {
            keyEven += 2;
            return keyEven;
        }
        else {
            keyOdd += 2;
            return keyOdd;
        }
    }

    void setKey(bool polarity, int newVal) {
        if (!polarity) {
            keyOdd = newVal;
        }
        else {
            keyEven = newVal;
        }
    }

};


int getUserRole(string roles[3]);

int getUserAction(int role, string actions[3][5]);

void clearScreen(string role, bool drawHeading = true);

void displayItemSynopsis(Item data);

void displayItemFull(Item data);


int main()
{
    AvlTree<Item, int> Catalogue;
    string roles[3] = { "Seller", "Buyer", "Manager" };
    string actions[3][5] = {
        {"Change role", "Sell an item", "View tree", "Search"},
        {"Change role", "Buy an item", "View tree", "Search"},
        {"Change role", "Remove a selling item", "View tree", "Search"} };

    int userRole = -1; int userAction = -2;
    keyHolder myKeys;

    // Determine user's role
    userRole = getUserRole(roles);

    // Add the root category to the tree/catalogue
    Item rootCat(myKeys.getKey(true)); // Define Root Category
    rootCat.setTitle("Root Category");
    Catalogue.AVL_Insert(rootCat);


    while (userAction != -1) {
        clearScreen(roles[userRole]);
        userAction = getUserAction(userRole, actions);

        switch (userAction) {
        
        case 0: 
            clearScreen(roles[userRole]);
            userRole = getUserRole(roles);
            break;

        case 1:
            if (userRole == 0) {
                // Seller is selling an item
                Item newItem(myKeys.getKey(false));
                newItem.inputAnItem();
                Catalogue.AVL_Insert(newItem);
            }
            else if (userRole == 1) {
                // Buyer is buying an item
                int desiredItemKey;
                Item temp;

                clearScreen(roles[userRole]); // cls
                // Get the key of the item
                cout << "Which item would you like to buy?" << endl;
                Catalogue.AVL_Traverse(displayItemSynopsis); // List each item to buy
                cout << "\nYour selection: ";
                cin >> desiredItemKey;
                desiredItemKey = desiredItemKey * 2 + 1;
                
                // Check AVL_count and if == 1 (Just the root), then exit
                
                // Ensure key is valid by checking it is in the list, then checking it is an item (not a category)
                while (!Catalogue.AVL_Retrieve(desiredItemKey, temp) || temp.key % 2 == 0) {
                    cout << "Selection invalid. Please check your input and try again.\nYour Selection: ";
                    cin >> desiredItemKey;

                    if (desiredItemKey == -1) {
                        break;
                    }
                    desiredItemKey = desiredItemKey * 2 + 1;

                    
                }

                // Sell the item to the buyer
                if (desiredItemKey != -1 && temp.buyASellingItem()) {
                    // Subtract one from the quantity
                    int newQty = temp.getQuantity() - 1;

                    Catalogue.AVL_Delete(desiredItemKey); // Either we delete or change value (Either way we first need to delete the value)

                    if (newQty > 0) {
                        //Update the quantity - we already have the item, so all we need to do is change the quantity value and re-add it to the tree
                        temp.setQuantity(newQty);
                        Catalogue.AVL_Insert(temp);
                    }
                    
                }

                Sleep(3000);
                
            }

            else {
                // Manager is deleting an item
                int desiredItemKey;
                Item temp;

                clearScreen(roles[userRole]); // cls
                // Get the key of the item
                cout << "Please select an item to delete/modify (-1 to go back)." << endl;
                Catalogue.AVL_Traverse(displayItemSynopsis); // List each item to buy
                cout << "\nYour selection: ";
                cin >> desiredItemKey;
                desiredItemKey = desiredItemKey * 2 + 1;

                // Check AVL_count and if == 1 (Just the root), then exit
                // 
                // Ensure key is valid by checking it is in the list, then checking it is an item (not a category)
                while (!Catalogue.AVL_Retrieve(desiredItemKey, temp) || temp.key % 2 == 0) {
                    cout << "Selection invalid. Please check your input and try again.\nYour Selection: ";
                    cin >> desiredItemKey;

                    if (desiredItemKey == -1) {
                        break;
                    }
                    desiredItemKey = desiredItemKey * 2 + 1;

                }

                // Delete/Modify
                clearScreen(roles[userRole]); // cls
                temp.printAItem(userRole);
                string userTextInput;
                cout << "\nWould you like to modify (m) or delete (d) this item?" << endl;
                
                while (userTextInput != "m" && userTextInput != "modify" && userTextInput != "d" && userTextInput != "delete") {
                    cout << "\nYour selection: ";
                    cin >> userTextInput;
                }

                if (userTextInput == "d" || userTextInput == "delete") {
                    // Deleting the target item
                    if (desiredItemKey != -1 && Catalogue.AVL_Delete(desiredItemKey)) { // Try to delete the specified item
                        cout << "Successfuly deleted item!" << endl;
                    }
                    else {
                        if (desiredItemKey != -1) {
                            cout << "Unable to delete item, please check the tree and try again." << endl;
                        }
                    }
                }
                else {
                    // Modifying the target item
                    temp.inputAnItem();
                    if (Catalogue.AVL_Insert(temp) && Catalogue.AVL_Delete(desiredItemKey)) {
                        cout << "\nSuccessfully updated target item!" << endl;
                    }
                    else {
                        cout << "\nUnable to update target item, please check your inputs and try again." << endl;
                    }

                }

                Sleep(3000);

            }

            break;
        case 2:
            Catalogue.AVL_Traverse(displayItemFull);
            Sleep(3000);
            break;


        case 3:
            // Search phrase
            clearScreen(roles[userRole]);
            string userSearchTerm;

            cout << "Enter a search term: ";
            cin >> userSearchTerm;

            Catalogue.AVL_Search(userSearchTerm);

            Sleep(3000);



            break;
        }


        


    }


}

void displayItemSynopsis(Item data) {

    if (data.key % 2 == 1) {
        cout << ceil(data.key / 2) << ") " << setw(20) << data.getTitle() << "Quantity: " << setw(5) << data.getQuantity() << "Price $" << setw(15) << data.getPrice() << endl;
    }
}

void displayItemFull(Item data) {
    data.printAItem(1);
}

void clearScreen(string role, bool drawHeading) {
    system("CLS");
    if (drawHeading == true) {
        cout << "You are logged in as " << role << "\n" << endl;
    }
}

int getUserRole(string roles[3]) {
    int returnRole = -1;

    // Determine user's role
    while (returnRole < 1 || returnRole > 3) {
        cout << "Please select the role to login to:" << endl;
        for (int i = 0; i < 3; i++) {
            cout << i+1 << ") " << roles[i] << endl;
        }

        cout << "Your selection: ";
        cin >> returnRole;
    }
    returnRole--;

    return returnRole;
}

int getUserAction(int role, string actions[3][5]) {
    int returnAction = -1;

    // Determine amount of actions 
    size_t possible_actions = 0;
    for (size_t i = 0; i < sizeof(actions[role]) / sizeof(*actions[role]); i++) {
        if (actions[role][i] != "") {
            possible_actions++;
        }
    }

    // Determine user's action selection
    while (returnAction < 0 || returnAction > possible_actions) {
        cout << "Which action would you like to perform?" << endl;
        cout << "===================================" << endl;
        cout << "0) Exit" << endl;
        for (int i = 0; i < possible_actions; i++) {
            cout << i + 1 << ") " << actions[role][i] << endl;
        }
        cout << "===================================" << endl;

        cin >> returnAction;
    }
    returnAction--;
    return returnAction;
}