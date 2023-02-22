#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

class EX{

private:
	int opcode_referencia1;
	int opcode_referencia2;
	
	void instrucoes_aritmeticas(bitset<32> instrucao_atual){
		
		string aux = instrucao_atual.to_string(); // converte para string para pegar os valores mais facilmente
		string opcode_referencia1 = aux.substr(0, 6); // pega os primeiros 6 caracteres
		string opcode_referencia2 = aux.substr(26); // pega os últimos 6 caracteres
		
		int rs = 10; //supondo que o registrador1 seja igual a 10
		int rt = 15; //supondo que o registrador2 seja igual a 15
		int rd = 0, HI = 0, LO = 0, imm = 0, sa = 0;
		
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "100000"){ // add
			rd = rs + rt;
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "100010"){ // sub
			rd = rs - rt;
		}
		if (opcode_referencia1 == "001000"){ // addi
			rt = rs + imm;
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "100100"){ // and
			rd = (rs & rt);
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "100101"){ // or
			rd = (rs | rt);
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "100111"){ // nor
			rd = ~(rs | rt);
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "011000"){ // mult
			HI = rs * rt;
			LO = HI;
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "000000"){ // sll
			rd = rt << sa;
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "000010"){ // srl
			rd = rt >> sa;
		}
		if (opcode_referencia1 == "011100" and opcode_referencia2 == "000010"){ // mul
			rd = rs * rt;
		}
		if (opcode_referencia1 == "000000" and opcode_referencia2 == "011010"){ // div
			HI = rs % rt; 
			LO = rs / rt;
		}
		
		cout<<endl;
		cout<<"Valor do Resgistrador de Destino: "<<rd<<endl;
		cout<<"Valor do Hi: "<<HI<<endl;
		cout<<"Valor do Lo: "<<LO<<endl;
		cout<<endl;
		
	}
	
public:

	void ALU (bitset<32> instrucao_atual){
		instrucoes_aritmeticas(instrucao_atual); //Realiza os calculos aritmeticos
	}
		
};


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
	
	EX instrucao1;
	
	ifstream arquivo_entrada("instrucoes_binario.bin", ios::binary);
	
	if(arquivo_entrada){
		
		bitset<32> instrucao_atual = retornar_instrucao(arquivo_entrada, 0);
		
		cout << instrucao_atual << endl;
		
		instrucao1.ALU(instrucao_atual);
		
		arquivo_entrada.close();
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
