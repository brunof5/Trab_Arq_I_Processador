class Registradores{
	
	friend class WR;
	
	private:
		bitset<tamanho_instrucao> zero;	// valor constante de zero
		bitset<tamanho_instrucao> at;	// reservado pelo compilador
		bitset<tamanho_instrucao> v0, v1;	// variaveis para retorno de funcoes e outras utilidades
		bitset<tamanho_instrucao> a0, a1, a2, a3;	// argumentos
		bitset<tamanho_instrucao> t0, t1, t2, t3, t4, t5, t6, t7;	// variaveis temporarias
		bitset<tamanho_instrucao> s0, s1, s2, s3, s4, s5, s6, s7;	// variaveis salvas
		bitset<tamanho_instrucao> t8, t9;	// variaveis usadas na adicao dos temporarios
		bitset<tamanho_instrucao> k0, k1;	// reservado pelo OS
		bitset<tamanho_instrucao> gp;	// global pointer
		bitset<tamanho_instrucao> sp;	// stack pointer
		bitset<tamanho_instrucao> fp;	// frame pointer
		bitset<tamanho_instrucao> ra;	// retorno de função
		
	public:
		Registradores();
		bitset<tamanho_instrucao> retornar_registrador(long numero_registrador);
		void setar_registrador(bitset<tamanho_instrucao> dado_para_escrever, long numero_registrador);
};

Registradores::Registradores(){
	
	gp = bitset<tamanho_instrucao>(comeco_memoria_dados);
	sp = bitset<tamanho_instrucao>(final_memoria_dados - tamanho_instrucao + 1);
}

bitset<tamanho_instrucao> Registradores::retornar_registrador(long numero_registrador){
	
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

void Registradores::setar_registrador(bitset<tamanho_instrucao> dado_para_escrever, long numero_registrador){
	
	switch(numero_registrador){
		
		case 0:
			throw runtime_error("Tentativa de setar algum valor no registrador zero - NEGADO!");
			break;
			
		case 1:
			at = dado_para_escrever;
			break;
			
		case 2:
			v0 = dado_para_escrever;
			break;
			
		case 3:
			v1 = dado_para_escrever;
			break;
			
		case 4:
			a0 = dado_para_escrever;
			break;
			
		case 5:
			a1 = dado_para_escrever;
			break;
			
		case 6:
			a2 = dado_para_escrever;
			break;
			
		case 7:
			a3 = dado_para_escrever;
			break;
			
		case 8:
			t0 = dado_para_escrever;
			break;
			
		case 9:
			t1 = dado_para_escrever;
			break;
			
		case 10:
			t2 = dado_para_escrever;
			break;
			
		case 11:
			t3 = dado_para_escrever;
			break;
			
		case 12:
			t4 = dado_para_escrever;
			break;
			
		case 13:
			t5 = dado_para_escrever;
			break;
			
		case 14:
			t6 = dado_para_escrever;
			break;
			
		case 15:
			t7 = dado_para_escrever;
			break;
			
		case 16:
			s0 = dado_para_escrever;
			break;
			
		case 17:
			s1 = dado_para_escrever;
			break;
			
		case 18:
			s2 = dado_para_escrever;
			break;
			
		case 19:
			s3 = dado_para_escrever;
			break;
			
		case 20:
			s4 = dado_para_escrever;
			break;
			
		case 21:
			s5 = dado_para_escrever;
			break;
			
		case 22:
			s6 = dado_para_escrever;
			break;
			
		case 23:
			s7 = dado_para_escrever;
			break;
			
		case 24:
			t8 = dado_para_escrever;
			break;
			
		case 25:
			t9 = dado_para_escrever;
			break;
			
		case 26:
			throw runtime_error("Tentativa de setar algum valor no registrador reservado ao OS - NEGADO!");
			break;
			
		case 27:
			throw runtime_error("Tentativa de setar algum valor no registrador reservado ao OS - NEGADO!");
			break;
			
		case 28:
			gp = dado_para_escrever;
			break;
			
		case 29:
			sp = dado_para_escrever;
			break;
			
		case 30:
			fp = dado_para_escrever;
			break;
			
		case 31:
			ra = dado_para_escrever;
			break;
	}
}
