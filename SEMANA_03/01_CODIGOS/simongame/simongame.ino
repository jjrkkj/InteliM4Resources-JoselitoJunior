// Define os pinos dos leds.
#define LED_YELLOW 22
#define LED_GREEN 17
#define LED_RED 2
#define LED_BLUE 15

// Define os pinos dos botões.
#define BUTTON_YELLOW 25
#define BUTTON_GREEN 26
#define BUTTON_RED 14
#define BUTTON_BLUE 12

// Define o pino do buzzer.
#define BUZZER 32

// Define o pino do botão de reset.
#define RESET_BUTTON 16

// Define o número máximo de rodadas/sequência.
#define MAX_ROUNDS 4

int pins[] = {LED_YELLOW, LED_GREEN, LED_RED, LED_BLUE}; // Define o vetor pins que é responsável por armazenar o número dos pinos dos leds.
int sequence[MAX_ROUNDS]; // Define um vetor com a quantidade da sequência de números que serão armazenados.
int counter = 0; // Contador que estará monitorando em qual rodada o jogador está.
int lastLed; // Variável responsável por armazenar o número de um led referente ao último botão pressionado pelo jogador.


// Função que define qual será a frequência de cada led para o buzzer e o aciona.
void toneBuzzer(int led) {
  if (led == LED_YELLOW) {
    tone(BUZZER, 3850);
  } else if (led == LED_GREEN) {
    tone(BUZZER, 3600);
  } else if (led == LED_RED) {
    tone(BUZZER, 3450);
  } else if (led == LED_BLUE) {
    tone(BUZZER, 4000);
  } 
}


// Função que faz piscar em sequência os leds indicando que começará o jogo.
void start() {
  delay(1000);
  for (int i = 0; i < 3; i++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(pins[i], HIGH);
      toneBuzzer(pins[i]);
      delay(180);
      digitalWrite(pins[i], LOW);
      noTone(BUZZER);
    }
  }
  delay(1000);
}


// Função que acende todos os leds armazenados na lista sequence e acende um outro led aleatório por último.
void roundSequence() {
  for (int i = 0; i < counter; i++) {
    digitalWrite(sequence[i], HIGH);
    toneBuzzer(sequence[i]);
    delay(800);
    digitalWrite(sequence[i], LOW);
    noTone(BUZZER);
    delay(500);
  }

  int randomNumber = random(4);
  sequence[counter] = pins[randomNumber];
  counter++;

  digitalWrite(pins[randomNumber], HIGH);
  toneBuzzer(pins[randomNumber]);
  delay(800);
  noTone(BUZZER);
  digitalWrite(pins[randomNumber], LOW);
}


// Função que lê todos os botões. Acende um led referente ao botão precionado ou reseta o jogo.
void readButtons() {
  for (int i = 0; i < counter; i++) {
    while (true) {
      if (digitalRead(BUTTON_YELLOW) == LOW) {
        digitalWrite(LED_YELLOW, HIGH);
        toneBuzzer(LED_YELLOW);
        delay(400);
        digitalWrite(LED_YELLOW, LOW);
        noTone(BUZZER);
        lastLed = LED_YELLOW;
        break;
      }
      if (digitalRead(BUTTON_GREEN) == LOW) {
        digitalWrite(LED_GREEN, HIGH);
        toneBuzzer(LED_GREEN);
        delay(400);
        noTone(BUZZER);
        digitalWrite(LED_GREEN, LOW);
        lastLed = LED_GREEN;
        break;
      }
      if (digitalRead(BUTTON_RED) == LOW) {
        digitalWrite(LED_RED, HIGH);
        toneBuzzer(LED_RED);
        delay(400);
        noTone(BUZZER);
        digitalWrite(LED_RED, LOW);
        lastLed = LED_RED;
        break;
      }
      if (digitalRead(BUTTON_BLUE) == LOW) {
        digitalWrite(LED_BLUE, HIGH);
        toneBuzzer(LED_BLUE);
        delay(400);
        noTone(BUZZER);
        digitalWrite(LED_BLUE, LOW);
        lastLed = LED_BLUE;
        break;
      }
      if (digitalRead(RESET_BUTTON) == LOW) {
        clearSequence();
        start();
        return;
      }
    }
    if (lastLed != sequence[i]) {
      delay(300);
      lose();
    }
  }
  delay(1000);
}


// Função que indica ao jogador que ele ganhou. Todos os leds piscam rapidamente durante 10 vezes.
void won() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    tone(BUZZER, 4000);
    delay(300);
    noTone(BUZZER);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    delay(150);
  }
  clearSequence();
  start();
}


// Função responsável por limpar os elementos do vetor "sequence" e resetar o contador que indica a rodada atual. Podendo assim, jogar novamente do zero.
void clearSequence() {
  for (int i = 0; i < MAX_ROUNDS; i++) {
    sequence[i] = 0;
  }
  counter = 0;
}


// Função que indica ao jogador que ele perdeu. Os leds ficam acesos durante um período e o buzzer é acionado com uma frequência baixa. 
void lose() {
  clearSequence();

  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  tone(BUZZER, 850);
  delay(2500);
  noTone(BUZZER);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);

  start();
}


// Função que é executada ao iniciar o código.
void setup() {
  randomSeed(analogRead(0)); // Define a semente para gerar números aleatórios.

  // Configura o pino dos leds como saída.
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  pinMode(BUZZER, OUTPUT); // Configura o pino do buzzer como saída.

  pinMode(RESET_BUTTON, INPUT_PULLUP); // Configura o pino do botão reset como entrada PULL UP.

  // Configura o pino dos botões como entrada PULL UP.
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  
  start();
}


// Função executa em loop os códigos dentro dela de forma síncrona.
void loop() {
  roundSequence();
  readButtons();

  // Condição em que se o contador for igual ao número máximo de rodadas, o jogador ganha!
  if (counter == MAX_ROUNDS) {
    won();
  }
}