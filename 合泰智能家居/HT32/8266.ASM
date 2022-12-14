; Generated by holtek-gcc v3.19, Sat Mar 26 18:07:02 2022
; 
; Configuration:
;       with long instruction
;       Multi-ROM, Multi-RAM
;       bits_per_rom_unit:16
;       with mp1
;       with tbhp, address(0x9)
;          Use tabrd-const
;       
; SFR address of long-instruction arch:
;    mp0 = -1,1,0
;    mp1 = 4,3,2
;    mp2 = 14,13,12
;    tbp = 9,7,8
;    acc = 5
;    pcl = 6
;    status = 10
;    bp = 11
;    intc = 16
;       
;       
; use 'tabrdc' instead of 'tabrd'
;       

#pragma translator "holtek-gcc 4.6.4" "3.19" "build 20130711"
; Rebuild 718

ir equ [2]
mp equ [3]
sbp equ [4]
acc equ [5]
bp equ [11]
tblp equ [7]
tbhp equ [9]
status equ [10]
c equ [10].0
ac equ [10].1
z equ [10].2
ov equ [10].3
cz equ [10].6
sc equ [10].7
intc equ [16]
mduwr0 equ [59h]
mduwr1 equ [5ah]
mduwr2 equ [5bh]
mduwr3 equ [5ch]
mduwr4 equ [5dh]
mduwr5 equ [5eh]
mduwrctrl equ [5fh]

extern ra:byte
extern rb:byte
extern rc:byte
extern rd:byte
extern re:byte
extern rf:byte
extern rg:byte
extern rh:byte
extern _Crom2Prom:near
extern _Crom2PromNext:near

RAMBANK 0 @BITDATASEC, @BITDATASEC1
@HCCINIT	.section 'data'
@HCCINIT0	.section 'data'
@BITDATASEC	 .section 'data'
@BITDATASEC1	 .section 'data'

#pragma debug scope 1 1
	extern __DELAY3:near
	extern __DELAYX3:near
	extern __DELAYX6:near
	extern __DELAYY5:near
	extern __DELAYY3:near
	extern _builtin_holtek_delay_2:byte
___pbs0 equ [335]
___u2cr1 equ [268]
___u2cr2 equ [269]
___mfi5 equ [56]
___brg2 equ [271]
___intc3 equ [19]
___intc0 equ [16]
public _esp8266_init
#pragma debug scope 2 1
#line 6 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
_esp8266_init .section 'code'
_esp8266_init proc
#line 7 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,240
	lorm a,[335]
#line 8 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,128
	lmov [268],a
#line 9 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,236
	lmov [269],a
#line 10 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	set [56].2
#line 12 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,207
	lmov [271],a
#line 13 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	set [19].3
#line 14 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	set [16].0
	ret
_esp8266_init endp
#line 14 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
#pragma debug scope 1 1
___txr_rxr2 equ [270]
___u2sr equ [267]
public _send_char
#pragma debug scope 3 1
#line 17 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
_send_char .section 'code'
_send_char proc
    local _send_char_2 db 1 dup(?)	; 0,1
	mov ra,a
	mov _send_char_2[0],a
#pragma debug variable 7 3 _send_char_2+0 "s"
#line 18 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,ra
	lmov [270],a
_L4:
#line 19 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	lsnz [267].1
	jmp _L4
	ret
_send_char endp
#line 19 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
#pragma debug scope 1 1
public _re_buff
#pragma debug scope 4 1
#line 22 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
_re_buff .section 'code'
_re_buff proc
public _re_buff_2
    local _re_buff_2 db 3 dup(?)	; 3,0
#pragma debug pointer 42 7 4
#pragma debug variable 42 4 _re_buff_2+0 "rbuff"
#pragma debug variable 7 4 _re_buff_2+2 "c"
_L8:
#line 25 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	lsnz [267].3
	jmp _L8
