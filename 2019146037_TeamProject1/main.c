#include "LCD.h"
#define  F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/portpins.h>
#define	   DO    1908 // 262Hz (3817us) 1908us
#define	   RE    1700 // 294Hz (3401us) 1701us
#define	   MI    1515 // 330Hz (3030us) 1515us
#define	   FA    1432 // 349Hz (2865us) 1433us
#define	   SOL   1275 // 370Hz (2703us) 1351us
#define	   LA    1136 // 440Hz (2273us) 1136us
#define	   SI    1012 // 494Hz (2024us) 1012us
#define    HIGH_DO    956 // 523Hz (1912us) 956us
#define    HIGH_RE    852 // 587Hz (1704us) 852us
#define    HIGH_MI    759 // 659Hz (1517us) 759us
#define    HIGH_FA    717 // 698Hz (1433us) 717us
#define	   HIGH_SOL   638 // 784Hz (1276us) 638us

void myDelay_us(unsigned int delay){
	int i;
	for(i=0; i<delay; i++){
		_delay_us(1);
	}
}

void SSound(int time) {
	int i, tim;
	tim = 25000 / time;     //0.05 초
	for(i=0; i<tim; i++){
		PORTG |= (1<<PG4); //buzzer on, PORTG의 4번 핀 off(out 1)
		myDelay_us(time);
		PORTG &= ~(1<<PG4); //buzzer off, PORTG의 4번 핀 on(on 1)
		myDelay_us(time);
	}
	PORTG |= (1<<PG4); // buzzer off, PORTG의 4번 핀 off(out 0)
}

void TWO_SSound(int time) {
	int i, tim;
	tim = 50000 / time;     //0.1 초
	for(i=0; i<tim; i++){
		PORTG |= (1<<PG4); //buzzer on, PORTG의 4번 핀 off(out 1)
		myDelay_us(time);
		PORTG &= ~(1<<PG4); //buzzer off, PORTG의 4번 핀 on(on 1)
		myDelay_us(time);
	}
	PORTG |= (1<<PG4); // buzzer off, PORTG의 4번 핀 off(out 0)
}

void THREE_SSound(int time) {
	int i, tim;
	tim = 250000 / time;     //0.5 초
	for(i=0; i<tim; i++){
		PORTG |= (1<<PG4); //buzzer on, PORTG의 4번 핀 off(out 1)
		myDelay_us(time);
		PORTG &= ~(1<<PG4); //buzzer off, PORTG의 4번 핀 on(on 1)
		myDelay_us(time);
	}
	PORTG |= (1<<PG4); // buzzer off, PORTG의 4번 핀 off(out 0)
}

void gen_sound(int sw)
{
	if(			sw == 0x01){ SSound(DO); _delay_ms(500);} // 1
	else if(	sw == 0x02){ SSound(RE); _delay_ms(500);} // 2
	else if(	sw == 0x03){ SSound(MI); _delay_ms(500);} // 3
	else if(	sw == 0x04){ SSound(HIGH_MI); _delay_ms(500);} // M1
	else if(	sw == 0x05){ SSound(FA); _delay_ms(500);} // 4
	else if(	sw == 0x06){ SSound(SOL); _delay_ms(500);} // 5
	else if(	sw == 0x07){ SSound(LA); _delay_ms(500);} // 6
	else if(	sw == 0x08){ SSound(HIGH_MI); _delay_ms(500);} // M2
	else if(	sw == 0x09){ SSound(SI); _delay_ms(500);} // 7
	else if(	sw == 0x0a){ SSound(HIGH_DO); _delay_ms(500);} // 8
	else if(	sw == 0x0b){ SSound(HIGH_RE); _delay_ms(500);} // 9
	else if(	sw == 0x0c){ SSound(HIGH_MI); _delay_ms(500);} // M3
	else if(	sw == 0x0d){ SSound(HIGH_MI); _delay_ms(500);} // *
	else if(	sw == 0x0e){ SSound(HIGH_MI); _delay_ms(500);} // 0
	else if(	sw == 0x0f){ SSound(HIGH_MI); _delay_ms(500);} // #
	else if(	sw == 0x10){ SSound(HIGH_MI); _delay_ms(500);} // M4
}
// 여기까지 부저

