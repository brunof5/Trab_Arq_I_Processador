#include <iostream>
#include <fstream>
#include <bitset>
#include <exception>
#include <climits>

using namespace std;

#define numero_total_enderecos 64000
#define tamanho_palavra 4
#define tamanho_instrucao 32
#define comeco_memoria_texto 256
#define comeco_memoria_dados 3456
#define final_memoria_dados 255999

class Memoria{
	
	friend class IF;
	friend class MEM;
	
	private:
		// 64000 endereços * 4 bytes cada = 256000
		// 0 eh a posicao inicial da memoria e 255999 eh a posicao final da memoria
		bitset<numero_total_enderecos*tamanho_palavra> memoria;
		
	public:
		Memoria(ifstream& arquivo_entrada);
		void depuracao_memoria();
};

Memoria::Memoria(ifstream& arquivo_entrada){
	
	arquivo_entrada.seekg(0, arquivo_entrada.end);
	int tamanho_arquivo = arquivo_entrada.tellg();
	
	cout << tamanho_arquivo << endl;
	
	int quantidade_instrucoes = tamanho_arquivo / tamanho_instrucao;
	
	if(tamanho_arquivo > comeco_memoria_dados - comeco_memoria_texto)
		throw runtime_error("ERRO NUMERO DE INSTRUCOES - capacidade de instrucoes passou do esperado!");
		
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

void Memoria::depuracao_memoria(){
	
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

// #######################################

class IF{
	
	friend class Controle;
	friend class EX;
	
	private:
		unsigned int PC;
		bitset<tamanho_instrucao> instrucao_atual;
		
	public:
		IF(Memoria *mem);
		bool verificar_instrucao()
		{
			if(instrucao_atual.to_string() == "00000000000000000000000000000000")
				return false;
				
			else
				return true;
		};
		bitset<tamanho_instrucao> get_instrucao_atual() { return instrucao_atual; };
		void trocar_instrucao(Memoria *mem);
		void mostrar_valor_atual_PC() { cout << "Valor atual do PC: " << PC << endl; };
};

IF::IF(Memoria *mem){
	
	PC = comeco_memoria_texto;
	
	for(int i = 0; i < tamanho_instrucao; i++)
		instrucao_atual[i] = mem->memoria[PC + i];
		
	PC += tamanho_instrucao;
}

void IF::trocar_instrucao(Memoria *mem){
	
	for(int i = 0; i < tamanho_instrucao; i++)
		instrucao_atual[i] = mem->memoria[PC + i];
		
	PC += tamanho_instrucao;
	
	cout << instrucao_atual << endl;
}

// #######################################

struct instrucaoDecodificada{
	
	string opcode;
	unsigned long rs = 0;
	unsigned long rt = 0;
	unsigned long rd = 0;
	string shamt;
	string funct;
	string offset_address;
	string target;
};

class Controle{
	
	friend class EX;
	friend class Memoria;
	friend class MEM;
	friend class WR;
	
	private:
		bool Regdst;
		bool Regwrite;
		string Aluctrl;
		bool Memread;
		bool Memwrite;
		int Aluop;
		bool Alusrc;
		bool MemtoReg;
		bool Branch;
		bool Jump;
		instrucaoDecodificada instrucao;
		
	public:
		Controle() { reset(); };
		void reset();
		void depuracao_controle();
		void decodificar_instrucao(Memoria *mem, IF *estagio_IF);
};

void Controle::reset(){
	
	Regdst = 0;
	Regwrite = 0;
	Aluctrl = "invalido";
	Memread = 0;
	Memwrite = 0;
	Aluop = 0;
	Alusrc = 0;
	MemtoReg = 0;
	Branch = 0;
	Jump = 0;
	instrucao.opcode = "invalido";
	instrucao.rs = -1;
	instrucao.rt = -1;
	instrucao.rd = -1;
	instrucao.shamt = "invalido";
	instrucao.funct = "invalido";
	instrucao.offset_address = "invalido";
	instrucao.target = "invalido";
}

void Controle::depuracao_controle(){
	
	cout << "Regdst: " << Regdst << endl;
	cout << "Regwrite: " << Regwrite << endl;
	cout << "Alusrc: " << Alusrc << endl;
	cout << "Memread: " << Memread << endl;
	cout << "Memwrite: " << Memwrite << endl;
	cout << "MemtoReg: " << MemtoReg << endl;
	cout << "Jump: " << Jump << endl;
	cout << "Branch: " << Branch << endl;
	cout << "Aluop: " << Aluop << endl;
	
	cout << "Aluctrl: " << Aluctrl << endl;
	cout << "opcode: " << instrucao.opcode << endl;
	cout << "rs (endereco): " << instrucao.rs << endl;
	cout << "rt (endereco): " << instrucao.rt << endl;
	cout << "rd (endereco): " << instrucao.rd << endl;
	cout << "shamt: " << instrucao.shamt << endl;
	cout << "funct: " << instrucao.funct << endl;
	cout << "offset ou address: " << instrucao.offset_address << endl;
	cout << "target: " << instrucao.target << endl;
	
}

void Controle::decodificar_instrucao(Memoria *mem, IF *estagio_IF){// Maioria não utilizada, apenas declarada.
	
	bitset<tamanho_instrucao> instrucao_binaria = estagio_IF->get_instrucao_atual();
	string instrucao_string = instrucao_binaria.to_string();
	
	string texto_binario;
	
	//tipo R
	instrucao.opcode = instrucao_string.substr(0, 6);//extrai os 6 bits mais significativos (opcode)
	
	texto_binario = instrucao_string.substr(6, 5);//extrai os 5 bits do primeiro operando
	bitset<5> aux_rs (texto_binario);
	instrucao.rs = aux_rs.to_ulong();
	
	texto_binario = instrucao_string.substr(11, 5);//extrai os 5 bits do segundo operando
	bitset<5> aux_rt (texto_binario);
	instrucao.rt = aux_rt.to_ulong();
	
	texto_binario = instrucao_string.substr(16, 5);//extrai os 5 bits do terceiro operando
	bitset<5> aux_rd (texto_binario);
	instrucao.rd = aux_rd.to_ulong();
	
	instrucao.shamt = instrucao_string.substr(21, 5);//extrai os 5 bits do shamt
	instrucao.funct = instrucao_string.substr(26, 6);//extrai os 6 bits menos significativos (funct)
	
	//Tipo I
	instrucao.offset_address = instrucao_string.substr(16, 16);//extrai os 16 bits menos significativos
	
	//Tipo J
	instrucao.target = instrucao_string.substr(6, 26);//extrai os 26 bits menos significativos
	
	//O opcode dela é diferente, daí o if separado.
	//instrução mul R-Type difere do mult em relação aos bits, acredito eu 
	if(instrucao.opcode == "011100" and instrucao.funct=="000010"){ //mul instruction, R-Type
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
	if(instrucao.opcode == "000000"){
		
		if(instrucao.funct == "100000"){
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
		
		else if(instrucao.funct == "100010"){//sub --verificado
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
		
		else if(instrucao.funct == "100100"){//and
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
		
		else if(instrucao.funct == "011000"){//mult -- 64 bits em 2 registradores de saída -- corrigida
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
		
		else if(instrucao.funct == "100101"){//or --corrigido
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
		
		else if(instrucao.funct == "100111"){ //nor --corrigido
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
		
		else if(instrucao.funct == "000000"){//sll -- verificar -- ok aparentemente
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
		
		else if(instrucao.funct == "000010"){//srl esta igual ao sub, mas acredito estar correto.
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
		
		else if(instrucao.funct == "011010"){ //div -- ok
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
		
		else if(instrucao.funct == "101010"){//slt -- verificado
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
		
		else if(instrucao.funct == "001000"){// jr
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
		
		else{
			cout<<"ERRO - Instrucao nao identificada"<<endl;
		}
	}
	
	//Instruções do tipo J
	//j
	else if(instrucao.opcode == "000010"){
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
	}
	
	//jal
	else if(instrucao.opcode == "000011"){
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
	else{
		if(instrucao.opcode == "001000" ){ //addi
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
		
		else if(instrucao.opcode == "100011"){ //lw
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
		
		else if(instrucao.opcode == "101011") { //sw
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
		
		else if(instrucao.opcode == "000100"){ //beq
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
		
		else if(instrucao.opcode == "000101"){ //bne
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
		
		else{
			cout<<" ERRO - Instrucao nao identificada"<<endl;
		}
	}
}

// #######################################

class Registradores{
	
	friend class EX;
	friend class MEM;
	
	private:
		bitset<tamanho_instrucao> zero; // valor constante de zero
		bitset<tamanho_instrucao> at; // reservado pelo compilador
		bitset<tamanho_instrucao> v0, v1; // variáveis para retorno de funções e outras utilidades
		bitset<tamanho_instrucao> a0, a1, a2, a3; // argumentos
		bitset<tamanho_instrucao> t0, t1, t2, t3, t4, t5, t6, t7; // variáveis temporárias
		bitset<tamanho_instrucao> s0, s1, s2, s3, s4, s5, s6, s7; // variáveis salvas
		bitset<tamanho_instrucao> t8, t9; // variáveis usadas na adição dos temporários
		bitset<tamanho_instrucao> k0, k1; // reservado pelo OS
		bitset<tamanho_instrucao> gp; // global pointer
		bitset<tamanho_instrucao> sp; //stack pointer
		bitset<tamanho_instrucao> fp; //frame pointer
		bitset<tamanho_instrucao> ra; // retorno de função
		bitset<tamanho_instrucao> HIGH;
		bitset<tamanho_instrucao> LOW;
		
	public:
		Registradores();
		bitset<tamanho_instrucao> retornar_registrador(int numero_registrador);
		void setar_registrador(long dado_para_escrever, int numero_registrador);
		void depuracao_registradores();
};

Registradores::Registradores(){
	
	s0.set(2, 1);
	s1.set(3, 1);
}

bitset<tamanho_instrucao> Registradores::retornar_registrador(int numero_registrador){
	
	cout << "retornar reg: " << numero_registrador << endl;
	
	if(numero_registrador == 0) return zero;
	if(numero_registrador == 1) return at;
	if(numero_registrador == 2) return v0;
	if(numero_registrador == 3) return v1;
	if(numero_registrador == 4) return a0;
	if(numero_registrador == 5) return a1;
	if(numero_registrador == 6) return a2;
	if(numero_registrador == 7) return a3;
	if(numero_registrador == 8) return t0;
	if(numero_registrador == 9) return t1;
	if(numero_registrador == 10) return t2;
	if(numero_registrador == 11) return t3;
	if(numero_registrador == 12) return t4;
	if(numero_registrador == 13) return t5;
	if(numero_registrador == 14) return t6;
	if(numero_registrador == 15) return t7;
	if(numero_registrador == 16) return s0;
	if(numero_registrador == 17) return s1;
	if(numero_registrador == 18) return s2;
	if(numero_registrador == 19) return s3;
	if(numero_registrador == 20) return s4;
	if(numero_registrador == 21) return s5;
	if(numero_registrador == 22) return s6;
	if(numero_registrador == 23) return s7;
	if(numero_registrador == 24) return t8;
	if(numero_registrador == 25) return t9;
	if(numero_registrador == 26) return k0;
	if(numero_registrador == 27) return k1;
	if(numero_registrador == 28) return gp;
	if(numero_registrador == 29) return sp;
	if(numero_registrador == 30) return fp;
	if(numero_registrador == 31) return ra;
	
	throw runtime_error("ERRO LEITURA DO REGISTRADOR - o registrador lido nao pertence ao conjunto de registradores existentes!");
}

void Registradores::setar_registrador(long dado_para_escrever, int numero_registrador){
	
	cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
	
	cout << numero_registrador << endl;
	cout << dado_para_escrever << endl;
	
	bitset<tamanho_instrucao> valor_para_modificar (dado_para_escrever);
	
	switch(numero_registrador){
		
		case 0:
			cout << "Tentativa de setar algum valor no registrador zero - NEGADO!" << endl;
			break;
			
		case 1:
			cout << "Tentativa de setar algum valor no registrador at - NEGADO!" << endl;
			break;
			
		case 2:
			v0 = valor_para_modificar;
			break;
			
		case 3:
			v1 = valor_para_modificar;
			break;
			
		case 4:
			a0 = valor_para_modificar;
			break;
			
		case 5:
			a1 = valor_para_modificar;
			break;
			
		case 6:
			a2 = valor_para_modificar;
			break;
			
		case 7:
			a3 = valor_para_modificar;
			break;
			
		case 8:
			t0 = valor_para_modificar;
			break;
			
		case 9:
			t1 = valor_para_modificar;
			break;
			
		case 10:
			t2 = valor_para_modificar;
			break;
			
		case 11:
			t3 = valor_para_modificar;
			break;
			
		case 12:
			t4 = valor_para_modificar;
			break;
			
		case 13:
			t5 = valor_para_modificar;
			break;
			
		case 14:
			t6 = valor_para_modificar;
			break;
			
		case 15:
			t7 = valor_para_modificar;
			break;
			
		case 16:
			s0 = valor_para_modificar;
			break;
			
		case 17:
			s1 = valor_para_modificar;
			break;
			
		case 18:
			s2 = valor_para_modificar;
			break;
			
		case 19:
			s3 = valor_para_modificar;
			break;
			
		case 20:
			s4 = valor_para_modificar;
			break;
			
		case 21:
			s5 = valor_para_modificar;
			break;
			
		case 22:
			s6 = valor_para_modificar;
			break;
			
		case 23:
			s7 = valor_para_modificar;
			break;
			
		case 24:
			t8 = valor_para_modificar;
			break;
			
		case 25:
			t9 = valor_para_modificar;
			break;
			
		case 26:
			cout << "Tentativa de setar algum valor no registrador reservado ao OS - NEGADO!" << endl;
			break;
			
		case 27:
			cout << "Tentativa de setar algum valor no registrador reservado ao OS - NEGADO!" << endl;
			break;
			
		case 28:
			gp = valor_para_modificar;
			break;
			
		case 29:
			sp = valor_para_modificar;
			break;
			
		case 30:
			fp = valor_para_modificar;
			break;
			
		case 31:
			ra = valor_para_modificar;
			break;
	}
}

void Registradores::depuracao_registradores(){
	
	cout << "Valor de 0/zero: " << zero.to_ulong() << endl;
	cout << "Valor de 1/at: " << at.to_ulong() << endl;
	cout << "Valor de 2/v0: " << v0.to_ulong() << endl;
	cout << "Valor de 3/v1: " << v1.to_ulong() << endl;
	cout << "Valor de 4/a0: " << a0.to_ulong() << endl;
	cout << "Valor de 5/a1: " << a1.to_ulong() << endl;
	cout << "Valor de 6/a2: " << a2.to_ulong() << endl;
	cout << "Valor de 7/a3: " << a3.to_ulong() << endl;
	cout << "Valor de 8/t0: " << t0.to_ulong() << endl;
	cout << "Valor de 9/t1: " << t1.to_ulong() << endl;
	cout << "Valor de 10/t2: " << t2.to_ulong() << endl;
	cout << "Valor de 11/t3: " << t3.to_ulong() << endl;
	cout << "Valor de 12/t4: " << t4.to_ulong() << endl;
	cout << "Valor de 13/t5: " << t5.to_ulong() << endl;
	cout << "Valor de 14/t6: " << t6.to_ulong() << endl;
	cout << "Valor de 15/t7: " << t7.to_ulong() << endl;
	cout << "Valor de 16/s0: " << s0.to_ulong() << endl;
	cout << "Valor de 17/s1: " << s1.to_ulong() << endl;
	cout << "Valor de 18/s2: " << s2.to_ulong() << endl;
	cout << "Valor de 19/s3: " << s3.to_ulong() << endl;
	cout << "Valor de 20/s4: " << s4.to_ulong() << endl;
	cout << "Valor de 21/s5: " << s5.to_ulong() << endl;
	cout << "Valor de 22/s6: " << s6.to_ulong() << endl;
	cout << "Valor de 23/s7: " << s7.to_ulong() << endl;
	cout << "Valor de 24/t8: " << t8.to_ulong() << endl;
	cout << "Valor de 25/t9: " << t9.to_ulong() << endl;
	cout << "Valor de 26/k0: " << k0.to_ulong() << endl;
	cout << "Valor de 27/k1: " << k1.to_ulong() << endl;
	cout << "Valor de 28/gp: " << gp.to_ulong() << endl;
	cout << "Valor de 29/sp: " << sp.to_ulong() << endl;
	cout << "Valor de 30/fp: " << fp.to_ulong() << endl;
	cout << "Valor de 31/ra: " << ra.to_ulong() << endl;
}

// #######################################

class EX{
	
	friend class MEM;
	friend class WR;
	
	private:
		int HI = 0, LO = 0, sa = 0;
		unsigned long valor_novo_ra = 0;
		unsigned long valor_novo_PC = 0;
		long resultado_ALU = 0;
		bool ALUzero = 0;
		bool Overflow = 0;
		unsigned long resultado_endereco = 0;
		void instrucoes_aritmeticas(Controle *estagio_controle, Registradores *reg);
		void instrucoes_de_desvio(Controle *estagio_controle, IF *estagio_IF, Registradores *reg);
		void instrucoes_de_memoria(Controle *estagio_controle, Registradores *reg);
		void verifica_overflow(long result, long a, long b, string operacao);
		
	public:
		void ALU(Controle *estagio_controle, IF *estagio_IF, Registradores *reg);
		void mostrar_flags_desvio();
};

void EX::ALU(Controle *estagio_controle, IF *estagio_IF, Registradores *reg){
	
	if((estagio_controle->Branch == 0) and (estagio_controle->Jump == 0) and (estagio_controle->Memwrite == 0) and (estagio_controle->Memread == 0)){
		instrucoes_aritmeticas(estagio_controle, reg); //Realiza os calculos aritmeticos
	}
	
	else if((estagio_controle->Branch == 1) or (estagio_controle->Jump == 1)){
		instrucoes_de_desvio(estagio_controle, estagio_IF, reg); //Realiza os calculos dos desvios
	}
	
	else if ((estagio_controle->Branch == 0) and (estagio_controle->Jump == 0) and ((estagio_controle->Memwrite == 1) or (estagio_controle->Memread == 1))){
		instrucoes_de_memoria(estagio_controle, reg); //Realiza os calculos para armazenar valores na memoria
	}
}

void EX::instrucoes_aritmeticas(Controle *estagio_controle, Registradores *reg){
	
	unsigned long uvalor_rs = 0;
	unsigned long uvalor_rt = 0;
	long valor_rs = 0;
	long valor_rt = 0;
	
	cout << estagio_controle->instrucao.rs << endl;
	
	try{
		uvalor_rs = (reg->retornar_registrador(estagio_controle->instrucao.rs)).to_ulong();
		valor_rs = static_cast<long>(uvalor_rs);
		
		uvalor_rt = (reg->retornar_registrador(estagio_controle->instrucao.rt)).to_ulong();
		valor_rt = static_cast<long>(uvalor_rt);
	}
	catch(exception& e){
		
		cout << e.what();
	}
	
	long zero = 0;
	
	//add
	if(estagio_controle->Aluctrl == "add"){
		resultado_ALU = valor_rs + valor_rt;
		verifica_overflow(resultado_ALU, valor_rs, valor_rt, "adicao");
		zero = resultado_ALU;
		
		cout<<"É uma instrução de add"<<endl;
	}
	//sub
	if(estagio_controle->Aluctrl == "sub"){
		resultado_ALU = valor_rs - valor_rt;
		verifica_overflow(resultado_ALU, valor_rs, valor_rt, "subtracao");
		zero = resultado_ALU;
		cout<<"É uma instrução de sub"<<endl;
	}
	//addi
	if(estagio_controle->Aluctrl == "addi"){
		resultado_ALU = valor_rs + stoi(estagio_controle->instrucao.offset_address);
		verifica_overflow(resultado_ALU, valor_rs, stoi(estagio_controle->instrucao.offset_address), "adicao");
		zero = resultado_ALU;
		cout<<"É uma instrução de addi"<<endl;
	}
	//and
	if(estagio_controle->Aluctrl == "and"){
		resultado_ALU = (valor_rs & valor_rt);
		//nao causa overflow
		zero = resultado_ALU;
		cout<<"É uma instrução de and"<<endl;
	}
	//or
	if(estagio_controle->Aluctrl == "or"){
		resultado_ALU = (valor_rs | valor_rt);
		//nao causa overflow
		zero = resultado_ALU;
		cout<<"É uma instrução de or"<<endl;
	}
	//nor
	if(estagio_controle->Aluctrl == "nor"){
		resultado_ALU = ~(valor_rs | valor_rt);
		//nao causa overflow
		zero = resultado_ALU;
		cout<<"É uma instrução de nor"<<endl;
	}
	//mult
	if(estagio_controle->Aluctrl == "mult"){
		HI = valor_rs * valor_rt;
		verifica_overflow(HI, valor_rs, valor_rt, "multiplicacao");
		LO = HI;
		zero = HI;
		cout<<"É uma instrução de mult"<<endl;
	}
	//mul
	if(estagio_controle->Aluctrl == "mul"){
		resultado_ALU = valor_rs * valor_rt;
		verifica_overflow(resultado_ALU, valor_rs, valor_rt, "multiplicacao");
		zero = resultado_ALU;
		cout<<"É uma instrução de mul"<<endl;
	}
	//div
	if(estagio_controle->Aluctrl == "div"){
		HI = valor_rs % valor_rt; 
		LO = valor_rs / valor_rt;
		verifica_overflow(LO, valor_rs, valor_rt, "divisao");
		zero = LO;
		cout<<"É uma instrução de div"<<endl;
	}
	//slt
	if(estagio_controle->Aluctrl == "slt"){
		resultado_ALU = valor_rs < valor_rt;
		//nao causa overflow
		zero = resultado_ALU;
		cout<<"É uma instrução de slt"<<endl;
	}
	//sll
	if(estagio_controle->Aluctrl == "sll"){
		resultado_ALU = valor_rt << sa;
		verifica_overflow(resultado_ALU, valor_rt, sa, "sll");
		zero = resultado_ALU;
		cout<<"É uma instrução de sll"<<endl;
	}
	//srl
	if(estagio_controle->Aluctrl == "srl"){
		resultado_ALU = valor_rt >> sa;
		//nao causa overflow
		zero = resultado_ALU;
		cout<<"É uma instrução de srl"<<endl;
	}
	
	if(zero == 0){ //Verifica se algum resultado corresponde a 0 para acionar a flag.
		ALUzero = 1;
	}
	
	cout<<endl;
}

void EX::instrucoes_de_desvio(Controle *estagio_controle, IF *estagio_IF, Registradores *reg){
	
	unsigned long valor_rs = 0;
	unsigned long valor_rt = 0;
	unsigned long valor_offset = 0;
	unsigned long valor_target = 0;
	
	try{
		valor_rs = (reg->retornar_registrador(estagio_controle->instrucao.rs)).to_ulong();
		valor_rt = (reg->retornar_registrador(estagio_controle->instrucao.rt)).to_ulong();
		valor_offset = bitset<16>(estagio_controle->instrucao.offset_address).to_ulong();
		valor_target = bitset<26>(estagio_controle->instrucao.target).to_ulong();
	}
	catch(exception& e){
		
		cout << e.what();
	}
	
	//beq
	if(estagio_controle->Aluctrl == "beq"){ 
		if(valor_rs == valor_rt){
			valor_novo_PC = estagio_IF->PC + valor_offset * 4;
		}
		cout<<"É uma instrução de beq"<<endl;
	}
	//bne
	if(estagio_controle->Aluctrl == "bne"){
		if(valor_rs != valor_rt){ 
			valor_novo_PC = estagio_IF->PC + valor_offset * 4;
		}
		cout<<"É uma instrução de bne"<<endl;
	}
	//j
	if(estagio_controle->Aluctrl == "j"){
		bitset<32> num_binario(estagio_IF->PC);
		bitset<4> pc_upper_binario(num_binario.to_string().substr(0,4));
		int pc_upper = (int)pc_upper_binario.to_ulong();
		
		valor_novo_PC = pc_upper + (valor_target * 4);
		
		cout<<"É uma instrução de j"<<endl;
	}
	//jal
	if(estagio_controle->Aluctrl == "jal"){
		valor_novo_ra = estagio_IF->PC;
		valor_novo_PC = valor_target * 4;
		cout<<"É uma instrução de jal"<<endl;
	}
	//jr
	if(estagio_controle->Aluctrl == "jr"){
		valor_novo_PC = estagio_controle->instrucao.rs;
		cout<<"É uma instrução de jr"<<endl;
	}
	
	if(valor_novo_PC != 0)
		resultado_endereco = valor_novo_PC;
}

void EX::instrucoes_de_memoria(Controle *estagio_controle, Registradores *reg){
	
	unsigned long valor_rs = 0;
	unsigned long valor_rt = 0;
	unsigned long valor_offset = 0;
	
	try{
		valor_rs = (reg->retornar_registrador(estagio_controle->instrucao.rs)).to_ulong();
		valor_rt = (reg->retornar_registrador(estagio_controle->instrucao.rt)).to_ulong();
		valor_offset = bitset<16>(estagio_controle->instrucao.offset_address).to_ulong();
	}
	catch(exception& e){
		
		cout << e.what();
	}
	
	//lw
	if(estagio_controle->Aluctrl == "lw"){
		valor_rt = *(int*)(valor_offset + valor_rs);
		cout<<"É uma instrução de lw"<<endl;
	}
	//sw
	if(estagio_controle->Aluctrl == "sw"){
		*(int*)(valor_offset + valor_rs) = valor_rt;
		cout<<"É uma instrução de sw"<<endl;
	}
}

void EX::verifica_overflow(long result, long a, long b, string operacao){
	
	
	if((operacao == "adicao") and ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0))) { // adicao
		Overflow = 1;
	}
	
	if((operacao == "subtracao") and ((a > 0 && b < 0 && result < 0) || (a < 0 && b > 0 && result > 0))) { // subtracao
		Overflow = 1;
	}
	
	if((operacao == "multiplicacao") and (a != 0 && b != 0 && result / a != b)) { // multiplicacao
		Overflow = 1;
	}
	
	if((operacao == "divisao") and (b == 0 || (a == INT_MIN && b == -1))) { // divisao
		Overflow = 1;
	}
	
	if((operacao == "sll") and (result > INT_MAX)) { //caso especial sll
		Overflow = 1;
	}
}

void EX::mostrar_flags_desvio(){
	
	cout << "Flag de Zero: " << ALUzero << endl;
	cout << "Sinal de Overflow: " << Overflow << endl;
	cout << "Calculo do endereco de desvio: " << resultado_endereco << endl;
}

// #######################################

class MEM{
	
	private:
		bitset<tamanho_instrucao> retorno_para_registrador;
		
	public:
		void leitura_escrita_memoria(Memoria *mem, Controle *estagio_controle, EX *estagio_execucao, Registradores *reg);
};

void MEM::leitura_escrita_memoria(Memoria *mem, Controle *estagio_controle, EX *estagio_execucao, Registradores *reg){
	
	if(estagio_controle->Memread == 1 and estagio_controle->Memwrite == 0){ //sw
		
		if(estagio_execucao->resultado_endereco >= comeco_memoria_dados and estagio_execucao->resultado_endereco <= (final_memoria_dados - tamanho_instrucao)){
			
			bitset<tamanho_instrucao> valor_rd = reg->retornar_registrador(estagio_controle->instrucao.rd);
			
			cout << "Operacao de escrita na memoria!" << endl;
			cout << "\tMemwrite: " << estagio_controle->Memwrite << endl;
			cout << "\t" << valor_rd.to_ulong() << endl;
			
			for(int i = 0; i < tamanho_instrucao; i++){
				
				mem->memoria[estagio_execucao->resultado_endereco + i] = valor_rd[i];
			}
		}
	}
	
	if(estagio_controle->Memread == 0 and estagio_controle->Memwrite == 1){//lw
		
		for(int i = 0; i < tamanho_instrucao; i++){
			
			retorno_para_registrador[i] = mem->memoria[estagio_execucao->resultado_endereco + i];
		}
		
		cout << "Operacao de leitura na memoria!" << endl;
		cout << "\tMemread: " << estagio_controle->Memread << endl;
		cout << "\t" << estagio_execucao->resultado_endereco << endl;
	}
}

// #######################################

class WR{
	
	public:
		void leitura_escrita_registrador(Controle *estagio_controle, Registradores *reg, EX *estagio_execucao);
};

void WR::leitura_escrita_registrador(Controle *estagio_controle, Registradores *reg, EX *estagio_execucao){
	
	if(estagio_controle->Regwrite == 1 and estagio_controle->Regdst == 1){
		
		reg->setar_registrador(estagio_execucao->resultado_ALU, estagio_controle->instrucao.rd);
	}
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
			
			IF *estagio_IF = new IF(mem);
			Controle *estagio_controle = new Controle();
			Registradores *reg = new Registradores();
			EX *estagio_execucao = new EX();
			MEM *estagio_memoria = new MEM();
			WR *estagio_registrador = new WR();
			
			while(estagio_IF->verificar_instrucao()){
				
				estagio_IF->mostrar_valor_atual_PC();
				mem->depuracao_memoria();
				estagio_controle->depuracao_controle();
				reg->depuracao_registradores();
				
				cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
				
				estagio_controle->decodificar_instrucao(mem, estagio_IF);
				
				cout << "#####################" << endl;
				
				estagio_controle->depuracao_controle();
				
				estagio_execucao->ALU(estagio_controle, estagio_IF, reg);
				
				estagio_execucao->mostrar_flags_desvio();
				
				estagio_registrador->leitura_escrita_registrador(estagio_controle, reg, estagio_execucao);
				
				estagio_controle->reset();
				reg->depuracao_registradores();
				
				cout << "@@@@@@@@@@@@@@@@@@@@@" << endl;
				
				estagio_IF->trocar_instrucao(mem);
			}
			
			delete estagio_IF;
			delete estagio_controle;
			delete reg;
			delete estagio_execucao;
			delete estagio_memoria;
			delete estagio_registrador;
		}
		
		delete mem;
		
		arquivo_entrada.close();
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
