#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <bitset>
#include <sstream>

using namespace std;

// inteiro para definir o valor das labels
int memoria_texto = 256;

// vector de labels com seus respectivos valores dentro da memoria de texto
vector<pair<string, int>> labels;

int retornar_instrucao(string instrucao){
	
	// instrucoes tipo R
	if(instrucao == "add") return 0;
	if(instrucao == "sub") return 1;
	if(instrucao == "and") return 2;
	if(instrucao == "or") return 3;
	if(instrucao == "nor") return 4;
	if(instrucao == "mult") return 5;
	if(instrucao == "sll") return 6;
	if(instrucao == "srl") return 7;
	if(instrucao == "mul") return 8;
	if(instrucao == "div") return 9;
	if(instrucao == "slt") return 10;
	if(instrucao == "jr") return 11;
	// instrucoes tipo I
	if(instrucao == "addi") return 12;
	if(instrucao == "lw") return 13;
	if(instrucao == "sw") return 14;
	if(instrucao == "beq") return 15;
	if(instrucao == "bne") return 16;
	if(instrucao == "bge") return 17;
	// instrucoes tipo J
	if(instrucao == "j") return 18;
	if(instrucao == "jal") return 19;
	// EOF
	if(instrucao == "EOF") return 20;
	
	throw runtime_error("ERRO LEITURA DA INSTRUCAO - a instrucao lida nao pertence ao conjunto de instrucoes suportadas!");
}

string formato_instrucao_R(int instrucao, string texto){
	
	string opcode = "000000";
	string primeiro_operando = "00000";	// rs
	string segundo_operando = "00000";	// rt
	string terceiro_operando = "00000";	// rd
	string shamt = "00000";
	string funct = "000000";
	
	stringstream ss(texto);
	string lixo;
	getline(ss, lixo, ' ');
	
	// rd, rs, rt
	if((instrucao >= 0 and instrucao <= 4)  or (instrucao == 8) or (instrucao == 10)){
		
		// rd
		getline(ss, terceiro_operando, ' ');
		terceiro_operando.erase(remove(terceiro_operando.begin(), terceiro_operando.end(), '$'), terceiro_operando.end());
		terceiro_operando = bitset<5>(stoi(terceiro_operando)).to_string();
		
		// rs
		getline(ss, primeiro_operando, ',');
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), ' '), primeiro_operando.end());
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), '$'), primeiro_operando.end());
		primeiro_operando = bitset<5>(stoi(primeiro_operando)).to_string();
		
		// rt
		getline(ss, segundo_operando, '\n');
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), ' '), segundo_operando.end());
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), '$'), segundo_operando.end());
		segundo_operando = bitset<5>(stoi(segundo_operando)).to_string();
		
		switch(instrucao){
			
			case 0:
				funct = "100000";
				break;
				
			case 1:
				funct = "100010";
				break;
				
			case 2:
				funct = "100100";
				break;
				
			case 3:
				funct = "100101";
				break;
				
			case 4:
				funct = "100111";
				break;
				
			case 8:
				opcode = "011100";
				funct = "000010";
				break;
				
			case 10:
				funct = "101010";
				break;
		}
	}
	
	// rd, rt, shamt
	if(instrucao == 6 or instrucao == 7){
		
		// rd
		getline(ss, terceiro_operando, ',');
		terceiro_operando.erase(remove(terceiro_operando.begin(), terceiro_operando.end(), '$'), terceiro_operando.end());
		terceiro_operando = bitset<5>(stoi(terceiro_operando)).to_string();
		
		// rt
		getline(ss, segundo_operando, ',');
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), ' '), segundo_operando.end());
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), '$'), segundo_operando.end());
		segundo_operando = bitset<5>(stoi(segundo_operando)).to_string();
		
		// shamt
		getline(ss, shamt, '\n');
		shamt.erase(remove(shamt.begin(), shamt.end(), ' '), shamt.end());
		shamt = bitset<5>(stoi(shamt)).to_string();
		
		if(instrucao == 7)
			funct = "000010";
	}
	
	// rs, rt
	if((instrucao == 5) or (instrucao == 9)){
		
		// rs
		getline(ss, primeiro_operando, ',');
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), '$'), primeiro_operando.end());
		primeiro_operando = bitset<5>(stoi(primeiro_operando)).to_string();
		
		// rt
		getline(ss, segundo_operando, '\n');
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), ' '), segundo_operando.end());
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), '$'), segundo_operando.end());
		segundo_operando = bitset<5>(stoi(segundo_operando)).to_string();
		
		switch(instrucao){
			
			case 5:
				funct = "011000";
				break;
				
			case 9:
				funct = "011010";
				break;
		}
	}
	
	// rs
	if(instrucao == 11){
		
		// rs
		getline(ss, primeiro_operando, '\n');
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), '$'), primeiro_operando.end());
		primeiro_operando = bitset<5>(stoi(primeiro_operando)).to_string();
		
		funct = "001000";
	}
	
	string instrucao_decodificada = opcode + primeiro_operando + segundo_operando + terceiro_operando + shamt + funct;
	
	return instrucao_decodificada;
}

