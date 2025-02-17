# Documentação do Projeto RFID_PICO

## 📄Introdução

O projeto **RFID_PICO** é uma aplicação desenvolvida para a plataforma **Raspberry Pi Pico W**, utilizando tecnologia **RFID** 
MFRC522 (Identificação por Radiofrequência), um display OLED SSD1306 e comunicação via MQTT para registro de acessos. O sistema se conecta a uma rede Wi-Fi e autentica cartões RFID previamente cadastrados, sinalizando a aceitação ou recusa com LEDs e notificando via MQTT. Este documento fornece uma visão geral do projeto, incluindo estrutura, dependências, instruções de uso e detalhes sobre hardware e comunicação.

## 📝Funcionalidades
* Leitura de cartões RFID utilizando o módulo MFRC522.

* Autenticação de usuários com base em uma lista de tags aceitas.

* Conexão Wi-Fi para comunicação em rede.

* Publicação de eventos via MQTT para monitoramento remoto.

* Indicação visual com LEDs e exibição de mensagens no display OLED.

## 🗂Estrutura do Repositório

O repositório contém os seguintes diretórios e arquivos principais:

```RFID_PICO/
├── .git/                     # Diretório do Git
├── build/                    # Diretório de arquivos de construção
│   ├── RFID_PICO.bin         # Binário do projeto
│   ├── RFID_PICO.elf         # Executável ELF
│   ├── RFID_PICO.hex         # Arquivo HEX
│   └── ...
├── src/                      # Código fonte do projeto
│   ├── MFRC522/              # Biblioteca para o módulo RFID MFRC522
│   ├── display/              # Código para controle de display
│   ├── mqtt/                 # Implementação de MQTT
│   └── ssd1306/              # Biblioteca para display OLED SSD1
│   
├── CMakeLists.txt            # Arquivo de configuração do CMake
├── LICENSE
├── lwipopts                  # Implementação para conexão Wi-Fi
├── RFID_PICO                 # Código principal do projeto
├── LICENSE                   # Licença do projeto
└── README.md                 # Documentação do projeto
```

## 📦 Dependências

O projeto utiliza as seguintes bibliotecas e ferramentas:

- **CMake**: Para gerenciamento de construção.
- **Pico SDK**: SDK para desenvolvimento na Raspberry Pi Pico.
- **MFRC522**: Biblioteca para comunicação com o módulo RFID MFRC522.
- **SSD1306**: Biblioteca para controle de displays OLED.
- **MQTT**: Protocolo de comunicação para transmissão de dados.
- **display**: configuraçoes oara o display Oled.


## 🪛Esquemático de Ligação

Aqui está a ligação entre os componentes e a Raspberry Pi Pico:

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

## 🔩Componentes Utilizados

* Raspberry Pi Pico W

* Módulo MFRC522 (Leitor RFID)

* Display OLED SSD1306 (I2C)

* LEDs indicadores

* Conexão Wi-Fi via chip CYW43439

## ⚙️Configuração

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
## 🛜Comunicação MQTT

O projeto se comunica via MQTT enviando mensagens para o tópico pico. Exemplos de mensagens:
* ```autenticado``` → Quando um cartão aceito é lido
* ```Nao autenticado``` → Quando um cartão não autorizado é lido.

## 🪪Licença

Este projeto está licenciado sob a Licença MIT. Veja o arquivo `LICENSE` para mais detalhes.

##### 👨🏻‍💻Autor
Desenvolvido por:  Carlos Eduardo Sousa Medeiros
Email: eduplayerg22@gmail.com

#### 🔍Extra

Site MQTT para teste: https://testclient-cloud.mqtt.cool/