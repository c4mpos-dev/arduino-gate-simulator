// Bibliotecas
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// Definição Lcd
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Definições Servos
const int pino_servo_esquerdo = 6;
const int pino_servo_direito = 7;
int posicaoServos = 0;
Servo servo_esquerdo;
Servo servo_direito;
bool estaAbertoPortao = false;

// Definições Infravermelho
const int pino_infra = 8;
const int botaoOnOff = 16580863;
IRrecv infra(pino_infra);
decode_results resultado;

// Definições leds
const int led_vermelho = 13;
const int led_amarelo = 9;
const int led_verde = 10;

// Definição Fotoresistor
const int pino_fotoresistor = A0;

void setup() {
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Fechado!        ");

    servo_esquerdo.attach(pino_servo_esquerdo);
    servo_direito.attach(pino_servo_direito);
    for (posicaoServos = 0; posicaoServos <= 90; posicaoServos++) {
        servo_esquerdo.write(posicaoServos);
        servo_direito.write(posicaoServos);
        delay(15);
    }

    pinMode(led_vermelho, OUTPUT);
    pinMode(led_amarelo, OUTPUT);
    pinMode(led_verde, OUTPUT);
    digitalWrite(led_vermelho, HIGH);

    infra.enableIRIn();
}

void loop() {
    int valorFotoresistor = analogRead(pino_fotoresistor);
    if (valorFotoresistor > 800) {
        // Permite o abrir e fechar o portão.
        if (infra.decode(&resultado)) {
            int valorLido = resultado.value;
            if (valorLido == botaoOnOff) {
                if (estaAbertoPortao == false) {
                AbrirFecharPortao(true, "Abrindo         ", "Aberto!         ", led_verde, led_vermelho); //AbrirPortao();
                }
                else {
                AbrirFecharPortao(false, "Fechando        ", "Fechado!        ", led_vermelho, led_verde); //FecharPortao();
                }
            }
            infra.resume();
        }
    }
    else {
        // Fecha o portão caso ele esteja aberto.
        if (estaAbertoPortao == true) {
        AbrirFecharPortao(false, "Fechando        ", "Fechado!        ", led_vermelho, led_verde);
        }
    }
    delay(100);
}

void AcenderLED(int ledAceso, int ledApagado1, int ledApagado2) {
  digitalWrite(ledAceso, HIGH);
  digitalWrite(ledApagado1, LOW);
  digitalWrite(ledApagado2, LOW);
}

void AbrirFecharPortao(bool estadoPortao, char primeiraFrase[16], char segundaFrase[16], int ledAceso, int ledApagado) {

    estaAbertoPortao = estadoPortao;
    lcd.setCursor(0, 0);
    lcd.print(primeiraFrase);

    AcenderLED(led_amarelo, led_vermelho, led_verde);
    if (estaAbertoPortao == true) {
        for (posicaoServos = 90; posicaoServos >= 0; posicaoServos--) {
        servo_esquerdo.write(posicaoServos);
        servo_direito.write(posicaoServos);
        delay(15);
        }
    }
    else {
        for (posicaoServos = 0; posicaoServos <= 90; posicaoServos++) {
        servo_esquerdo.write(posicaoServos);
        servo_direito.write(posicaoServos);
        delay(15);
        }
    }
    lcd.setCursor(0, 0);
    lcd.print(segundaFrase);
    AcenderLED(ledAceso, ledApagado, led_amarelo);
    delay(1000);
}