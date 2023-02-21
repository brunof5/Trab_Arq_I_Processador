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

// Todo registrador possui 32 bits (4 bytes)
// logo os 32 registradores ocupam 4 * 32 = 128 bytes
class Registadores{
	
	private:
		byte zero[4];	// valor constante 0
		byte v0[4], v1[4];	// variáveis para retorno de funcões e outras utilidades
		byte a0[4], a1[4], a2[4], a3[4];	// argumentos
		byte t0[4], t1[4], t2[4], t3[4], t4[4], t5[4], t6[4], t7[4], t8[4], t9[4];	// variáveis temporarias
		byte s0[4], s1[4], s2[4], s3[4], s4[4], s5[4], s6[4], s7[4];	// variáveis salvas
		byte *gp;	// globar pointer
		byte *sp;	// stack pointer
		byte *fp;	// frame pointer
		byte ra[4];		// retorno de funcao
		
	public:
		Registadores();
		~Registadores();
};

Registadores::Registadores(){
	
	gp = new byte[4];
	sp = new byte[4];
	fp = new byte[4];
}

Registadores::~Registadores(){
	
	delete[] gp;
	delete[] sp;
	delete[] fp;
}

int main(){
	
	Memoria mem;
	
	cout << sizeof(mem.memoria_teste) << endl;
	
	mem.imprimir_endereco();
	
	Registadores Rs;
	
	return 0;
}
