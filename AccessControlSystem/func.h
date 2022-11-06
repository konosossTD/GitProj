#ifndef _FUNC_H_
#define _FUNC_H_

#define uchar unsigned char
#define uint unsigned int
	

short trans(int lastkey);
	
int pwdcheck(bit r);

int match(uint key[],uint box[]);

void chgpwd(void);

void waitforaccess(uchar buffer);
#endif