#define SetRow(x)	PORTC |= 0xF0;	PORTC &= ~(1<<((x-1)+PC4));
unsigned char Port_char[] = {
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90, 0x88, 0x83, 0xc4, 0xa1, 0x84, 0x8e
}; // 애노드 공통

unsigned int Port_fnd[] = {0x1f, 0x2f, 0x4f, 0x8f}; // FND0 ON, FND1 ON, FND2 ON, FND3 ON

void PORT_Init(void){
	DDRG  |= (1<<PG4);   // 부저와 연결되는 PORTG.4를 출력으로 설정
	PORTG |= (1<<PG4);   // 교육용보드의 BUZZ는 회로가 ACctive-Low로 되어있음으로
	// HIGH 상태 출력하여 부저 꺼줌
	
	DDRE = 0xf0;		// FND 제어위한 출력 설정(PE4..7)
	DDRB = 0xff;		// FND 제어위한 출력 설정(PB0..7)
	DDRC = 0xF0;		// PORTC 4 ~ 7 은 Row 선택위한 출력으로 설정
	// PORTC 0 ~ 3 은 col 값 읽기위한 입력으로 설정
}

void FND_Disp(int dec){
	// FND Digit 선택,   // FND에 폰트 출력              // 눈에 보이도록 시간지연 // FND 끔
	PORTE = Port_fnd[0]; PORTB = Port_char[(dec/1000%10)]; _delay_ms(1); PORTB = 0xFF;
	PORTE = Port_fnd[1]; PORTB = Port_char[(dec/100%10)]; _delay_ms(1); PORTB = 0xFF;
	PORTE = Port_fnd[2]; PORTB = Port_char[(dec/10%10)]; _delay_ms(1); PORTB = 0xFF;
	PORTE = Port_fnd[3]; PORTB = Port_char[(dec/1%10)]; _delay_ms(1); PORTB = 0xFF;
	PORTE = 0x00;
}

void setFND(int value){
	// 키패드 숫자값을 불러와서 PORTB에 적용
	PORTB = Port_char[value];
}

int getNum(unsigned char keyCode){
	// 키패드 숫자 배정
	unsigned char keyNum[16] = {1,2,3,'A', 4,5,6,'B', 7,8,9,'C', '*',0,'#','D'};
	if(keyCode > 16)
	return -1;
	return keyNum[keyCode-1];
}
// 여기까지 키패드 및 GND

unsigned char Count;
ISR (INT0_vect){
	unsigned char str_password_set[] = "PASSWORD SET";
	unsigned char str_input_password_set[] = "--------------";
	unsigned char str_success[] = "-SUCCESS-";
	// 비밀번호 변경
	if (Count == 0){
		LCD_Clear();
		LCD_pos(0,1); LCD_STR(str_password_set);
		LCD_pos(1,1); LCD_STR(str_input_password_set);
		Count++;
	}
	// 비밀번호 변경 완료
	else{
		LCD_Clear();
		SSound(DO); _delay_ms(500); SSound(RE); _delay_ms(500); SSound(MI);
		LCD_pos(0,1); LCD_STR(str_password_set);
		LCD_pos(1,1); LCD_STR(str_success);
		Count = 0;
	}
}

void interrupt_init(void)
{
	EIMSK = 0x01;		// 외부인터럽트 종류 선택
	EICRA = 0x02;		// 인터럽트 제어방법 선택(INT0 하강에지트리거)
	DDRD = 0x00;		 // TACT 스위치를 입력으로 설정 + 포트 D를 입력으로 선택
	sei();				// 인터럽트 동작시작(전체인터럽트허가)
}
// 여기까지 외부 인터럽트

unsigned int T2_DUTY_TIME_us;	 // PWM 파형 듀티 폭 시간
unsigned int T2_CYCLE_TIME_us;	 // PWM 파형 주기 시간

