struct instrucaoDecodificada{
	
	string opcode;
	string rs;
	string rt;
	string rd;
	string shamt;
	string funct;
	string offset_address;
	string target;
};

class Controle{
	
	friend class EX;
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
		IF *estagio_IF;
		
	public:
		Controle(IF *estagio_IF_pointer);
		void reset();
		void depuracao_controle();
		void decodificar_instrucao();
};

Controle::Controle(IF *estagio_IF_pointer){
	
	estagio_IF = estagio_IF_pointer;
	
	reset();
}

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
	instrucao.rs = "invalido";
	instrucao.rt = "invalido";
	instrucao.rd = "invalido";
	instrucao.shamt = "invalido";
	instrucao.funct = "invalido";
	instrucao.offset_address = "invalido";
	instrucao.target = "invalido";
}

void Controle::depuracao_controle(){
	
	cout << "Valores dos sinais de controle: " << endl;
	cout << "\tRegdst: " << Regdst << endl;
	cout << "\tRegwrite: " << Regwrite << endl;
	cout << "\tAlusrc: " << Alusrc << endl;
	cout << "\tMemread: " << Memread << endl;
	cout << "\tMemwrite: " << Memwrite << endl;
	cout << "\tMemtoReg: " << MemtoReg << endl;
	cout << "\tJump: " << Jump << endl;
	cout << "\tBranch: " << Branch << endl;
	cout << "\tAluop: " << Aluop << endl << endl;
}

void Controle::decodificar_instrucao(){
	
	bitset<tamanho_instrucao> instrucao_binaria = estagio_IF->get_instrucao_atual();
	string instrucao_string = instrucao_binaria.to_string();
	
	string texto_binario;
	
	// opcode
	instrucao.opcode = instrucao_string.substr(0, 6);	// extrai os 6 bits mais significativos
	
	// Tipo R
	instrucao.rs = instrucao_string.substr(6, 5);	// extrai os 5 bits do primeiro operando
	instrucao.rt = instrucao_string.substr(11, 5);	// extrai os 5 bits do segundo operando
	instrucao.rd = instrucao_string.substr(16, 5);	// extrai os 5 bits do terceiro operando
	instrucao.shamt = instrucao_string.substr(21, 5);	// extrai os 5 bits do shamt
	instrucao.funct = instrucao_string.substr(26, 6);	// extrai os 6 bits menos significativos (funct)
	
	// Tipo I
	instrucao.offset_address = instrucao_string.substr(16, 16);	// extrai os 16 bits menos significativos
	
	// Tipo J
	instrucao.target = instrucao_string.substr(6, 26);	// extrai os 26 bits menos significativos
	
	// mul - tipo R
	if(instrucao.opcode == "011100" and instrucao.funct == "000010"){
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
	
	// Instrucoes do tipo R
	else if(instrucao.opcode == "000000"){
		
		// add
		if(instrucao.funct == "100000"){
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
		
		// sub
		else if(instrucao.funct == "100010"){
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
		
		// and
		else if(instrucao.funct == "100100"){
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
		
		// mult
		else if(instrucao.funct == "011000"){
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
		
		// or
		else if(instrucao.funct == "100101"){
			Regdst = 1;
			Regwrite = 1;
			Alusrc = 0;
			Aluop = 01;
			// nao sao utilizados
			MemtoReg = 0;
			Jump = 0;
			Branch = 0;
			Memwrite = 0;
			Memread = 0;
			Aluctrl = "or";
		}
		
		// nor
		else if(instrucao.funct == "100111"){
			Regdst = 1;
			Regwrite = 1;
			Alusrc = 0;
			Aluop = 11;
			// nao sao utilizados
			MemtoReg = 0;
			Jump = 0;
			Branch = 0;
			Memwrite = 0;
			Memread = 0;
			Aluctrl = "nor";
		}
		
		// sll
		else if(instrucao.funct == "000000"){
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
		
		// srl
		else if(instrucao.funct == "000010"){
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
		
		// div
		else if(instrucao.funct == "011010"){
			Regdst = 0;
			Regwrite = 0;
			Alusrc = 0;
			Aluop = 11;
			MemtoReg = 0;
			Jump = 0;
			Branch = 0;
			Memwrite = 0;
			Memread = 0;
			Aluctrl = "div";
		}
		
		// slt
		else if(instrucao.funct == "101010"){
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
		
		// jr
		else if(instrucao.funct == "001000"){
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
			throw runtime_error("ERRO - Instrucao nao identificada");
		}
	}
	
	// Instrucoes do tipo J
	// j
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
	
	// jal
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
	
	// Instrucoes do tipo I
	else{
		// addi
		if(instrucao.opcode == "001000"){
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
		
		// lw
		else if(instrucao.opcode == "100011"){
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
		
		// sw
		else if(instrucao.opcode == "101011"){
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
		
		// beq
		else if(instrucao.opcode == "000100"){
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
		
		// bne
		else if(instrucao.opcode == "000101"){
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
			throw runtime_error("ERRO - Instrucao nao identificada");
		}
	}
}
