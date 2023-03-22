class EX{
	
	friend class MEM;
	friend class WR;
	
	private:
		long HI = 0, LO = 0;
		unsigned long valor_novo_ra = 0;
		unsigned long valor_novo_PC = 0;
		long resultado_ALU = 0;
		unsigned long resultado_endereco = 0;
		bool ALUzero = 0;
		bool Overflow = 0;
		Registradores *reg;
		IF *estagio_IF;
		Controle *controle;
		void instrucoes_aritmeticas();
		void instrucoes_de_desvio();
		void instrucoes_de_memoria();
		void verifica_overflow(long result, long a, long b, string operacao);
		
	public:
		EX(Registradores *reg_pointer, IF *estagio_IF_pointer, Controle *controle_pointer);
		void ALU();
		void mostrar_flags_desvio();
		void reset_flags_desvio_variaveis();
};

EX::EX(Registradores *reg_pointer, IF *estagio_IF_pointer, Controle *controle_pointer){
	
	reg = reg_pointer;
	estagio_IF = estagio_IF_pointer;
	controle = controle_pointer;
}

void EX::ALU(){
	
	// Realiza os calculos aritmeticos
	if((controle->Branch == 0) and (controle->Jump == 0) and (controle->Memwrite == 0) and (controle->Memread == 0))
		instrucoes_aritmeticas();
		
	// Realiza os calculos dos desvios
	else if((controle->Branch == 1) or (controle->Jump == 1))
		instrucoes_de_desvio();
		
	// Realiza os calculos para armazenar valores na memoria
	else if((controle->Branch == 0) and (controle->Jump == 0) and ((controle->Memwrite == 1) or (controle->Memread == 1)))
		instrucoes_de_memoria();
}

void EX::verifica_overflow(long result, long a, long b, string operacao){
	
	// adicao
	if((operacao == "adicao") and ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0)))
		Overflow = 1;
		
	// subtracao
	if((operacao == "subtracao") and ((a > 0 && b < 0 && result < 0) || (a < 0 && b > 0 && result > 0) || (a > 0 && b > 0 && result < 0)))
		Overflow = 1;
		
	// multiplicacao
	if((operacao == "multiplicacao") and (a != 0 && b != 0 && result / a != b))
		Overflow = 1;
		
	// divisao
	if((operacao == "divisao") and (b == 0 || (a == INT_MIN && b == -1)))
		Overflow = 1;
		
	// caso especial sll
	if((operacao == "sll") and (result > INT_MAX))
		Overflow = 1;
}

void EX::instrucoes_aritmeticas(){
	
	unsigned long reg_rs = conversor_sem_sinal_binario(controle->instrucao.rs);
	bitset<tamanho_instrucao> valor_rs_bitset = reg->retornar_registrador(reg_rs);
	long valor_rs = conversor_com_sinal_binario(valor_rs_bitset.to_string());
	
	unsigned long reg_rt = conversor_sem_sinal_binario(controle->instrucao.rt);
	bitset<tamanho_instrucao> valor_rt_bitset = reg->retornar_registrador(reg_rt);
	long valor_rt = conversor_com_sinal_binario(valor_rt_bitset.to_string());
	
	long valor_shamt = conversor_com_sinal_binario(controle->instrucao.shamt);
	long valor_offset_address = conversor_com_sinal_binario(controle->instrucao.offset_address);
	
	long zero = 0;
	
	// add
	if(controle->Aluctrl == "add"){
		resultado_ALU = valor_rs + valor_rt;
		verifica_overflow(resultado_ALU, valor_rs, valor_rt, "adicao");
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de add" << endl << endl;
	}
	
	// sub
	if(controle->Aluctrl == "sub"){
		resultado_ALU = valor_rs - valor_rt;
		verifica_overflow(resultado_ALU, valor_rs, valor_rt, "subtracao");
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de sub" << endl << endl;
	}
	
	// addi
	if(controle->Aluctrl == "addi"){
		resultado_ALU = valor_rs + valor_offset_address;
		verifica_overflow(resultado_ALU, valor_rs, valor_offset_address, "adicao");
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de addi" << endl << endl;
	}
	
	// and
	if(controle->Aluctrl == "and"){
		resultado_ALU = (valor_rs & valor_rt);
		// nao causa overflow
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de and" << endl << endl;
	}
	
	// or
	if(controle->Aluctrl == "or"){
		resultado_ALU = (valor_rs | valor_rt);
		// nao causa overflow
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de or" << endl << endl;
	}
	
	// nor
	if(controle->Aluctrl == "nor"){
		resultado_ALU = ~(valor_rs | valor_rt);
		// nao causa overflow
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de nor" << endl << endl;
	}
	
	// mult
	if(controle->Aluctrl == "mult"){
		HI = valor_rs * valor_rt;
		verifica_overflow(HI, valor_rs, valor_rt, "multiplicacao");
		LO = HI;
		zero = HI;
		
		cout << "Eh uma instrucao de mult" << endl << endl;
	}
	
	// mul
	if(controle->Aluctrl == "mul"){
		resultado_ALU = valor_rs * valor_rt;
		verifica_overflow(resultado_ALU, valor_rs, valor_rt, "multiplicacao");
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de mul" << endl << endl;
	}
	
	// div
	if(controle->Aluctrl == "div"){
		if(valor_rt != 0){
			HI = valor_rs % valor_rt; 
			LO = valor_rs / valor_rt;
		}
		else{
			HI = 0;
			LO = 0;
		}
		verifica_overflow(LO, valor_rs, valor_rt, "divisao");
		zero = LO;
		
		cout << "Eh uma instrucao de div" << endl << endl;
	}
	
	// slt
	if(controle->Aluctrl == "slt"){
		resultado_ALU = valor_rs < valor_rt;
		// nao causa overflow
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de slt" << endl << endl;
	}
	
	// sll
	if(controle->Aluctrl == "sll"){
		resultado_ALU = valor_rt << valor_shamt;
		verifica_overflow(resultado_ALU, valor_rt, valor_shamt, "sll");
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de sll" << endl << endl;
	}
	
	// srl
	if(controle->Aluctrl == "srl"){
		resultado_ALU = valor_rt >> valor_shamt;
		// nao causa overflow
		zero = resultado_ALU;
		
		cout << "Eh uma instrucao de srl" << endl << endl;
	}
	
	// Verifica se algum resultado corresponde a 0 para acionar a flag
	if(zero == 0)
		ALUzero = 1;
}

