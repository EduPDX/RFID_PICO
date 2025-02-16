/*
 * Código para Raspberry Pi Pico W com RFID, Wi-Fi e MQTT
 * Autor: Carlos Eduardo Sousa Medeiros
 * Descrição: 
 *   - Conecta-se a uma rede Wi-Fi.
 *   - Lê cartões RFID utilizando o módulo MFRC522.
 *   - Indica a autenticação via LEDs e exibe mensagens no display OLED.
 *   - Envia informações via MQTT.
 */
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "lwip/init.h"
#include "lwip/netif.h"
#include "pico/binary_info.h"
#include "src/MFRC522/mfrc522.h"
#include "src/mqtt/mqtt.h"
#include "src/ssd1306/ssd1306_i2c.h"
#include "src/ssd1306/ssd1306.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "src/display/display.h"

// Configurações de Wi-Fi
#define WIFI_SSID "rede" // Nome da rede
#define WIFI_PASS "senha" // Senha da rede

// Pino do LED para indicar conexão
#define LED_SUCESS_PIN 11
#define LED_FAILED_PIN 13

// display oled i2c
const uint i2c_sda = 14;
const uint i2c_scl = 15;


//Card UID: 73 40 92 2E
uint8_t tagsAvailable[] = {0x73, 0x40, 0x92, 0x2E}; // UID das tags disponíveis

// Lista de tags aceitas
uint8_t acceptedTags[][4] = {
    {0x73, 0x40, 0x92, 0x2E},
    {0x93, 0xE3, 0x9A, 0x92}, // Exemplo de outra tag permitida
};

// Quantidade de tags aceitas na lista
#define NUM_TAGS (sizeof(acceptedTags) / sizeof(acceptedTags[0]))

/*
 * Função: isTagAccepted
 * Descrição: Verifica se o UID lido corresponde a uma tag permitida.
 * Parâmetro: uint8_t *uid - UID do cartão RFID lido.
 * Retorno: 1 (autorizado) ou 0 (não autorizado).
 */
// Função para verificar se a tag lida é aceita
int isTagAccepted(uint8_t *uid) {
    for (int i = 0; i < NUM_TAGS; i++) {
        if (memcmp(uid, acceptedTags[i], 4) == 0) {
            return 1; // Tag encontrada na lista
        }
    }
    return 0; // Tag não encontrada
}

