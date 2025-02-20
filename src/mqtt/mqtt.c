#include "mqtt.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "lwip/init.h"
#include "lwip/netif.h"

// Definição da variável global mqtt_client
mqtt_client_t *mqtt_client;

// Função de callback para receber mensagens
void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    printf("Mensagem recebida no tópico: %s\n", topic);
}

void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    printf("Dados recebidos: %.*s\n", len, data);

    // Converte os dados recebidos para uma string
    char data_str[len + 1];
    memcpy(data_str, data, len);
    data_str[len] = '\0';

    // Remove quebra de linha
    for (int i = 0; i < len; i++)
    {
        if (data_str[i] == '\n')
            data_str[i] = ' ';
    }
}

// Função de callback para confirmação de inscrição (opcional)
void mqtt_subscribe_cb(void *arg, err_t err)
{
    if (err == ERR_OK)
    {
        printf("Inscrição confirmada!\n");
    }
    else
    {
        printf("Erro na confirmação da inscrição: %d\n", err);
    }
}

// Função para se inscrever em um tópico
void subscribe_to_topic(mqtt_client_t *client, const char *topic, uint8_t qos)
{
    err_t err = mqtt_subscribe(client, topic, qos, mqtt_subscribe_cb, NULL);
    if (err != ERR_OK)
    {
        printf("Erro ao se inscrever no tópico %s: %d\n", topic, err);
    }
    else
    {
        printf("Inscrito no tópico %s com QoS %d\n", topic, qos);
    }
}

// Função para publicar uma mensagem
void publish_message(mqtt_client_t *client, const char *topic, const char *message, uint8_t qos, uint8_t retain)
{
    err_t err = mqtt_publish(client, topic, message, strlen(message), qos, retain, NULL, NULL);
    if (err != ERR_OK)
    {
        printf("Erro ao publicar mensagem no tópico %s: %d\n", topic, err);
    }
    else
    {
        printf("Mensagem publicada no tópico %s: %s\n", topic, message);
    }
}

// Função de callback para conexão MQTT
void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        printf("Conectado ao broker MQTT!\n");

        // Inscreve-se em um tópico após a conexão
        subscribe_to_topic(client, "pico/topic", 0);
    }
    else
    {
        printf("Erro na conexao MQTT: %d\n", status);
    }
}

// Inicialização do MQTT
void mqtt_init()
{
    ip_addr_t broker_ip;
    if (!ipaddr_aton(MQTT_BROKER_IP, &broker_ip)) // Converte o endereço IP para o formato correto
    {
        printf("Erro ao converter o endereço IP do broker MQTT\n");
        return;
    }

    mqtt_client = mqtt_client_new();
    if (mqtt_client == NULL)
    {
        printf("Erro ao criar cliente MQTT\n");
        return;
    }

    // Configura os callbacks para receber mensagens
    mqtt_set_inpub_callback(mqtt_client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);

    // Estrutura de informações do cliente MQTT
    struct mqtt_connect_client_info_t client_info = {
        .client_id = "pico_client",
        .client_user = NULL, // Se precisar de autenticação, adicione usuário e senha
        .client_pass = NULL,
        .keep_alive = 60,
        .will_topic = NULL,
        .will_msg = NULL,
        .will_qos = 0,
        .will_retain = 0};

    // Tenta conectar ao broker MQTT
    err_t err = mqtt_client_connect(mqtt_client, &broker_ip, MQTT_BROKER_PORT, mqtt_connection_cb, NULL, &client_info);
    if (err != ERR_OK)
    {
        printf("Erro ao tentar conectar ao broker MQTT: %d\n", err);
    }
}