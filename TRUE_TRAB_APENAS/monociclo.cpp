#include <iostream>
#include <fstream>
#include <bitset>
#include <exception>
#include <limits.h>

using namespace std;

#define numero_total_enderecos 64000
#define tamanho_palavra 4
#define tamanho_instrucao 32
#define comeco_memoria_texto 256
#define comeco_memoria_dados 3456
#define final_memoria_dados 255999

long conversor_com_sinal_binario(string bin){
	
	long retorno = 0;
	bool negativo = false;
	
	// binario negativo
	if(bin[0] == '1'){
		
		negativo = true;
		
		for(int i = bin.size() - 1; i >= 0; i--){
			
			if(bin[i] == '1')
				bin[i] = '0';
				
			else if(bin[i] == '0')
				bin[i] = '1';
		}
		
		int k = bin.size() - 1;
		bool achou_1 = false;
		
		while(k >= 0 and !achou_1){
			
			if(bin[k] == '1'){
				
				achou_1 = true;
				
				while(bin[k] == '1'){
					
					bin[k] = '0';
					k--;
				}
				
				bin[k] = '1';
			}
			
			k--;
		}
		
		if(!achou_1)
			bin[bin.size() - 1] = '1';
	}
	
	long potencia_dois = 1;
	
	for(int i = bin.size() - 1; i >= 0; i--){
		
		if(bin[i] == '1')
			retorno += potencia_dois;
			
		potencia_dois *= 2;
	}
	
	if(negativo)
		retorno *= -1;
		
	return retorno;
}

unsigned long conversor_sem_sinal_binario(string bin){
	
	long retorno = 0;
	long potencia_dois = 1;
	
	for(int i = bin.size() - 1; i >= 0; i--){
		
		if(bin[i] == '1')
			retorno += potencia_dois;
			
		potencia_dois *= 2;
	}
	
	return retorno;
}

#include "headers/registradores.h"
#include "headers/memoria.h"
#include "headers/estagio_IF.h"
#include "headers/controle.h"
#include "headers/estagio_EX.h"
#include "headers/estagio_MEM.h"
#include "headers/estagio_WR.h"

int main(){
	
	ifstream arquivo_entrada("data/instrucoes_binario.bin", ios::binary);
	
	if(arquivo_entrada){
		
		Memoria *mem = NULL;
		bool integridade = true;
		
		try{
			
			mem = new Memoria(arquivo_entrada);
		}
		catch(exception& e){
			
			cout << e.what();
			delete mem;
			integridade = false;
		}
		
		if(integridade){
			
			Registradores *reg = new Registradores();
			IF *estagio_IF = new IF(mem);
			Controle *controle = new Controle(estagio_IF);
			EX *estagio_execucao = new EX(reg, estagio_IF, controle);
			MEM *estagio_memoria = new MEM(mem, reg, controle, estagio_execucao);
			WR *estagio_registrador = new WR(reg, controle, estagio_execucao, estagio_memoria);
			
			while(estagio_IF->verificar_instrucao()){
				
				estagio_IF->mostrar_valor_atual_PC();
				
				try{
					
					controle->decodificar_instrucao();
				}
				catch(exception& e){
					
					cout << e.what();
					break;
				}
				
				controle->depuracao_controle();
				
				estagio_execucao->ALU();
				estagio_execucao->mostrar_flags_desvio();
				
				try{
					
					estagio_memoria->leitura_escrita_memoria();
				}
				catch(exception& e){
					
					cout << e.what();
					break;
				}
				
				estagio_registrador->leitura_escrita_registrador();
				
				controle->reset();
				estagio_execucao->reset_flags_desvio_variaveis();
				
				estagio_IF->trocar_instrucao();
				
				cout << endl << "Digite enter para ler a proxima instrucao . . . ";
				cin.get();
				cout << endl;
			}
			
			delete mem;
			delete reg;
			delete estagio_IF;
			delete controle;
			delete estagio_execucao;
			delete estagio_memoria;
			delete estagio_registrador;
		}
		
		arquivo_entrada.close();
	}
	
	else
		cout << "Arquivo de entrada nao foi encontrado!" << endl;
		
	return 0;
}
