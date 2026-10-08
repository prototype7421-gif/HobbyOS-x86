 volatile short* ptr =(volatile short *)0xB8000;
   unsigned int cursor = 0;
  void print( const char* message){
  for(int i =0; message[i]!= '\0';i++){
      ptr[cursor]=((unsigned short)0x0F << 8)
                 | (unsigned char)message[i];
                 cursor ++;
  }
  }
   void  kernel_entry(void){
  print("hello lukita\n");
  print("wow\n");
 }