import socket
import time

#ipAddressApp = "192.168.3.199"
ipAddressApp = "127.0.0.1"

sepPortIndicatorsApp = 10001

clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while 1:

    #                                                                        GG  GG       I  S    S                                                                    
    #                                            P        E  VB  TF       E  nn  nn       n  iO   h                                   F   B   Z  SS  SS  PH  ZT  TA  AT                                  H   B   H                  
    #                        SepVersion   8  75  z        4  44  HH  E    n  tt  tt  97  6d  cM   o  VZiel   LVSoll  LEntf     V      S   K   K  ii  ii  as  sü  sa  fr  VMaxE   VAfb        Zeit        L   Z   B    Position      
    #                        ----------  B5  05  b/  SH  G0  00  zz  lÜ  Sd  tÜ  GS  55  0u  SU   w  ------  ------  ------  ------  --  --  --  FL  HB  n   sS  Lv  bT  ------  ------  --------------  --  --  -- --------------- 
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x00\x00\x00\x00\x00\x00\x00\x10\x04\x10\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(2)
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x00\x00\x00\x00\x00\x00\x00\x10\x04\x40\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(5)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x01\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x02\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x03\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x04\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x05\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x06\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x07\x00\x00\x00\x30\x01\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x08\x00\x00\x00\x30\x01\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x09\x00\x00\x00\x30\x01\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x0A\x00\x00\x00\x30\x01\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x0B\x00\x00\x00\x30\x01\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x0C\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x0D\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x0E\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x0F\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x10\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x11\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x12\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x13\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x14\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x15\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x16\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x17\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x18\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x19\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x1A\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x1B\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x1C\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x1D\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x1E\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x1F\x00\x00\x00\x30\x00\x00\x10\x04\x00\x11\xA0\x00\x1F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    time.sleep(0.25)
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(5)
    telegramIndiPzbLm  = b'\x01\x00\x00\x15\x00\x10\x00\x00\x00\x10\x00\x00\x00\x00\x00\x00\x33\x60\x00\x00\x64\x00\xaa\x00\x46\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x46\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(5)
    #                                                                        GG  GG       I  S    S                                                                    
    #                                            P        E  VB  TF       E  nn  nn       n  iO   h                                   F   B   Z  SS  SS  PH  ZT  TA  AT                                  H   B   H                  
    #                        SepVersion   8  75  z        4  44  HH  E    n  tt  tt  97  6d  cM   o  VZiel   LVSoll  LEntf     V      S   K   K  ii  ii  as  sü  sa  fr  VMaxE   VAfb        Zeit        L   Z   B    Position      
    #                        ----------  B5  05  b/  SH  G0  00  zz  lÜ  Sd  tÜ  GS  55  0u  SU   w  ------  ------  ------  ------  --  --  --  FL  HB  n   sS  Lv  bT  ------  ------  --------------  --  --  -- --------------- 
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC8\x00\x94\x11\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(5)
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC8\x00\x98\x03\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC7\x00\x80\x03\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC6\x00\x70\x03\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC5\x00\x60\x03\xAA\x00\x00\x00\x00\x01\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC4\x00\x50\x03\xAA\x00\x00\x00\x00\x01\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC3\x00\x40\x03\xAA\x00\x00\x00\x00\x01\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC2\x00\x30\x03\xAA\x00\x00\x00\x00\x01\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC1\x00\x20\x03\xAA\x00\x00\x00\x00\x01\x10\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xC0\x00\x10\x03\xAA\x00\x00\x00\x00\x01\x10\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xBF\x00\x00\x03\xAA\x00\x00\x00\x00\x01\x10\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xBE\x00\xF0\x02\xAA\x00\x00\x00\x00\x01\x10\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xBD\x00\xE0\x02\xAA\x00\x00\x00\x00\x01\x10\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xBC\x00\xD0\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xBB\x00\xC0\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xBA\x00\xB0\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB9\x00\xA0\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB7\x00\x90\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB6\x00\x80\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB5\x00\x70\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB4\x00\x60\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB3\x00\x50\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB2\x00\x40\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB1\x00\x30\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xB0\x00\x20\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xAF\x00\x10\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xAE\x00\x00\x02\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xAD\x00\xF0\x01\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xAC\x00\xE0\x01\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xAB\x00\xD0\x01\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xAA\x00\xC0\x01\xAA\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xAA\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA9\x00\xB0\x01\xA9\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA9\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA7\x00\xA0\x01\xA7\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA7\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA6\x00\x90\x01\xA6\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA6\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA5\x00\x80\x01\xA5\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA5\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA4\x00\x70\x01\xA4\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA4\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA3\x00\x60\x01\xA3\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA3\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA2\x00\x50\x01\xA2\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA2\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA1\x00\x40\x01\xA1\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA1\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\xA0\x00\x30\x01\xA0\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\xA0\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x9F\x00\x20\x01\x9F\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x9F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x9E\x00\x10\x01\x9E\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x9E\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x9D\x00\x00\x01\x9D\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x9D\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x9C\x00\xF0\x00\x9C\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x9C\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x9B\x00\xE0\x00\x9B\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x9B\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x9A\x00\xD0\x00\x9A\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x9A\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x99\x00\xC0\x00\x99\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x99\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x97\x00\xB0\x00\x97\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x97\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x96\x00\xA0\x00\x96\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x96\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x95\x00\x90\x00\x95\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x95\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x94\x00\x80\x00\x94\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x94\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x93\x00\x70\x00\x93\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x93\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x92\x00\x60\x00\x92\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x92\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x91\x00\x50\x00\x91\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x91\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x90\x00\x40\x00\x90\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x90\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x8F\x00\x30\x00\x8F\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8F\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x8E\x00\x20\x00\x8E\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8E\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x8D\x00\x10\x00\x8D\x00\x00\x30\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8D\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x8C\x00\x00\x00\x8C\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8C\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(0.25)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x33\x61\x8C\x00\x8C\x00\x10\x27\x8C\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8C\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(5)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x05\x00\x00\x00\x00\x33\x61\x8C\x00\x8C\x00\x10\x27\x8C\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8C\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(3)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x01\x00\x00\x00\x00\x33\x61\x8C\x00\x8C\x00\x10\x27\x8C\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8C\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(3)
    telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x8C\x00\x8C\x00\x10\x27\x8C\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8C\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x33\x60\x8C\x00\x8C\x00\x10\x27\x8C\x00\x00\x00\x00\x00\x00\x10\x04\x00\x11\xA0\x00\x8C\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
    clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
    time.sleep(3)
