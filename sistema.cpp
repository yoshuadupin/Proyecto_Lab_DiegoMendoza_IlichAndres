	// Sistema.cpp

#include "Includes.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <vector>
#include <sstream>
#include <ctime>
#include <typeinfo>

#include <iostream>
#include <fstream>
#include <exception>
#include <regex>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>

// Escribe en archivo binario.
void writeFile(vector<Console*>, vector<Game*>);
// Lee archivo binario.
void readFile();
// Crea consolas predeterminadas.
vector<Console*> initializeConsoles();
// Crea videojuegos predeterminados.
vector<Game*> initializeVideoGames();
// Crea el ticket.
void doTicket(Sale*, UserSeller*);
// Obtiene la fecha.
string getDate();
// Obtiene la hora.
string getHour();
// Crea el userLog.
void doUserLog(int , double, UserSeller*);
// Devuelve opción del usuario (administrador o vendedor).
int getUserOption();
// Devuelve eliminar o modificar del administrador.
int getAdminOption();

void printVectorGame(vector<Game*>);
void printVectorConsole(vector<Console*> consoles);

void save(const BD &s, const char* filename){
    // make an archive
	std::ofstream ofs(filename, std::ios::binary);
	boost::archive::polymorphic_binary_oarchive oa(ofs);
	oa << s;
}

void restore(BD &s, const char* filename)
{
    // open the archive
	std::ifstream ifs(filename, std::ios::binary);
	boost::archive::polymorphic_binary_iarchive ia(ifs);

    // restore the schedule from the archive
	ia >> s;
}



int main() {
	/*vector<Console*> consoles;
	vector<Game*> videoGames;*/
	
	BD baseDatos;
	restore(baseDatos , "new.bin");
	string name, checkIn, checkOut;
	int option, password;
	int objetosVendidos = 0;
	double subTotalVendedor= 0;
	baseDatos.consoles = initializeConsoles();
	baseDatos.games = initializeVideoGames();
	save(baseDatos , "new.bin");


	// TODO: Validar si es admin, que el password sea el mismo al leer el binario.

	/*User* user;*/
	UserAdmin* user = new UserAdmin("Dilich", 123);

	cout << "Ingrese nombre del usuario: ";
	cin >> name;
	UserSeller* userSeller = new UserSeller(name, checkIn, "");
	userSeller -> setCheckIn(getHour());
	Console * console = new Nintendo(2004, "Nintendo 64", "Good", 1, 1100);

	do {

		option = getUserOption();

		if (option == 1) { // Administrador.
			int position;
			
			cout << "Ingrese password del administrador: ";
			cin >> password;


			option = getAdminOption();

			if (option == 1) { // Eliminar consola.
				cout << "Ingrese posición a eliminar: ";
				cin >> position;

				baseDatos.consoles = user -> deleteConsole(baseDatos.consoles, position);
			} else if (option == 2) { // Eliminar juego.
				cout << "Ingrese posición a eliminar: ";
				cin >> position;
				
				baseDatos.games = user -> deleteGame(baseDatos.games, position);
			} else if (option == 3) { // Modificar consola.
				cout << "Ingrese posición a modificar: ";
				cin >> position;
				
				baseDatos.consoles = user -> modifyConsole(baseDatos.consoles, position);
			} else if (option == 4) { // Modificar juego.
				cout << "Ingrese posición a modificar: ";
				cin >> position;

				baseDatos.games = user -> modifyGame(baseDatos.games, position);
			}else if(option == 5){
				cout<<"Agregara un juego:"<<endl;
				baseDatos.games =user -> addGame(baseDatos.games);   
			}else if(option == 6){
				cout<<"Agregara una consola:"<<endl;
				baseDatos.consoles = user -> addConsole(baseDatos.consoles);
			} else {
				cout << "Opción inválida!... Regresás al main, por maule .-." << endl;
				//option = 3;
			}

			// Escribir al archivo binario para que guarde todo.
		} else if (option == 2) { // Vendedor.
			
			Sale* sale = userSeller -> makeSale(baseDatos.consoles, videoGames);
			doTicket(sale, userSeller);
			objetosVendidos += sale ->getConsoles().size()+ sale->getGames().size();
			subTotalVendedor += sale ->getSubtotal();			
			delete sale;
		}
	} while (option != 3);
	userSeller -> setCheckOut(getHour());
	doUserLog(objetosVendidos,subTotalVendedor , userSeller);
	delete userSeller;
	delete user;
	return 0;
}

void writeFile(vector<Console*> consoles, vector<Game*> videoGames) {
	Management* management = new Management(consoles, videoGames);

	FILE* file = fopen("Test.dat", "wb");

	fwrite(management, sizeof(Management), sizeof(management), file); // Escribo el tipo Register en el archivo.
	fclose(file);
}

