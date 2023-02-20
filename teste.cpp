#include <iostream>

using namespace std;

typedef unsigned char byte;	// 8 bits
#define numero_total_enderecos 64000
#define tamanho_palavra 4
#define barramento_dados byte[4]	// 32 bits
#define barramento_endereco byte[2]	// 16 bits

class Memoria{
	
	public:
		Memoria();
		byte memoria_teste[numero_total_enderecos][tamanho_palavra];	// 64000 endereços, 4 bytes cada
		void imprimir_endereco();
};

Memoria::Memoria(){
	
	// Toda a memoria começa com null (0)
	for(int i = 0; i < numero_total_enderecos; i++)
		for(int j = 0; j < tamanho_palavra; j++)
			memoria_teste[i][j] = 0;
			
	memoria_teste[0][0] = 109;
	memoria_teste[0][1] = 97;
	memoria_teste[0][2] = 101;
	memoria_teste[0][3] = 33;
}

void Memoria::imprimir_endereco(){
	
	for(int i = 0; i < tamanho_palavra; i++)
		cout << memoria_teste[0][i] << " ";
		
	cout << endl;
}

class Registadores : public Memoria{
	
	public:
		Registadores(Memoria *mem) : Memoria(*mem) {};
};

int main(){
	
	Memoria *mem = new Memoria;
	
	cout << sizeof(mem->memoria_teste) << endl;
	
	mem->imprimir_endereco();
	
	Registadores Rs(mem);
	
	delete mem;
	
	return 0;
}