#line 26 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	lmov a,[270]
	mov ra,a
	mov a,_re_buff_2[0]
	mov rb,a
	mov a,_re_buff_2[1]
	mov rc,a
#line 29 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,rb
	mov mp,a
	mov a,rc
	mov sbp,a
#line 27 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,ra
	sub a,_re_buff_2[2]
	sz z
	jmp _L9
#line 29 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,ra
	mov ir,a
#line 39 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,1
	mov ra,a
	clr rb
	jmp _L10
_L9:
#line 34 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,0
	mov ir,a
#line 37 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	clr ra
	clr rb
_L10:
	ret
_re_buff endp
#line 37 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
#pragma debug scope 1 1
public _ESP8266_SendCmd
#pragma debug scope 5 1
#line 42 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
_ESP8266_SendCmd .section 'code'
_ESP8266_SendCmd proc
public _ESP8266_SendCmd_2
    local _ESP8266_SendCmd_2 db 2 dup(?)	; 2,0
#pragma debug pointer 43 7 5
#pragma debug variable 43 5 _ESP8266_SendCmd_2+0 "pbuf"
_L12:
#line 43 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,_ESP8266_SendCmd_2[0]
	mov mp,a
	mov a,_ESP8266_SendCmd_2[1]
	mov sbp,a
	fcall _Crom2Prom
	mov ra,a
	snz ra
	jmp _L14
#line 45 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,ra
	fcall _send_char
#line 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 10>263690
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 10of the argument exceeds the maxinum 263690'
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 10<0
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 10 of the argument exceeds the mininum 0'
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 10==0
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	JMP $
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=1) AND (10<( 4 + __PBP__ * 2))
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=2) AND (10<( 4 + __PBP__ * 2))
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=3) AND (10<( 4 + __PBP__ * 2))
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=4) AND (10<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=5) AND (10<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=6) AND (10<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=7) AND (10<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10>=8) AND (10<( 4 + __PBP__ * 2))  AND ( __PBP__ > 2 )
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10==9)  AND ( __PBP__ > 2 )
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10< ( 8 + __PBP__ * 2)) AND (10>( 3 + __PBP__ * 2))
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAY3 + ( 7 + __PBP__ * 2) -10
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10< ( 0404h + __PBP__ * 2)) AND (10>( 7 + __PBP__ * 2))
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(10-( 4 + __PBP__ * 2))/4
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX3 + 3 -((10-( 4 + __PBP__ * 2)) MOD 4) 
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10< ( 040bh + __PBP__ * 2)) AND (10> ( 0403h + __PBP__ * 2))
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0 
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX6 + ( 040ah + __PBP__ * 2) -10
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10 >= ( 040bh + __PBP__ * 2)) AND ((((10-( 9 + __PBP__ * 2))%0402h)/4+1)>256)
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(10-( 9 + __PBP__ * 2))/0402h
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY5 +5- (((10-( 9 + __PBP__ * 2))%0402h)%6) 
	ENDIF
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (10 >= ( 040bh + __PBP__ * 2)) AND ((((10-( 9 + __PBP__ * 2))%0402h)/4+1)<=256)
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(10-( 9 + __PBP__ * 2))/0402h
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,((10-( 9 + __PBP__ * 2))%0402h)/4+1
;# 46 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY3 +3-(((10-( 9 + __PBP__ * 2))%0402h)%4) 
	ENDIF
#line 47 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	inc _ESP8266_SendCmd_2[0]
	sz z
	inc _ESP8266_SendCmd_2[1]
	jmp _L12