void readFile() {
	FILE* file;
	long lSize;
	Management* management;
	size_t result;

	file = fopen("Test.dat", "rb");
	
	if (file == NULL) {
		fputs("File error", stderr);
		exit (1);
	}

	// Obtiene el size del archivo:

	fseek(file, 0, SEEK_END);
	lSize = ftell(file);
	rewind(file);

	// Guarda memoria para guardar todo el archivo:

	management = (Management*) malloc(sizeof(Management)*lSize);

	if (management == NULL) {
		fputs("Memory error", stderr);
		exit(2);
	}

	// Copia el archivo en el búfer:
	
	result = fread (management,1,lSize,file);

	if (result != lSize) {
		fputs("Reading error", stderr);
		exit(3);
	}

	// Ahora, todo el archivo está cargado en la memoria del búfer.

	//vector<Console*> consoles = management -> getConsoles();
	vector<Game*> videoGames = management -> getVideoGames();

/*	for (int i = 0; i < consoles.size(); ++i) {
		cout << management -> getConsole(i) -> getSerialNumber() << endl;
	}

	for (int i = 0; i < videoGames.size(); ++i) {
		cout << management -> getVideoGame(i) -> getPrice() << endl;
	}*/

	// Cierro el archivo.

	fclose (file);
	free (management);
}

vector<Console*> initializeConsoles() {
	vector<Console*> consoles;

	for (int i = 0; i < 10; ++i) { // Nintendo 64.

		consoles.push_back(new Nintendo(2004, "Nintendo 64", "Good", i + 1, 1100));
	}

	for (int i = 10; i < 20; ++i) { // Play Station.
		Console* console = new Sony(2005, "Play Station", "Good", i + 1, 1400);
		consoles.push_back(console);
	}

	for (int i = 20; i < 30; ++i) { // Wii.
		Console* console = new Nintendo(2007, "Wii", "Good", i + 1, 1350);
		consoles.push_back(console);
	}

	for (int i = 30; i < 40; ++i) { // Nintendo Gamecube.
		Console* console = new Nintendo(2005, "Nintendo Gamecube", "Good", i + 1, 1200);
		consoles.push_back(console);
	}
	
	return consoles;
}

vector<Game*> initializeVideoGames() {
	vector<Game*> videoGames;

	for (int i = 0; i < 30; ++i) { // Mario Bros.
		Console* console = new Nintendo(2004, "Nintendo 64", "Good", i + 1, 1100);
		videoGames.push_back(new Konami("Mario Bros " + (i + 1), 2005, console, 4, "Entertainment", "Good", i + 1, 250));
	}

	for (int i = 30; i < 60; ++i) { // Donkey Kong.
		Console* console = new Sony(2005, "Play Station", "Good", i + 1, 1400);
		videoGames.push_back(new Sega("Donkey Kong " + (i + 1), 2005, console, 3, "Entertainment", "Good", i + 1, 290));
	}

	for (int i = 60; i < 90; ++i) { // Need For Speed.
		Console* console = new Nintendo(2007, "Wii", "Good", i + 1, 1350);
		videoGames.push_back(new Ubisoft("Need For Speed " + (i + 1), 2007, console, 4, "Races", "Good", i + 1, 350));
	}

	for (int i = 90; i < 120; ++i) { // RFinal Fantasy.
		Console* console = new Microsoft(2005, "Nintendo Gamecube", "Good", i + 1, 1200);
		videoGames.push_back(new Game("Final Fantasy " + (i + 1), 2003, console, 1, "Adventure", "Good", i + 1, 230));
	}

	return videoGames;
}

void doTicket(Sale* sale, UserSeller* user) {
	ofstream file;
	stringstream stringStream;
	string myString;
	double subtotal = 0, isv, total = 0;

	stringStream << "./log_ventas/" << getDate() << getHour() << ".log";
	myString = stringStream.str();

	file.open(myString.c_str());

	file << "			GAMEHUB          " << endl << endl;
	file << "FECHA: " << getDate() << endl;
	file << "HORA: " << getHour() << endl;
	file << "VENDEDOR: " << sale -> getUserName() << endl;
	file << "CLIENTE: " << sale -> getClientName() << endl << endl;
	file << "CANTIDAD DE ARTÍCULOS: " << endl << endl;

	// TODO: Nombre de artículos.
	vector<Console*> consoles = sale -> getConsoles();
	vector<Game*> videoGames = sale -> getGames();

	for (int i = 0; i < consoles.size(); ++i) { // Consolas compradas.
		subtotal += consoles.at(i) -> getPrice();
		file << consoles.at(i) -> getModel() << "	" << consoles.at(i) -> getPrice() << endl;
	}

	for (int i = 0; i < videoGames.size(); ++i) { // Videojuegos comprados.
		subtotal += videoGames.at(i) -> getPrice();
		file << videoGames.at(i) -> getName() << "	  " << videoGames.at(i) -> getPrice() << endl;
	}

	file << "SUBTOTAL: " << subtotal << endl;

	isv = subtotal * 0.15;
	total = subtotal + isv;

	file << "IMPUESTO (15%): " << isv << endl;
	file << "TOTAL: " << total << endl;

	file.close();
}