// função principal
int main()
{
    stdio_init_all(); // Inicializa a interface serial

    // Inicializa o display OLED I2C
    i2c_init(i2c1, ssd1306_i2c_clock * 1000); // Configura o clock do I2C
    gpio_set_function(i2c_sda, GPIO_FUNC_I2C); // Configura o pino SDA como I2C
    gpio_set_function(i2c_scl, GPIO_FUNC_I2C); // Configura o pino SCL como I2C
    gpio_pull_up(i2c_sda); // Habilita o pull-up para o pino SDA
    gpio_pull_up(i2c_scl); // Habilita o pull-up para o pino SCL

    ssd1306_init(); // Inicializa o display
    sleep_ms(1000); // Aguarda 1 segundo

    // Limpa o display
    uint8_t ssd[ssd1306_buffer_length]; // Buffer para o display
    memset(ssd, 0, ssd1306_buffer_length); // Limpa o buffer
    render_on_display(ssd, NULL); // Renderiza o display

    
    
    // Inicializa os LEDs
    gpio_init(LED_SUCESS_PIN);
    gpio_init(LED_FAILED_PIN);
    // Configura os LEDs como saída
    gpio_set_dir(LED_SUCESS_PIN, GPIO_OUT);
    gpio_set_dir(LED_FAILED_PIN, GPIO_OUT);
    // Desliga os LEDs
    gpio_put(LED_SUCESS_PIN, 0);
    gpio_put(LED_FAILED_PIN, 0);
    char *text[] = { // Exibe o texto no display
        "            ",
        "            ",
        "            ",
        "  Iniciando ",
        "            "};
    showTextDisplay(text, 5); // Exibe a lista de textos

    MFRC522Ptr_t mfrc = MFRC522_Init(); // Inicializa o MFRC522
    PCD_Init(mfrc, spi0); // Inicializa o chip de comunicação SPI

    sleep_ms(5000); // Aguarda 5 segundos

    // Inicializa o Wi-Fi
    if (cyw43_arch_init()){
        printf("Erro ao inicializar o Wi-Fi\n");
        char *text[] = { // Exibe o texto no display
            "            ",
            "Erro ao conectar",
            "   Wi-Fi    ",
            "            "};
        showTextDisplay(text, 4); // Exibe a lista de textos
        return 1;
    }

    // Habilita o modo de acesso ao ponto de acesso
    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");
    char *wifi[] = { // Exibe o texto no display
        "            ",
        "            ",
        "            ",
        "conectando WIFI",
        "            "};
    showTextDisplay(wifi, 5); // Exibe a lista de textos
    sleep_ms(2000); // Aguarda 2 segundos

    // Conecta ao ponto de acesso
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)){
        printf("Falha ao conectar ao Wi-Fi\n");
        char *text[] = { // Exibe o texto no display
            "            ",
            "Erro ao conectar",
            "   Wi-Fi    ",
            "            "};
        showTextDisplay(text, 4); // Exibe a lista de textos
        return 1;
    }
    else{
        printf("Conectado ao Wi-Fi\n");
        // Lê o endereço IP de forma legível
        uint8_t *ip_address = (uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
        char ip_string[16]; // String para armazenar o endereço IP
        // Converte o endereço IP para uma string
        snprintf(ip_string, sizeof(ip_string), "%d.%d.%d.%d", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
        printf("Endereço IP: %s\n", ip_string); // Imprime o endereço IP
        char *text[] = { // Exibe o texto no display
            "WIFI Conectado",
            "            ",
            "            ",
            "Endereco IP",
            "            ",
            ip_string};
        showTextDisplay(text, 6); // Exibe a lista de textos
    }

    mqtt_init(); // Inicializa o MQTT

    // Loop infinito para receber cartões
    while (true){
        cyw43_arch_poll(); // Necessário para manter o Wi-Fi ativo
        printf("Esperando por cartão\n\r"); // Imprime uma mensagem
        while(!PICC_IsNewCardPresent(mfrc)); // Espera por cartão

       // printf("Selecting card\n\r");
        PICC_ReadCardSerial(mfrc);

        // Imprime UID do cartão no serial
        printf("Uid is: ");
        for(int i = 0; i < 4; i++) { 
           printf("%x ", mfrc->uid.uidByte[i]);
        } 
        printf("\n\r");

        // Verifica se o cartão foi aceito
        if (isTagAccepted(mfrc->uid.uidByte)){
            printf("Sucesso na autenticação\n\r"); // Imprime uma mensagem de sucesso
            gpio_put(LED_SUCESS_PIN, 1); // Acende o LED
            sleep_ms(1000); // Aguarda 1 segundo
            gpio_put(LED_SUCESS_PIN, 0); // Apaga o LED
            publish_message(mqtt_client, "pico", "autenticado", 0, 0); // Envia uma mensagem MQTT
        }
        // Se o cartão não foi aceito
        else {
            printf("Falha na autenticação\n\r"); // Imprime uma mensagem de falha
            gpio_put(LED_FAILED_PIN, 1); // Acende o LED
            sleep_ms(1000); // Aguarda 1 segundo
            gpio_put(LED_FAILED_PIN, 0); // Apaga o LED
            publish_message(mqtt_client, "pico", "Nao autenticado", 0, 0); // Envia uma mensagem MQTT
        }
        sleep_ms(100); // Aguarda 1 segundo antes de ler o próximo cartão
    }

    return 0; // Retorna 0 para indicar sucesso
}


