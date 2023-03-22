class IF{
	
	friend class EX;
	
	private:
		unsigned long PC;
		bitset<tamanho_instrucao> instrucao_atual;
		Memoria *mem;
		
	public:
		IF(Memoria *mem_pointer);
		bool verificar_instrucao();
		void trocar_instrucao();
		bitset<tamanho_instrucao> get_instrucao_atual() { return instrucao_atual; };
		void mostrar_valor_atual_PC() { cout << "Valor atual do PC: " << PC << endl << endl; };
		void setar_novo_PC(long novo_PC) { PC = novo_PC; };
};

IF::IF(Memoria *mem_pointer){
	
	mem = mem_pointer;
	
	PC = comeco_memoria_texto;
	
	for(int i = 0; i < tamanho_instrucao; i++)
		instrucao_atual[i] = mem->memoria[PC + i];
		
	PC += tamanho_instrucao;
}

bool IF::verificar_instrucao(){
	
	if(instrucao_atual.to_string() == "00000000000000000000000000000000")
		return false;
		
	else
		return true;
}

void IF::trocar_instrucao(){
	
	for(int i = 0; i < tamanho_instrucao; i++)
		instrucao_atual[i] = mem->memoria[PC + i];
		
	PC += tamanho_instrucao;
}
