class MEM{
	
	friend class WR;
	
	private:
		bitset<tamanho_instrucao> retorno_para_registrador;
		Memoria *mem;
		Registradores *reg;
		Controle *controle;
		EX *estagio_execucao;
		
	public:
		MEM(Memoria *mem_pointer, Registradores *reg_pointer, Controle *controle_pointer, EX *estagio_execucao_pointer);
		void leitura_escrita_memoria();
};

MEM::MEM(Memoria *mem_pointer, Registradores *reg_pointer, Controle *controle_pointer, EX *estagio_execucao_pointer){
	
	mem = mem_pointer;
	reg = reg_pointer;
	controle = controle_pointer;
	estagio_execucao = estagio_execucao_pointer;
}

void MEM::leitura_escrita_memoria(){
	
	// sw
	if(controle->Memread == 0 and controle->Memwrite == 1){
		
		if(estagio_execucao->resultado_endereco >= comeco_memoria_dados and estagio_execucao->resultado_endereco <= (final_memoria_dados - tamanho_instrucao + 1)){
			
			unsigned long reg_rt = conversor_sem_sinal_binario(controle->instrucao.rt);
			bitset<tamanho_instrucao> valor_rt = reg->retornar_registrador(reg_rt);
			
			cout << "Operacao de escrita na memoria!" << endl;
			cout << "\tMemwrite: " << controle->Memwrite << endl;
			cout << "\tEndereco da Memoria: " << estagio_execucao->resultado_endereco << endl;
			cout << "\tDado a ser escrito na Memoria: " << conversor_com_sinal_binario(valor_rt.to_string()) << endl;
			
			for(int i = 0, k = 31; i < tamanho_instrucao; i++, k--){
				
				mem->memoria[estagio_execucao->resultado_endereco + k] = valor_rt[k];
			}
		}
		
		else
			throw runtime_error("ERRO ESCRITA NA MEMORIA - o endereco em que deseja escrever nao pertence a regiao de dados da memoria!");
	}
	
	// lw
	else if(controle->Memread == 1 and controle->Memwrite == 0){
		
		if(estagio_execucao->resultado_endereco >= comeco_memoria_dados and estagio_execucao->resultado_endereco <= (final_memoria_dados - tamanho_instrucao + 1)){
			
			for(int i = 0; i < tamanho_instrucao; i++){
				
				retorno_para_registrador[i] = mem->memoria[estagio_execucao->resultado_endereco + i];
			}
			
			cout << "Operacao de leitura na memoria!" << endl;
			cout << "\tMemread: " << controle->Memread << endl;
			cout << "\tEndereco da Memoria: " << estagio_execucao->resultado_endereco << endl << endl;
		}
		
		else
			throw runtime_error("ERRO LEITURA NA MEMORIA - o endereco em que deseja ler nao pertence a regiao de dados da memoria!");
	}
}
