
#include <iostream>
#include <queue>
#include <systemc.h>

using namespace std;

/* Buffer -> Temos 8 num total
	4 de entrada
	4 de saida
	Os buffers de saida recebem os dados do Mux
	Os buffers de entrada recebem dados de outros roteadores
*/
SC_MODULE (buffer){

public:
	std::queue< sc_uint <32> > data_flit; // Buffer de fato
	sc_in<bool> enable;
	//32 bits
	//0123 - Destino X
	//4568 - Destino Y
	// Resto = Payload
	sc_in<sc_uint <32> > data; // Entrada do buffer
	sc_out<sc_uint <32> > data_out; // Saida do buffer
	sc_signal<int> entrou; // Essa variável serve pra dar trigger em funções do roteador.

	void add(){
		if(enable){
			data_flit.push(data.read());
			//cout << "Entrando num buffer: " << data_flit.front() << endl;
			entrou = entrou+1;
		}
	}

	void consome(){
		if(enable){
			//cout << "Saindo de um buffer: " << data_flit.front() << endl;
			data_out.write(data_flit.front());
			data_flit.pop();
		}
	}

	void consome2(){
		data_flit.pop();
	}

	int espaco(){
		if(data_flit.size() < 8){
			return 1;
		}else{
			return 0;
		}
	}

	sc_uint<32> coordenadaFrente(){
		return data_flit.front();
	}

	bool vazio(){
		return data_flit.empty();
	}

	SC_CTOR(buffer){
		entrou = 0;
        SC_METHOD(add);
        	sensitive << data;
	}
};

/*
	Mux. Temos 4 ao total (por roteador);
*/
SC_MODULE(mux){
	sc_in<sc_uint<32> > e1, e2, e3, e4; //Entradas
	int sel = 5;
	bool enable = false;
	sc_out<sc_uint<32> > saida; 
	sc_uint<32> envio; //Podia ter ficado sem isso, mas eventualmente eu escrevi ela e só deixei ai
	//sc_in_clk rel;

	/*
	void ativo(){
		if(enable){
			switch(sel){
				case 0:
					//cout << "Chegou do buffer norte: " << e1.read() << endl;
					envio = e1.read();
					saida.write(envio);
					setEnable(false);
					break;
				case 1:
					//cout << "Chegou do buffer sul: " << e2.read() << endl;
					envio = e2.read();
					saida.write(envio);
					setEnable(false);
					break;
				case 2:
					//cout << "Chegou do buffer leste: " << e3.read() << endl;
					envio = e3.read();
					saida.write(envio);
					setEnable(false);
					break;
				case 3:
					//cout << "Chegou do buffer oeste: " << e4.read() << endl;
					envio = e4.read();
					saida.write(envio);
					setEnable(false);
					break;
			}
		}
	}
	*/

	void manda1(){
		if(enable){
			//cout << "Chegou do buffer norte: " << e1.read() << endl;
			envio = e1.read();
			saida.write(envio);
		}
	}
	void manda2(){
		if(enable){
			//cout << "Chegou do buffer sul: " << e2.read() << endl;
			envio = e2.read();
			saida.write(envio);
			
		}
	}

	void manda3(){
		if(enable){
			//cout << "Chegou do buffer leste: " << e3.read() << endl;
			envio = e3.read();
			saida.write(envio);
			
		}
	}

	void manda4(){
		if(enable){
			//cout << "Chegou do buffer oeste: " << e4.read() << endl;
			envio = e4.read();
			saida.write(envio);
		}
	}


	void setSel(int x){
		sel = x;
	}

	void setEnable(bool e){
		enable = e;
	}

	SC_CTOR(mux){ 
		
		SC_METHOD(manda1){
			sensitive << e1;
		}
		SC_METHOD(manda2){
			sensitive << e2;
		}
		SC_METHOD(manda3){
			sensitive << e3;
		}
		SC_METHOD(manda4){
			sensitive << e4;
		}


	}	
};