unsigned int T2_DUTY_TIME_CNT_us; // PWM 파형 듀티 시간을 세기 위한 카운트 변수
unsigned int T2_CYCLE_TIME_CNT_us; // PWM 파형 주기 시간을 세기 위한 카운트 변수

// 100uS 주기 인터럽트
ISR(TIMER2_COMP_vect){
	T2_CYCLE_TIME_CNT_us += 100;
	T2_DUTY_TIME_CNT_us += 100;
	if(T2_DUTY_TIME_CNT_us <= T2_DUTY_TIME_us){
		PORTF |= (1<<PF3);
		}else{
		PORTF &= ~(1<<PF3);
	}
	
	if(T2_CYCLE_TIME_CNT_us == T2_CYCLE_TIME_us){
		T2_CYCLE_TIME_CNT_us = 0;
		T2_DUTY_TIME_CNT_us = 0;
	}
}

void Init_Timer2(void) {  //타이머/카운터 2 – 2초 설정 및 PC PWM 듀티비 설정
	TIMSK |= (1 << OCIE2);                // ① 출력비교 인터럽트2 허가
	TCCR2 = (1 << WGM21) | (2 << CS20);   // ② CTC 모드, 1024분주
	OCR2 = 184;                            // ② 약 100us
	// OCR2 = (14745600Hz / 8 분주비) * 100us = 184.32
}

void Init_TimerINT(void) {
	Init_Timer2();
	SREG |= 0x80;
}
void SetServoDeg(unsigned int deg) {
	T2_DUTY_TIME_us = 500 + (deg*200/18); // 듀티 폭 시간 생성부분
}

void PORT_TIMER_Init(void){
	DDRF |= (1 << PF3);
}

// 여기까지 타이머 및 서브 모터 제어

void Siren(void){
	for (int i =1700; i>637; i--){
		SSound(i);
		i -=10;
	}
	
	for (int i = 638; i<1701; i++){
		SSound(i);
		i+=20;
	}
}
// 사이렌 소리

void Stealing(void) {
	unsigned char str_stealwarning[] = "warning.....";
	unsigned char str_stealing[] = "stealing!!!";
	Siren();
	LCD_pos(0,1); LCD_STR(str_stealwarning);
	LCD_pos(1,2); LCD_STR(str_stealing);
}

// 진동센서 관련 함수

void Master(void) {
	unsigned char str_mastermode[] = "MASTER MODE";
	unsigned char masterkey[] = "2019146037";
	unsigned char user_master[15];
	unsigned char star[] = "**************";
	LCD_pos(0,1); LCD_STR(str_mastermode);
	LCD_pos(1,1); LCD_STR(star);
}

