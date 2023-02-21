#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

bitset<32> retornar_instrucao(ifstream& arquivo_entrada, int posicao_da_instrucao){
	
	char instrucao_atual_char[32];
	bool valor_bit = false;
	bitset<32> instrucao_atual;
	
	arquivo_entrada.seekg(32 * posicao_da_instrucao, arquivo_entrada.beg);
	
	// leitura de 32 bytes (32*char) para 32 bits (bitset)
	for(unsigned int i = 0, j = 31; i < 32; i++, j--){
		
		arquivo_entrada.read((char*)(&instrucao_atual_char[i]), sizeof(char));
		
		if(instrucao_atual_char[i] == '0')
			valor_bit = false;
			
		if(instrucao_atual_char[i] == '1')
			valor_bit = true;
			
		instrucao_atual.set(j, valor_bit);
	}
	
	return instrucao_atual;
}

int main(){
	
	ifstream arquivo_entrada("instrucoes_binario.bin", ios::binary);
	
	if(arquivo_entrada){
		
		bitset<32> instrucao_atual = retornar_instrucao(arquivo_entrada, 0);
		
		cout << instrucao_atual << endl;
		
		arquivo_entrada.close();
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
