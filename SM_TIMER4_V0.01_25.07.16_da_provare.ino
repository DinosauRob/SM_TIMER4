
#define p(value)  ( Serial.print  (F(value)) )
#define pn(value) ( Serial.println(F(value)) )

#define DIR_M_SET    (PORTE |= (1 << PE3))   // HIGH Pin 5
#define DIR_M_RESET  (PORTE &= ~(1 << PE3))  // LOW  Pin 5

#define STEP_M_SET     (PORTH |= (1 << PH3))   // HIGH pin 6
#define STEP_M_RESET   (PORTH &= ~(1 << PH3))  // LOW  pin 6

#define DIS_M   (PORTH |=  (1 << PH4))   // HIGH Pin 7 
#define ENA_M   (PORTH &= ~(1 << PH4))   // LOW  Pin 7 

/*
#define STEP_M_SET     asm volatile("lds r24, %0 \n\t  ori r24, %1 \n\t sts %0, r24" : : "M" (_SFR_MEM_ADDR(PORTH)), "M" (1 << PH3))
#define STEP_M_RESET   asm volatile("lds r24, %0 \n\t andi r24, %1 \n\t sts %0, r24" : : "M" (_SFR_MEM_ADDR(PORTH)), "M" (~(1 << PH3)))
*/


#define SET_FREE_RUNNING_MODE (TCCR4A = 0x00, TCCR4B = (TCCR4B & 0xF8))
#define SET_CTC_MODE_OCR4A (TCCR4A = 0x00, TCCR4B = (TCCR4B & 0xF8) | (1 << WGM12))

