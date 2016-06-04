#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>

using namespace std;
int obtenerPrimoAleatorio();
bool esPrimo(int num);
int euler(int numero);
bool sonCoprimos(int num1, int num2);
int menorNumero (int num1, int num2);
int obtenerExponenteAleatorio(int euler);
long exp_modular(long base, long exponente);
int obtenerMCD(int num1, int num2, int &xMcd);
int obtenerInverso(int exponente, int fi);
void explode_num(int* res,double num);
double binario(int num);
int expo_modular(int base,int exponente,int modulo);

int main() {
	cout << "Generando claves.." << endl;
	
    int p, q;

    // Obtengo p como numero primo
    do{
        p = obtenerPrimoAleatorio();
    }while(p > 10);

    // Obtengo q como numero primo distinto de p
    do{
        q = obtenerPrimoAleatorio();
    }while(p == q);

    int n = p * q;
    // Como p y q son primos, es lo mismo hacer euler(n) que la multiplicacion de (p - 1) y (q - 1)
    int eulerN = ((p - 1) * (q - 1));
    int e = obtenerExponenteAleatorio(eulerN);
    int d = obtenerInverso(e, eulerN);

	string mensaje;
	cout << "Introduzca un palabra a encriptar" << endl;
	cin >> mensaje;
	int size = mensaje.size();
	int crypted[size];
	cout << "Original:  "<< mensaje << endl;
	for (int x=0; x<=size-1; x++) {
		//Utilizo un array auxiliar para guardar la encripcion, ya que no puedo guardar cualquier numero en un espacio de string
		//De todos modos encripto el mensaje, como para que se vea una aproximación a lo encriptado
		crypted[x]=expo_modular(mensaje[x],e,n);
		mensaje[x]=expo_modular(mensaje[x],e,n);
	}
	cout << "Encriptado:  " << mensaje << endl;
	for (int x=0; x<=size-1; x++) {
		//Al desencriptar, unicamente utilizo el array auxiliar
		mensaje[x]=expo_modular(crypted[x],d,n);
	}
	cout << "Desencriptado:  " << mensaje << endl;
	
    return 0;
}

// Devuelve el resultado de elevar un numero (base) a otro determinado (exponente)
long exp_modular(long base, long exponente){
    if (exponente < 0){
        return exp_modular((1 / base), (-exponente));
    }else if (exponente == 0){
        return 1;
    }else if (exponente == 1){
        return base;
    }else if (exponente % 2 == 0){
        return exp_modular((base * base), (exponente / 2));
    }else{
        return (base * exp_modular((base * base), ((exponente - 1) / 2)));
    }
}

// Devuelve el inverso si existiese
int obtenerInverso(int exponente, int fi){
    int mcd, xMcd;
    int x = 0;

    mcd = obtenerMCD(fi, exponente, xMcd);

    // Si el MCD es igual a 1, entonces existe un inverso
    if (mcd == 1){
        x = xMcd;

        if (x < 0){
            x = (x + fi);
        }
    }

    return x;
}

// Devuelve un array donde el primer valor es el MCD, y en caso de existir un inverso, el mismo lo devuelve en el segundo valor
int obtenerMCD(int num1, int num2, int &xMcd){
    int mcd;
    int x, d;

    if (num2 == 0){
        mcd = num1;
        xMcd = 0;
    }else{
        int x1 = 1, x2 = 0;
        int q = 0, r = 0;

        while(num2 > 0){
            q = (num1 / num2);
            r = num1 - (q * num2);
            x = x2 - (q * x1);
            num1 = num2;
            num2 = r;
            x2 = x1;
            x1 = x;
        }

        mcd = num1;
        xMcd = x2;
    }

    return mcd;
}

// Devuelve un numero aleatorio positivo y menor al euler recibido hasta que ambos sean coprimos
int obtenerExponenteAleatorio(int euler){
    int exponente;

    do{
        // Obtengo un numero aleatorio positivo menor al euler recibido hasta que sean coprimos
        exponente = rand() % (euler - 1) + 1;
    }while(!sonCoprimos(exponente, euler));

    return exponente;
}

