#define CROW_MAIN

/*
* Lab written by Salah Salame
* Prof: Dr. Elliot Coleshill, PhD.
* CSCN72050 - Mobile & Networked Systems
* Tuesday, April 9, 2024
* 
* Lab 10 - Design Sprint - Shopping Website.
*/

#include <iostream>
#include <fstream>
#include <filesystem>

#include "crow_all.h"

void fileCheck()
{
	std::ifstream myFile("Cart.txt.");
	if (!myFile)
	{
		std::cout << "Cannot open the file, or does not exist. Creating new file." << std::endl;
		std::ofstream myFile("Cart.txt");
	}
	else
	{
		std::cout << "An already existing file has been loaded." << std::endl;
	}
	myFile.close();
}

void sendFile(crow::response& res, std::string fileName, std::string contentType)
{
	std::ifstream in("../public/" + fileName, std::ifstream::in);

	if (in)
	{
		std::ostringstream contents;
		contents << in.rdbuf();
		in.close();
		res.set_header("Content-Type", contentType);
		res.write(contents.str());
	}
	else
	{
		res.code = 404;
		res.write("Not Found");
	}

	res.end();
}

void addToCart(crow::response& res, std::string itemName, int quantity)
{
	std::ofstream myFile("Cart.txt", std::ios::app);
	if (!myFile) { std::cout << "Cannot open the file." << std::endl; }

	myFile << "Item Name: " << itemName << std::endl;
	myFile << "Quantity: " << quantity << std::endl;

	myFile.close();
	res.write("The item has been added to your shopping cart!\n");
}

void sendHTML(crow::response& res, std::string fileName)
{
	sendFile(res, fileName, "text/html");
}

void sendImage(crow::response& res, std::string fileName)
{
	sendFile(res, "images/" + fileName, "image/jpeg");
}

void sendScripts(crow::response& res, std::string fileName)
{
	sendFile(res, "scripts/" + fileName, "text/javascript");
}

void sendStyle(crow::response& res, std::string fileName)
{
	sendFile(res, "styles/" + fileName, "text/css");
}

int main(void)
{
	fileCheck();

	crow::SimpleApp lawnDefender;

	CROW_ROUTE(lawnDefender, "/")
		([](const crow::request& req, crow::response& res)
			{
				sendHTML(res, "index.html");
			}
		);

	CROW_ROUTE(lawnDefender, "/<string>")

		([](const crow::request& req, crow::response& res, std::string fileName)
			{
				sendHTML(res, fileName);
			}
		);

	CROW_ROUTE(lawnDefender, "/scripts/<string>")
		([](const crow::request& req, crow::response& res, std::string fileName) 
			{
				sendScripts(res, fileName);
			}
		);

	CROW_ROUTE(lawnDefender, "/styles/<string>")
		([](const crow::request& req, crow::response& res, std::string fileName) 
			{
				sendStyle(res, fileName);
			}
		);

	CROW_ROUTE(lawnDefender, "/images/<string>")
		([](const crow::request& req, crow::response& res, std::string fileName)	
			{
				sendImage(res, fileName);
			}
		);
	CROW_ROUTE(lawnDefender, "/<string>/<int>").methods(crow::HTTPMethod::Post)
		([](const crow::request& req, crow::response& res, std::string itemName, int quantity)
			{
				addToCart(res, itemName, quantity);
			}
		);


	lawnDefender.port(23500).multithreaded().run();

	return 0;
	
}