string formato_instrucao_I(int instrucao, string texto){
	
	string opcode = "000000";
	string primeiro_operando = "00000";	// rs
	string segundo_operando = "00000";	// rt
	string offset = "0000000000000000";
	string instrucao_decodificada;
	
	stringstream ss(texto);
	string lixo;
	getline(ss, lixo, ' ');
	
	// rt, rs, immediate
	if(instrucao == 12){
		
		// rt
		getline(ss, segundo_operando, ',');
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), '$'), segundo_operando.end());
		segundo_operando = bitset<5>(stoi(segundo_operando)).to_string();
		
		// rs
		getline(ss, primeiro_operando, ',');
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), ' '), primeiro_operando.end());
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), '$'), primeiro_operando.end());
		primeiro_operando = bitset<5>(stoi(primeiro_operando)).to_string();
		
		// immediate
		getline(ss, offset, '\n');
		offset.erase(remove(offset.begin(), offset.end(), ' '), offset.end());
		offset = bitset<16>(stoi(offset)).to_string();
		
		opcode = "001000";
	}
	
	// rt, offset, rs
	if(instrucao == 13 or instrucao == 14){
		
		// rt
		getline(ss, segundo_operando, ',');
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), '$'), segundo_operando.end());
		segundo_operando = bitset<5>(stoi(segundo_operando)).to_string();
		
		// offset
		getline(ss, offset, '(');
		offset.erase(remove(offset.begin(), offset.end(), ' '), offset.end());
		offset = bitset<16>(stoi(offset)).to_string();
		
		// rs
		getline(ss, primeiro_operando, '\n');
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), ')'), primeiro_operando.end());
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), '$'), primeiro_operando.end());
		primeiro_operando = bitset<5>(stoi(primeiro_operando)).to_string();
		
		switch(instrucao){
			
			case 13:
				opcode = "100011";
				break;
				
			case 14:
				opcode = "101011";
				break;
		}
	}
	
	// rs, rt, label
	if(instrucao >= 15 and instrucao <= 17){
		
		// rs
		getline(ss, primeiro_operando, ',');
		primeiro_operando.erase(remove(primeiro_operando.begin(), primeiro_operando.end(), '$'), primeiro_operando.end());
		primeiro_operando = bitset<5>(stoi(primeiro_operando)).to_string();
		
		// rt
		getline(ss, segundo_operando, ',');
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), ' '), segundo_operando.end());
		segundo_operando.erase(remove(segundo_operando.begin(), segundo_operando.end(), '$'), segundo_operando.end());
		segundo_operando = bitset<5>(stoi(segundo_operando)).to_string();
		
		// label
		getline(ss, offset, '\n');
		offset.erase(remove(offset.begin(), offset.end(), ' '), offset.end());
		bool achou_label = false;
		for(auto it = labels.begin(); it != labels.end(); it++){
			
			if(offset == (*it).first){
				
				offset = bitset<16>((*it).second).to_string();
				
				achou_label = true;
			}
		}
		if(!achou_label)
			throw runtime_error("ERRO LEITURA DA LABEL - a label lida nao existe no conjunto de labels!");
			
		switch(instrucao){
			
			case 15:
				opcode = "000100";
				break;
				
			case 16:
				opcode = "000101";
				break;
				
			// bge rs rt label (pseudo-instrucao) = 
			//  	slt at, rs, rt
			//  	beq at, zero, label
			case 17:
				// slt
				opcode = "000000";
				string at_address = "00001";
				string shamt = "00000";
				string funct = "101010";
				instrucao_decodificada = opcode + primeiro_operando + segundo_operando + at_address + shamt + funct + "\n";
				
				// beq
				opcode = "000100";
				string zero_address = "00000";
				instrucao_decodificada += opcode + at_address + zero_address + offset;
				break;
		}
	}
	
	if(instrucao != 17)
		instrucao_decodificada = opcode + primeiro_operando + segundo_operando + offset;
		
	return instrucao_decodificada;
}