//마스터키 함수
void main(){
	unsigned char sel_row;
	unsigned char keypad_code = 0;
	unsigned char hkeypad_code = 0;
	int fnd_value = 0;
	unsigned int state = 0;
	unsigned char col;
	
	unsigned int sw;
	unsigned char str_password[] = "PASSWORD";
	unsigned char number[]  = "**************";
	unsigned char str_dooropen[] = "DOOR OPEN";
	unsigned char str_error[] = "error.......";
	unsigned char str_warning[] = "warning.....";
	unsigned char str_theft[] = "if,theft";
	
	unsigned int  position = 1;
	
	unsigned char user_password[15] = {1,2,3,4,5,6,7,8,9,0,10,10,10,10,10};
	unsigned char test_password[15] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
	unsigned char new_password[15] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
	
	unsigned int accuracy = 0;
	
	Count = 0; // 외부인터럽트 발생 횟수 초기화
	
	unsigned int error_count = 0;
	unsigned int select_count = 0;
	
	PortInit();  // LCD 포트 제어
	LCD_Init();  // LCD 초기화 함수
	
	PORT_Init(); // 부저 및 키패드 포트 제어
	interrupt_init();
	
	PORT_TIMER_Init(); // 서브 모터 포트 제어
	Init_TimerINT();  // 타이머 초기화
	
	TWO_SSound(SOL);
	
	
	
	while(1){
		// 초기 상태로 '#'버튼을 눌러 패스워드 입력창이 뜨기 전까지 FND가 켜지지 않게 한다.
		if(state == 0){
			state = state;
		}
		else{
			FND_Disp(fnd_value); // FND에 숫자 출력 [4자리]
		}
		
		keypad_code = 0xFF;  // 키패드 입력감지 위해 초기값 설정(이후에 0xff가 아니면 키패드 입력 있음)
		for(sel_row=1; sel_row<=4; sel_row++){// row를 바꾸어 가며 키패드 일기
			SetRow(sel_row);   // row 선택
			_delay_ms(10);      // 안정적인 col 읽기 위함
			col = (PINC & 0x0F);
			switch(col){
				case 0x01 : keypad_code = (sel_row - 1)*4 + 1; break; // 1번 col의 스위치가 눌린 경우
				case 0x02 : keypad_code = (sel_row - 1)*4 + 2; break; // 2번 col의 스위치가 눌린 경우
				case 0x04 : keypad_code = (sel_row - 1)*4 + 3; break; // 3번 col의 스위치가 눌린 경우
				case 0x08 : keypad_code = (sel_row - 1)*4 + 4; break; // 4번 col의 스위치가 눌린 경우
			}
		}
		sw = keypad_code;
		if(Count ==1){
			select_count = 0;
			if(			sw == 0x01){LCD_pos(1,position); LCD_CHAR('1'); new_password[position-1] = 1; position += 1;} // 1
			else if(	sw == 0x02){LCD_pos(1,position); LCD_CHAR('2'); new_password[position-1] = 2; position += 1;} // 2
			else if(	sw == 0x03){LCD_pos(1,position); LCD_CHAR('3'); new_password[position-1] = 3; position += 1;} // 3
			else if(	sw == 0x04){position = position;} // M1
			else if(	sw == 0x05){LCD_pos(1,position); LCD_CHAR('4'); new_password[position-1] = 4; position += 1;} // 4
			else if(	sw == 0x06){LCD_pos(1,position); LCD_CHAR('5'); new_password[position-1] = 5; position += 1;} // 5
			else if(	sw == 0x07){LCD_pos(1,position); LCD_CHAR('6'); new_password[position-1] = 6; position += 1;} // 6
			else if(	sw == 0x08){position = position;} // M2
			else if(	sw == 0x09){LCD_pos(1,position); LCD_CHAR('7'); new_password[position-1] = 7; position += 1;} // 7
			else if(	sw == 0x0a){LCD_pos(1,position); LCD_CHAR('8'); new_password[position-1] = 8; position += 1;} // 8
			else if(	sw == 0x0b){LCD_pos(1,position); LCD_CHAR('9'); new_password[position-1] = 9; position += 1;} // 9
			else if(	sw == 0x0c){position = position;} // M3
			else if(	sw == 0x0d){position = position;} // *
			
			else if(	sw == 0x0e){LCD_pos(1,position); LCD_CHAR('0'); new_password[position-1] = 0; position += 1;} // 0
			else if(	sw == 0x0f){position = position;} // #
			else if(	sw == 0x10){position = position;} //M4
			
			// 다음번 번호 입력할 때 이번 입력과 겹치지 않도록 배열을 초기화 하는 과정
			for(int i=0; i<(position-1); i++){
				user_password[i] = new_password[i];
				test_password[i] = 10;
			}
			for(int i=position-1; i<15; i++){
				user_password[i] = 10;
				new_password[i] = 10;
				test_password[i] = 10;
			}
		}
		
		else{
			if(			sw == 0x01){LCD_pos(1,position); LCD_CHAR('1'); test_password[position-1] = 1; position += 1; select_count = 0;} // 1
			else if(	sw == 0x02){LCD_pos(1,position); LCD_CHAR('2'); test_password[position-1] = 2; position += 1; select_count = 0;} // 2
			else if(	sw == 0x03){LCD_pos(1,position); LCD_CHAR('3'); test_password[position-1] = 3; position += 1; select_count = 0;} // 3
			else if(	sw == 0x04){position = position;} // M1
			else if(	sw == 0x05){LCD_pos(1,position); LCD_CHAR('4'); test_password[position-1] = 4; position += 1; select_count = 0;} // 4
			else if(	sw == 0x06){LCD_pos(1,position); LCD_CHAR('5'); test_password[position-1] = 5; position += 1; select_count = 0;} // 5
			else if(	sw == 0x07){LCD_pos(1,position); LCD_CHAR('6'); test_password[position-1] = 6; position += 1; select_count = 0;} // 6
			else if(	sw == 0x08){position = position;} // M2
			else if(	sw == 0x09){LCD_pos(1,position); LCD_CHAR('7'); test_password[position-1] = 7; position += 1; select_count = 0;} // 7
			else if(	sw == 0x0a){LCD_pos(1,position); LCD_CHAR('8'); test_password[position-1] = 8; position += 1; select_count = 0;} // 8
			else if(	sw == 0x0b){LCD_pos(1,position); LCD_CHAR('9'); test_password[position-1] = 9; position += 1; select_count = 0;} // 9
			else if(	sw == 0x0c){position = position; select_count = 0;} // M3
			else if(	sw == 0x0d){
				select_count +=1;
				for(int i = 0; i <sizeof(user_password); i++){
					if(test_password[i] == user_password[i]) { accuracy += 1; }
					else { accuracy = accuracy;}}
					
					if (accuracy == sizeof(user_password)) {LCD_Clear(); error_count = 0;
						THREE_SSound(DO); _delay_ms(500); THREE_SSound(MI); _delay_ms(500); THREE_SSound(SOL); _delay_ms(500);  THREE_SSound(HIGH_DO);
						SetServoDeg(90);
						LCD_pos(0,1); LCD_STR(str_dooropen);
					}
					else {LCD_Clear(); error_count += 1;
						THREE_SSound(LA); _delay_ms(500); THREE_SSound(LA); _delay_ms(500); THREE_SSound(LA);
						LCD_pos(0,1); LCD_STR(str_password);
						LCD_pos(1,2); LCD_STR(str_error);
						_delay_ms(3000); LCD_Clear(); fnd_value = 0; state = 0;
					}
					for (int i=0; i<15; i++){
						
						test_password[i] = 10; // 다음번에 번호를 입력할 때 겹치지 않도록 초기화 시켜주는 과정
					}
					position = 1; accuracy = 0;
				} // *
				
				else if(	sw == 0x0e){LCD_pos(1,position); LCD_CHAR('0'); test_password[position-1] = 0; position += 1; select_count = 0;} // 0
				else if(	sw == 0x0f){state = 1; position = 1; select_count = 0; LCD_Clear(); LCD_pos(0,1); _delay_ms(1); LCD_STR(str_password); LCD_pos(1,1); LCD_STR(number); select_count = 0;} // #
				else if(	sw == 0x10){position = position; select_count = 0;} // M4
				// 4번
				if(select_count >=2 ) {
					SSound(SOL); _delay_ms(500); SSound(FA); _delay_ms(500); SSound(MI); _delay_ms(500); SSound(RE);
					SetServoDeg(0);
					select_count = 0;
				}
				else {
					select_count = select_count;
				}
				// 6번
				if(error_count >= 3) {
					// 비밀번호 3번 이상 틀릴 시
					Siren();
					LCD_pos(0,1); LCD_STR(str_warning);
					LCD_pos(1,2); LCD_STR(str_theft);
				}
			}
			
			gen_sound(keypad_code);
			
			if((keypad_code != 0xFF) && (hkeypad_code != keypad_code)){
				fnd_value %= 1000;
				fnd_value *= 10;
				fnd_value += getNum(keypad_code);
			}
			hkeypad_code = keypad_code; // 키패드가 눌렸으면 FND 표시	
		}
	}