_L14:
#line 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 100>263690
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 100of the argument exceeds the maxinum 263690'
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 100<0
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 100 of the argument exceeds the mininum 0'
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 100==0
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	JMP $
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=1) AND (100<( 4 + __PBP__ * 2))
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=2) AND (100<( 4 + __PBP__ * 2))
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=3) AND (100<( 4 + __PBP__ * 2))
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=4) AND (100<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=5) AND (100<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=6) AND (100<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=7) AND (100<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=8) AND (100<( 4 + __PBP__ * 2))  AND ( __PBP__ > 2 )
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100==9)  AND ( __PBP__ > 2 )
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100< ( 8 + __PBP__ * 2)) AND (100>( 3 + __PBP__ * 2))
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAY3 + ( 7 + __PBP__ * 2) -100
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100< ( 0404h + __PBP__ * 2)) AND (100>( 7 + __PBP__ * 2))
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(100-( 4 + __PBP__ * 2))/4
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX3 + 3 -((100-( 4 + __PBP__ * 2)) MOD 4) 
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100< ( 040bh + __PBP__ * 2)) AND (100> ( 0403h + __PBP__ * 2))
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0 
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX6 + ( 040ah + __PBP__ * 2) -100
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100 >= ( 040bh + __PBP__ * 2)) AND ((((100-( 9 + __PBP__ * 2))%0402h)/4+1)>256)
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(100-( 9 + __PBP__ * 2))/0402h
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY5 +5- (((100-( 9 + __PBP__ * 2))%0402h)%6) 
	ENDIF
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100 >= ( 040bh + __PBP__ * 2)) AND ((((100-( 9 + __PBP__ * 2))%0402h)/4+1)<=256)
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(100-( 9 + __PBP__ * 2))/0402h
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,((100-( 9 + __PBP__ * 2))%0402h)/4+1
;# 49 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY3 +3-(((100-( 9 + __PBP__ * 2))%0402h)%4) 
	ENDIF
#line 50 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,13
	fcall _send_char
#line 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 100>263690
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 100of the argument exceeds the maxinum 263690'
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 100<0
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 100 of the argument exceeds the mininum 0'
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 100==0
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	JMP $
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=1) AND (100<( 4 + __PBP__ * 2))
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=2) AND (100<( 4 + __PBP__ * 2))
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=3) AND (100<( 4 + __PBP__ * 2))
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=4) AND (100<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=5) AND (100<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=6) AND (100<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=7) AND (100<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100>=8) AND (100<( 4 + __PBP__ * 2))  AND ( __PBP__ > 2 )
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100==9)  AND ( __PBP__ > 2 )
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100< ( 8 + __PBP__ * 2)) AND (100>( 3 + __PBP__ * 2))
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAY3 + ( 7 + __PBP__ * 2) -100
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100< ( 0404h + __PBP__ * 2)) AND (100>( 7 + __PBP__ * 2))
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(100-( 4 + __PBP__ * 2))/4
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX3 + 3 -((100-( 4 + __PBP__ * 2)) MOD 4) 
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100< ( 040bh + __PBP__ * 2)) AND (100> ( 0403h + __PBP__ * 2))
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0 
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX6 + ( 040ah + __PBP__ * 2) -100
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100 >= ( 040bh + __PBP__ * 2)) AND ((((100-( 9 + __PBP__ * 2))%0402h)/4+1)>256)
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(100-( 9 + __PBP__ * 2))/0402h
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY5 +5- (((100-( 9 + __PBP__ * 2))%0402h)%6) 
	ENDIF
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (100 >= ( 040bh + __PBP__ * 2)) AND ((((100-( 9 + __PBP__ * 2))%0402h)/4+1)<=256)
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(100-( 9 + __PBP__ * 2))/0402h
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,((100-( 9 + __PBP__ * 2))%0402h)/4+1
;# 51 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY3 +3-(((100-( 9 + __PBP__ * 2))%0402h)%4) 
	ENDIF
#line 52 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,10
	fcall _send_char