string formato_instrucao_J(int instrucao, string texto){
	
	string opcode = "000000";
	string target = "00000000000000000000000000";
	
	stringstream ss(texto);
	string lixo;
	getline(ss, lixo, ' ');
	
	getline(ss, target, '\n');
	bool achou_target = false;
	for(auto it = labels.begin(); it != labels.end(); it++){
		
		if(target == (*it).first){
			
			target = bitset<26>((*it).second).to_string();
			
			achou_target = true;
		}
	}
	if(!achou_target)
		throw runtime_error("ERRO LEITURA DA TARGET - a target lida nao existe no conjunto de labels!");
		
	if(instrucao == 18){
		
		opcode = "000010";
	}
	
	if(instrucao == 19){
		
		opcode = "000011";
	}
	
	string instrucao_decodificada = opcode + target;
	
	return instrucao_decodificada;
}

int main(){
	
	string nome_arquivo_entrada;
	
	cout << "Digite o nome do arquivo de entrada assembly MIPS (sem extensao): ";
	cin >> nome_arquivo_entrada;
	
	ifstream arquivo_entrada("programas/" + nome_arquivo_entrada + ".txt");
	
	if(arquivo_entrada){
		
		string texto;
		int numero_instrucao;
		bool integridade = true;
		
		ofstream arquivo_saida("data/instrucoes_binario.bin", ios::binary);
		
		// primeiro laco para adquirir as possiveis labels
		while(texto != "EOF"){
			
			// leitura de uma linha
			getline(arquivo_entrada, texto);
			
			// se achar ':', ou seja, se achar uma label
			bool label = false;
			char dois_pontos = ':';
			size_t found_2_pontos = texto.find(dois_pontos);
			if(found_2_pontos != string::npos){
				
				label = true;
				
				texto.erase(remove(texto.begin(), texto.end(), ':'), texto.end());
				
				labels.push_back(make_pair(texto, memoria_texto));
			}
			
			// leitura da instrucao (add, sub, lw, sw, ...)
			if(!label){
				
				string instrucao = "EOF";
				
				char espaco = ' ';
				size_t found_esp = texto.find(espaco);
				if(found_esp != string::npos){
					
					char char_instrucao[found_esp];
					
					for(unsigned int i = 0; i < found_esp; i++)
						char_instrucao[i] = texto[i];
						
					instrucao.assign(char_instrucao, char_instrucao + found_esp);
				}
				
				try{
					
					numero_instrucao = retornar_instrucao(instrucao);
					if(numero_instrucao != 20 and numero_instrucao != 17)
						memoria_texto += 32;
						
					// bge
					if(numero_instrucao != 20 and numero_instrucao == 17)
						memoria_texto += 64;
				}
				catch(exception& e){
					
					cout << e.what();
					integridade = false;
				}
			}
		}
		
		arquivo_entrada.seekg(0, arquivo_entrada.beg);
		
		// outro laco para ler as intrucoes em si e armazenas em um arquivo binario
		while(integridade){
			
			getline(arquivo_entrada, texto);
			cout << texto << endl;
			
			bool label = false;
			char dois_pontos = ':';
			size_t found_2_pontos = texto.find(dois_pontos);
			if(found_2_pontos != string::npos)
				label = true;
				
			if(!label){
				
				string instrucao = "EOF";
				
				char espaco = ' ';
				size_t found_esp = texto.find(espaco);
				if(found_esp != string::npos){
					
					char char_instrucao[found_esp];
					
					for(unsigned int i = 0; i < found_esp; i++)
						char_instrucao[i] = texto[i];
						
					instrucao.assign(char_instrucao, char_instrucao + found_esp);
				}
				
				try{
					
					numero_instrucao = retornar_instrucao(instrucao);
					if(numero_instrucao == 20)
						break;
				}
				catch(exception& e){
					
					cout << e.what();
					integridade = false;
				}
				
				if(integridade){
					
					string resultado;
					
					if(numero_instrucao >= 0 and numero_instrucao <= 11)
						resultado = formato_instrucao_R(numero_instrucao, texto);
						
					try{
						
						if(numero_instrucao >= 12 and numero_instrucao <= 17)
							resultado = formato_instrucao_I(numero_instrucao, texto);
							
						if(numero_instrucao == 18 or numero_instrucao == 19)
							resultado = formato_instrucao_J(numero_instrucao, texto);
					}
					catch(exception& e){
						
						cout << e.what();
						integridade = false;
					}
					
					cout << resultado << endl;
					
					// bge
					if(numero_instrucao == 17)
						for(int i = 0; i < 65; i++)
							if(resultado[i] != '\n')
								arquivo_saida.write((char*)(&resultado[i]), sizeof(char));
								
					if(numero_instrucao != 17)
						for(int i = 0; i < 32; i++)
							arquivo_saida.write((char*)(&resultado[i]), sizeof(char));
				}
			}
		}
		
		arquivo_saida.close();
		arquivo_entrada.close();
		
		if(!integridade)
			remove("instrucoes_binario.bin");
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