void EX::instrucoes_de_desvio(){
	
	unsigned long reg_rs = conversor_sem_sinal_binario(controle->instrucao.rs);
	bitset<tamanho_instrucao> valor_rs_bitset = reg->retornar_registrador(reg_rs);
	long valor_rs = conversor_com_sinal_binario(valor_rs_bitset.to_string());
	
	unsigned long reg_rt = conversor_sem_sinal_binario(controle->instrucao.rt);
	bitset<tamanho_instrucao> valor_rt_bitset = reg->retornar_registrador(reg_rt);
	long valor_rt = conversor_com_sinal_binario(valor_rt_bitset.to_string());
	
	long valor_offset_address = conversor_com_sinal_binario(controle->instrucao.offset_address);
	long valor_target = conversor_com_sinal_binario(controle->instrucao.target);
	
	// beq
	if(controle->Aluctrl == "beq"){
		if(valor_rs == valor_rt)
			valor_novo_PC = valor_offset_address;
			
		cout << "Eh uma instrucao de beq" << endl << endl;
	}
	
	// bne
	if(controle->Aluctrl == "bne"){
		if(valor_rs != valor_rt)
			valor_novo_PC = valor_offset_address;
			
		cout << "Eh uma instrucao de bne" << endl << endl;
	}
	
	// j
	if(controle->Aluctrl == "j"){
		bitset<tamanho_instrucao> num_binario(estagio_IF->PC);
		bitset<tamanho_palavra> pc_upper_binario(num_binario.to_string().substr(0,4));
		unsigned long pc_upper = pc_upper_binario.to_ulong();
		
		valor_novo_PC = pc_upper + valor_target;
		
		cout << "Eh uma instrucao de j" << endl << endl;
	}
	
	// jal
	if(controle->Aluctrl == "jal"){
		valor_novo_ra = estagio_IF->PC;
		valor_novo_PC = valor_target;
		
		cout << "Eh uma instrucao de jal" << endl << endl;
	}
	
	// jr
	if(controle->Aluctrl == "jr"){
		valor_novo_PC = valor_rs;
		
		cout << "Eh uma instrucao de jr" << endl << endl;
	}
	
	if(valor_novo_PC != 0){
		resultado_endereco = valor_novo_PC;
		estagio_IF->setar_novo_PC(resultado_endereco);
	}
}

void EX::instrucoes_de_memoria(){
	
	unsigned long reg_rs = conversor_sem_sinal_binario(controle->instrucao.rs);
	bitset<tamanho_instrucao> valor_rs_bitset = reg->retornar_registrador(reg_rs);
	long valor_rs = conversor_com_sinal_binario(valor_rs_bitset.to_string());
	
	long valor_offset_address = conversor_com_sinal_binario(controle->instrucao.offset_address);
	
	// lw
	if(controle->Aluctrl == "lw"){
		resultado_endereco = valor_rs + ((valor_offset_address / 4) * tamanho_instrucao);
		
		cout << "Eh uma instrucao de lw" << endl << endl;
	}
	
	// sw
	if(controle->Aluctrl == "sw"){
		resultado_endereco = valor_rs + ((valor_offset_address / 4) * tamanho_instrucao);
		
		cout << "Eh uma instrucao de sw" << endl << endl;
	}
}

void EX::mostrar_flags_desvio(){
	
	cout << "Flags da ALU e endereco de desvio: " << endl;
	cout << "\tFlag de Zero: " << ALUzero << endl;
	cout << "\tSinal de Overflow: " << Overflow << endl;
	cout << "\tCalculo do endereco de desvio: " << resultado_endereco << endl;
}

void EX::reset_flags_desvio_variaveis(){
	
	ALUzero = 0;
	Overflow = 0;
	resultado_endereco = 0;
	HI = LO = resultado_ALU = 0;
	valor_novo_ra = valor_novo_PC = 0;
}
