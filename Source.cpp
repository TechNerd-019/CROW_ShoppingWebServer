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

/*
* This function runs when the webserver is executed.
* It is in charge of processing the cart document where
* data will be written to and read from.
* 
* If a file does not exist, it shall be created by this function
* and stored on the /build directory.
*/
void fileCheck()
{
	std::ifstream myFile("Cart.txt");
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
/*
* This function is executed by the crow route handling
* the POST method. It will parse the parameters from
* the URL (which is provided by the "action" variable from
* the JavaScript function) and write its contents to a file
* where it is properly formatted.
*/
void addToCart(std::string itemName, int quantity)
{
	std::ofstream myFile("Cart.txt", std::ios::app);
	if (!myFile) { std::cout << "Cannot open the file." << std::endl; }

	myFile << "Item Name: " << itemName << std::endl;
	myFile << "Quantity: " << quantity << std::endl;

	myFile.close();

}
/*
* All these functions are required for processing various
* data types.
*/
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

/*
* This function takes two arguments, in addition to the crow response.
* It compares the values of what is entered in the log in form (from the
* JavaScript code) and returns the appropriate page where necessary.
*/
void checkLogInCredentials(crow::response& res, std::string username, std::string password)
{
	std::string serverUsername = "TacoLover23";
	std::string serverPassword = "DrZombossIsOutThere";

	res.set_header("Content-Type", "text/html");

	if (username == serverUsername && password == serverPassword)
	{
		sendHTML(res, "checkout-success.html");
		res.code = 202;
		res.end();
	}
	else
	{
		sendHTML(res, "checkout-failure.html");
		res.code = 401;
		res.end();
	}
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
				addToCart(itemName, quantity);
			}
		);
							
	CROW_ROUTE(lawnDefender, "/checkout").methods(crow::HTTPMethod::Post)
		([](const crow::request& req, crow::response& res)
			{
				std::string username = req.url_params.get("username");
				std::string password = req.url_params.get("password");

				checkLogInCredentials(res, username, password);

			}
		);

	CROW_ROUTE(lawnDefender, "/checkout/showCart")
		([](const crow::request& req, crow::response& res)
			{
				std::ifstream in("Cart.txt");
				if (in)
				{
					std::ostringstream contents;
					contents << in.rdbuf();
					in.close();
					res.set_header("Content-Type", "text/plain"); /*Needed for proper text formatting. */
					res.write(contents.str());
				}
				else
				{
					res.code = 404;
					res.write("Not Found");
				}

				res.end();
			}
		);

	lawnDefender.port(23500).multithreaded().run();

	return 0;
}