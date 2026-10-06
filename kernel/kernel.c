 void  kernel_entry(void){
 volatile short* ptr =(volatile short *)0xB8000;
  const char * type="Hello!! Lukita";
  for(int i =0; type[i]!= '\0';i++){
    ptr[i]=((unsigned short)0x0F << 8)
                 | (unsigned char)type[i];
  }
}