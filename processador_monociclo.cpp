#include <iostream>
#include <fstream>
#include <bitset>
#include <exception>

using namespace std;

#define numero_total_enderecos 64000
#define tamanho_palavra 4
#define tamanho_instrucao 32
#define comeco_memoria_texto 256

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

class Memoria{
	
	protected:
		// 64000 endereços * 4 bytes cada = 256000
		// 0 eh a posicao inicial da memoria e 255999 eh a posicao final da memoria
		bitset<numero_total_enderecos*tamanho_palavra> memoria;
		
	public:
		Memoria(ifstream& arquivo_entrada);
		void depuracao();
};

Memoria::Memoria(ifstream& arquivo_entrada){
	
	arquivo_entrada.seekg(0, arquivo_entrada.end);
	int tamanho_arquivo = arquivo_entrada.tellg();
	
	if(tamanho_arquivo > 3200)
		throw runtime_error("ERRO NUMERO DE INSTRUCOES - capacidade de instrucoes passou do esperado!");
		
	int quantidade_instrucoes = tamanho_arquivo / tamanho_instrucao;
	arquivo_entrada.seekg(0, arquivo_entrada.beg);
	
	char instrucao_atual_char[tamanho_instrucao];
	bool valor_bit = false;
	bitset<tamanho_instrucao> instrucao_atual;
	int aux = comeco_memoria_texto;
	
	for(int i = 0; i < quantidade_instrucoes; i++){
		
		for(unsigned int j = 0, k = 31; j < tamanho_instrucao; j++, k--){
			
			arquivo_entrada.read((char*)(&instrucao_atual_char[j]), sizeof(char));
			
			if(instrucao_atual_char[j] == '0')
				valor_bit = false;
				
			if(instrucao_atual_char[j] == '1')
				valor_bit = true;
				
			memoria.set(aux + k, valor_bit);
		}
		
		cout << instrucao_atual_char << endl;
		
		aux += tamanho_instrucao;
	}
}

void Memoria::depuracao(){
	
	bitset<tamanho_instrucao> instrucao_atual;
	int aux = comeco_memoria_texto;
	
	for(int j = 0; j < 21; j++){
		
		cout << aux << endl;
		
		for(int i = 0; i < tamanho_instrucao; i++)
			instrucao_atual[i] = memoria[aux + i];
			
		cout << instrucao_atual << endl;
		
		aux += tamanho_instrucao;
	}
}

class IF : protected Memoria{
	
	protected:
		unsigned int PC;
		
	public:
		IF(Memoria *mem);
		bitset<tamanho_instrucao> retornar_instrucao();
};

IF::IF(Memoria *mem) : Memoria(*mem){
	
	PC = comeco_memoria_texto;
}

bitset<tamanho_instrucao> IF::retornar_instrucao(){
	
	bitset<tamanho_instrucao> instrucao_atual;
	
	for(int i = 0; i < tamanho_instrucao; i++)
		instrucao_atual[i] = memoria[PC + i];
		
	PC += tamanho_instrucao;
	
	return instrucao_atual;
}

int main(){
	
	ifstream arquivo_entrada("instrucoes_binario.bin", ios::binary);
	
	if(arquivo_entrada){
		
		Memoria *mem = NULL;
		bool integridade = true;
		
		try{
			
			mem = new Memoria(arquivo_entrada);
		}
		catch(exception& e){
			
			cout << e.what();
			integridade = false;
		}
		
		if(integridade){
			
			IF estagio_IF(mem);
			
			mem->depuracao();
			
			cout << "#####################" << endl;
			
			for(int i = 0; i < 21; i++)
				cout << estagio_IF.retornar_instrucao() << endl;
		}
		
		delete mem;
		
		arquivo_entrada.close();
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