#line 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 1000>263690
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 1000of the argument exceeds the maxinum 263690'
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 1000<0
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	ERRMESSAGE 'the value 1000 of the argument exceeds the mininum 0'
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF 1000==0
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	JMP $
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=1) AND (1000<( 4 + __PBP__ * 2))
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=2) AND (1000<( 4 + __PBP__ * 2))
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=3) AND (1000<( 4 + __PBP__ * 2))
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=4) AND (1000<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=5) AND (1000<( 4 + __PBP__ * 2)) AND ( __PBP__ > 0 )
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=6) AND (1000<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=7) AND (1000<( 4 + __PBP__ * 2))  AND ( __PBP__ > 1 )
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000>=8) AND (1000<( 4 + __PBP__ * 2))  AND ( __PBP__ > 2 )
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000==9)  AND ( __PBP__ > 2 )
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	NOP
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000< ( 8 + __PBP__ * 2)) AND (1000>( 3 + __PBP__ * 2))
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAY3 + ( 7 + __PBP__ * 2) -1000
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000< ( 0404h + __PBP__ * 2)) AND (1000>( 7 + __PBP__ * 2))
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(1000-( 4 + __PBP__ * 2))/4
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX3 + 3 -((1000-( 4 + __PBP__ * 2)) MOD 4) 
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000< ( 040bh + __PBP__ * 2)) AND (1000> ( 0403h + __PBP__ * 2))
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0 
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYX6 + ( 040ah + __PBP__ * 2) -1000
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000 >= ( 040bh + __PBP__ * 2)) AND ((((1000-( 9 + __PBP__ * 2))%0402h)/4+1)>256)
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(1000-( 9 + __PBP__ * 2))/0402h
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,0
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY5 +5- (((1000-( 9 + __PBP__ * 2))%0402h)%6) 
	ENDIF
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	IF (1000 >= ( 040bh + __PBP__ * 2)) AND ((((1000-( 9 + __PBP__ * 2))%0402h)/4+1)<=256)
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,(1000-( 9 + __PBP__ * 2))/0402h
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV _builtin_holtek_delay_2,A
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	MOV A,((1000-( 9 + __PBP__ * 2))%0402h)/4+1
;# 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c" 1
	fcall __DELAYY3 +3-(((1000-( 9 + __PBP__ * 2))%0402h)%4) 
	ENDIF
	ret
_ESP8266_SendCmd endp
#line 53 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
#pragma debug scope 1 1
@crom	.section 'crom'
_LC0:
	db 041h,054h,02bh,052h,053h,054h,00h
@crom	.section 'crom'
_LC1:
	db 041h,054h,02bh,043h,057h,04dh,04fh,044h,045h,03dh
	db 031h,00h
@crom	.section 'crom'
_LC2:
	db 041h,054h,02bh,043h,057h,04ah,041h,050h,03dh,022h
	db 07ah,06ah,06ah,022h,02ch,022h,031h,032h,033h,034h
	db 035h,036h,037h,038h,022h,00h
@crom	.section 'crom'
_LC3:
	db 041h,054h,02bh,043h,049h,050h,04dh,055h,058h,03dh
	db 031h,00h
@crom	.section 'crom'
_LC4:
	db 041h,054h,02bh,043h,049h,050h,053h,045h,052h,056h
	db 045h,052h,03dh,031h,02ch,038h,038h,039h,039h,00h
public _ESP8266_ModeInit
#pragma debug scope 6 1
#line 57 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
_ESP8266_ModeInit .section 'code'
_ESP8266_ModeInit proc
#line 58 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,low offset _LC0
	mov _ESP8266_SendCmd_2[0],a
	mov a,high offset _LC0
	mov _ESP8266_SendCmd_2[1],a
	fcall _ESP8266_SendCmd
#line 59 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,-24
	mov _delay_ms_2[0],a
	mov a,3
	mov _delay_ms_2[1],a
	fcall _delay_ms
#line 60 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,low offset _LC1
	mov _ESP8266_SendCmd_2[0],a
	mov a,high offset _LC1
	mov _ESP8266_SendCmd_2[1],a
	fcall _ESP8266_SendCmd
#line 61 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,-24
	mov _delay_ms_2[0],a
	mov a,3
	mov _delay_ms_2[1],a
	fcall _delay_ms
