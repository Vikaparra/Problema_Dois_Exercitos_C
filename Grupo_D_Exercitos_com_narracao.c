#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define probNegar 1
#define probCaptura 45

void verificacaoTempoVermelho();
void azulEnviarMsg();
void vermelhoEnviarMsg();
int probabilidade();
void azulNegandoCapturado();
void startTimestamp();
void azulConfirmandoCapturado();
long long int randomizarTempo();

int mensVerm = 5, mensAzul = 10, resultado = 0, sinalizador = 0, rejeitado = 0, seed, auxTempoVerm = 1;
long long int tempo = 0, tempoVermelho = 0, inicio = 0, inicioVermelho = 0, auxTempo = 0;


int main()
{

    //----------------------------------------------------------------------------//
    //-----------------------Inicio da troca de mensagens-------------------------//
    //----------------------------------------------------------------------------//

    int horas, minutos, segundos, horas_seg = 3600;
    startTimestamp(&tempo);
    inicio = tempo;
    tempoVermelho = tempo;
    inicioVermelho = tempo;
    srand(seed);

    while ((sinalizador == 0) && (mensVerm > 0) && (mensAzul > 0))
    {
        (mensVerm)--;
        printf("O mensageiro vermelho foi enviado.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
        
        if (probabilidade(probCaptura) == 1) //O mensageiro vermelho chegar
        {
            tempo += randomizarTempo(); //adicionar tempo no contador (entre 3600 e 4200 para a travessia do mensageiro)
            tempoVermelho += auxTempo; //adicionar tempo no contador (entre 3600 e 4200 para a travessia do mensageiro)
            verificacaoTempoVermelho(); //verificar se ja é hora do vermelho enviar outro mensageiro
            rejeitado = 0; //inicializando rejeitado como 0 novamente para reiniciar o processo caso vermelho envie outro horário

            printf("O mensageiro vermelho chegou no azul.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));   
            
            while ((sinalizador == 0) && (rejeitado == 0) && (mensAzul > 0) && (mensVerm > 0)) //Enquanto o sinalizador não tiver sido disparado (mens azul chegou) ou mens azul chegou e foi rejeitado
            {
                azulEnviarMsg(); //começando a função do azul aceitar ou não aceitar o horário e enviar outro mensageiro
                verificacaoTempoVermelho(); //verificar se ja é hora do vermelho enviar outro mensageiro

                switch (resultado)
                {
                    case 1: //caso a função azulEnviarMsg sete o resultado para 1, deu tudo certo
                    {
                        (tempo) += randomizarTempo(); //adicionar mais tempo de travessia no contador
                        tempoVermelho += auxTempo; //adicionar tempo no contador (entre 3600 e 4200 para a travessia do mensageiro)

                        verificacaoTempoVermelho();       
                        
                        printf("O mensageiro azul chegou no vermelho.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
                        sinalizador++; //se a função finalizou é pois o mensageiro chegou no vermelho e o sinalizador foi liberado
                        break;
                    }

                    case 2: //caso a função azulEnviarMsg sete o resultado para 2, rejeitaram horário
                    {
                        (tempo) += randomizarTempo();            
                        tempoVermelho += auxTempo; //adicionar tempo no contador (entre 3600 e 4200 para a travessia do mensageiro)
                                        
                        verificacaoTempoVermelho();      
                        
                        printf("Azul negou o pedido, e o mensageiro chegou no vermelho.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
                        rejeitado++;
                        inicioVermelho = tempo; //reinicia a contagem do tempo vermelho, para alinhar o tempo de reenviar um novo mensageiro, pois vermelho ira reiniciar todo o processo ja que azul negou o horário
                        tempoVermelho = inicioVermelho; //reinicia o tempovermelho como o inicio do tempovermelho pelo mesmo motivo
                        auxTempoVerm = 1; // reinicia a auxiliar de mensageiros enviados para 1 pelo mesmo motivo
                        break;
                    }

                    case 3: //caso a função azulEnviarMsg sete o resultado para 3, capturado confirmando
                    {
                        azulConfirmandoCapturado();
                        break;
                    }

                    default: //caso a função azulEnviarMsg sete o resultado para 4, capturado negando
                    {
                        azulNegandoCapturado();
                        break;
                    }
                }
            }
        }

        else //caso o mensageiro seja capturado de primeira
        {
            tempo += 12601; //adiciona 12601 no tempo pois é o que eles gastam pra tirar a conclusao de que foi capturado
            tempoVermelho += 12601;
            auxTempoVerm++; //adiciona 1 na auxiliar que verifica se ja se passaram 210 min e 1seg desde que vermelho enviou um mensageiro
            printf("O exercito vermelho percebeu que o mensageiro vermelho foi capturado.\nCaso nao receba nenhuma resposta, ira enviar outro no tempo: %lld segundos.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld\n\n", ((tempo-inicio) + 12601), mensVerm, mensAzul, (tempo-inicio));
        }
    }

    printf("------------------------------------------\n");
    if (sinalizador > 0)
    {
        printf("Ganharam!!\n");
    }
    else
    {
        printf("Perderam \n");
    }

    printf("------------------------------------------");
    printf("\nMensageiros restantes - Vermelho: %d, Azul: %d", mensVerm, mensAzul);
    printf("\nInicio da troca de mensagem: %lld\nTermino da troca de mensagem: %lld", inicio, tempo);
    double tempoGasto = difftime((time_t)tempo, (time_t)inicio);

    horas = (tempoGasto / horas_seg); //resultado da hora
    minutos = (tempoGasto - (horas_seg * horas)) / 60;
    segundos = (tempoGasto - (horas_seg * horas) - (minutos * 60));
    printf("\nTempo total gasto: %dh:%dm:%ds", horas, minutos, segundos);
    printf("\n------------------------------------------\n");
    printf("Seed utilizada: %d",seed);
    printf("\n------------------------------------------\n");

    return 0;
}

//----------------------------------------------------------------------------//
//----------------------------Função da Timestamp-----------------------------//
//----------------------------------------------------------------------------//
void startTimestamp()
{
    time_t segundos;
    segundos = time(NULL);
    tempo = segundos;
    seed = segundos;
}

//----------------------------------------------------------------------------//
//---------------Função do azul enviando mensageiro de volta------------------//
//----------------------------------------------------------------------------//
void azulEnviarMsg()
{
    mensAzul--;

    if (probabilidade(probNegar) == 1) // Azul aceita
    {
        if (probabilidade(probCaptura) == 1) // Mensageiro chega
        {
            resultado = 1; // caso prob 45 ==1 e prob 1 == 1
        }

        else
        {
            resultado = 3; // caso prob 45 == 0  e prob 1 == 0 mensageiro foi capturado
        }
    }

    else
    {
        if (probabilidade(probCaptura) == 1) // Mensageiro chega mas azul nao aceita
        {
            resultado = 2; // prob 45 == 1 e prob 1 == 0, mensageiro chega mas nao aceitaram
        }
        else
        {
            resultado = 4; //  prob 45 == 0 e prob 1 == 0 mensageiro foi capturado
        }
    }
}

//----------------------------------------------------------------------------//
//-------------------------Função para probabilidades-------------------------//
//----------------------------------------------------------------------------//
int probabilidade(int porcentagem)
{
    int nAleatorio = rand() % 100; //definindo um numero aleatorio até 100

    switch (porcentagem)
    {
        case 1:
        {
            printf("***Numero randomico: %d (caso seja 1 eles negam o horario, caso nao, eles aceitam)***\n", nAleatorio);
            if (nAleatorio > porcentagem) //numero aleatorio > 1 , logo azul aceita
                return 1;                 //deu bão, aceitaram
            else
                return 0;                 //deu ruim, negaram
            break;
        }

        default:
        {
            printf("***Numero randomico: %d (caso seja ate 45 o mensageiro e capturado, caso acima, ele chega no destino)***\n", nAleatorio);
            if (nAleatorio > porcentagem) //numero aleatorio < 45 , logo capturado
                return 1;                 //deu bão, nao foram capturados
            else
                return 0;                 //deu ruim, capturados
            break;
        }
    }
}

//----------------------------------------------------------------------------//
//-------------------------Função de Randomizar Tempo-------------------------//
//----------------------------------------------------------------------------//
long long int randomizarTempo()
{
    auxTempo = 3600 + rand() % (4200 + 1 - 3600);
    return (auxTempo);
}

//----------------------------------------------------------------------------//
//----------------Função para quando Azul nega e é capturado------------------//
//----------------------------------------------------------------------------//
void azulNegandoCapturado()
{
    tempo += 4201; //adicionando tempo ao contador pois é o tempo de perceberem que foi capturado e mandarem outro
    tempoVermelho += 4201;
    verificacaoTempoVermelho(); //verificando se ja deu tempo do vermelho enviar outro
    mensAzul--;
    printf("Passaram 70min e 1 seg, logo, o mensageiro azul negando o pedido foi capturado, enviaram outro mensageiro azul.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
    
    while ((probabilidade(probCaptura) == 0) && (mensAzul > 0) && (mensVerm > 0)) //enquanto ele continuar sendo capturado
    {
        tempo += 4201; //adicionando tempo que demora para enviarem outro            
        tempoVermelho += 4201; //adicionar tempo no contador (entre 3600 e 4200 para a travessia do mensageiro)
        verificacaoTempoVermelho(); //verificando tempo vermelho
        printf("Passaram mais 70min e 1 seg, logo, o mensageiro azul negando o pedido foi capturado, enviaram outro mensageiro azul.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
        mensAzul--;
    }

    (tempo) += randomizarTempo(); //quando não é capturado, leva entre 3600 a 4200 segundos pra chegar
    tempoVermelho += auxTempo; //adicionar tempo no contador (entre 3600 e 4200 para a travessia do mensageiro)
    verificacaoTempoVermelho(); 
    printf("O mensageiro azul negando o pedido chegou no exercito vermelho.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
    rejeitado++;
    inicioVermelho = tempo;
    tempoVermelho = inicioVermelho;
    auxTempoVerm = 1;
}

//----------------------------------------------------------------------------//
//----------------Função para quando Azul aceita e é capturado----------------//
//----------------------------------------------------------------------------//
void azulConfirmandoCapturado()
{
    tempo += 4201; //adicionando tempo ao contador pois é o tempo de perceberem que foi capturado e mandarem outro
    tempoVermelho += 4201;
    verificacaoTempoVermelho(); //verificando se ja deu tempo do vermelho enviar outro
    mensAzul--;
    printf("Passaram 70min e 1 seg, logo, o mensageiro azul confirmando o pedido foi capturado, enviaram outro mensageiro azul.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));

    while ((probabilidade(probCaptura) == 0) && (mensAzul > 0) && (mensVerm > 0))
    {
        tempo += 4201; //tempo que demora para enviarem outro
        tempoVermelho += 4201;
        verificacaoTempoVermelho();
        printf("Se passaram mais 70min e 1seg, logo, mais um mensageiro azul confirmando o pedido foi capturado, enviaram outro mensageiro azul.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
        (mensAzul)--;
    }

    (tempo) += randomizarTempo(); //3600 a 4200 seg pra chegar
    tempoVermelho += auxTempo; //adicionar tempo no contador (entre 3600 e 4200 para a travessia do mensageiro)
    verificacaoTempoVermelho(); 
    printf("O mensageiro azul confirmando o pedido chegou no exercito vermelho.\n Num Vermelhos: %d || Num Azuis: %d || tempo: %lld || tempo vermelho: %lld\n\n", mensVerm, mensAzul, (tempo-inicio), (tempoVermelho-inicioVermelho));
    sinalizador++;
}

//----------------------------------------------------------------------------//
//Função para verificar se ja é o momento do ex vermelho enviar + 1 mensageiro//
//----------------------------------------------------------------------------//
void verificacaoTempoVermelho()
{
    if(((tempoVermelho-inicioVermelho) - (auxTempoVerm * 12601)) > 0) //caso o tempo seja maior do que 120min e 1 seg desde o momento que o vermelho enviou o ultimo mensageiro
    {
        auxTempoVerm++; //quantidade de vezes que vermelho enviou o mensageiro
        mensVerm--; 
        printf("\nSe passaram 210min e 1seg desde que o exercito vermelho enviou seu mensageir\nlogo, outro mensageiro vermelho esta sendo enviado. Num Vermelhos: %d || Num Azuis: %d || \n\n", mensVerm, mensAzul);
    }
}