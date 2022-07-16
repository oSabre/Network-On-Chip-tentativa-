

#include <iostream>
#include "systemc.h"
// Controlador de fluxo de entrada. 4 por roteador
SC_MODULE (fluxcontrol_e){

	sc_in <int> inputf; // Saida do fluxo de saida de outro roteador
	sc_out<int> outputf; // Vai pra o fluxo de saida de outro roteador com o ACK;
	sc_signal<bool> val; //Pra saber se pode executar a função que cuida do fluxo de entrada em modules.h

	// Se chegar algum valor, valid = true
	void entrada(){
		if(inputf.read() == 1){
			val.write(true);
		}else{
			saida(0);
		}

	}

	void saida(int x){
		val = false;
		//cout << "Result: " << x <<endl;
		outputf.write(x);
	}

	bool getVal(){
		return val;
	}

	SC_CTOR(fluxcontrol_e){
		val = false;

		SC_METHOD(entrada){
			sensitive << inputf;
		}
	}
	
};

// Recebe solicitação pra enviar de alguém
// Ao receber, entra no estado solicitando e manda uma mensagem pra entrada de fluxo do próximo roteador
// Roda a função EntradaControleDeFluxo no modules.h
// Volta pra cá na função recebe, manda a resposta da solicitação pra o alguém que pediu a solicitação.
SC_MODULE (fluxcontrol_s)
{
	sc_in <int> ack; // Comunicação de controladores de fluxo (retorno)
	sc_out<int> saida; // Comunicação de controladores de fluxo (envio)
	sc_signal<int> resSol;
	bool solicitando = false;

	void recebeSolicitacao(){
		if(solicitando == false){
			//cout << "Enviou solicitação" << endl;
			solicitando = true;
			manda(1);
		}
	}

	void manda(int x){
		saida.write(x);
	}

	int getResSol(){
		return resSol;
	}

	void recebe(){
		if(ack.read() == 1 && solicitando == true){
			//cout << "Recebeu respota positiva, pode mandar." << endl;
			resSol = 1;
			solicitando = false;
		}else if(ack.read() == 0){
			resSol = 0;
			solicitando = false;
		}
		manda(0);
	}

	SC_CTOR(fluxcontrol_s){
		resSol = 0;
		SC_METHOD(recebe);
			sensitive << ack;	
	}
	
};
