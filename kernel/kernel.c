 volatile short* ptr =(volatile short *)0xB8000;
    int cursor = 0;
void scroll_screen(void);
  void print( const char* message){
  for(int i =0; message[i]!= '\0';i++){
    if(message[i]=='\n'){
      cursor = ((cursor / 80) + 1) * 80;
    
    if(cursor>=2000){
      scroll_screen();
    }
    continue;
  }
   if(cursor>=2000){
      scroll_screen();
    }
    ptr[cursor]=((unsigned short)0x0F << 8)
                 | (unsigned char)message[i];
           cursor++;      
}
  }
void scroll_screen(void){
   int  screenSize = 80 * 25;
   int screenWidth =80;
  if(cursor >= screenSize){
    for(int i =0;i< screenSize-screenWidth;i++){
      ptr[i]=ptr[i+screenWidth];
    }
    for(int j =screenSize-screenWidth;j< screenSize;j++){
      ptr[j] = ((unsigned short)0x0F << 8) | ' ';
      }
      cursor=screenSize-screenWidth;
}
}

void kernel_entry(void)
{
    print("hello Modric");
}