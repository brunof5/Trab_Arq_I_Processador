#include <iostream>
#include <fstream>
#include <bitset>
#include <exception>

using namespace std;

#define numero_total_enderecos 64000
#define tamanho_palavra 4
#define tamanho_instrucao 32
#define comeco_memoria_texto 256

//Implementação do segundo estágio de Processamento - ID
//Parte do Davi

struct instrucaoDecodificada {
    string opcode;
    int rs;// Acabei por não os utilizar (rs, rt,rd)
    int rt;
    int rd;
    string shamt;
    string funct;
};

class controle {
	
	friend class EX;
	
    private :
        bool Regdst;
        bool Regwrite;
        string Aluctrl;
        bool Memread;
        bool Memwrite;
        int  Aluop;
        bool Alusrc;
        bool MemtoReg;
        bool Branch;
        bool Jump;
        
        string offset;
        string address;
        
    public :
        void decodificar_instrucao(string instrucao_binaria);

};

void controle :: decodificar_instrucao (string instrucao_binaria){// Maioria não utilizada, apenas declarada.
    instrucaoDecodificada instrucao;


    //bitset<32> bits(instrucao_binaria); Não está mais sendo utilizada.

    //tipo R
    instrucao.opcode = instrucao_binaria.substr(0, 6);//extrai os 6 bits mais significativos

    /*
    instrucao.rs = instrucao_binaria.substr(6, 5);//extrai os 5 bits seguintes
    instrucao.rt = instrucao_binaria.substr(11, 5); //"              "
    instrucao.rd = instrucao_binaria.substr(16, 5); //"              "
    */

    instrucao.shamt = instrucao_binaria.substr(21, 5);//"            "
    instrucao.funct = instrucao_binaria.substr(26, 6);//extrai os 6 bits menos significativos
    //Tipo I
    offset = instrucao_binaria.substr(16, 16);//extrai os 16 bits menos significativos

    //Tipo J
    address = instrucao_binaria.substr(6, 26);//extrai os 26 bits menos significativos


    //O opcode dela é diferente, daí o if separado.
    //instrução mul R-Type difere do mult em relação aos bits, acredito eu 
    if (instrucao.opcode == "011100" and instrucao.funct=="000010"){ //mul instruction, R-Type
            Regdst = 1;
            Regwrite = 1;
            Alusrc = 0;
            Aluop = 10;
            MemtoReg = 0;
            Jump = 0;
            Branch = 0;
            Memwrite = 0;
            Memread = 0;
            Aluctrl = "mul";
    }

    // Instruções do tipo R
    if (instrucao.opcode == "000000"){
               if (instrucao.funct == "100000"){
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
                    Aluctrl = "add";

                }
                else if (instrucao.funct == "100010"){//sub --verificado
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 10;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "sub";
                }

                else if (instrucao.funct == "100100"){//and
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 00;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "and";
                }
                else if (instrucao.funct == "100111"){//mult -- 64 bits em 2 registradores de saída -- corrigida
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 11;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "mult";
                }

                else if (instrucao.funct == "100101"){//or --corrigido
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 01;
                    //nao sao utilizados
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "or";
                }

                else if (instrucao.funct == "100111"){ //nor --corrigido
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 11;
                    //nao sao utilizados
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "nor";
                }

                else if (instrucao.funct == "000000"){//sll -- verificar -- ok aparentemente
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 10;
                    MemtoReg = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Jump = 0;
                    Branch = 0;
                    Aluctrl = "sll";
                    
                  }

                else if (instrucao.funct == "000010"){//srl esta igual ao sub, mas acredito estar correto.
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 10;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "srl";
                }

                else if (instrucao.funct == "011010"){ //div -- ok
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 11;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "div";
                }

                else if (instrucao.funct == "101010"){//slt -- verificado
                    Regdst = 1;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 11;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "slt";
                }

                else if (instrucao.funct == "001000"){// jr
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 0; 
                    MemtoReg = 0;
                    Jump = 1;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "jr";
                }

                else {
                    cout<<"ERRO - Instrucao nao identificada"<<endl;
                }
        }


    //Instruções do tipo J
    //j
    else if (instrucao.opcode == "000010"){
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 0;
                    MemtoReg = 0;
                    Jump = 1;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "j";

    }//jal
    else if (instrucao.opcode == "000011"){
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 0;
                    Aluop = 0;
                    MemtoReg = 0;
                    Jump = 1;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "jal";
    }

    //Instruções do tipo I
    else
    {
         
                if (instrucao.opcode == "001000" ){ //addi
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 10;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "addi";
                }

                else if (instrucao.opcode == "100011"){ //lw
                    Regdst = 0;
                    Regwrite = 1;
                    Alusrc = 1;
                    Aluop = 00;
                    MemtoReg = 1;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 0;
                    Memread = 1;
                    Aluctrl = "lw";
                }

                else if (instrucao.opcode == "101011") { //sw
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 1;
                    Aluop = 00;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 0;
                    Memwrite = 1;
                    Memread = 0;
                    Aluctrl = "sw";
                }

                else if (instrucao.opcode == "000100"){ //beq
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 01;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 1;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "beq";
                }

                else if (instrucao.opcode == "000101"){ //bne
                    Regdst = 0;
                    Regwrite = 0;
                    Alusrc = 0;
                    Aluop = 01;
                    MemtoReg = 0;
                    Jump = 0;
                    Branch = 1;
                    Memwrite = 0;
                    Memread = 0;
                    Aluctrl = "bne";
                }

                else {
                    cout<<" ERRO - Instrucao nao identificada"<<endl;
                }
        }

}

class EX{
	
private:
	
	int rs = 10; //supondo que o registrador1 seja igual a 10
	int rt = 15; //supondo que o registrador2 seja igual a 15
	int rd = 0, HI = 0, LO = 0, imm = 0, sa = 0, r31 = 0;
	
