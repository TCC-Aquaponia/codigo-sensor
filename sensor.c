#include<Thermistor.h>;

Thermistor temperaturaPort(A2);

#define ldr A1
#define alimentador 12
#define bomba 11
#define nivel 12

float correttivo = 30.3;
float valoremedio = 0.00; // zona di transito, utilizzata nei calcoli
int tabval[50]; // tabella di memorizzazione di 10 letture consecutive.
int temp = 0; // zona di transito, utilizzata nel loop di ordinamento dei valori letti
byte indice = 0; // indice scorrimento tabella valori
byte indice1 = 0; // indice utilizzato nel loop di ordinamento dei valori letti
float sommavalori = 0; // somma dei 6 valori intermedi rilevati sul pin analogico 0
float PHmedio = 0; // PH calcolato sulla media delle tensioni rilevate sul pin 0

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	pinMode(alimentador, OUTPUT);
	pinMode(bomba, OUTPUT);
	pinMode(nivel, INPUT_PULLUP);
	delay(300);
}
void loop() {
	// put your main code here, to run repeatedly:
	delay(1000);
	int temperatura = temperaturaPort.getTemp();
	int luz = analogRead(ldr);
	Serial.print("Temperatura: ");
	Serial.print(temperatura);
	Serial.println(" ºC");
	Serial.print("luz:");
	Serial.println(luz);
	
	if (luz < 20){
		Serial.println("Peixe Alimentado");
		digitalWrite(alimentador,LOW);
	}else{
		Serial.println("Alimentando Peixe");
		digitalWrite(alimentador,HIGH);
	}
	
	delay(1000);
	
	if(digitalRead(nivel)){
		Serial.println("Tanque abaixo do nivel ");
		Serial.println("bomba ligada");
		digitalWrite(bomba,HIGH);
	} else {
		Serial.println("Tanque no nivel ");
		Serial.println("bomba desligada ");
		digitalWrite(bomba,LOW);
	}
	for (indice = 0; indice < 50; indice++){ // memorizza 50 letture consecutive - stores 50 readings
		tabval[indice] = analogRead(0);
		delay(60);
	}
	// loop di ordinamento dei valori. Questa routine, piuttosto interessante, ordina i valori
	//presenti in tabval, mettendoli in fila, dal piu&#39; basso al piu&#39; alto. - Sorting loops. This
	//rather interesting routine sorts values in tabval array, putting them in order, from lowest to
	//highest.
	for (indice = 0; indice < 49; indice++){
		for (indice1 = indice + 1; indice1 < 50; indice1++){
			if (tabval[indice] > tabval[indice1]){
				temp = tabval[indice];
				tabval[indice] = tabval[indice1];
				tabval[indice1] = temp;
			}
		}
	}
	sommavalori = 0; // azzera preventivamente la zona di somma dei valori
	
	for (int indice = 10; indice < 39; indice++) // nel calcolo della media considera solo i valori
		// intermedi, dalla decima alla trentanovesima posizione in tabella tabval.
		sommavalori += tabval[indice]; // media dei sei valori intermedi memorizzati in tabval
		valoremedio = (float)sommavalori * 5.0 / 1024 / 30; // tensione media sul pin analogico 0
		PHmedio = -5.70 * valoremedio + correttivo; // calcolo del PH, da una formula reperita in rete
		Serial.print("PH rilevato = ");
		Serial.println(PHmedio);
		delay(500);
}
