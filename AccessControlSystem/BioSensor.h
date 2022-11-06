#ifndef __BIOSENSOR_H__
#define __BIOSENSOR_H__


uint finger_detect(void);
void BIO_Cmd_Get_Img(void);
void BIO_Cmd_Check(void);
void Device_Check(void);
void BIO_Receive_Data(unsigned char ucLength);
void BIO_Delete_All_Fingerprint();
bit BIO_Find_Fingerprint();
void BIO_Cmd_Search_Finger(void);
void BIO_Add_Fingerprint(int login);	// Q:quit 
void BIO_Cmd_Reg_Model();
void BIO_Cmd_Save_Finger( unsigned int storeID );
void BIO_Cmd_Delete_All_Model(void);
//void BIO_Cmd_Delete_All_Data(void);

#endif

