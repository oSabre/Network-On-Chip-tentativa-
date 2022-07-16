#ifndef _MODULES_H_
#define _MODULES_H_ 

#include <systemc.h>
#include <iostream>
#include "buffer.h"
#include "fluxcontrol.h"

using namespace std;

SC_MODULE(roteador){
	//Coordenadas do roteador
	sc_in<bool> enab;
	sc_in<int> x,y;
	// Relógio
	sc_in_clk clock;
	//Entradas e saidas Controle Fluxo Entrada
	sc_in<int> efn, efs, efl, efo;
	sc_out<int> sfn, sfs, sfl, sfo;
	//Entradas e saidas Controle Fluxo Saida
	sc_in<int> efsn, efss, efsl, efso;
	sc_out<int> sfsn, sfss, sfsl, sfso;
	//Entrada dos Buffers de Entrada.
	sc_in<sc_uint <32> > EB_norte, EB_sul, EB_leste, EB_oeste;
	// Buffers Entrada
	buffer *b_norte, *b_sul, *b_leste, *b_oeste;
	// Buffers Saída
	buffer *bs_norte, *bs_sul, *bs_leste, *bs_oeste; 
	// Muxes
	mux *m_norte, *m_sul, *m_leste, *m_oeste;
	// Controladores de fluxo
	fluxcontrol_e *fe_norte, *fe_sul, *fe_leste, *fe_oeste;
	fluxcontrol_s *fs_norte, *fs_sul, *fs_leste, *fs_oeste;
	// Saída dos buffers de Saída
	sc_out< sc_uint <32> > SBS_norte, SBS_sul, SBS_leste, SBS_oeste;

	//Sinal Mux - Buffer Saida;
	sc_signal< sc_uint<32> > muxLOCbs, muxNbs, muxSbs, muxLbs, muxObs;
	//Sinal Buffer Entrada -> Mux
	sc_signal< sc_uint<32> > beLOCmux, beNmux, beSmux, beLmux, beOmux;

	// Quando chegar algum valor no Controle de fluxo de entrada
	// O valid vai ficar true e essa função vai rodar.
	// Checa se há espaço no buffer, se sim retorna para o roteador anterior o ACK.
	void EntradaControledeFluxo(){

		if(fe_norte->getVal() == true){
			//cout << "entrando no controle de fluxo de entrada norte "<< x << y << endl;
			if(b_norte->espaco()){
				fe_norte->saida(1);
			}else{
				fe_norte->saida(0);
			}
		}

		if(fe_sul->val == true){
			//cout << "entrando no controle de fluxo de entrada sul "<< x << y << endl;
			if(b_sul->espaco()){
				fe_sul->saida(1);
			}else{
				fe_sul->saida(0);
			}
		}

		if(fe_leste->val == true){
			//cout << "entrando no controle de fluxo de entrada leste "<< x << y << endl;
			if(b_leste->espaco()){
				fe_leste->saida(1);
			}else{
				fe_leste->saida(0);
			}
		}

		if(fe_oeste->val == true){
			//cout << "entrando no controle de fluxo de entrada oeste "<< x << y << endl;
			if(b_oeste->espaco()){
				//cout << "Enviando de volta saida 1" << endl;
				fe_oeste->saida(1);
			}else{
				//cout << "Enviando de volta saida 0" << endl;
				fe_oeste->saida(0);
			}
		}

	}

	void ControleDeFluxoSaidaN(){
		if(!bs_norte->vazio()){
			fs_norte->recebeSolicitacao();
		}
	}
	void EnvioFSN(){
		int z = fs_norte->getResSol();
		
		if(z == 1){
			bs_norte->consome();
		}
	}

	void ControleDeFluxoSaidaS(){
		if(!bs_sul->vazio()){
			
			fs_sul->recebeSolicitacao();
		}
	}
	void EnvioFSS(){
		int z = fs_sul->getResSol();
		if(z == 1){
			bs_sul->consome();
		}
	}

	void ControleDeFluxoSaidaL(){
		if(!bs_leste->vazio()){
			
			fs_leste->recebeSolicitacao();
		}
	}
	void EnvioFSL(){
		int z = fs_leste->getResSol();
		if(z == 1){
			bs_leste->consome();
		}
	}

	void ControleDeFluxoSaidaO(){
		if(!bs_oeste->vazio()){
			fs_oeste->recebeSolicitacao();
		}
	}
	void EnvioFSO(){
		int z = fs_oeste->getResSol();

		if(z == 1){
			bs_oeste->consome();
		}
	}

	void roteamento(){
		if(!b_norte->vazio()){
			// Se o buffer não estiver vazio, tenta ler as coordenadas dele
			sc_uint<32> pacote = b_norte->coordenadaFrente();
			int destinoX = pacote[0]*1 + pacote[1]*2 + pacote[2]*4 + pacote[3]*8;
			int destinoY = pacote[4]*1 + pacote[5]*2 + pacote[6]*4 + pacote[7]*8;

			int xResult = destinoX - x;
			int yResult = destinoY - y;

			//cout << "Norte do roteador "<<x<<y<<". Caminho: "<< xResult << yResult << endl;

			// Se tive espaço no buffer de saida destino, envia.
			if(xResult != 0){
				if(xResult > 0){
					if(bs_leste->espaco()){
						m_leste->setSel(0);
						m_leste->setEnable(true);
						b_norte->consome();
						//return;
					}
				}else if(xResult < 0){
					if(bs_oeste->espaco()){
						m_oeste->setSel(0);
						m_oeste->setEnable(true);
						b_norte->consome();
						//return;
					}
				}
			}else if(destinoY != 0){
				if(yResult > 0){
					// Sul
					if(bs_sul->espaco()){
						m_sul->setSel(0);
						m_sul->setEnable(true);
						b_norte->consome();
						
						//return;
					}
				}else if(yResult < 0){
					// Norte
					if(bs_norte->espaco()){
						m_norte->setSel(0);
						m_norte->setEnable(true);
						b_norte->consome();
						//return;
					}
				}
			}
			if(xResult == 0 && yResult == 0){
				//Local
				b_norte->consome2();
				cout << "Chegou! Esse é o roteador " << x <<" " << y << endl;
			}

			return;
		}

		if(!b_sul->vazio()){
			// Se o buffer não estiver vazio, tenta ler as coordenadas dele
			sc_uint<32> pacote = b_sul->coordenadaFrente();
			int destinoX = pacote[0]*1 + pacote[1]*2 + pacote[2]*4 + pacote[3]*8;
			int destinoY = pacote[4]*1 + pacote[5]*2 + pacote[6]*4 + pacote[7]*8;

			int xResult = destinoX - x;
			int yResult = destinoY - y;

			//cout << "Sul do roteador "<<x << y <<". Caminho: "<< xResult << yResult << endl;

			// Se tive espaço no buffer de saida destino, envia.
			if(xResult != 0){
				if(xResult > 0){
					if(bs_leste->espaco()){
						m_leste->setSel(1);
						m_leste->setEnable(true);
						b_sul->consome();
						
						//return;
					}
				}else if(xResult < 0){
					if(bs_oeste->espaco()){
						m_oeste->setSel(1);
						m_oeste->setEnable(true);
						b_sul->consome();
						
						//return;
					}
				}
			}else if(destinoY != 0){
				if(yResult > 0){
					// Sul
					if(bs_sul->espaco()){
						m_sul->setSel(1);
						m_sul->setEnable(true);
						b_sul->consome();
						//return;
						
					}
				}else if(yResult < 0){
					// Norte
					if(bs_norte->espaco()){
						m_norte->setSel(1);
						m_norte->setEnable(true);
						b_sul->consome();
						
						//return;
					}
				}
			}
			if(xResult == 0 && yResult == 0){
				//Local
				b_sul->consome2();
				cout << "Chegou! Esse é o roteador " << x <<" " << y << endl;
			}
			return;
		}

		if(!b_leste->vazio()){
			// Se o buffer não estiver vazio, tenta ler as coordenadas dele
			sc_uint<32> pacote = b_leste->coordenadaFrente();
			int destinoX = pacote[0]*1 + pacote[1]*2 + pacote[2]*4 + pacote[3]*8;
			int destinoY = pacote[4]*1 + pacote[5]*2 + pacote[6]*4 + pacote[7]*8;

			int xResult = destinoX - x;
			int yResult = destinoY - y;

			//cout << "Leste do roteador "<< x << y << ". Caminho: "<< xResult << yResult << endl;

			// Se tive espaço no buffer de saida destino, envia.
			if(xResult != 0){
				if(xResult > 0){
					if(bs_leste->espaco()){
						m_leste->setSel(2);
						m_leste->setEnable(true);
						b_leste->consome();
						

						//return;
					}
				}else if(xResult < 0){
					if(bs_oeste->espaco()){
						m_oeste->setSel(2);
						m_oeste->setEnable(true);
						b_leste->consome();
						
						//return;
					}
				}
			}else if(destinoY != 0){
				if(yResult > 0){
					// Sul
					if(bs_sul->espaco()){
						m_sul->setSel(2);
						m_sul->setEnable(true);
						b_leste->consome();
						//m_sul->manda();
						//return;
					}
				}else if(yResult < 0){
					// Norte
					if(bs_norte->espaco()){
						m_norte->setSel(2);
						m_norte->setEnable(true);
						b_leste->consome();
						
						//return;
					}
				}
			}
			if(xResult == 0 && yResult == 0){
				//Local
				b_leste->consome2();
				cout << "Chegou! Esse é o roteador " << x <<" " << y << endl;
			}
			return;
		}

		if(!b_oeste->vazio()){
			// Se o buffer não estiver vazio, tenta ler as coordenadas dele
			sc_uint<32> pacote = b_oeste->coordenadaFrente();
			int destinoX = pacote[0]*1 + pacote[1]*2 + pacote[2]*4 + pacote[3]*8;
			int destinoY = pacote[4]*1 + pacote[5]*2 + pacote[6]*4 + pacote[7]*8;

			int xResult = destinoX - x;
			int yResult = destinoY - y;

			//cout << "Oeste do roteador "<< x << y << ". Caminho: "<< xResult << yResult << endl;

			// Se tive espaço no buffer de saida destino, envia.
			if(xResult != 0){
				if(xResult > 0){
					if(bs_leste->espaco()){
						m_leste->setSel(3);
						m_leste->setEnable(true);
						b_oeste->consome();
						
						//return;
					}
				}else if(xResult < 0){
					if(bs_oeste->espaco()){
						m_oeste->setSel(3);
						m_oeste->setEnable(true);
						b_oeste->consome();
						
						//return;
					}
				}
			}else if(destinoY != 0){
				if(yResult > 0){
					// Sul
					if(bs_sul->espaco()){
						m_sul->setSel(3);
						m_sul->setEnable(true);
						b_oeste->consome();
					
						//return;
					}
				}else if(yResult < 0){
					// Norte
					if(bs_norte->espaco()){
						m_norte->setSel(3);
						m_norte->setEnable(true);
						b_oeste->consome();
						//return;
					}
				}
			}
			if(xResult == 0 && yResult == 0){
				//Local
				b_oeste->consome2();
				cout << "Chegou! Esse é o roteador " << x <<" " << y << endl;
			}
			return;
		}
	}


	SC_CTOR(roteador){
		//Iniciação
		fe_norte = new fluxcontrol_e("EntradaNorte");
		fe_sul = new fluxcontrol_e("EntradaSul");
		fe_leste= new fluxcontrol_e("EntradaLeste");
		fe_oeste = new fluxcontrol_e("EntradaOeste");
		//fe_local = new fluxcontrol_e("EntradaLocal");

		fs_norte = new fluxcontrol_s("SaidaFluxoNorte");
		fs_sul = new fluxcontrol_s("SaidaFluxoSul");
		fs_leste = new fluxcontrol_s("SaidaFluxoLeste");
		fs_oeste = new fluxcontrol_s("SaidaFluxoOeste");

		b_norte = new buffer("bufferEntradaNorte");
		b_sul = new buffer("bufferEntradaSul");
		b_leste= new buffer("bufferEntradaLeste");
		b_oeste = new buffer("bufferEntradaOeste");
		//b_local = new buffer("bufferEntradalocal");

		bs_norte = new buffer("bufferSaidaNorte");
		bs_sul = new buffer("bufferSaidaSul");
		bs_leste= new buffer("bufferSaidaLeste");
		bs_oeste = new buffer("bufferSaidaOeste");
		//bs_local = new buffer("bufferSaidalocal");

		m_norte = new mux("muxNorte");
		m_sul = new mux("muxSul");
		m_leste= new mux("muxLeste");
		m_oeste = new mux("muxOeste");
		//m_local = new mux("muxlocal");
		
		//Conexões
		//Entradas e saidas dos controladores de fluxo de entrada
		fe_norte->inputf(efn);
		fe_sul->inputf(efs);
		fe_leste->inputf(efl);
		fe_oeste->inputf(efo);
		//fe_local->inputf(efloc);

		fe_norte->outputf(sfn);
		fe_sul->outputf(sfs);
		fe_leste->outputf(sfl);
		fe_oeste->outputf(sfo);
		//fe_local->outputf(sfloc);

		//Entradas e saidas dos controladores de fluxo de saída
		fs_norte->ack(efsn);
		fs_sul->ack(efss);
		fs_leste->ack(efsl);
		fs_oeste->ack(efso);

		fs_norte->saida(sfsn);
		fs_sul->saida(sfss);
		fs_leste->saida(sfsl);
		fs_oeste->saida(sfso);

		//Entradas dos buffers de entrada
		b_norte->data(EB_norte);
		b_sul->data(EB_sul);
		b_leste->data(EB_leste);
		b_oeste->data(EB_oeste);
		//b_local->data(EB_local);

		//Saida dos buffers de entrada
		b_norte->data_out(beNmux);
		b_sul->data_out(beSmux);
		b_leste->data_out(beLmux);
		b_oeste->data_out(beOmux);
		//b_local->data_out(beLOCmux);

		b_norte->enable(enab);
		b_sul->enable(enab);
		b_leste->enable(enab);
		b_oeste->enable(enab);

		bs_norte->enable(enab);
		bs_sul->enable(enab);
		bs_leste->enable(enab);
		bs_oeste->enable(enab);
		
		//Entrada Muxes
		m_norte->e1(beNmux);
		m_norte->e2(beSmux);
		m_norte->e3(beLmux);
		m_norte->e4(beOmux);

		m_sul->e1(beNmux);
		m_sul->e2(beSmux);
		m_sul->e3(beLmux);
		m_sul->e4(beOmux);

		m_leste->e1(beNmux);
		m_leste->e2(beSmux);
		m_leste->e3(beLmux);
		m_leste->e4(beOmux);

		m_oeste->e1(beNmux);
		m_oeste->e2(beSmux);
		m_oeste->e3(beLmux);
		m_oeste->e4(beOmux);

		// Saida Mux
		//m_local->saida(muxLOCbs);
		m_norte->saida(muxNbs);
		m_sul->saida(muxSbs);
		m_leste->saida(muxLbs);
		m_oeste->saida(muxObs);

		// Entrada Buffer de saída
		bs_norte->data(muxNbs);
		bs_sul->data(muxSbs);
		bs_leste->data(muxLbs);
		bs_oeste->data(muxObs);
		//bs_local->data(muxLOCbs);

		// Saida Buffer de Saída
		bs_norte->data_out(SBS_norte);
		bs_sul->data_out(SBS_sul);
		bs_leste->data_out(SBS_leste);
		bs_oeste->data_out(SBS_oeste);

		SC_METHOD(EntradaControledeFluxo){
			//sensitive << clock.pos();
			sensitive << fe_norte->val;
			sensitive << fe_sul->val;
			sensitive << fe_leste->val;
			sensitive << fe_oeste->val;
		}

		SC_METHOD(roteamento){
			sensitive << clock.pos();
			//sensitive << b_norte->entrou;
			//sensitive << b_sul->entrou;
			//sensitive << b_leste->entrou;
			//sensitive << b_oeste->entrou;
		}

		SC_METHOD(ControleDeFluxoSaidaN){
			sensitive << bs_norte->entrou;
		}
		SC_METHOD(EnvioFSN){
			sensitive << fs_norte->resSol;
		}

		SC_METHOD(ControleDeFluxoSaidaS){
			sensitive << bs_sul->entrou;
		}
		SC_METHOD(EnvioFSS){
			sensitive << fs_sul->resSol;
		}

		SC_METHOD(ControleDeFluxoSaidaL){
			sensitive << bs_leste->entrou;
		}
		SC_METHOD(EnvioFSL){
			sensitive << fs_leste->resSol;
		}

		SC_METHOD(ControleDeFluxoSaidaO){
			sensitive << bs_oeste->entrou;
		}
		SC_METHOD(EnvioFSO){
			sensitive << fs_oeste->resSol;
		}
	}


};
#endif