# Controle de Acesso com RFID e MQTT

## Descrição
Este projeto implementa um sistema de controle de acesso utilizando a Raspberry Pi Pico W, um leitor RFID MFRC522, um display OLED SSD1306 e comunicação via MQTT para registro de acessos. O sistema se conecta a uma rede Wi-Fi e autentica cartões RFID previamente cadastrados, sinalizando a aceitação ou recusa com LEDs e notificando via MQTT.

## Funcionalidades
* Leitura de cartões RFID utilizando o módulo MFRC522.

* Autenticação de usuários com base em uma lista de tags aceitas.

* Conexão Wi-Fi para comunicação em rede.

* Publicação de eventos via MQTT para monitoramento remoto.

* Indicação visual com LEDs e exibição de mensagens no display OLED.

## Componentes Utilizados

* Raspberry Pi Pico W

* Módulo MFRC522 (Leitor RFID)

* Display OLED SSD1306 (I2C)

* LEDs indicadores

* Conexão Wi-Fi via chip CYW43439

## Configuração

1. Defina as credenciais Wi-Fi no código:
``` C
#define WIFI_SSID "Nome_da_Rede"
#define WIFI_PASS "Senha_da_Rede"
```
2. Cadastre os cartões aceitos modificando a matriz acceptedTags:

``` C
uint8_t acceptedTags[][4] = {
    {0x73, 0x40, 0x92, 0x2E},
    {0x93, 0xE3, 0x9A, 0x92},
};
```
3. Compile e carregue o código na Raspberry Pi Pico W.
4. Conecte os módulos conforme os pinos definidos no código.

## Fluxo de Funcionamento

1. Inicializa os periféricos (Wi-Fi, RFID, OLED, LEDs).

2. Conecta-se à rede Wi-Fi e exibe o IP no display.

3. Aguarda a aproximação de um cartão RFID.

4. Compara o UID do cartão com a lista de aceitos:

5. Se aceito, acende o LED verde e publica "autenticado" via MQTT.

6. Se recusado, acende o LED vermelho e publica "Não autenticado" via MQTT.

7. Repete o processo continuamente.

## Comunicação MQTT

O projeto se comunica via MQTT enviando mensagens para o tópico pico. Exemplos de mensagens:
* ```autenticado``` → Quando um cartão aceito é lido
* ```Nao autenticado``` → Quando um cartão não autorizado é lido.

<table>
  <tr style="background-color:rgb(69, 190, 75);">
    <th>Módulo</th>
    <th>Pino Pico W</th>
  </tr>
  <tr>
    <td>MFRC522 SDA</td>
    <td>GPIO 17</td>
  </tr>
  <tr>
    <td>MFRC522 SCK</td>
    <td>GPIO 18</td>
  </tr>
  <tr>
    <td>MFRC522 MOSI</td>
    <td>GPIO 19</td>
  </tr>
  <tr>
    <td>MFRC522 MISO</td>
    <td>GPIO 16</td>
  </tr>
  <tr>
    <td>MFRC522 RST</td>
    <td>GPIO 20</td>
  </tr>
  <tr>
    <td>OLED SDA</td>
    <td>GPIO 14</td>
  </tr>
  <tr>
    <td>OLED SCL</td>
    <td>GPIO 15</td>
  </tr>
  <tr>
    <td>LED Sucesso</td>
    <td>GPIO 11</td>
  </tr>
  <tr>
    <td>LED Falha</td>
    <td>GPIO 13</td>
  </tr>
</table>

## Dependências

SDK do Raspberry Pi Pico

#### Bibliotecas 
* MFRC522
* MQTT 
* SSD1306
* lwIP

##### Autor
Desenvolvido por:  Carlos Eduardo Sousa Medeiros

#### Extra

Site MQTT para teste: https://testclient-cloud.mqtt.cool/