#pragma once

#ifndef ITEM_H_
#define ITEM_H_

class Item { //a class to represent a category or a sellingItem
protected:
	char title[20];
	bool isACategory;
	char description[50];
	double price;
	int quantity;
	double maxDiscount;

public:
	int key; //even numbers for categories and odd numbers for selling items

	Item() { //default constructor
		key = 0;
		isACategory = true;
		price = 0;
		quantity = 0;
		maxDiscount = 0;
	}

	Item(int k) {
		key = k;
		if (key % 2 == 0) //even numbers
			isACategory = true;
		else
			// odd numbers
			isACategory = false;

		price = 0;
		quantity = 0;
		maxDiscount = 0;
	}

	Item(const Item& item) { // copy constructor
		key = item.key;
		strcpy(title, item.title);
		isACategory = item.isACategory;
		strcpy(description, item.description);
		price = item.price;
		quantity = item.quantity;
		maxDiscount = item.maxDiscount;
	}

	string getTitle() {
		return title;
	}

	void setTitle(const string n) {
		strcpy(title, n.c_str());
	}

	bool isCategory() {
		return isACategory;
	}

	string getDescription() {
		return description;
	}

	double getPrice() {
		return price;
	}

	int getQuantity() {
		return quantity;
	}

	double getMaxDiscount() {
		return maxDiscount;
	}

	void displayTitle() {
		if (isACategory)
			cout << "Category" << ": " << title;
		else
			cout << "Selling item" << ": " << title;
	}

	void setPrice(double p) {
		price = p;
	}

	void setQuantity(int q) {
		quantity = q;
	}

	void printAItem(int role) {
		cout.setf(ios::left);
		if (isACategory) {
			cout << "| " << setw(15) << "Category Key " << " | " << setw(20)
				<< "Category Title" << endl;
			cout << "| " << setw(15) << key << " | " << setw(20) << title
				<< endl;
		}
		else {
			cout << setw(10) << "| Item key " << setw(20) << "| Item Title"
				<< setw(15) << "| Stock Quantity " << setw(15)
				<< "| List Price " << setw(15);

			if (role == 0 || role == 2)
				cout << "| Max Discount(%)";

			cout << setw(50) << "| Item Description" << endl;

			cout << "| " << setw(9) << key << "| " << setw(18) << title << "| "
				<< setw(15) << quantity << "| $" << setw(12) << price;

			if (role == 0 || role == 2)
				cout << "| " << setw(15) << (maxDiscount * 100);

			cout << "| " << setw(50) << description << endl;
		}
	}

	void inputAnItem() {
		if (isACategory) {
			cout << "Input the title of the category : " << endl;
			cin.get();
			cin.getline(title, 20, '\n');
		}
		else {
			cout << "Input the title of the item: " << endl;
			cin.get();
			cin.getline(title, 20, '\n');

			cout << "\nInput a brief description of the item: " << endl;
			//cin.ignore();
			cin.getline(description, 50, '\n');

			cout << "\nInput your selling price ($): " << endl;
			char input[10];
			cin >> input;

			price = atof(input);

			cout << "\nInput the quantity of the item: " << endl;
			cin >> input;

			quantity = atoi(input);

			cout
				<< "\nInput your maximal discount in percentage (value between 0 - 100): "
				<< endl;
			cin >> input;
			double maxDis = atof(input);
			if (maxDis < 0 || maxDis > 100)
				maxDis = 100;
			maxDiscount = maxDis * 0.01;

			printAItem(0); //Seller's view

			cout << endl;
		}

		return;
	}

	bool buyASellingItem() {
		cout << "\nDo you want to buy this item (y/n)? ";
		char c;
		cin >> c;
		if (c == 'Y' || c == 'y') {
			char input[10];
			cout << "\nPlease input your offer price: ";
			cin >> input;
			double offerPrice;
			offerPrice = atof(input);
			if (offerPrice >= getPrice() * (1.0 - getMaxDiscount())) {
				cout
					<< "\nThank you for buying our products. You have bought the following item at price $"
					<< offerPrice << ":" << endl;

				printAItem(1); //Buyer's view
				cout << endl;

				return true;
			}
			else {
				cout << "\nSorry, we can't sell it to you at this price.\n"
					<< endl;
				return false;
			}
		}
		return false;
	}

	bool confirmAction(string s) {
		cout << "\nAre you sure you want to " << s
			<< " this category/selling item? ";
		char c;
		cin >> c;
		if (c == 'Y' || c == 'y') {
			return true;
		}
		return false;
	}
};

#endif /* ITEM_H_ */
