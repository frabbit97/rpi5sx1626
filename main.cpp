#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Per sleep
#include "config.h"  // Supponiamo che la libreria LoRa sia già configurata in C





// Funzione di setup (equivalente di Arduino setup)
void setup() {
    printf("Setup ...\n");

    // Inizializza il modulo LoRa
    printf("Inizializza il modulo radio\n");
    int16_t state = radio.begin();
    debug(state != RADIOLIB_ERR_NONE, "Inizializzazione del modulo radio fallita", state, 1);

    // Configura la sessione OTAA
    printf("Configura la sessione OTAA\n");
    state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);
    debug(state != RADIOLIB_ERR_NONE, "Inizializzazione del nodo fallita", state, 1);

    // Unisciti alla rete LoRaWAN
    printf("Connessione alla rete LoRaWAN\n");
    state = node.activateOTAA();
    debug(state != RADIOLIB_LORAWAN_NEW_SESSION, "Connessione fallita", state, 1);

    printf("Pronto!\n");
}

// Funzione principale (equivalente di Arduino loop)
void loop() {
    printf("Invio uplink...\n");

    // Genera valori casuali come esempio (simulazione di input da sensori)
    uint8_t value1 = rand() % 100;
    uint16_t value2 = rand() % 2000;

    // Crea il payload dell'uplink
    uint8_t uplinkPayload[3];
    uplinkPayload[0] = value1;
    uplinkPayload[1] = (value2 >> 8) & 0xFF;  // byte alto
    uplinkPayload[2] = value2 & 0xFF;         // byte basso

    // Esegui l'uplink
    int16_t state = node.sendReceive(uplinkPayload, sizeof(uplinkPayload));
    debug(state < RADIOLIB_ERR_NONE, "Errore durante l'invio del pacchetto", state, 0);

    // Verifica se è stato ricevuto un downlink
    if (state > 0) {
        printf("Ricevuto un downlink\n");
    } else {
        printf("Nessun downlink ricevuto\n");
    }

    // Tempo prima del prossimo uplink
    printf("Prossimo uplink in %u secondi\n", uplinkIntervalSeconds);
    sleep(uplinkIntervalSeconds);  // Attesa prima del prossimo invio
}

int main() {
    // Esegue la funzione di setup una sola volta
    setup();

    // Esegue la funzione loop in un ciclo continuo
    while (1) {
        loop();
    }

    return 0;
}