/*--------------------------------------------------------------*/
#define SET_FREE_RUNNING_MODE2 \
  (TCCR4A = (TCCR4A & ~((1 << WGM41) | (1 << WGM40))), \
  (TCCR4B = (TCCR4B & ~((1 << WGM43) | (1 << WGM42))) & 0xF8)

#define SET_CTC_MODE_OCR4A2 \                                    
  (TCCR4A = (TCCR4A & ~((1 << WGM41) | (1 << WGM40))), \
  TCCR4B = (TCCR4B & ~(1 << WGM43)) | (1 << WGM42) & 0xF8)

#define SET_FAST_PWM_OCR4A_MODE2 \
  (TCCR4A = (TCCR4A & ~(1 << WGM41)) | (1 << WGM40), \
  TCCR4B = (TCCR4B & ~(1 << WGM43)) | (1 << WGM42) & 0xF8)

#define SET_PHASE_CORRECT_PWM_ICR4_MODE2 \
  (TCCR4A = (TCCR4A & ~(1 << WGM40)) | (1 << WGM41), \
  TCCR4B = (TCCR4B | (1 << WGM43)) & 0xF8)
/*--------------------------------------------------------------*/

#define SET_PRESCALER_1    (TCCR4B = (TCCR4B & 0xF8) | (1 << CS40))
#define SET_PRESCALER_8    (TCCR4B = (TCCR4B & 0xF8) | (1 << CS41))
#define SET_PRESCALER_64   (TCCR4B = (TCCR4B & 0xF8) | (1 << CS40) | (1 << CS41))
#define SET_PRESCALER_256  (TCCR4B = (TCCR4B & 0xF8) | (1 << CS42))
#define SET_PRESCALER_1024 (TCCR4B = (TCCR4B & 0xF8) | (1 << CS42) | (1 << CS40))

#define SET_PRESCALER_EXT_T4_FALLING (TCCR4B = (TCCR4B & 0xF8) | (1 << CS42) | (1 << CS41))  
#define SET_PRESCALER_EXT_T4_RISING  (TCCR4B = (TCCR4B & 0xF8) | (1 << CS42) | (1 << CS41) | (1 << CS40))  

#define ENABLE_OCR4A_INTERRUPT()  (TIMSK4 |=  (1 << OCIE4A))
#define DISABLE_OCR4A_INTERRUPT() (TIMSK4 &= ~(1 << OCIE4A))

#define ENABLE_OCR4B_INTERRUPT()  (TIMSK4 |=  (1 << OCIE4B))
#define DISABLE_OCR4B_INTERRUPT() (TIMSK4 &= ~(1 << OCIE4B))

#define wd_t_40 0
#define wd_link 1
#define wd_live 2
#define wd_3 3
#define wd_4 4
#define wd_5 5
#define wd_6 6
#define wd_7 7

bitRead(z1v[0][0],E7)  // NC(PORTE |=  (1 << PE3))
#define res &= ~(1 << 5)
#define set |=  (1 << 5)

volatile unsigned int _POS = 0;
volatile signed _INC = 0;
int fase = 0;
unsigned long t_40=0;
byte wd=0;

ISR(TIMER4_COMPB_vect)
{ 
  STEP_M_RESET; 
}

ISR(TIMER4_COMPA_vect) 
{  
  STEP_M_SET;
  OCR4A += INC;
  POS++;
}

void startTimer() 
{
  STEP_M_RESET;
  TCNT4 = 0;
  ENABLE_OCR4A_INTERRUPT();
  ENABLE_OCR4B_INTERRUPT();
  SET_PRESCALER_256;
  Serial.println("start"); 
}

void stopTimer() 
{
  TCCR4B &= ~( (1 << CS42) | (1 << CS41) | (1 << CS40) );
  TCNT4 = 0;
  DISABLE_OCR4A_INTERRUPT();
  DISABLE_OCR4B_INTERRUPT();
  STEP_M_RESET;   // STEP LOW
  Serial.println("stop"); 
}

void setup() 
{
  Serial.begin(115200);

  SET_CTC_MODE_OCR4A;
  
  DDRE |= (1 << PE3);    // Pin 5  (DIR) come OUTPUT
  DDRH |= (1 << PH3);    // Pin 6 (STEP) come OUTPUT
  DDRH |= (1 << PH4);    // Pin 7  (ENA) come OUTPUT

  STEP_M_RESET;
  DIR_M_RESET;
  ENA_M;
}
byte rampa()
{
  if(POS==target)
}

void loop() 
{
  if(millis()>t_40)
  {
    t_40+=40;
    Serial.write('#');
    Serial.print(POS);
    Serial.write(WD?'*':'!');
    wd=0b00000001;
  }
  wd &= ~(1 << wd_t_40);
  
  fase=shrittmotoren();
  
  seriale();
  
} 



void seriale()
{
  if (Serial.available())
  {
    if (predestinato)
    {
      
    }
    else
    {
      switch(Serial.read())
      {
      /* NUL */ case   0: break;
      /* SOH */ case   1: break;
      /* STX */ case   2: break;
      /* ETX */ case   3: break;
      /* EOT */ case   4: break;
      /* ENQ */ case   5: break;
      /* ACK */ case   6: break;
      /* BEL */ case   7: break;
      /* BS  */ case   8: break;
      /* HT  */ case   9: break;
      /* LF  */ case  10: break;
      /* VT  */ case  11: break;
      /* FF  */ case  12: break;
      /* CR  */ case  13: break;
      /* SO  */ case  14: break;
      /* SI  */ case  15: break;
      /* DLE */ case  16: break;
      /* DC1 */ case  17: break;
      /* DC2 */ case  18: break;
      /* DC3 */ case  19: break;
      /* DC4 */ case  20: break;
      /* NAK */ case  21: break;
      /* SYN */ case  22: break;
      /* ETB */ case  23: break;
      /* CAN */ case  24: break;
      /* EM  */ case  25: break;
      /* SUB */ case  26: break;
      /* ESC */ case  27: break;
      /* FS  */ case  28: break;
      /* GS  */ case  29: break;
      /* RS  */ case  30: break;
      /* US  */ case  31: break;
      /*     */ case  32: break;
      /*  !  */ case  33: break;
      /*  "  */ case  34: break;
      /*  #  */ case  35: break;
      /*  $  */ case  36: break;
      /*  %  */ case  37: break;
      /*  &  */ case  38: break;
      /*  '  */ case  39: break;
      /*  (  */ case  40: break;
      /*  )  */ case  41: break;
      /*  *  */ case  42: break;
      /*  +  */ case  43: break;
      /*  ,  */ case  44: break;
      /*  -  */ case  45: break;
      /*  .  */ case  46: break;
      /*  /  */ case  47: break;
      /*  0  */ case  48: break;
      /*  1  */ case  49: break;
      /*  2  */ case  50: break;
      /*  3  */ case  51: break;
      /*  4  */ case  52: break;
      /*  5  */ case  53: break;
      /*  6  */ case  54: break;
      /*  7  */ case  55: break;
      /*  8  */ case  56: break;
      /*  9  */ case  57: break;
      /*  :  */ case  58: break;
      /*  ;  */ case  59: break;
      /*  <  */ case  60: break;
      /*  =  */ case  61: break;
      /*  >  */ case  62: break;
      /*  ?  */ case  63: break;
      /*  @  */ case  64: break;
      /*  A  */ case  65: Serial.println("startTimer"); break;
      /*  B  */ case  66: Serial.println( "stopTimer"); break;
      /*  C  */ case  67: break;
      /*  D  */ case  68: break;
      /*  E  */ case  69: break;
      /*  F  */ case  70: break;
      /*  G  */ case  71: break;
      /*  H  */ case  72: break;
      /*  I  */ case  73: break;
      /*  J  */ case  74: break;
      /*  K  */ case  75: break;
      /*  L  */ case  76: break;
      /*  M  */ case  77: break;
      /*  N  */ case  78: break;
      /*  O  */ case  79: break;
      /*  P  */ case  80: break;
      /*  Q  */ case  81: break;
      /*  R  */ case  82: break;
      /*  S  */ case  83: break;
      /*  T  */ case  84: break;
      /*  U  */ case  85: break;
      /*  V  */ case  86: break;
      /*  W  */ case  87: break;
      /*  X  */ case  88: break;
      /*  Y  */ case  89: break;
      /*  Z  */ case  90: break;
      /*  [  */ case  91: break;
      /*  \  */ case  92: break;
      /*  ]  */ case  93: break;
      /*  ^  */ case  94: break;
      /*  _  */ case  95: break;
      /*  `  */ case  96: break;
      /*  a  */ case  97: break;
      /*  b  */ case  98: break;
      /*  c  */ case  99: break;
      /*  d  */ case 100: break;
      /*  e  */ case 101: break;
      /*  f  */ case 102: break;
      /*  g  */ case 103: break;
      /*  h  */ case 104: break;
      /*  i  */ case 105: break;
      /*  j  */ case 106: break;
      /*  k  */ case 107: break;
      /*  l  */ case 108: break;
      /*  m  */ case 109: break;
      /*  n  */ case 110: break;
      /*  o  */ case 111: break;
      /*  p  */ case 112: break;
      /*  q  */ case 113: break;
      /*  r  */ case 114: break;
      /*  s  */ case 115: break;
      /*  t  */ case 116: break;
      /*  u  */ case 117: break;
      /*  v  */ case 118: break;
      /*  w  */ case 119: break;
      /*  x  */ case 120: break;
      /*  y  */ case 121: break;
      /*  z  */ case 122: break;
      /*  {  */ case 123: break;
      /*  |  */ case 124: break;
      /*  }  */ case 125: break;
      /*  ~  */ case 126: break;
      /* DEL */ case 127: break;
      default: break;
    }
    }
  }
}
//-void seriale-
  

  
byte shrittmotoren()
{
  switch(shritt)
  {
    case  10: 
      OCR4A=2000; 
      startTimer(); 
      nf=20; 
    return nf;
    
    case  15: 
      OCR4A=4000; 
      digitalWrite(5,HIGH);
      startTimer(); 
      nf=20; 
    return nf;
    
    case  20: 
      if (POS==1600){     nf=21; Serial.println("=200");} 
      else if (POS>3200){ nf=21; Serial.println(">200");} 
      
    return nf;
    
    case  21: stopTimer(); nf=0;  return nf;
    
    default:  return nf;
  }
}
