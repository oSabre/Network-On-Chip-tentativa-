#include <systemc.h>
#include <iostream>
#include "modules.h"

using namespace std;

int sc_main (int argc, char* argv[]) {
	roteador rot1("Roteador1");
	roteador rot2("Roteador2");
	roteador rot3("Roteador3");
	roteador rot4("Roteador4");

	sc_clock relogio("clock", 1, SC_NS, 1, 1, SC_NS);
	sc_signal<bool> englob;
	englob = false;
	// FLuxo Entrda Rot1
	sc_signal<int> ENFLUXS1, ENFLUXL1;
	sc_signal<int> SFLUXS1, SFLUXL1;

	// Fluxo Saida Rot1
	sc_signal<int> SENFLUXS1, SENFLUXL1;
	sc_signal<int> SSFLUXS1, SSFLUXL1;

	sc_signal< sc_uint<32> > eben, ebes, ebel, ebeo;
	sc_signal< sc_uint<32> > sbsn, sbss, sbsl, sbso;

	// FLuxo Entrda Rot2
	sc_signal<int> ENFLUXS2, ENFLUXO2;
	sc_signal<int> SFLUXS2, SFLUXO2;

	// Fluxo Saida Rot2
	sc_signal<int> SENFLUXS2, SENFLUXO2;
	sc_signal<int> SSFLUXS2, SSFLUXO2;

	sc_signal< sc_uint<32> > eben2, ebes2, ebel2, ebeo2;
	sc_signal< sc_uint<32> > sbsn2, sbss2, sbsl2, sbso2;

	// Sinais entre Rot3 e rot4
	sc_signal<int> FSS3FEE4, FSE3FES4, FEE3FSS4, FES3FSE4;
	sc_signal< sc_uint<32> > SBSL3EBEO4, EBEL3SBSO4;

	sc_signal< sc_uint<32> > eben4, ebes4, ebel4, ebeo4;
	sc_signal< sc_uint<32> > sbsn4, sbss4, sbsl4, sbso4;

	sc_signal< sc_uint<32> > eben3, ebes3, ebel3, ebeo3;
	sc_signal< sc_uint<32> > sbsn3, sbss3, sbsl3, sbso3;

	//Sinais extas
	sc_signal<int> extraF;

	sc_signal<int> v1, v0;

	v1 = 2;
	v0 = 1;

	rot1.x(v0);
	rot1.y(v0);
	rot2.x(v1);
	rot2.y(v0);
	rot3.x(v0);
	rot3.y(v1);
	rot4.x(v1);
	rot4.y(v1);

	// Conexões
	rot1.clock(relogio);
	rot1.enab(englob);

	// Entrada Controle de fluxo Entrada (int)
	rot1.efn(extraF);
	rot1.efs(ENFLUXS1); //Conectado com saida de controle de fluxo de saida norte Rot3;
	rot1.efl(ENFLUXL1); //Conectado com saida de controle de fluxo de saida oeste Rot2;
	rot1.efo(extraF);
	// Saida controle de fluxo entrada (int)
	rot1.sfn(extraF);
	rot1.sfs(SFLUXS1); //Conectado com entrada de controle de fluxo de saida norte Rot3;
	rot1.sfl(SFLUXL1); //Conectado com entrada de controle de fluxo de saida oeste Rot2;
	rot1.sfo(extraF);
	// Entrada controle de fluxo saida (int);
	rot1.efsn(extraF);
	rot1.efss(SENFLUXS1); //Conectado com saida de controle de fluxo de entrada norte Rot3;
	rot1.efsl(SENFLUXL1); //Conectado com saida de controle de fluxo de entrada oeste Rot2;
	rot1.efso(extraF);
	// Saida controle de fluxo saida (int);
	rot1.sfsn(extraF);
	rot1.sfss(SSFLUXS1); // Conectado com entrada de controle de fluxo entrada norte Rot3;
	rot1.sfsl(SSFLUXL1); // Conectado com entrada de controle de fluxo entrada oeste Rot2;
	rot1.sfso(extraF);
	// Entrada buffer de entrada (sc_uint<32>)
	rot1.EB_norte(eben);
	rot1.EB_sul(ebes); //Conectado com a saida do buffer de saida norte rot3
	rot1.EB_leste(sbso2); //Conectado com a saida do buffer de saida oeste Rot2
	rot1.EB_oeste(ebeo);
	// Saida Buffer de saida (sc_uint<32>)
	rot1.SBS_norte(sbsn);
	rot1.SBS_sul(sbss); //Conectado com a entrada do buffer de entrada norte Rot3
	rot1.SBS_leste(sbsl); //Conectado com a entrada do buffer de entrada oeste Rot2;
	rot1.SBS_oeste(sbso);

	rot2.clock(relogio);
	rot2.enab(englob);

	// Entrada Controle de fluxo Entrada (int)
	rot2.efn(extraF);
	rot2.efs(ENFLUXS2); // Conectado com saida de controle de fluxo saida norte Rot4;
	rot2.efl(extraF);
	rot2.efo(SSFLUXL1); // Conectado com saida de controle de fluxo saida leste Rot1;
	// Saida controle de fluxo entrada (int)
	rot2.sfn(extraF);
	rot2.sfs(SFLUXS2); // Conectado com entrada de controle de fluxo saida norte Rot4;
	rot2.sfl(extraF);
	rot2.sfo(SENFLUXL1); // Conectado com entrada de controle de fluxo saida leste Rot1;
	// Entrada controle de fluxo saida (int);
	rot2.efsn(extraF);
	rot2.efss(SENFLUXS2); //Conectado com saida de controle de fluxo de entrada norte Rot4;
	rot2.efsl(extraF);
	rot2.efso(SFLUXL1); // Conectado com saida de controle de fluxo de entrada leste Rot1;
	// Saida controle de fluxo saida (int);
	rot2.sfsn(extraF);
	rot2.sfss(SSFLUXS2); //Conectado com entrada de controle de fluxo de entrada norte Rot4;
	rot2.sfsl(extraF);
	rot2.sfso(ENFLUXL1); //Conectado com entrada de controle de fluxo de entrada leste Rot1;
	// Entrada buffer de entrada (sc_uint<32>)
	rot2.EB_norte(eben2);
	rot2.EB_sul(ebes2); //Conectado com a saida do buffer de saida norte rot4;
	rot2.EB_leste(ebel2);
	rot2.EB_oeste(sbsl); // Conectado com a saida do buffer de saida leste Rot1;
	// Saida Buffer de saida (sc_uint<32>)
	rot2.SBS_norte(sbsn2);
	rot2.SBS_sul(sbss2); //Conectado com entrada buffer de entrada norte rot4;
	rot2.SBS_leste(sbsl2);
	rot2.SBS_oeste(sbso2); // Conectado com a entrada buffer de entrada leste Rot1;

	rot3.clock(relogio);
	rot3.enab(englob);

	// Entrada Controle de fluxo Entrada (int)
	rot3.efn(SSFLUXS1); // Conectado com Saida controle de fluxo saida sul Rot1
	rot3.efs(extraF);
	rot3.efl(FEE3FSS4); // Conectado com saida controle de fluxo saida oeste Rot4
	rot3.efo(extraF); 
	// Saida controle de fluxo entrada (int)
	rot3.sfn(SENFLUXS1); // Conectado com Entrada Controle de fluxo de saida sul Rot1
	rot3.sfs(extraF);
	rot3.sfl(FES3FSE4); //Conectado com Entrada controle de fluxo de saida oeste Rot4
	rot3.sfo(extraF);
	// Entrada controle de fluxo saida (int);
	rot3.efsn(SFLUXS1); // Conectado com a saida controle de fluxo de entrada sul Rot1
	rot3.efss(extraF);
	rot3.efsl(FSE3FES4); // Conectado com a saida controle de fluxo de entrada oeste Rot4
	rot3.efso(extraF);
	// Saida controle de fluxo saida (int);
	rot3.sfsn(ENFLUXS1); // Conectado com a entrada controle de fluxo de entrada sul Rot1
	rot3.sfss(extraF);
	rot3.sfsl(FSS3FEE4); //Conectado com a entrada controle de fluxo de entrada oeste Rot4
	rot3.sfso(extraF);
	// Entrada buffer de entrada (sc_uint<32>)
	rot3.EB_norte(sbss); //Conectado com a saida do buffer de saida sul Rot1;
	rot3.EB_sul(ebes3);
	rot3.EB_leste(EBEL3SBSO4); //Conectado com a saida do buffer de saida oeste Rot4;
	rot3.EB_oeste(ebeo3);
	// Saida Buffer de saida (sc_uint<32>)
	rot3.SBS_norte(ebes); //Conectado com a entrada do buffer de entrada sul Rot1;
	rot3.SBS_sul(sbss3);
	rot3.SBS_leste(SBSL3EBEO4); //Conectado com a entrada do buffer de entrada oeste rot4;
	rot3.SBS_oeste(sbso3);

	rot4.clock(relogio);
	rot4.enab(englob);

	// Entrada Controle de fluxo Entrada (int)
	rot4.efn(SSFLUXS2); //Conectado com saida controle de fluxo de saida sul Rot2;
	rot4.efs(extraF);
	rot4.efl(extraF);
	rot4.efo(FSS3FEE4); //Conectado com saida controle de fluxo de saida leste Rot3;
	// Saida controle de fluxo entrada (int)
	rot4.sfn(SENFLUXS2); //Conectado com entrada controle de fluxo de saida sul Rot2
	rot4.sfs(extraF);
	rot4.sfl(extraF);
	rot4.sfo(FSE3FES4); //Conectado com a entrada controle de fluxo de saida leste Rot3
	// Entrada controle de fluxo saida (int);
	rot4.efsn(SFLUXS2); //Conectado com saida controle de fluxo entrada sul Rot2;
	rot4.efss(extraF);
	rot4.efsl(extraF);
	rot4.efso(FES3FSE4); //Conectado com saida controle de fluxo entrada leste Rot3
	// Saida controle de fluxo saida (int);
	rot4.sfsn(ENFLUXS2); // Conectado com entrada controle de fluxo entrada sul Rot2;
	rot4.sfss(extraF);
	rot4.sfsl(extraF);
	rot4.sfso(FEE3FSS4);
	// Entrada buffer de entrada (sc_uint<32>)
	rot4.EB_norte(sbss2); //Conectado com saida buffer de saida sul rot2;
	rot4.EB_sul(ebes4);
	rot4.EB_leste(ebel4);
	rot4.EB_oeste(SBSL3EBEO4); //Conectado com saida buffer de saida leste rot3;
	// Saida Buffer de saida (sc_uint<32>)
	rot4.SBS_norte(ebes2); //Conectado com entrada buffer de entrada sul rot2;
	rot4.SBS_sul(sbss4);
	rot4.SBS_leste(sbsl4);
	rot4.SBS_oeste(EBEL3SBSO4); //Conectado com entrada buffer de entrada leste rot3


	
	sc_uint<32> envio;
	
	envio[0] = 0;
	envio[1] = 1;
	envio[2] = 0;
	envio[3] = 0;

	envio[4] = 0;
	envio[5] = 1;
	envio[6] = 0;
	envio[7] = 0;

	for(int i = 8; i < 32; i++){
		envio[i] = 0;
	}
	sc_start(1, SC_NS);

	englob = true;
	/*
	detalhe para se ligar na hora de enviar mensagens:
	- Suponha que duas mensagens tem o corpo igual. Mesmo destino e mesmo payload
		Agora suponha que elas tão no mesmo roteador.
		Quando a mensagem 1 for embora, vai funcionar
		Quando a mensagem 2 for tentar sair, não vai funcionar
		único problema atualmente.

	Nota 2 - Como eu abstrai as portas locais, tô enviando a mensagem diretamente por uma das portas que não
	está ligada em nenhum outro roteador. Exemplo: Rot1 não usa a porta Norte, nem a oeste.
	Isso poderia ser problema entretanto se eu fosse simular uma rede a partir de 3x3.
	Caso eu quisesse simular uma mensagem saindo do roteador do meio, precisaria sim da porta local.
	*/

	envio[0] = 1;
	envio[1] = 0;

	cout << "Temos 4 roteadores. Rot1 - 11, Rot2 - 21, Rot3- 12, Rot4 - 22" << endl;
	cout << "Vamos começar mandando 3 mensagens: 2xRot1 -> Rot3 e Rot2->Rot3" << endl;
	
	eben = envio;
	envio[10] = 1;
	eben2 = envio;
	envio[11] = 1;
	//ebeo = envio;

	sc_start(1, SC_NS);
	sc_start(1, SC_NS);
	sc_start(1, SC_NS);
	sc_start(1, SC_NS);

	envio[0] = 0;
	envio[1] = 1;
	envio[2] = 0;
	envio[3] = 0;

	envio[4] = 1;
	envio[5] = 0;
	envio[6] = 0;
	envio[7] = 0;

	eben = envio;

	envio[0] = 1;
	envio[1] = 0;
	eben2 = envio;

	cout << "Agora, uma mensagem do Rot1 -> Rot2 e do Rot2 -> Rot1" << endl;
	sc_start(1, SC_NS);
	sc_start(1, SC_NS);
	sc_start(1, SC_NS);
	sc_start(1, SC_NS);


	//ebeo = envio;
	



}