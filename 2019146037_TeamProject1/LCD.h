/*
 * LCD.h
 *
 * Created: 2023-06-19 오후 6:26:51
 *  Author: ghdtj
 */ 


#ifndef LCD_H_
#define LCD_H_

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD_WDATA PORTA		// LCD 데이터 버스 정의 (데이터 쓰기)
#define LCD_WINST PORTA     // LCD 데이터 버스 정의 (명령어 쓰기)
#define LCD_CTRL  PORTG     // LCD 제어 신호 정의
#define LCD_EN    0			// Enable 신호
#define LCD_RW    1         // 읽기(1)/쓰기(0)
#define LCD_RS    2			// 데이터(1)/명령어(0)
#define RIGHT 1
#define LEFT 0

void PortInit (void)
{
	DDRA = 0xFF; // PORTA(LCD data pin)를 출력으로
	DDRG = 0x0F; // PORTG의 하위 4비트를 출력으로
}

void LCD_Comm(unsigned ch)
{
	LCD_CTRL &= ~(1 << LCD_RS);		// RS==0으로 명령(IR 레지스터 접근 설정)
	LCD_CTRL &= ~(1 << LCD_RW);		// RW==0으로 쓰기 설정
	LCD_CTRL |=  (1<< LCD_EN);		// LCD Enable
	_delay_us(50);					// 시간지연
	LCD_WINST = ch;					// 명령어쓰기
	_delay_us(50);					// 시간지연
	LCD_CTRL &= ~(1 << LCD_EN);		// LCD Enable
}

void LCD_Data( unsigned char ch )
{
	LCD_CTRL |= (1 << LCD_RS);		// RS==1으로 데이터(DR 레지스터 접근 설정)
	LCD_CTRL &= ~(1 << LCD_RW);     // RW==0으로 쓰기 설정
	LCD_CTRL |= (1 << LCD_EN);		// LCD Enable
	_delay_us(50);					// 시간지연
	LCD_WDATA = ch;				    // 명령어쓰기
	_delay_us(50);					// 시간지연
	LCD_CTRL &= ~(1 << LCD_EN);		// LCD Enable
}

void LCD_CHAR( unsigned char c )	// 한 문자 출력
{
	LCD_Data(c);
	_delay_ms(1);
}

void LCD_STR( unsigned char *str )  // 문자열 출력
{
	while(*str != 0) {
		LCD_CHAR(*str);
		str++;
	}
}

void LCD_pos( unsigned char row, unsigned char col ) // LCD 포지션 설정
{
	LCD_Comm( 0x80 | ( (row * 0x40) + col ) ); // row = 문자행, col = 문자열
}

void LCD_Clear(void) // 화면 클리어 (1)
{
	LCD_Comm(0x01); // LCD Clear
	_delay_ms(2); // 1.64ms 이상을 기다림
}

void LCD_Init(void)
{
	LCD_Comm(0x30);		// 초기화 Set
	_delay_us(4100);	// 4.1ms 지연
	LCD_Comm(0x30);		// 초기화 Set
	_delay_us(100);     // 1000us 지연
	LCD_Comm(0x30);		// 초기화 Set
	_delay_us(100);		// 1000us 지연
	
	LCD_Comm(0x38);		// 초기화 Set, 데이터 길이 8Bit, 표시라인 2행 사용을 위한 기능 설정
	_delay_us(1000);	// 명령을 처리 하는데 최소 40us 지연이 발생하기에 여유를 고려하여 설정
	
	LCD_Comm(0x0c); // 화면 On/Off(3) , Display ON/Off(2), Cursor On/Off(1), Blink On/Off(1)
	_delay_us(1000); // 1000us 지연
	
	LCD_Comm(0x01); // LCD Clear
	_delay_us(2000); // 2000us 지연
	LCD_Comm(0x06);  // Cursor Entry Mode Set, 표시 위치 +`씩 증가
	_delay_us(1000); // 1000us 지연
}

void Cursor_Home(void)
{
	LCD_Comm(0x02);		 // Cursor Home
	_delay_ms(2);		 // 2ms 시간지연
}

void LCD_Shift(char p)    // 디스플레이 시프트 (5)
{
	// 표시 화면 전체를 오른쪽으로 이동
	if(p == RIGHT) {
		LCD_Comm(0x1C);	 // * A에서 C로 바꿈
		_delay_ms(1);	 // 시간 지연
	}
	// 표시 화면 전체를 왼쪽으로 이동
	else if(p == LEFT) {
		LCD_Comm(0x18);
		_delay_ms(1);
	}
}

void Cursor_Shift(char p)
{
	if(p == RIGHT) {
		LCD_Comm(0x14);
		_delay_ms(1);
	}
	else if(p == LEFT) {
		LCD_Comm(0x10);
		_delay_ms(1);
	}
}



#endif /* LCD_H_ */