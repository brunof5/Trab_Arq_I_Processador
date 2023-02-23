#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

#define numero_total_enderecos 64000
#define tamanho_palavra 4
#define comeco_memoria_texto 256
#define tamanho_instrucao 32

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


//Implementação do segundo estágio de Processamento - ID
//Parte do Davi**


//notes
/*The if statement for the mul instruction checks instrucao.funct against a character literal ('000010')
instead of an integer literal (0x10). This will not match the correct value, as the character literal
represents the ASCII value for the character '0' rather than the binary value.*/

/*The comparisons in the if statements inside the switch statement use character literals (e.g. '011100')
instead of integer literals (e.g. 0x1C). This will not work as expected, as character literals represent
individual characters rather than binary values. You should replace them with integer literals*/
/*Está com alguns problemas para identificar o cases, diz duplicate cases, mas não há duplicatas*/
#include <iostream>
#include <bitset>
using namespace std;
struct instrucaoDecodificada {
    int opcode;
    int rs;
    int rt;
    int rd;
    int shamt;
    int funct;
    int offset;
    int address;
};
class controle {
    private :
        bool Regdst;
        bool Regwrite;
        bool Aluctrl = false;
        bool Memread;
        bool Memwrite;
        int  Aluop;
        bool Alusrc;
        bool MemtoReg;
        bool Branch;
        bool Jump;
    public :
        void decodificar_instrucao(string instrucao_binaria);

};
void controle :: decodificar_instrucao (string instrucao_binaria){// Maioria não utilizada, apenas declarada.
    instrucaoDecodificada instrucao;


    bitset<32> bits(instrucao_binaria);

    //tipo R
    instrucao.opcode = bits.to_ulong() >> 26; // extrai os 6 bits mais significativos
    instrucao.rs = (bits.to_ulong() >> 21) & 0x1F; //extrai os 5 bits seguintes
    instrucao.rt = (bits.to_ulong() >> 16) & 0x1F; //extrai os 5 bits seguintes
    instrucao.rd = (bits.to_ulong() >> 11) & 0x1F;//proximos 5 bits da inst binaria, correspondem ao rd
    instrucao.shamt = (bits.to_ulong() >> 6) & 0x1F;
    instrucao.funct = bits.to_ulong() & 0x3F;//extrai os 6 bits menos significativos

    //Para instruções do tipo I, o campo offset deve ser extraído. Para instruções do tipo J, o campo address deve ser extraído

    //tipo I
    instrucao.offset = bits.to_ulong() & 0xFFFF;// extrai os 16 bits menos significativos

    //tipo J
    instrucao.address = bits.to_ulong() & 0x3FFFFFF;// extrai os 26 bits menos significativos

    //instrução mul R-Type
    if (instrucao.opcode == '011100' and instrucao.funct=='000010'){ //mul instruction, R-Type
            Regdst = 1;
            Regwrite = 1;
            Alusrc = 0;
            Aluop = 11;
            MemtoReg = 0;
            Jump = 0;
            Branch = 0;
            Memwrite = 0;
            Memread = 0;
    }

    // Instruções do tipo R
    if (instrucao.opcode == 000000){
        switch(instrucao.funct)
        {
                case '100000':
                    //return add
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 10;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;

                case '100010'://sub
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 10;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;

                case '100100'://and
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 00;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;

                case '100101' ://or
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 01;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;

                case '100111': //nor
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 01;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;
/*
                case '000000'://sll
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 10;
                    MemtoReg = 0;Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 01;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;

                case '000010'://srl
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 10;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;
*/
                case '011010': //div
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 11;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;
/*
                case '101010'://slt
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 11;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;
*/
                case '001000':// jr
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 00; // não achei o valor, acho que não importa.
                    MemtoReg = 0;
                    Jump = 1;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;

                default : //sll 000000
                    cout<<"ERRO - Instrucao nao identificada"<<endl;
        }
    }

    //Instruções do tipo J
    //jal e j
    else if (instrucao.opcode == '000010' or instrucao.opcode == '000011'){
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 11;
                    MemtoReg = 0;
                    Jump = 1;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;

    }

    //Instruções do tipo I
    else
    {
        switch (instrucao.opcode){ //Tipo I
                case '001000': //addi
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 10;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    break;
                case '100011': //lw
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 00;
                    MemtoReg = 1;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 1;
                    break;

                case '101011': //sw
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 1;
                    Aluop = 00;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 1;
                    Memread = 0;
                    break;

                case '000100': //beq
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 01;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 1;
                    Memwrite = 0;
                    Memread = 0;
                    break;

                case '000101': //bne
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 01;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 1;
                    Memwrite = 0;
                    Memread = 0;
                    break;
/*
                case '000101': // slt + beq 000000 000100 / Não entendi muito bem essa instrução no anotacoes.txt?
                    //bge
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 11;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 1;
                    Memwrite = 0;
                    Memread = 0;
                    break;
*/
                default :
                    cout<<" ERRO - Instrucao nao identificada"<<endl;
        }
    }

}

int main(){
	
	ifstream arquivo_entrada("instrucoes_binario.bin", ios::binary);
	
	if(arquivo_entrada){
		
		Memoria *mem = new Memoria(arquivo_entrada);
		
		IF estagio_I(mem);
		
		mem->depuracao();
		
		cout << "#####################" << endl;
		
		for(int i = 0; i < 21; i++)
			cout << estagio_I.retornar_instrucao() << endl;
			
		delete mem;
		
		arquivo_entrada.close();
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
