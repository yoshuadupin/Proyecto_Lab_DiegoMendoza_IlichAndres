#include "sony.h"

Sony::Sony(string name , int year , /*Console* console,*/ int numberPlayers , 
string genre , string status , int serialNumber , double  price)
:Game( name ,  year , /* console,*/  numberPlayers , 
 genre ,  status ,  serialNumber ,   price){
	this->name = name; 	
	this->year = year; 
	//this->console = console;
	this->numberPlayers = numberPlayers ;
	this->genre = genre ;
	this->status = status;
	this->serialNumber = serialNumber;
	this->price = price;
}

Sony::~Sony(){
}