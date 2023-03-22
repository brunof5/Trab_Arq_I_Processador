class WR{
	
	private:
		Registradores *reg;
		Controle *controle;
		EX *estagio_execucao;
		MEM *estagio_memoria;
		
	public:
		WR(Registradores *reg_pointer, Controle *controle_pointer, EX *estagio_execucao_pointer, MEM *estagio_memoria_pointer);
		void leitura_escrita_registrador();
};

WR::WR(Registradores *reg_pointer, Controle *controle_pointer, EX *estagio_execucao_pointer, MEM *estagio_memoria_pointer){
	
	reg = reg_pointer;
	controle = controle_pointer;
	estagio_execucao = estagio_execucao_pointer;
	estagio_memoria = estagio_memoria_pointer;
}

void WR::leitura_escrita_registrador(){
	
	// jal
	if(controle->Regwrite == 1 and controle->Jump == 1){
		
		bitset<tamanho_instrucao> novo_ra(estagio_execucao->valor_novo_ra);
		
		cout << "Valores e sinais do estagio WR: " << endl;
		cout << "\tSinal Mem2Reg: " << controle->MemtoReg << endl;
		cout << "\tRegistrador de destino: " << 31 << endl;
		cout << "\tDado a ser escrito no registrador: " << estagio_execucao->valor_novo_ra << endl;
		
		reg->setar_registrador(novo_ra, 31);
	}
	
	// lw
	else if(controle->Regwrite == 1 and controle->MemtoReg == 1){
		
		unsigned long reg_rt = conversor_sem_sinal_binario(controle->instrucao.rt);
		
		cout << "Valores e sinais do estagio WR: " << endl;
		cout << "\tSinal Mem2Reg: " << controle->MemtoReg << endl;
		cout << "\tRegistrador de destino: " << reg_rt << endl;
		cout << "\tDado a ser escrito no registrador: " << conversor_com_sinal_binario(estagio_memoria->retorno_para_registrador.to_string()) << endl;
		
		reg->setar_registrador(estagio_memoria->retorno_para_registrador, reg_rt);
	}
	
	else if(controle->Regwrite == 1 and controle->Regdst == 1 and estagio_execucao->Overflow == 0){
		
		unsigned long reg_rd = conversor_sem_sinal_binario(controle->instrucao.rd);
		
		bitset<tamanho_instrucao> novo_dado(estagio_execucao->resultado_ALU);
		
		cout << "Valores e sinais do estagio WR: " << endl;
		cout << "\tSinal Mem2Reg: " << controle->MemtoReg << endl;
		cout << "\tRegistrador de destino: " << reg_rd << endl;
		cout << "\tDado a ser escrito no registrador: " << estagio_execucao->resultado_ALU << endl;
		
		reg->setar_registrador(novo_dado, reg_rd);
	}
	
	else if(controle->Regwrite == 1 and controle->Regdst == 0 and estagio_execucao->Overflow == 0){
		
		unsigned long reg_rt = conversor_sem_sinal_binario(controle->instrucao.rt);
		
		bitset<tamanho_instrucao> novo_dado(estagio_execucao->resultado_ALU);
		
		cout << "Valores e sinais do estagio WR: " << endl;
		cout << "\tSinal Mem2Reg: " << controle->MemtoReg << endl;
		cout << "\tRegistrador de destino: " << reg_rt << endl;
		cout << "\tDado a ser escrito no registrador: " << estagio_execucao->resultado_ALU << endl;
		
		reg->setar_registrador(novo_dado, reg_rt);
	}
}