string getDate() {
	stringstream stringStream;
	string date;

	time_t now = time(0);
	tm* time  = localtime(&now); // Obteniendo el tiempo.
	int year = 1900 + time -> tm_year;
	int month = time -> tm_mon;
	int day = time -> tm_mday;

	stringStream << day << "-" << month + 1 << "-" << year;
	date = stringStream.str();

	return date;
}

string getHour() {
	stringstream stringStream;
	string actualHour;
	vector<int> hours;

	hours.push_back(12);
	hours.push_back(1);
	hours.push_back(2);
	hours.push_back(3);
	hours.push_back(4);
	hours.push_back(5);
	hours.push_back(6);
	hours.push_back(7);
	hours.push_back(8);
	hours.push_back(9);
	hours.push_back(10);
	hours.push_back(11);
	hours.push_back(12);
	hours.push_back(1);
	hours.push_back(2);
	hours.push_back(3);
	hours.push_back(4);
	hours.push_back(5);
	hours.push_back(6);
	hours.push_back(7);
	hours.push_back(8);
	hours.push_back(9);
	hours.push_back(10);
	hours.push_back(11);

	time_t now = time(0);
	tm* time  = localtime(&now); // Obteniendo el tiempo.
	int hour = time -> tm_hour;
	int minute = time -> tm_min;
	int second = time -> tm_sec;

	stringStream << hours[hour] << ":" << minute << ":" << second;
	actualHour = stringStream.str();

	return actualHour;
}

void doUserLog(int cant , double subTotal, UserSeller* user) {
	ofstream file;
	stringstream stringStream;
	string myString;

	stringStream << "./usuarios_log/" << user -> getName() << getDate() << ".log";
	myString = stringStream.str();

	file.open(myString.c_str());

	file << "			GAMEHUB          " << endl << endl;
	file << "NOMBRE: " << user -> getName() << endl;
	file << "HORA DE ENTRADA: " << user -> getCheckIn() << endl;
	file << "HORA DE SALIDA: " << user -> getCheckOut() << endl << endl;
	file << "CANTIDAD DE ARTÍCULOS VENDIDOS: " << cant << endl;
	file << "CANTIDAD DE DINERO GENERADO: " << subTotal << endl;
	// TODO: Crear archivo con información de ventas del ususario.
}

int getUserOption() {
	int option;

	cout << "Ingrese tipo de usuario: \n1. Administrador \n2. Vendedor \n3. Salir \nOpción: ";
	cin >> option;

	return option;
}

int getAdminOption() {
	int option;
	do{
		cout << "Qué desea hacer?: \n1. Eliminar consola \n2. Eliminar juego \n3. Modificar consola \n4. Modificar juego \n5. Agregar Juego \n6.AgregarConsola \nOpción: ";
		cin >> option;
	}while(option<1 && option>6);

	return option;
}


void printVectorGame(vector<Game*> games){
	for (int i = 0; i < games.size(); ++i){
		for (int i = 0; i < games.size(); ++i){
			cout << "Índice: "<< i << endl;
			cout << "Nombre: " << games[i] -> getName() << endl;
			cout << "Consola: " << games[i] -> getConsole()->getModel() << endl;
			cout << "Número de Jugadores: " << games[i] -> getNumberPlayers() << endl;
			cout << "Estado: " << games[i] -> getStatus() << endl;
			cout << "Precio: " << games[i] -> getPrice() << endl;
			cout << "Número de serie: " << games[i] -> getSerialNumber() << endl;
			cout << endl;	
		}
	}
}

void printVectorConsole(vector<Console*> consoles){
	for (int i = 0; i < consoles.size(); ++i){
		cout << "Índice: "<< i << endl;
		cout << "Modelo: " << consoles[i] -> getModel() << endl;
		cout << "Estado: " << consoles[i] -> getCondition() << endl;
		cout << "Precio: " << consoles[i] -> getPrice() << endl;
		cout << "Número de serie: " << consoles[i] -> getSerialNumber() << endl;
		cout << endl;
	}
}