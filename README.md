# Projeto Final Embarcatech: Sensin

Bem-vindo ao Sensin, um projeto desenvolvido como projeto final do Embarcatech. Sensin é um programa regulador de estímulos para autistas. O nome Sensin é inspirado no latim "sentir" e se propõe a ajudar autistas a regular estímulos, sejam eles visuais ou sonoros. 
Autistas frequentemente sofrem com hiperestimulação ou hipoestimulação e precisam de ferramentas para ajudá-los a se regular. Este projeto utiliza a placa **BitDogLab** com o **RP2040**, além de periféricos como **buzzer, LEDs, microfone, WS2812** e **display OLED SSD1306** para criar essa ferramenta acessível e interativa.

## Funcionalidades
O Sensin conta com um **menu interativo** que permite ao usuário escolher entre diferentes modos de regulação sensorial:

### Menu Inicial
O menu inicial apresenta três opções principais:

1. **Submenu 1 - Modo Sensível**
   - Indicado para usuários que sofrem com hiperestimulação.
   - Oferece estímulos reduzidos para maior conforto.
   - Inclui o **Modo Calmo**, que proporciona estímulos suaves para se acalmar com um som emitido pelo buzzer e luz branca.
   - O **Modo Monitoramento** capta os níveis de ruído do ambiente e emite um alerta caso os estímulos estejam sobrecarregando a pessoa, sugerindo ativar o **Modo Calmo**. Para sair do modo monitoramento, o botão A pode ser pressionado e assim retornar ao menu principal.

2. **Submenu 2 - Modo Estímulo**
   - Destinado a autistas com hipossensibilidade.
   - Oferece opções de estímulos:
     - **1 - Recursos Visuais**: Utiliza a matriz de LEDs para estímulos luminosos.
     - **2 - Recursos Sonoros**: Ativa o buzzer para estímulos auditivos.

3. **Modo Alerta**
   - Para momentos de sobrecarga, auxiliando na comunicação e busca de ajuda.
   - **1 - Emoções**: Exibe emoções no display para facilitar a comunicação em situações difíceis.
   - **2 - Modo Ajuda**: Quando necessário, o LED vermelho é aceso e um sinal sonoro é ativado para chamar suporte.

## Hardware Utilizado
- **Placa**: BitDogLab (RP2040)
- **Display OLED SSD1306**: Para exibir o menu interativo e informações em tempo real
- **Buzzer**: Emissão de sinais sonoros
- **LEDs RGB e WS2812**: Iluminação programável para estímulos visuais
- **Microfone**: Monitoramento de ruídos ambientais
- **Joystick e Botões**: Controle do menu interativo

## Conexões dos Periféricos
| Componente | Pino RP2040 |
|------------|------------|
| Botão A | GP5 |
| Matriz de LEDs WS2812 | GP7 |
| LED Verde | GP11 |
| LED Azul | GP12 |
| LED Vermelho | GP13 |
| Display ssd1306 | GP14 e 15 |
| Buzzer | GP21 |
| Joystick Botão | GP22 |
| Joystick Eixo X | GP26 |
| Joystick Eixo Y | GP27 |
| Microfone | GP28 |
## Linguagem utilizada
- **C**

## Como Usar
1. Clone o repositório:
   ```sh
   git clone https://github.com/Camila-RG/Projeto_Final_Embarcatech.git
   ```
2. Compile e carregue o código na **BitDogLab**. Ao importar o repositório no vscode, lembre de instalar e importar com a extensão da Raspberry Pi Pico. Também, pode-se usar o simulador wokwi para ver parte das funcionalidades do projeto. 
3. Utilize o **joystick** para navegar pelo menu interativo.
4. Selecione com o botão do joystick um dos modos conforme sua necessidade:
   - **Modo Estímulo**: Proporciona estímulos sensoriais com luzes e sons.
   - **Modo Sensível**: Reduz a intensidade dos estímulos para maior conforto.
   - **Modo Calmo**: Minimiza estímulos para promover relaxamento.
   - **Modo Monitoramento**: Detecta o nível de ruído do ambiente e alerta caso esteja excessivo.
5. No **Modo Monitoramento**, o sistema avisará caso o som ambiente esteja alto e sugerirá ativar o **Modo Calmo** para ajudar na regulação sensorial. Para sair do modo monitoramento aperte o botão A.

## Objetivo
O Sensin foi criado com o propósito de proporcionar uma ferramenta acessível e eficiente para pessoas autistas se autorregularem em diferentes situações, promovendo bem-estar e autonomia.

## Vídeo Explicativo
[Assista ao vídeo explicativo do projeto](https://youtu.be/xaVh2IheoE8?si=FJ2tyCtoIUaFb1SD)

## Contato
Caso tenha dúvidas ou sugestões, entre em contato através do [GitHub](https://github.com/Camila-RG/Projeto_Final_Embarcatech).

## Licença
Este projeto está licenciado sob a **MIT License** - veja o arquivo [LICENSE](https://github.com/Camila-RG/Projeto_Final_Embarcatech/blob/main/LICENSE) para mais detalhes.

---
Desenvolvido por **Camila Ramos Gomes**.