	void instrucoes_de_desvio (bitset<tamanho_instrucao> instrucao_atual, controle instrucao);
	void instrucoes_aritmeticas(bitset<tamanho_instrucao> instrucao_atual, controle instrucao);
		
public:
	
	unsigned int PC;
	bool ALUzero = 0;
	void ALU (bitset<tamanho_instrucao> instrucao_atual);
		
};
	
void EX::ALU(bitset<tamanho_instrucao> instrucao_atual){
			
	controle instrucao;
			
	string aux = instrucao_atual.to_string();
	instrucao.decodificar_instrucao(aux);
			
	if((instrucao.Branch == 0) and (instrucao.Jump == 0) and (instrucao.Memwrite == 0) and (instrucao.Memread == 0)){
		instrucoes_aritmeticas(instrucao_atual, instrucao); //Realiza os calculos aritmeticos
	}
	else if ((instrucao.Branch == 1) or (instrucao.Jump == 1)){
		instrucoes_de_desvio(instrucao_atual, instrucao); //Realiza os calculos dos desvios
	}
			
}

void EX::instrucoes_aritmeticas(bitset<tamanho_instrucao> instrucao_atual, controle instrucao){
			
	//add
	if (instrucao.Aluctrl == "add"){ 
		rd = rs + rt;
		cout<<"É uma instrução de add"<<endl;
	}
	//sub
	if (instrucao.Aluctrl == "sub"){
		rd = rs - rt;
		cout<<"É uma instrução de sub"<<endl;
	}
	//addi
	if (instrucao.Aluctrl == "addi"){
		rt = rs + imm;
		cout<<"É uma instrução de addi"<<endl;
	}
	//and
	if (instrucao.Aluctrl == "and"){
		rd = (rs & rt);
		cout<<"É uma instrução de and"<<endl;
	}
	//or
	if (instrucao.Aluctrl == "or"){
		rd = (rs | rt);
		cout<<"É uma instrução de or"<<endl;
	}
	//nor
	if (instrucao.Aluctrl == "nor"){
		rd = ~(rs | rt);
		cout<<"É uma instrução de nor"<<endl;
	}
	//mult
	if (instrucao.Aluctrl == "mult"){
		HI = rs * rt;
		LO = HI;
		cout<<"É uma instrução de mult"<<endl;
	}
	//sll
	if (instrucao.Aluctrl == "sll"){
		rd = rt << sa;
		cout<<"É uma instrução de sll"<<endl;
	}
	//srl
	if (instrucao.Aluctrl == "srl"){
		rd = rt >> sa;
		cout<<"É uma instrução de srl"<<endl;
	}
	//mul
	if (instrucao.Aluctrl == "mul"){
		rd = rs * rt;
		cout<<"É uma instrução de mul"<<endl;
	}
	//div
	if (instrucao.Aluctrl == "div"){
		HI = rs % rt; 
		LO = rs / rt;
		cout<<"É uma instrução de div"<<endl;
	}
	//slt
	if (instrucao.Aluctrl == "slt"){
		rd = rs < rt;
		cout<<"É uma instrução de slt"<<endl;
	}
			
	//Verifica se algum resultado corresponde a 0 para acionar a flag.
			
	if(((rd == 0) or (rt == 0)) and ((HI == 0) and (LO == 0))){ //Tenho que checar com calma
		ALUzero = 1;
	}
			
	cout<<endl;
			
}

void EX::instrucoes_de_desvio(bitset<tamanho_instrucao> instrucao_atual, controle instrucao){
			
	int offset = bitset<16>(instrucao.offset).to_ulong();
	int target = bitset<26>(instrucao.address).to_ulong();
			
	//beq
	if (instrucao.Aluctrl == "beq"){ 
		if(rs == rt){
			PC += offset * 4;
		}
		cout<<"É uma instrução de beq"<<endl;
	}
	//bne
	if (instrucao.Aluctrl == "bne"){
		if(rs != rt){ 
			PC += offset * 4;
		}
		cout<<"É uma instrução de bne"<<endl;
	}
	//j
	if (instrucao.Aluctrl == "j"){
				
		bitset<32> num_binario(PC);
		bitset<4> pc_upper_binario(num_binario.to_string().substr(0,4));
		int pc_upper = (int)pc_upper_binario.to_ulong();
				
		PC = pc_upper + (target * 4);
		cout<<"É uma instrução de j"<<endl;
				
	}
	//jal
	if (instrucao.Aluctrl == "jal"){
		r31 = PC; 
		PC = target * 4;
		cout<<"É uma instrução de jal"<<endl;
	}
	//jr
	if (instrucao.Aluctrl == "jr"){
		PC = rs;
		cout<<"É uma instrução de jr"<<endl;
	}
			
	cout <<"Desvio em decimal: "<< PC << endl << endl;
			
}

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
		bitset<tamanho_instrucao> retornar_instrucao(EX *e1);
};

IF::IF(Memoria *mem) : Memoria(*mem){
	
	PC = comeco_memoria_texto;
}

bitset<tamanho_instrucao> IF::retornar_instrucao(EX *e1){
	
	bitset<tamanho_instrucao> instrucao_atual;
	
	for(int i = 0; i < tamanho_instrucao; i++)
		instrucao_atual[i] = memoria[PC + i];
		
	e1 -> PC = PC;	
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
			
			for(int i = 0; i < 21; i++){
				
				EX e1;
				bitset<tamanho_instrucao> aux = estagio_IF.retornar_instrucao(&e1);
				
				cout << aux << endl;
				
				e1.ALU(aux);
			}
			
		}
		
		delete mem;
		
		arquivo_entrada.close();
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
