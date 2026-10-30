// note scrise de Stefan Stolniceanu dupa pitches.h din Examples->Digital->toneMelody:
#define do  131
#define do_diez 139
#define re  147
#define re 156
#define mi  165
#define fa  175
#define fa_diez 185
#define sol  196
#define sol_diez 208
#define la  220
#define la_diez 233
#define si  247
 
// aici incepe cheia Sol (octava lui C4):
#define Do  262 
#define Do_diez 277
#define Re  294
#define Re_diez 311
#define Mi  330
#define Fa  349
#define Fa_diez 370
#define Sol  392 
#define Sol_diez 415
#define La  440
#define La_diez 466
#define Si  494
 
// aici incepe cheia sol cu o octava mai sus (octava lui C5)
#define DO  523
#define DO_diez 554
#define RE  587
#define RE_diez 622
#define MI  659
#define FA  698
#define FA_diez 740
#define SOL  784
#define SOL_diez 831
#define LA  880
#define LA_diez 932
#define SI  988

int note[] = {
    Sol,6,   Do,4,   Do,8,   DO,12,  Si,4,
    La,4,    Sol,4,  La,4,   Sol,4,  
    Fa,4,    Mi,4,   Re,4,   Do,4,
    Mi,8,    Fa,8,   Sol,16,
    Mi,8,    Fa,8,   Sol,8,   Mi,8,
    
    Do,8,    Mi,8,   Re,8,   Do,8,
    si,8,    la,8,   Re,8,   si,8,
    sol,16,  Sol,8,  Fa,8,
    Sol,8,   La,8,   Si,16, o,32,
    
    Sol,8,   DO,12,  Si,4,
    La,8,    Sol,8,  Fa,8, Mi,8,
    Re,8,    Do,8,   Mi,8, Fa,8, Sol,32,
    
    Mi,8,    Fa,8, Sol,8,   Mi,8,
    Do,8,    Mi,8,   Re,8,   Do,8,
    
    si,8,    la,8,   Re,8,   si,8,
    Do,8,    Re,8,   Mi,16,
    Sol,16,  Sol,8,  Fa,8,
    Mi,8,    Re,8,   Do,16,
    Do,32
};

const int NUMAR_NOTE = sizeof(note) / sizeof(note[0]) / 2;

const unsigned long BAZA_TIMP_MS = 1200UL;

bool gata = false;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (gata) {
    return;
  }

  for (int i = 0; i < NUMAR_NOTE; i++) {
    int frecventa = note[i * 2];
    int divizor = note[i * 2 + 1];

    if (divizor <= 0) {
      divizor = 8;
    }

    unsigned long durata_totala = BAZA_TIMP_MS / (unsigned long)divizor;
    unsigned long durata_sunet = (durata_totala * 85UL) / 100UL;
    unsigned long pauza_intre_note = durata_totala - durata_sunet;

    if (frecventa > 0) {
      tone(BUZZER_PIN, frecventa, durata_sunet);
    } else {
      noTone(BUZZER_PIN);
    }

    delay(durata_sunet);
    noTone(BUZZER_PIN);
    delay(pauza_intre_note);
  }

  noTone(BUZZER_PIN);
  gata = true;
}