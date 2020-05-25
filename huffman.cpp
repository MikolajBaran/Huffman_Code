#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Element{
	bool isSgn;
	char sgn;
	int num;
	string code;
	bool isConnected;
	int refA, refB;
	public:
		Element(char s){
			isSgn = true;
			sgn = s;
			num = 1;
			isConnected = false;
			code = "";
		}
		
		Element(int n, int a, int b){
			isSgn = false;
			sgn = '¶';
			num = n;
			isConnected = false;
			code = "";
			refA = a;
			refB = b;
		}
		
		bool ifIsSign(){
			if(this->isSgn){
				return true;
			}
			else{
				return false;
			}
		}
		
		bool ifIsConnected(){
			if(this->isConnected){
				return true;
			}
			else{
				return false;
			}
		}
		
		void connect(){
			this->isConnected = true;
		}
		
		char showSign(int n){
			if(this->num == n){
				return this->sgn;
			}
		}
		
		char showSign(){
			return this->sgn;
		}
		
		int showNumber(){
			return this->num;
		}
		
		int showA(){
			return this->refA;
		}
		
		int showB(){
			return this->refB;
		}
		
		void incrementNum(){
			this->num++;
		}
		
		void changeCode(string str){
			this->code = str + this->code;
		}
		string showCode(){
			return this->code;
		}
};

class Sign{
	char sign;
	string code;
	public:
		Sign(char s, string c){
			sign = s;
			code = c;
		}
		char showSign(){
			return this->sign;
		}
		string showCode(){
			return this->code;
		}
};

vector <Element*> Elements;
vector <Sign*> Signs;

bool sortingVector(Element* a, Element* b){
	return a->showNumber() < b->showNumber();
}

void updateCode(Element* el, string symbol){
	if(el->ifIsSign()){
		el->changeCode(symbol);
	}
	else{
		updateCode(Elements[el->showA()],symbol);
		updateCode(Elements[el->showB()],symbol);
	}
}

string fileName;

bool readFile(){
	fstream file;
	file.open(fileName.c_str(), std::ios::in);
	if(file.good()){
		cout << "Plik odczytany\n" << endl;
		char temp;
		file.get(temp);
		while(!file.eof())
        {
            if(Elements.size()>0){
            	int size = Elements.size();
            	bool createNewObj = true;
            	for(int i=0; i<size; i++){
            		if(Elements[i]->showSign() == temp){
            			Elements[i]->incrementNum();
            			createNewObj = false;
            			break;
					}
				}
				if(createNewObj){
					Elements.push_back(new Element(temp));
				}
			}
			else{
				Elements.push_back(new Element(temp));
			}
			file.get(temp);
        }
        cout << endl;
        file.close();
		return true;
	}
	else{
		cout << "Nie udalo sie uzyskac dostepu do pliku o nazwie: " << fileName << ". Sprobuj podac inna nazwe." << endl;
		file.close();
		return false;
	}
}

bool everythingConnected(){
	bool connected = true;
	for(int i=0; i<Elements.size()-1; i++){
		if(!Elements[i]->ifIsConnected()){
			connected = false;
			break;
		}
	}
	return connected;
}

bool sortAlphabet(Sign* a, Sign* b){
	return a->showCode().length() > b->showCode().length();
}

void codeText(){
	fstream file;
	file.open(fileName.c_str(), std::ios::in);
	if(file.good()){
		string code = "";
		cout << "Zakodowany tekst:\n" << endl;
		char temp;
		file.get(temp);
		while(!file.eof())
        {
            for(int i=0; i<Signs.size(); i++){
            	if(temp==Signs[i]->showSign()){
            		cout << Signs[i]->showCode();
            		code+=Signs[i]->showCode();
				}
			}
			file.get(temp);
        }
        cout << "\n\n";
        file.close();
        decodeText(code);
	}
	else{
		cout << "Nie udalo sie uzyskac dostepu do pliku o nazwie: " << fileName << ". Sprobuj podac inna nazwe." << endl;
		file.close();
	}
}

void createCode(){
	while(!everythingConnected()){
		sort(Elements.begin(), Elements.end(),sortingVector);		
		
		char a = 'x',b = 'x';
		for(int i=0; i<Elements.size(); i++){
			if(!Elements[i]->ifIsConnected()){
				if(a=='x'){
					a=i;
					Elements[i]->connect();
					updateCode(Elements[i],"0");
				}
				else{
					b=i;
					Elements[i]->connect();
					updateCode(Elements[i],"1");
					break;
				}
			}
		}
		
		int sumOfWeights = Elements[a]->showNumber() + Elements[b]->showNumber();
		Elements.push_back(new Element(sumOfWeights,a,b));
	}
	for(int y=0; y<Elements.size(); y++){
		if(Elements[y]->ifIsSign()){
			Signs.push_back(new Sign(Elements[y]->showSign(),Elements[y]->showCode()));
		}
	}
	codeText();
}

int main() {
	bool inType;
	cout << "Program dokonuje kompresji Huffmana na danych tekstowych." << endl;
	cout << "Wybierz metode wprowadzenia danych do kompresji \n[0 dla wprowadzania z klawiatury i 1 dla wprowadzania z pliku tekstowego]: ";
	cin >> inType;
	if(inType){
		cout << "\n\nWybrano metode wprowadzenia z pliku. Podaj nazwe pliku, z ktorego ma byc pobrany tekst: ";
		cin >> fileName;
		while(!readFile()){
			cout << "\nPodaj nazwe pliku, z ktorego ma byc pobrany tekst: ";
			cin >> fileName;
		}
		createCode();
	}
	
	for(int x=0; x<Elements.size(); x++){
		delete Elements[x];
	}
	for(int y=0; y<Signs.size(); y++){
		delete Signs[y];
	}
	return 0;
}
