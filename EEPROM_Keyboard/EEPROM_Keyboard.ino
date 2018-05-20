// programa para salvar valores de um sensor na EEPROM e lê-los como texto em qualquer programa
// duas opções: ler o sensor (r) e visualizar os valores lidos (w)
// abrir o monitor serial e selecionar uma das opções, digitando r ou w
// para escrever os valores, abrir editor de texto, clicar no botão acoplado ao arduino.
// Pode ser então salvo como .csv, por exemplo

//  verificar se o valor pode ser salvo de 0 a 1023 na EEPROM. parece limlitar a 255


#include <Keyboard.h>
#include <EEPROM.h>

int sensorValue = 0;
int opcao = 0;          // ideia: manter default como w, alterar também na saída do for
int tempo_delay = 100;  // intervalo entre as leituras. definir isso com opções de cliques no botão e exibir no display

// número de leituras a cada vez que o r é pressionado - mudar isso para uma chave física?
// o número de sensores pode ser definido com uma chave de 3 posições?
int n_leituras = 100;
int led = 10;

void setup(){
  Serial.begin(115200);

  while (!Serial);
  Serial.println("Data Science");
  Keyboard.begin();
}

void loop() {
  int button = digitalRead(2);

  if(Serial.available()) {
    opcao = Serial.read();
    Serial.print("Tecla pressionada: ");
    Serial.println(opcao);
  }

  // após fazer o upload do programa, abrir o monitor serial e digitar w (write) ou r (read)
  // se digitar r, o arduino  entra em modo de leitura e começa a salvar os dados da EEPROM
  // se digitar w, o arduino entra em modo de escrita:
  // quando pressionamos o botão, o arduino lê os dados da EEPROM e os escreve como se fosse um
  // teclado em qualquer lugar que aceite texto
  // TODO: mudar o modo leitura e escrita não através do monitor, mas com uma chave com 2 posições

  switch (opcao){
    case 'r': //lê os valores do sensor
      // lê os valores e salva na EEPROM
      for (int i=0; i<n_leituras; i++) {
        sensorValue = analogRead(A0);
        EEPROM.write(i, sensorValue);
        Serial.println(EEPROM.read(i));
        digitalWrite(led,HIGH);
        delay(tempo_delay/2); //enquanto está gravando pisca um LED a cada gravada
        digitalWrite(led,LOW);
        delay(tempo_delay/2);
        //preencher o restante das posições com zero
      }
      opcao='w';
      break;
    case 'w': // escreve os valores quando o botão for clicado
      //quando o botão é clicado, são impressos os valores no computador (no word, por exemplo). o arduino se comporta como um teclado que digita sozinho
      if (button == 1) {
        Keyboard.print("delay=");
        Keyboard.print(tempo_delay);
        Keyboard.print(", ");
        for (int i =1; i<n_leituras; i++) {
          String leitura_eeprom = String(EEPROM.read(i));
          Keyboard.println(leitura_eeprom);
          Keyboard.print(",");
          delay(10);
          }
        }
      break;
  }
}