// Devuelve TRUE si el numero recibido es primo
bool esPrimo(int num) {
	if ((num == 0) || (num == 1)) {
		return false;
	}

	// Si encuentra algun divisor excluyendo el 1 y el mismo numero, el mismo ya no es primo
	for (int divisor = 2; divisor < num; divisor++){
        if (num % divisor == 0){
            return false;
        }
	}

	// No se encontro ningun divisor por lo que el numero es primo
	return true;
}

// Devuelve un numero primo aleatorio entre 1 y 255
int obtenerPrimoAleatorio(){
    int primo;

    // Obtengo numeros de manera aleatoria entre el 1 y 255, hasta que alguno sea primo
    do{
        srand(time(NULL));
		primo = rand() % 255 + 1;
    }while (!esPrimo(primo));

    return primo;
}

// Devuelve la cantidad de numeros coprimos respecto del numero recibido
int euler(int numero){
    int euler = 1;

    // Por propiedades de Euler, si el numero recibido es primo, el resultado es igual al numero menos 1
    if (esPrimo(numero)){
        return (numero - 1);
    }

    // Obtengo la cantidad total de numeros coprimos respecto del numero recibido
    for (int coprimoTemp = 2; coprimoTemp < numero; coprimoTemp++){
        if (sonCoprimos(numero, coprimoTemp)){
            // Si los dos numeros son coprimos, aumento la variable euler en 1
            euler++;
        }
    }

    return euler;
}

// Devuelve TRUE si los dos numeros recibidos son coprimos entre si
bool sonCoprimos(int num1, int num2){
    int menor = menorNumero(num1, num2);
    // Definimos que son coprimos hasta detectar lo contrario

    for (int i = 2; i <= menor; i++){
        if ((num1 % i == 0) && (num2 % i == 0)){
            // Los dos numeros son multiplos de i, por lo que no son coprimos
            return false;
        }
    }

    if (((num1 == 0) && (num2 != 1)) || ((num1 != 1) && (num2 == 0))){
        // Si alguno de los dos numeros es cero, y el otro es distinto de uno, entonces no son primos
        return false;
    }

    // No se encontraron multiplos entre si, por lo que los dos numeros son coprimos
    return true;
}

// Devuelve el menor de dos numeros
int menorNumero (int num1, int num2){
    if (num1 < num2){
        return num1;
    }else{
        return num2;
    }
}
int expo_modular(int base,int exponente,int modulo) {
	int res=1;
	double bin=binario(exponente);
	//cout << bin << endl;
	int decimales=0;
	//calculo decimales
	while (bin>1) {
		bin=bin/10;
		decimales++;
	}
	//Genero el array con el largo de decimales
	int bin_array[decimales];
	explode_num(bin_array,binario(exponente));
	int z=0;
	for (int x=decimales-1;x>=0;x--) {
		int exp_frac=bin_array[z]*pow(2,x);
		if (exp_frac>4) {
			for (int y=1;y<=exp_frac/4;y++) {
				int moduloIntermedio=fmod(pow(base,4),modulo);
				res=fmod(res*moduloIntermedio,modulo);
				//cout << res << "," <<moduloIntermedio << "-+" << endl;
			}
		} else {
			int moduloIntermedio=fmod(pow(base,exp_frac),modulo);
			res=fmod(res*moduloIntermedio,modulo);
			//cout << res << "," <<moduloIntermedio << "--" << endl;
		}
		z++;
	}
	return res;
}
double binario(int num) {
	int x=0;
	double res=0;
	while(num!=0) {
		res=res+(pow(10,x)*fmod(num,2));
		num=num/2;
		x++;
	}
	return res;
}
//Modifica el primer parámetro, colocando el numero num, decimal por decimal en un array
void explode_num(int* res,double num) {
	int x=0;
	while (num>1) {
		res[x]=fmod(num,10);
		num=num/10;
		x++;
	}
	int indice=x;
	int aux[indice];
	for (int x=0;x<indice;x++) {
		aux[x]=res[x];
	}
	int y=0;
	for(x=indice-1;x>=0;x--) {
		res[x]=aux[y];
		y++;
	}
}
