
unsigned char USART_Receive()
{
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void UART_TxChar(char ch)
{
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = ch;
}

void USART_Flush(void)
{
unsigned char dummy;
while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}

void setup() {
  // put your setup code here, to run once:

  //Enable transmission and reception in USART b
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);

  //Set data size for communication c
  UCSR0C &= (~(1<<UMSEL00)) & (~(1<<UMSEL01)) & (~(1<<UPM00)) & (~(1<<UPM01)) & (~(1<<USBS0));

  //Set data length to 8-bits
  UCSR0B &= (~(1<<UCSZ02));
  UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);

  //Set transmission speed
  UCSR0A &= ~(1<<U2X0);

  //Set baud rate
  UBRR0 = 103; // 9600

  //Serial.begin(9600);

}

unsigned int data = 0;
void loop() {
  // put your main code here, to run repeatedly:
  
  USART_Flush();
  
  data = USART_Receive();
  
  if(data == 'a')
  {
    UART_TxChar('-');
  }
  if(data == 'b')
  {
    UART_TxChar('+');
  }

    if(data == 'c' )
  {
    UART_TxChar('0');
  }
}