#line 62 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,low offset _LC2
	mov _ESP8266_SendCmd_2[0],a
	mov a,high offset _LC2
	mov _ESP8266_SendCmd_2[1],a
	fcall _ESP8266_SendCmd
#line 63 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,16
	mov _delay_ms_2[0],a
	mov a,39
	mov _delay_ms_2[1],a
	fcall _delay_ms
#line 65 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,low offset _LC3
	mov _ESP8266_SendCmd_2[0],a
	mov a,high offset _LC3
	mov _ESP8266_SendCmd_2[1],a
	fcall _ESP8266_SendCmd
#line 66 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,-24
	mov _delay_ms_2[0],a
	mov a,3
	mov _delay_ms_2[1],a
	fcall _delay_ms
#line 67 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,low offset _LC4
	mov _ESP8266_SendCmd_2[0],a
	mov a,high offset _LC4
	mov _ESP8266_SendCmd_2[1],a
	fcall _ESP8266_SendCmd
#line 68 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
	mov a,-24
	mov _delay_ms_2[0],a
	mov a,3
	mov _delay_ms_2[1],a
	fcall _delay_ms
	ret
_ESP8266_ModeInit endp
#line 68 "E:\HT66\2019合泰杯线上讲课资料\HT8\HT66F2390UART例程_V1.1\HT66F2390_UART_1602\8266.c"
#pragma debug scope 1 1
#pragma debug struct_begin 44 "__intc0_bits"
#pragma debug field 21 8 0 1 "__emi" ;0,1
#pragma debug field 21 8 0 1 "__int0e" ;1,1
#pragma debug field 21 8 0 1 "__int1e" ;2,1
#pragma debug field 21 8 0 1 "__cp0e" ;3,1
#pragma debug field 21 8 0 1 "__int0f" ;4,1
#pragma debug field 21 8 0 1 "__int1f" ;5,1
#pragma debug field 21 8 0 1 "__cp0f" ;6,1
#pragma debug field 21 8 0 1 ;7,1
#pragma debug struct_end
#pragma debug union_begin 45 ""
#pragma debug field 2 44 "bits"
#pragma debug field 0 8 "byte"
#pragma debug union_end
#pragma debug variable 45 1 ___intc0 "__intc0" 1
#pragma debug struct_begin 46 "__intc3_bits"
#pragma debug field 21 8 0 1 "__int2e" ;0,1
#pragma debug field 21 8 0 1 "__int3e" ;1,1
#pragma debug field 21 8 0 1 "__mf4e" ;2,1
#pragma debug field 21 8 0 1 "__mf5e" ;3,1
#pragma debug field 21 8 0 1 "__int2f" ;4,1
#pragma debug field 21 8 0 1 "__int3f" ;5,1
#pragma debug field 21 8 0 1 "__mf4f" ;6,1
#pragma debug field 21 8 0 1 "__mf5f" ;7,1
#pragma debug struct_end
#pragma debug union_begin 47 ""
#pragma debug field 2 46 "bits"
#pragma debug field 0 8 "byte"
#pragma debug union_end
#pragma debug variable 47 1 ___intc3 "__intc3" 1
#pragma debug struct_begin 48 "__mfi5_bits"
#pragma debug field 21 8 0 1 "__ur0e" ;0,1
#pragma debug field 21 8 0 1 "__ur1e" ;1,1
#pragma debug field 21 8 0 1 "__ur2e" ;2,1
#pragma debug field 21 8 0 1 ;3,1
#pragma debug field 21 8 0 1 "__ur0f" ;4,1
#pragma debug field 21 8 0 1 "__ur1f" ;5,1
#pragma debug field 21 8 0 1 "__ur2f" ;6,1
#pragma debug field 21 8 0 1 ;7,1
#pragma debug struct_end
#pragma debug union_begin 49 ""
#pragma debug field 2 48 "bits"
#pragma debug field 0 8 "byte"
#pragma debug union_end
#pragma debug variable 49 1 ___mfi5 "__mfi5" 1
#pragma debug struct_begin 50 "__u2sr_bits"
#pragma debug field 21 8 0 1 "__txif2" ;0,1
#pragma debug field 21 8 0 1 "__tidle2" ;1,1
#pragma debug field 21 8 0 1 "__rxif2" ;2,1
#pragma debug field 21 8 0 1 "__ridle2" ;3,1
#pragma debug field 21 8 0 1 "__oerr2" ;4,1
#pragma debug field 21 8 0 1 "__ferr2" ;5,1
#pragma debug field 21 8 0 1 "__nf2" ;6,1
#pragma debug field 21 8 0 1 "__perr2" ;7,1
#pragma debug struct_end
#pragma debug union_begin 51 ""
#pragma debug field 2 50 "bits"
#pragma debug field 0 8 "byte"
#pragma debug union_end
#pragma debug variable 51 1 ___u2sr "__u2sr" 1
#pragma debug struct_begin 52 "__u2cr1_bits"
#pragma debug field 21 8 0 1 "__tx82" ;0,1
#pragma debug field 21 8 0 1 "__rx82" ;1,1
#pragma debug field 21 8 0 1 "__txbrk2" ;2,1
#pragma debug field 21 8 0 1 "__stops2" ;3,1
#pragma debug field 21 8 0 1 "__prt2" ;4,1
#pragma debug field 21 8 0 1 "__pren2" ;5,1
#pragma debug field 21 8 0 1 "__bno2" ;6,1
#pragma debug field 21 8 0 1 "__uarten2" ;7,1
#pragma debug struct_end
#pragma debug union_begin 53 ""
#pragma debug field 2 52 "bits"
#pragma debug field 0 8 "byte"
#pragma debug union_end
#pragma debug variable 53 1 ___u2cr1 "__u2cr1" 1
#pragma debug struct_begin 54 "__u2cr2_bits"
#pragma debug field 21 8 0 1 "__teie2" ;0,1
#pragma debug field 21 8 0 1 "__tiie2" ;1,1
#pragma debug field 21 8 0 1 "__rie2" ;2,1
#pragma debug field 21 8 0 1 "__wake2" ;3,1
#pragma debug field 21 8 0 1 "__adden2" ;4,1
#pragma debug field 21 8 0 1 "__brgh2" ;5,1
#pragma debug field 21 8 0 1 "__rxen2" ;6,1
#pragma debug field 21 8 0 1 "__txen2" ;7,1
#pragma debug struct_end
#pragma debug union_begin 55 ""
#pragma debug field 2 54 "bits"
#pragma debug field 0 8 "byte"
#pragma debug union_end
#pragma debug variable 55 1 ___u2cr2 "__u2cr2" 1
#pragma debug variable 8 1 ___txr_rxr2 "__txr_rxr2" 1
#pragma debug variable 8 1 ___brg2 "__brg2" 1
#pragma debug struct_begin 56 "__pbs0_bits"
#pragma debug field 21 8 0 1 "__pbs00" ;0,1
#pragma debug field 21 8 0 1 "__pbs01" ;1,1
#pragma debug field 21 8 0 1 "__pbs02" ;2,1
#pragma debug field 21 8 0 1 "__pbs03" ;3,1
#pragma debug field 21 8 0 1 "__pbs04" ;4,1
#pragma debug field 21 8 0 1 "__pbs05" ;5,1
#pragma debug field 21 8 0 1 "__pbs06" ;6,1
#pragma debug field 21 8 0 1 "__pbs07" ;7,1
#pragma debug struct_end
#pragma debug union_begin 57 ""
#pragma debug field 2 56 "bits"
#pragma debug field 0 8 "byte"
#pragma debug union_end
#pragma debug variable 57 1 ___pbs0 "__pbs0" 1
extern _delay_ms_2:byte
extern _delay_ms:near

; 
; Generated by holtek-gcc v3.19, Sat Mar 26 18:07:02 2022
; Rebuild 718
; end of file
